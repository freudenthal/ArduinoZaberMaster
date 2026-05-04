#include "CommandMessage.hpp"

namespace Zaber
{
	// enums and properties

	// command enum type array and conversions

	const CommandMessage::CommandTypeProperties CommandMessage::CommandTypeProperties::ALL_COMMANDS[]=
	{
		[(uint8_t)CommandType::None] = { "", Scope::Axis },
		[(uint8_t)CommandType::EStop] = { "estop", Scope::Axis, .CausesBusy = true },
		[(uint8_t)CommandType::Get] = { "get", Scope::DeviceOrAxis },
		[(uint8_t)CommandType::Home] = { "home", Scope::Axis, .CausesBusy = true },
		[(uint8_t)CommandType::IOInfo] = { "io info", Scope::Device },
		[(uint8_t)CommandType::IOGet] = { "io get", Scope::Device },
		[(uint8_t)CommandType::IOSet] = { "io set", Scope::Device },
		[(uint8_t)CommandType::Joystick] = { "joystick", Scope::Device },
		[(uint8_t)CommandType::Key] = { "key", Scope::Device },
		[(uint8_t)CommandType::LockStep] = { "lockstep", Scope::Device },
		[(uint8_t)CommandType::Move] = { "move", Scope::Axis, .CausesBusy = true },
		[(uint8_t)CommandType::Renumber] = { "renumber", Scope::Device },
		[(uint8_t)CommandType::Set] = { "set", Scope::DeviceOrAxis },
		[(uint8_t)CommandType::Stop] = { "stop", Scope::Axis, .CausesBusy = true },
		[(uint8_t)CommandType::Stream] = { "stream", Scope::Device },
		[(uint8_t)CommandType::SystemReset] = { "system reset", Scope::Device },
		[(uint8_t)CommandType::SystemRestore] = { "system restore", Scope::Device },
		[(uint8_t)CommandType::ToolsEcho] = { "tools echo", Scope::Device },
		[(uint8_t)CommandType::ToolsFindRange] = { "tools findrange", Scope::Axis, .CausesBusy = true },
		[(uint8_t)CommandType::ToolsGoToLimit] = { "tools gotolimit", Scope::Axis, .CausesBusy = true },
		[(uint8_t)CommandType::ToolsParking] = { "tools parking", Scope::Device },
		[(uint8_t)CommandType::ToolsSetComm] = { "tools setcomm", Scope::Device },
		[(uint8_t)CommandType::ToolsStorePos] = { "tools storepos", Scope::Axis },
		[(uint8_t)CommandType::Trigger] = { "trigger", Scope::Device },
		[(uint8_t)CommandType::TriggerDist] = { "trigger dist", Scope::Device },
		[(uint8_t)CommandType::TriggerTime] = { "trigger time", Scope::Device },
		[(uint8_t)CommandType::Virtual] = { "virtual", Scope::Device },
		[(uint8_t)CommandType::Warnings] = { "warnings", Scope::DeviceOrAxis },
	};

	const CommandMessage::CommandTypeProperties& CommandMessage::CommandTypeProperties::Of(CommandType type)
	{
		const uint8_t COUNT = ARRAY_SIZE(ALL_COMMANDS);
		uint8_t index = (uint8_t)type;
		// if index is invalid, set index to index of invalid entry
		if (index >= COUNT)
			index = COUNT - 1;
		return ALL_COMMANDS[index];
	}

	// message enum type array and conversions

