#ifndef ZABER_REPLY_PARSER_HPP //check for multiple inclusions
#define ZABER_REPLY_PARSER_HPP

#include "Types.hpp"

#include <vector>
#include <limits>
#include <charconv>
#include <system_error>

#define ZaberMaxReplyData 4

namespace Zaber
{
	class ReplyParser
	{
	public:
		enum class ReplyDataType : uint8_t 
		{
			None,
			Float,
			Integer,
			Error,
			Warning,
		};
		static const char* toString(ReplyDataType v);

		union ReplyDataValue
		{
			float Float;
			int32_t Integer;
			WarningType Warning;
			RejectionErrorType Error;
		};

		struct ReplyData
		{
			ReplyDataType Type = ReplyDataType::None;
			ReplyDataValue Value = {.Integer = 0};

			void print(Print& serial = Serial) const;

			static ReplyData None();
			static ReplyData Float(float value);
			static ReplyData Integer(int32_t value);
			static ReplyData Error(RejectionErrorType value);
			static ReplyData Warning(WarningType value);
		};

		struct ReplyMessage
		{
			bool MessageStarts = false;
			bool MessageEnds = false;
			ResponseType Type = ResponseType::Reply;
			uint8_t Device = 0;
			uint8_t Axis = 0;
			FlagType Flag = FlagType::Okay;
			StatusType Status = StatusType::Idle;
			WarningType Warning = WarningType::None;
			ReplyData Data[ZaberMaxReplyData] = {};
			size_t DataLength = 0;

			void print(Print& serial = Serial) const;
			void PrintVerboseMultiLine(Print& serial = Serial) const;
		};

		static std::optional<ReplyMessage> ParseLine(std::string_view buf);
	private:
		static std::vector<std::string_view> tokenize_space(std::string_view string);

		// parse numbers from strings

		static std::optional<float> parse_float(std::string_view string, bool Verbose=false);
		static std::optional<double> parse_double(std::string_view string, bool Verbose=false);

		template <typename T>
		static std::enable_if_t<std::is_integral_v<T>, std::optional<T>> parse_integer(std::string_view string, bool Verbose=false)
		{
			if (string.empty())
			{
				// can't parse an empty string
				if (Verbose) Serial.print("<ZABERPARSE>(Integer: Failed: buffer empty.)\n");
				return std::nullopt;
			}
			T number;
			std::from_chars_result result = std::from_chars(string.data(), string.data() + string.size(), number);
			if (std::make_error_code(result.ec))
			{
				// there was an error (error code is 0 if no error)
				if (Verbose) Serial.printf("<ZABERPARSE>(Integer: Failed: Errorcode from std::from_chars: %d)\n", std::make_error_code(result.ec).value());
				return std::nullopt;
			}
			if (result.ptr != string.data() + string.size())
			{
				// did not evaluate entire string to convert into number
				if (Verbose) Serial.printf("<ZABERPARSE>(Integer: Failed: Only parsed %d of %d characters.)\n", result.ptr - string.data(), string.size());
				return std::nullopt;
			}
			return number;
		}
	};
}

#endif // ZABER_REPLY_PARSER_HPP
