#ifndef ZABER_COMMAND_MESSAGE_HPP //check for multiple inclusions
#define ZABER_COMMAND_MESSAGE_HPP

#include "Types.hpp"
#include <array>

namespace Zaber
{
	class CommandMessage
	{
	public:
		// types
		using DeviceAxisListener = std::function<void(uint8_t Device, uint8_t Axis)>;
	
		enum class CommandType : uint8_t 
		{
			None,
			EStop,
			Get,
			Home,
			IOInfo,
			IOGet,
			IOSet,
			Joystick,
			Key,
			LockStep,
			Move,
			Renumber,
			Set,
			Stop,
			Stream,
			SystemReset,
			SystemRestore,
			ToolsEcho,
			ToolsFindRange,
			ToolsGoToLimit,
			ToolsParking,
			ToolsSetComm,
			ToolsStorePos,
			Trigger,
			TriggerDist,
			TriggerTime,
			Virtual,
			Warnings
		};
		struct CommandTypeProperties
		{
			const char* code;
			Scope scope;
			bool CausesBusy = false;

			static const CommandTypeProperties ALL_COMMANDS[];
			static const CommandTypeProperties& Of(CommandType type);
		};

		enum class ParameterMessageType : uint8_t 
		{
			Ao,
			Ai,
			Do,
			Di,
			Target,
			SpeedProfile,
			MaxSpeed,
			Invert,
			Resolution,
			Info,
			Calibrate,
			Limits,
			Start,
			DeadBands,
			Add,
			Alert,
			Clear,
			Stream,
			Setup,
			Enable,
			Disable,
			Live,
			Call,
			Move,
			Abs,
			Rel,
			Vel,
			Min,
			Max,
			Stored,
			Store,
			Erase,
			Print,
			Arc,
			Cw,
			Ccw,
			Circle,
			Fifo,
			Cork,
			Uncork,
			Io,
			Set,
			Tanaccel,
			Centripaccel,
			Wait,
			Stop,
			EStop,
			Home,
			Away,
			A,
			B,
			C,
			D,
			Update,
			Current,
			When,
			Action,
			Equals,
			Pos,
			Park,
			Unpark,
			Angle,
			Ratio,
		};
		struct ParameterMessageTypeProperties
		{
			const char* String;

			static const ParameterMessageTypeProperties ALL_PARAMETER_MESSAGES[];
			static const ParameterMessageTypeProperties& Of(ParameterMessageType type);
		};

		enum class CommandParameterType : uint8_t 
		{
			None,
			Integer,
			Float,
			Message,
			Setting,
		};
		union CommandParameterValue
		{
			float Float;
			int32_t Integer;
			ParameterMessageType Message;
			SettingType Setting;
		};
		struct CommandParameter
		{
			CommandParameterValue Value = {.Integer = 0};
			CommandParameterType Type = CommandParameterType::None;

			static CommandParameter None() { return {{.Integer = 0}, CommandParameterType::None}; };
			static CommandParameter Float(float value) { return {{.Float = value}, CommandParameterType::Float}; };
			static CommandParameter Integer(int32_t value) { return {{.Integer = value}, CommandParameterType::Integer}; };
			static CommandParameter Message(ParameterMessageType value) { return {{.Message = value}, CommandParameterType::Message}; };
			static CommandParameter Setting(SettingType value) { return {{.Setting = value}, CommandParameterType::Setting}; };

			void PrintVerbose(Print& serial = Serial) const;
			void PrintForCommand(Print& serial = Serial) const;
		};

		// static constants
		static constexpr char COMMAND_MARKER_CHARACTER = '/';
		static constexpr size_t PARAMETER_MAX_LENGTH = 5;

		// fields
		CommandType Type = CommandType::None;
		uint8_t Axis = 0;
		uint8_t Device = 0;
		std::array<CommandParameter, PARAMETER_MAX_LENGTH> Parameters = {};
		uint8_t ParameterCount = 0;
		DeviceAxisListener Callback = nullptr;

	private:
		// private constructors
		template<size_t S, typename _ = std::enable_if_t<S <= PARAMETER_MAX_LENGTH>>
		CommandMessage(CommandType type, uint8_t device, uint8_t axis, CommandParameter const (&& parameters)[S], DeviceAxisListener returnCallback=nullptr) :
			Type(type), Axis(axis), Device(device), Parameters{}, ParameterCount(S), Callback(returnCallback)
		{
			Parameters.fill(CommandParameter::None());
			std::copy(std::begin(parameters), std::end(parameters), Parameters.begin());
		}

		CommandMessage(CommandType type, uint8_t device=0, uint8_t axis=0, DeviceAxisListener returnCallback=nullptr);
		CommandMessage(CommandType type, DeviceAxisListener returnCallback);

	public:
		// main constructor
		CommandMessage();

		// instance methods

		// if you call this to send the command, you must remember to print a newline '\n' afterwards.
		void PrintWithNoNewline(Print& serial = Serial) const;
		void PrintVerbose(Print& serial = Serial) const;

		// static named constructors

		// unique static constructors
		static CommandMessage GetStatus(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage Renumber(DeviceAxisListener returnCallback=nullptr);
		static CommandMessage EStop(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage Stop(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage Home(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage SystemReset(uint8_t device, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage SystemRestore(uint8_t device, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage ClearWarnings(uint8_t device, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage FindRange(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage MoveRel(uint8_t device, uint8_t axis, uint32_t steps, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage MoveAbs(uint8_t device, uint8_t axis, uint32_t steps, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage SetParked(uint8_t device, uint8_t axis, bool enable, DeviceAxisListener returnCallback=nullptr);

		// Get and Set helper constructors
	private: // The Set helper needs to be private to prevent the user from making malformed Set commands with parameters of the wrong type.
		static CommandMessage Set(uint8_t device, uint8_t axis, SettingType type, CommandParameter value, DeviceAxisListener returnCallback=nullptr);
	public:
		static CommandMessage Get(uint8_t device, uint8_t axis, SettingType type, DeviceAxisListener returnCallback=nullptr);

		// unique Setter and Getter static constructors
		static CommandMessage SetAcceleration(uint8_t device, uint8_t axis, uint32_t acceleration, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage GetAcceleration(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage SetMaxSpeed(uint8_t device, uint8_t axis, uint32_t maxSpeed, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage GetMaxSpeed(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage SetLimitMax(uint8_t device, uint8_t axis, uint32_t limitMax, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage GetLimitMax(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage SetLimitMin(uint8_t device, uint8_t axis, uint32_t limitMin, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage GetLimitMin(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage SetAlertStatus(uint8_t device, bool enable, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage GetAlertStatus(uint8_t device, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage SetKnobEnable(uint8_t device, uint8_t axis, bool enable, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage GetKnobEnable(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage SetResolution(uint8_t device, uint8_t axis, uint32_t resolution, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage GetResolution(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback=nullptr);

		static CommandMessage GetPosition(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage GetEncoderPosition(uint8_t device, uint8_t axis, DeviceAxisListener returnCallback=nullptr);
		static CommandMessage GetAxes(uint8_t device, DeviceAxisListener returnCallback=nullptr);
	};
}

#endif // ZABER_COMMAND_MESSAGE_HPP