	const CommandMessage::ParameterMessageTypeProperties CommandMessage::ParameterMessageTypeProperties::ALL_PARAMETER_MESSAGES[]=
	{
		[(uint8_t)CommandMessage::ParameterMessageType::Ao] = { "ao" },
		[(uint8_t)CommandMessage::ParameterMessageType::Ai] = { "ai" },
		[(uint8_t)CommandMessage::ParameterMessageType::Do] = { "do" },
		[(uint8_t)CommandMessage::ParameterMessageType::Di] = { "di" },
		[(uint8_t)CommandMessage::ParameterMessageType::Target] = { "target" },
		[(uint8_t)CommandMessage::ParameterMessageType::SpeedProfile] = { "speedprofile" },
		[(uint8_t)CommandMessage::ParameterMessageType::MaxSpeed] = { "maxspeed" },
		[(uint8_t)CommandMessage::ParameterMessageType::Invert] = { "invert" },
		[(uint8_t)CommandMessage::ParameterMessageType::Resolution] = { "resolution" },
		[(uint8_t)CommandMessage::ParameterMessageType::Info] = { "info" },
		[(uint8_t)CommandMessage::ParameterMessageType::Calibrate] = { "calibrate" },
		[(uint8_t)CommandMessage::ParameterMessageType::Limits] = { "limits" },
		[(uint8_t)CommandMessage::ParameterMessageType::Start] = { "start" },
		[(uint8_t)CommandMessage::ParameterMessageType::DeadBands] = { "deadbands" },
		[(uint8_t)CommandMessage::ParameterMessageType::Add] = { "add" },
		[(uint8_t)CommandMessage::ParameterMessageType::Alert] = { "alert" },
		[(uint8_t)CommandMessage::ParameterMessageType::Clear] = { "clear" },
		[(uint8_t)CommandMessage::ParameterMessageType::Stream] = { "stream" },
		[(uint8_t)CommandMessage::ParameterMessageType::Setup] = { "setup" },
		[(uint8_t)CommandMessage::ParameterMessageType::Enable] = { "enable" },
		[(uint8_t)CommandMessage::ParameterMessageType::Disable] = { "disable" },
		[(uint8_t)CommandMessage::ParameterMessageType::Live] = { "live" },
		[(uint8_t)CommandMessage::ParameterMessageType::Call] = { "call" },
		[(uint8_t)CommandMessage::ParameterMessageType::Move] = { "move" },
		[(uint8_t)CommandMessage::ParameterMessageType::Abs] = { "abs" },
		[(uint8_t)CommandMessage::ParameterMessageType::Rel] = { "rel" },
		[(uint8_t)CommandMessage::ParameterMessageType::Vel] = { "vel" },
		[(uint8_t)CommandMessage::ParameterMessageType::Min] = { "min" },
		[(uint8_t)CommandMessage::ParameterMessageType::Max] = { "max" },
		[(uint8_t)CommandMessage::ParameterMessageType::Stored] = { "stored" },
		[(uint8_t)CommandMessage::ParameterMessageType::Store] = { "store" },
		[(uint8_t)CommandMessage::ParameterMessageType::Erase] = { "erase" },
		[(uint8_t)CommandMessage::ParameterMessageType::Print] = { "print" },
		[(uint8_t)CommandMessage::ParameterMessageType::Arc] = { "arc" },
		[(uint8_t)CommandMessage::ParameterMessageType::Cw] = { "cw" },
		[(uint8_t)CommandMessage::ParameterMessageType::Ccw] = { "ccw" },
		[(uint8_t)CommandMessage::ParameterMessageType::Circle] = { "circle" },
		[(uint8_t)CommandMessage::ParameterMessageType::Fifo] = { "fifo" },
		[(uint8_t)CommandMessage::ParameterMessageType::Cork] = { "cork" },
		[(uint8_t)CommandMessage::ParameterMessageType::Uncork] = { "uncork" },
		[(uint8_t)CommandMessage::ParameterMessageType::Io] = { "io" },
		[(uint8_t)CommandMessage::ParameterMessageType::Set] = { "set" },
		[(uint8_t)CommandMessage::ParameterMessageType::Tanaccel] = { "tanaccel" },
		[(uint8_t)CommandMessage::ParameterMessageType::Centripaccel] = { "centripaccel" },
		[(uint8_t)CommandMessage::ParameterMessageType::Wait] = { "wait" },
		[(uint8_t)CommandMessage::ParameterMessageType::Stop] = { "stop" },
		[(uint8_t)CommandMessage::ParameterMessageType::EStop] = { "estop" },
		[(uint8_t)CommandMessage::ParameterMessageType::Home] = { "home" },
		[(uint8_t)CommandMessage::ParameterMessageType::Away] = { "away" },
		[(uint8_t)CommandMessage::ParameterMessageType::A] = { "a" },
		[(uint8_t)CommandMessage::ParameterMessageType::B] = { "b" },
		[(uint8_t)CommandMessage::ParameterMessageType::C] = { "c" },
		[(uint8_t)CommandMessage::ParameterMessageType::D] = { "d" },
		[(uint8_t)CommandMessage::ParameterMessageType::Update] = { "update" },
		[(uint8_t)CommandMessage::ParameterMessageType::Current] = { "current" },
		[(uint8_t)CommandMessage::ParameterMessageType::When] = { "when" },
		[(uint8_t)CommandMessage::ParameterMessageType::Action] = { "action" },
		[(uint8_t)CommandMessage::ParameterMessageType::Equals] = { "equals" },
		[(uint8_t)CommandMessage::ParameterMessageType::Pos] = { "pos" },
		[(uint8_t)CommandMessage::ParameterMessageType::Park] = { "park" },
		[(uint8_t)CommandMessage::ParameterMessageType::Unpark] = { "unpark" },
		[(uint8_t)CommandMessage::ParameterMessageType::Angle] = { "angle" },
		[(uint8_t)CommandMessage::ParameterMessageType::Ratio] = { "ratio" },
		// invalid entry
		{ "INVALID" },
	};

