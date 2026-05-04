#include "ReplyParser.hpp"
#include <cctype>

namespace Zaber
{
	// ReplyDataType methods

	const char* ReplyParser::toString(ReplyDataType v)
	{
		switch (v)
		{
			case ReplyDataType::None: return "None";
			case ReplyDataType::Float: return "Float";
			case ReplyDataType::Integer: return "Integer";
			case ReplyDataType::Error: return "Error";
			case ReplyDataType::Warning: return "Warning";
			default: return "Unknown";
		}
	}

	// ReplyData methods

	void ReplyParser::ReplyData::print(Print& serial) const
	{
		switch (Type)
		{
			case ReplyDataType::Float:
				serial.print(Value.Float, 6);
				break;
			case ReplyDataType::Integer:
				serial.print(Value.Integer);
				break;
			case ReplyDataType::Error:
				serial.print(RejectionError::Of(Value.Error).ReadableDescription);
				break;
			case ReplyDataType::Warning:
				serial.print(WarningTypeProperties::Of(Value.Warning).ReadableName);
				break;
			default:
				serial.print("UnknownReplyDataType");
				break;
		}
	}

	ReplyParser::ReplyData ReplyParser::ReplyData::None()
	{
		return { ReplyDataType::None, {.Integer = 0} };
	}

	ReplyParser::ReplyData ReplyParser::ReplyData::Float(float value)
	{
		return { ReplyDataType::Float, {.Float = value} };
	}

	ReplyParser::ReplyData ReplyParser::ReplyData::Integer(int32_t value)
	{
		return { ReplyDataType::Integer, {.Integer = value} };
	}

	ReplyParser::ReplyData ReplyParser::ReplyData::Error(RejectionErrorType value)
	{
		return { ReplyDataType::Error, {.Error = value} };
	}

	ReplyParser::ReplyData ReplyParser::ReplyData::Warning(WarningType value)
	{
		return { ReplyDataType::Warning, {.Warning = value} };
	}

	// Reply Message methods

	void ReplyParser::ReplyMessage::print(Print& serial) const
	{
		Serial.print(MessageStarts ? "started" : "not started");
		Serial.print(',');
		Serial.print(MessageEnds ? "completed" : "not completed");
		Serial.print(',');
		Serial.print(ResponseTypeProperties::Of(Type).ReadableName);
		Serial.print(',');
		Serial.print(Device);
		Serial.print(',');
		Serial.print(Axis);
		Serial.print(',');
		Serial.print(FlagTypeProperties::Of(Flag).ReadableName);
		Serial.print(',');
		Serial.print(StatusTypeProperties::Of(Status).ReadableName);
		Serial.print(',');
		Serial.print(WarningTypeProperties::Of(Warning).ReadableName);
		Serial.print(',');
		Serial.print(DataLength);
		Serial.print(",[");
		for (size_t i = 0; i < DataLength; i++)
		{
			if (i != 0)
				Serial.print(',');
			const auto& d = Data[i];
			Serial.print(toString(d.Type));
			Serial.print('{');
			d.print(Serial);
			Serial.print('}');
		}
		Serial.print(']');
	}

	void ReplyParser::ReplyMessage::PrintVerboseMultiLine(Print& serial) const
	{
		serial.print("---- ReplyMessage ----\n");
		serial.printf("MessageStarts: %s\n", MessageStarts ? "true" : "false");
		serial.printf("MessageEnds: %s\n", MessageEnds ? "true" : "false");
		serial.printf("ResponseType: %s\n", ResponseTypeProperties::Of(Type).ReadableName);
		serial.printf("Device: %d\n", Device);
		serial.printf("Axis: %d\n", Axis);
		serial.printf("Flag: %s\n", FlagTypeProperties::Of(Flag).ReadableName);
		serial.printf("Status: %s\n", StatusTypeProperties::Of(Status).ReadableName);
		serial.printf("Warning: %s\n", WarningTypeProperties::Of(Warning).ReadableName);
		serial.printf("DataLength: %d\n", DataLength);
		serial.print("---- Data ----\n");
		for (size_t i = 0; i < DataLength; i++)
		{
			const ReplyData& d = Data[i];
			serial.printf("  [%d] %s(", i, toString(d.Type));
			d.print(serial);
			serial.print(")\n");
		}
		serial.print("-----------------------\n");
	}

	// Parser methods

	std::vector<std::string_view> ReplyParser::tokenize_space(std::string_view source)
	{
		// Split on ASCII spaces; skip CR/LF. Keeps slices into original buffer.
		size_t startIndex = 0;
		std::vector<std::string_view> tokens{};
		// skip to the start by skipping all newlines
		while (startIndex < source.length() && (source[startIndex] == '\r' || source[startIndex] == '\n'))
		{
			startIndex++;
		}
		// parse tokens as long as the current start index is not at the end
		while (startIndex < source.length())
		{
			// skip spaces to the next token
			while (startIndex < source.length() && source[startIndex] == ' ')
			{
				startIndex++;
			}
			// exit if at EOL (no tokens left)
			if (startIndex >= source.length() || source[startIndex] == '\r' || source[startIndex] == '\n')
			{
				break;
			}
			// find end of current token by skipping all non-delimiters-and-endlines
			size_t endIndex = startIndex;
			while (endIndex < source.length() && source[endIndex] != ' ' && source[endIndex] != '\r' && source[endIndex] != '\n')
			{
				endIndex++;
			}
			// save the token by adding it to the list
			tokens.push_back(std::string_view(&source[startIndex], endIndex - startIndex));
			// the next search for tokens should start at the end of the current token
			startIndex = endIndex;
		}
		return tokens;
	}