	const CommandMessage::ParameterMessageTypeProperties& CommandMessage::ParameterMessageTypeProperties::Of(ParameterMessageType type)
	{
		const uint8_t COUNT = ARRAY_SIZE(ALL_PARAMETER_MESSAGES);
		uint8_t index = (uint8_t)type;
		// if index is invalid, set index to index of invalid entry
		if (index >= COUNT)
			index = COUNT - 1;
		return ALL_PARAMETER_MESSAGES[index];
	}

	// parameter methods

	void CommandMessage::CommandParameter::PrintVerbose(Print& serial) const
	{
		switch(Type)
		{
			case CommandParameterType::Float:
				serial.printf("%g", Value.Float);
				break;
			case CommandParameterType::Integer:
				serial.print(Value.Integer);
				break;
			case CommandParameterType::Message:
				serial.print(ParameterMessageTypeProperties::Of(Value.Message).String);
				break;
			case CommandParameterType::Setting:
				serial.print(SettingTypeProperties::Of(Value.Setting).Code);
				break;
			case CommandParameterType::None:
				serial.print("None");
				break;
			default:
				serial.print("UnknownType");
				break;
		}
	};

	void CommandMessage::CommandParameter::PrintForCommand(Print& serial) const
	{
		switch(Type)
		{
			case CommandParameterType::Float:
				serial.printf("%1.3f", Value.Float);
				break;
			case CommandParameterType::Integer:
				serial.print(Value.Integer);
				break;
			case CommandParameterType::Message:
				serial.print(ParameterMessageTypeProperties::Of(Value.Message).String);
				break;
			case CommandParameterType::Setting:
				serial.print(SettingTypeProperties::Of(Value.Setting).Code);
				break;
			case CommandParameterType::None:
			default:
				break;
		}
	};

	// command message implementations

	// constructors
	CommandMessage::CommandMessage(CommandType type, uint8_t device, uint8_t axis, DeviceAxisListener returnCallback) :
		Type(type), Axis(axis), Device(device), Parameters{}, ParameterCount(0), Callback(returnCallback) {}
	CommandMessage::CommandMessage(CommandType type, DeviceAxisListener returnCallback) :
		CommandMessage(type, 0, 0, returnCallback) {}

	CommandMessage::CommandMessage() : CommandMessage(CommandType::None) {}

	// instance methods