	// Main parser. Returns message if it looks like a valid Zaber line.
	std::optional<ReplyParser::ReplyMessage> ReplyParser::ParseLine(std::string_view buf)
	{
		// Reset ReplyMessage
		ReplyMessage out = {};

		// messages must have content
		if (buf.empty())
		{
			// no string to parse
			Serial.print("<ZABERPARSE>(Failed: buffer empty.)\n");
			return std::nullopt; 
		}

		// end with newline or carriage return (optional)
		out.MessageEnds = buf[buf.length()-1] == '\n' || buf[buf.length()-1] == '\r';

		// Type
		auto optType = ToResponseType(buf[0]);
		out.MessageStarts = optType.has_value();
		if (!optType)
		{
			// no valid type
			Serial.print("<ZABERPARSE>(Failed: Response does not start with a recognized type character.)\n");
			return std::nullopt; 
		}
		out.Type = optType.value();

		if (out.Type == ResponseType::Info)
		{
			// don't parse info messages
			Serial.print("<ZABERPARSE>(Failed: Cannot parse Info Responses.)\n");
			return std::nullopt;
		}

		// Tokenize after the first char (message type)
		auto tokens = tokenize_space(buf.substr(1)); // tokenize after 1st char because that's the type character
		if (tokens.size() < 2)
		{
			 // need at least device, axis
			Serial.print("<ZABERPARSE>(Failed: Not enough Tokens for Device and Axis.)\n");
			return std::nullopt;
		}
		size_t idx = 0;

		// Device (2 digits)
		if (idx >= tokens.size())
		{
			 // missing device token
			Serial.print("<ZABERPARSE>(Failed: Missing token for Device.)\n");
			return std::nullopt;
		}
		if (tokens[idx].length() != 2)
		{
			// device param length is 2
			Serial.printf("<ZABERPARSE>(Failed: Device token length (%d) is not expected 2 digits.)\n", tokens[idx].length());
			return std::nullopt;
		}
		auto optDevice = parse_integer<uint8_t>(tokens[idx], true);
		if (!optDevice)
		{
			// failed to parse device
			Serial.printf("<ZABERPARSE>(Failed: could not parse Device \"%.*s\" into integer.)\n", tokens[idx].length(), tokens[idx].data());
			return std::nullopt;
		}
		out.Device = optDevice.value();
		idx++;

		// Axis (1 digit)
		if (idx >= tokens.size())
		{
			// missing axis token
			Serial.print("<ZABERPARSE>(Failed: Missing token for Axis.)\n");
			return std::nullopt;
		}
		if (tokens[idx].length() != 1)
		{
			// axis param length is 1
			Serial.printf("<ZABERPARSE>(Failed: Axis token length (%d) is not expected 1 digit.)\n", tokens[idx].length());
			return std::nullopt;
		}
		auto optAxis = parse_integer<uint8_t>(tokens[idx], true);
		if (!optAxis)
		{
			// failed to parse axis
			Serial.printf("<ZABERPARSE>(Failed: could not parse Axis \"%.*s\" into integer.)\n", tokens[idx].length(), tokens[idx].data());
			return std::nullopt;
		}
		out.Axis = optAxis.value();
		idx++;

		// Optional Message ID token (2 digits); if present, shift indices by +1
		if (idx < tokens.size())
		{
			if (tokens[idx].length() == 2)
			{
				auto optMessageID = parse_integer<uint8_t>(tokens[idx]);
				if (optMessageID)
				{
					// there is a messageID
					idx++;
				}
			}
		}

		// parsing changes based on type
		if (out.Type == ResponseType::Reply)
		{
			// Flag
			if (idx >= tokens.size())
			{
				// missing token for flag
				Serial.print("<ZABERPARSE>(Failed: Missing token for Flag.)\n");
				return std::nullopt;
			}
			auto optFlag = ToFlagType(tokens[idx]);
			if (!optFlag)
			{
				// failed to parse flag
				Serial.printf("<ZABERPARSE>(Failed: Could not parse \"%.*s\" to Flag.)\n", tokens[idx].length(), tokens[idx].data());
				return std::nullopt;
			}
			out.Flag = optFlag.value();
			idx++;

			// Status
			if (idx >= tokens.size())
			{
				// missing token for status
				Serial.print("<ZABERPARSE>(Failed: Missing token for Status.)\n");
				return std::nullopt;
			}
			auto optStatus = ToStatusType(tokens[idx]);
			if (!optStatus)
			{
				// failed to parse status
				Serial.printf("<ZABERPARSE>(Failed: Could not parse \"%.*s\" to Status.)\n", tokens[idx].length(), tokens[idx].data());
				return std::nullopt;
			}
			out.Status = optStatus.value();
			idx++;

			// Warning
			if (idx >= tokens.size())
			{
				// out of tokens for warning
				Serial.print("<ZABERPARSE>(Failed: Missing token for Warning.)\n");
				return std::nullopt;
			}
			auto optWarning = ToWarningType(tokens[idx]);
			if (!optWarning)
			{
				// failed to parse warning
				Serial.printf("<ZABERPARSE>(Failed: Could not parse \"%.*s\" to Warning.)\n", tokens[idx].length(), tokens[idx].data());
				return std::nullopt;
			}
			out.Warning = optWarning.value();
			idx++;

			// Remaining tokens are data (zero or more)
			while (idx < tokens.size())
			{
				const auto& t = tokens[idx++];
				// ignore explicit "no data"
				if (t != "--")
				{
					if (out.DataLength >= ARRAY_SIZE(out.Data))
					{
						// too many tokens, not enough data space
						Serial.printf("<ZABERPARSE>(Failed: Too many data packets to store. Count %d exceeded max capacity %d.)\n",
							(tokens.size() - idx) + out.DataLength, ARRAY_SIZE(out.Data));
						return std::nullopt;
					}

					if (auto optInt = parse_integer<int32_t>(t))
					{
						out.Data[out.DataLength++] = ReplyData::Integer(optInt.value());
					}
					else if (auto optFloat = parse_float(t))
					{
						out.Data[out.DataLength++] = ReplyData::Float(optFloat.value());
					}
					else if (auto optWarning = ToWarningType(t))
					{
						out.Data[out.DataLength++] = ReplyData::Warning(optWarning.value());
					}
					else if (auto optError = ToRejectionErrorType(t))
					{
						out.Data[out.DataLength++] = ReplyData::Error(optError.value());
					}
					else
					{
						Serial.printf("<ZABERPARSE>(WARN: Bad data parse: \"%.*s\".)\n", (int)t.length(), t.data());
					}
				}
			}

			// valid reply
			return out;
		}
		else if (out.Type == ResponseType::Alert)
		{
			// Alerts do NOT include a flag. Format is: STATUS WARNING
			// Minimum tokens after (dev,axis[,msgid]) is 1 (status); warning may follow.
			
			// STATUS (required)
			if (idx >= tokens.size())
			{
				// missing token for status
				Serial.print("<ZABERPARSE>(Failed: Missing token for Status.)\n");
				return std::nullopt;
			}
			auto optStatus = ToStatusType(tokens[idx]);
			if (!optStatus)
			{
				// failed to parse status
				Serial.printf("<ZABERPARSE>(Failed: Could not parse \"%.*s\" to Status.)\n", tokens[idx].length(), tokens[idx].data());
				return std::nullopt;
			}
			out.Status = optStatus.value();
			idx++;

			// WARNING (optional but typical)
			if (idx < tokens.size())
			{
				// if any tokens exist at this point, they must be a warning
				auto optWarning = ToWarningType(tokens[idx]);
				if (!optWarning)
				{
					// failed to parse warning
					Serial.printf("<ZABERPARSE>(Failed: Could not parse \"%.*s\" to Warning.)\n", tokens[idx].length(), tokens[idx].data());
					return std::nullopt;
				}
				out.Warning = optWarning.value();
				idx++;
			}

			// Alerts usually have no trailing data; ignore anything extra but warn once.
			if (idx < tokens.size())
			{
				Serial.println("<ZABERPARSE>(WARN: Extra tokens after alert; ignoring.)");
			}

			// valid alert
			return out;
		}

		// invalid type
		Serial.printf("<ZABERPARSE>(Failed: Parsing for response type \"%s\" not implemented.)\n", ResponseTypeProperties::Of(out.Type).ReadableName);
		return std::nullopt;
	}