	/**
	 * Usable for printing out command over serial to zaber motors.
	 */
	void CommandMessage::PrintWithNoNewline(Print& serial) const
	{
		serial.print(COMMAND_MARKER_CHARACTER);
		serial.print(Device);
		serial.print(' ');
		serial.print(Axis);
		serial.print(' ');
		serial.print(CommandTypeProperties::Of(Type).code);

		for (uint8_t i = 0; i < ParameterCount; i++)
		{
			serial.print(' ');
			Parameters[i].PrintForCommand(serial);
		}
	}

	/**
	 * Usable for printing out command fields to text for human readability.
	 */
	void CommandMessage::PrintVerbose(Print& serial) const
	{
		Serial.print(CommandTypeProperties::Of(Type).code);
		Serial.print(',');
		Serial.print(Device);
		Serial.print(',');
		Serial.print(Axis);
		Serial.print(',');
		Serial.print(ParameterCount);
		Serial.print(",[");
		for (uint8_t i = 0; i < ParameterCount; i++)
		{
			if (i != 0)
				Serial.print(',');
			Parameters[i].PrintVerbose(serial);
		}
		Serial.print("],");
		Serial.print((Callback != nullptr) ? 'Y' : 'N');
	}

	// unique static constructors
	CommandMessage CommandMessage::GetStatus(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback) {
		return CommandMessage(CommandType::None, device, axis, returnCallback);
	}
	CommandMessage CommandMessage::Renumber(DeviceAxisListener returnCallback) {
		return CommandMessage(CommandType::Renumber, returnCallback);
	}
	CommandMessage CommandMessage::EStop(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback) {
		return CommandMessage(CommandType::EStop, device, axis, returnCallback);
	}
	CommandMessage CommandMessage::Stop(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback) {
		return CommandMessage(CommandType::EStop, device, axis, returnCallback);
	}
	CommandMessage CommandMessage::Home(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback) {
		return CommandMessage(CommandType::Home, device, axis, returnCallback);
	}
	CommandMessage CommandMessage::SystemReset(uint8_t device, DeviceAxisListener returnCallback) {
		return CommandMessage(CommandType::SystemReset, device, 0, returnCallback);
	}
	CommandMessage CommandMessage::SystemRestore(uint8_t device, DeviceAxisListener returnCallback) {
		return CommandMessage(CommandType::SystemRestore, device, 0, returnCallback);
	}
	CommandMessage CommandMessage::ClearWarnings(uint8_t device, DeviceAxisListener returnCallback) {
		return CommandMessage(CommandType::Warnings, device, 0,
			{ CommandParameter::Message(ParameterMessageType::Clear) },
			returnCallback
		);
	}
	CommandMessage CommandMessage::FindRange(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback) {
		return CommandMessage(CommandType::ToolsFindRange, device, axis, returnCallback);
	}
	CommandMessage CommandMessage::MoveRel(uint8_t device, uint8_t axis, uint32_t steps, DeviceAxisListener returnCallback) {
		return CommandMessage(CommandType::Move, device, axis,
			{ CommandParameter::Message(ParameterMessageType::Rel), CommandParameter::Integer(steps) },
			returnCallback
		);
	}
	CommandMessage CommandMessage::MoveAbs(uint8_t device, uint8_t axis, uint32_t steps, DeviceAxisListener returnCallback) {
		return CommandMessage(CommandType::Move, device, axis,
			{ CommandParameter::Message(ParameterMessageType::Abs), CommandParameter::Integer(steps) },
			returnCallback
		);
	}
	CommandMessage CommandMessage::SetParked(uint8_t device, uint8_t axis, bool enable, DeviceAxisListener returnCallback) {
		return CommandMessage(CommandType::ToolsParking, device, axis,
			{ CommandParameter::Message(enable ? ParameterMessageType::Park : ParameterMessageType::Unpark) },
			returnCallback
		);
	}

	// static Get and Set helper constructors
	CommandMessage CommandMessage::Set(uint8_t device, uint8_t axis, SettingType type, CommandParameter value, DeviceAxisListener returnCallback) {
		return CommandMessage(CommandType::Set, device, axis, { CommandParameter::Setting(type), value }, returnCallback);
	}
	CommandMessage CommandMessage::Get(uint8_t device, uint8_t axis, SettingType type, DeviceAxisListener returnCallback) {
		return CommandMessage(CommandType::Get, device, axis, { CommandParameter::Setting(type) }, returnCallback);
	}

	// unique Setter and Getter static constructors
	CommandMessage CommandMessage::SetAcceleration(uint8_t device, uint8_t axis, uint32_t acceleration, DeviceAxisListener returnCallback) {
		return Set(device, axis, SettingType::Accel, CommandParameter::Integer(acceleration), returnCallback);
	}
	CommandMessage CommandMessage::GetAcceleration(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback) {
		return Get(device, axis, SettingType::Accel, returnCallback);
	}
	CommandMessage CommandMessage::SetMaxSpeed(uint8_t device, uint8_t axis, uint32_t maxSpeed, DeviceAxisListener returnCallback) {
		return Set(device, axis, SettingType::MaxSpeed, CommandParameter::Integer(maxSpeed), returnCallback);
	}
	CommandMessage CommandMessage::GetMaxSpeed(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback) {
		return Get(device, axis, SettingType::MaxSpeed, returnCallback);
	}
	CommandMessage CommandMessage::SetLimitMax(uint8_t device, uint8_t axis, uint32_t limitMax, DeviceAxisListener returnCallback) {
		return Set(device, axis, SettingType::LimitMax, CommandParameter::Integer(limitMax), returnCallback);
	}
	CommandMessage CommandMessage::GetLimitMax(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback) {
		return Get(device, axis, SettingType::LimitMax, returnCallback);
	}
	CommandMessage CommandMessage::SetLimitMin(uint8_t device, uint8_t axis, uint32_t limitMin, DeviceAxisListener returnCallback) {
		return Set(device, axis, SettingType::LimitMin, CommandParameter::Integer(limitMin), returnCallback);
	}
	CommandMessage CommandMessage::GetLimitMin(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback) {
		return Get(device, axis, SettingType::LimitMin, returnCallback);
	}
	CommandMessage CommandMessage::SetAlertStatus(uint8_t device, bool enable, DeviceAxisListener returnCallback) {
		return Set(device, 0, SettingType::CommAlert, CommandParameter::Integer(enable), returnCallback);
	}
	CommandMessage CommandMessage::GetAlertStatus(uint8_t device, DeviceAxisListener returnCallback) {
		return Get(device, 0, SettingType::CommAlert, returnCallback);
	}
	CommandMessage CommandMessage::SetKnobEnable(uint8_t device, uint8_t axis, bool enable, DeviceAxisListener returnCallback) {
		return Set(device, axis, SettingType::KnobEnable, CommandParameter::Integer(enable), returnCallback);
	}
	CommandMessage CommandMessage::GetKnobEnable(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback) {
		return Get(device, axis, SettingType::KnobEnable, returnCallback);
	}
	CommandMessage CommandMessage::SetResolution(uint8_t device, uint8_t axis, uint32_t resolution, DeviceAxisListener returnCallback) {
		return Set(device, axis, SettingType::Resolution, CommandParameter::Integer(resolution), returnCallback);
	}
	CommandMessage CommandMessage::GetResolution(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback) {
		return Get(device, axis, SettingType::Resolution, returnCallback);
	}

	CommandMessage CommandMessage::GetPosition(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback) {
		return Get(device, axis, SettingType::Pos, returnCallback);
	}
	CommandMessage CommandMessage::GetEncoderPosition(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback) {
		return Get(device, axis, SettingType::EncoderPos, returnCallback);
	}
	CommandMessage CommandMessage::GetAxes(uint8_t device, DeviceAxisListener returnCallback) {
		return Get(device, 0, SettingType::SystemAxisCount, returnCallback);
	}
}