	std::optional<float> ReplyParser::parse_float(std::string_view string, bool Verbose)
	{
		if (string.empty())
		{
			// can't parse an empty string
			if (Verbose) Serial.print("<ZABERPARSE>(Float: Failed: buffer empty.)\n");
			return std::nullopt;
		}

		std::string stringCopy = std::string(string);
		char* endPtr = nullptr;
		errno = 0;
		float number = std::strtof(stringCopy.data(), &endPtr);
		if (errno != 0)
		{
			// there was an error (error code is 0 if no error)
			if (Verbose) Serial.printf("<ZABERPARSE>(Float: Failed: Errorcode from strtof: %d)\n", errno);
			return std::nullopt;
		}

		if (endPtr != (stringCopy.data() + stringCopy.size()))
		{
			// did not evaluate entire string to convert into number
			if (Verbose) Serial.printf("<ZABERPARSE>(Float: Failed: Only parsed %d of %d characters.)\n", endPtr - stringCopy.data(), stringCopy.size());
			return std::nullopt;
		}
		return number;
	}

	std::optional<double> ReplyParser::parse_double(std::string_view string, bool Verbose)
	{
		if (string.empty())
		{
			// can't parse an empty string
			if (Verbose) Serial.print("<ZABERPARSE>(Double: Failed: buffer empty.)\n");
			return std::nullopt;
		}

		std::string stringCopy = std::string(string);
		char* endPtr = nullptr;
		errno = 0;
		double number = std::strtod(stringCopy.data(), &endPtr);
		if (errno != 0)
		{
			// there was an error (error code is 0 if no error)
			if (Verbose) Serial.printf("<ZABERPARSE>(Float: Failed: Errorcode from strtof: %d)\n", errno);
			return std::nullopt;
		}

		if (endPtr != (stringCopy.data() + stringCopy.size()))
		{
			// did not evaluate entire string to convert into number
			if (Verbose) Serial.printf("<ZABERPARSE>(Float: Failed: Only parsed %d of %d characters.)\n", endPtr - stringCopy.data(), stringCopy.size());
			return std::nullopt;
		}
		return number;
	}
}
