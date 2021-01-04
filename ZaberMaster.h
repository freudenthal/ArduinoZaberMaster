/*

Zaber master controller. Takes a serial bus and controls a Zaber bus.

*/

#ifndef ZaberMaster_h	//check for multiple inclusions
#define ZaberMaster_h

#include "Arduino.h"

#define ZaberMaxDataString 8
#define ZaberMaxReplyData 4
#define ZaberMaxDevices 1
#define ZaberMaxAxes 2
#define ZabberCommandParameterStringMaxLength 
#define CommandBufferSize 24
#define ReturnBufferSize 24
#define ZaberParameterMaxLength 5
#define ZaberMasterInitializationStepsCount 4

typedef void ( *ZaberFinishedListener )();
typedef void ( *ZaberFinishedListenerDeviceAxis )(uint8_t Device, uint8_t Axis);
class ZaberMaster
{
	public:
		typedef void(ZaberMaster::*CommandSentCallback)();
		typedef void(ZaberMaster::*ReplyReceivedCallback)();
		enum class MessageType : uint8_t 
		{
			Reply,
			Info,
			Alert,
			Count
		};
		enum class StatusType : uint8_t 
		{
			Idle,
			Busy,
			Count
		};
		enum class MessageParts : uint8_t 
		{
			Type,
			Device,
			Axis,
			Command,
			Parameter,
			Count
		};
		enum class ReplyParts : uint8_t 
		{
			Type,
			Device,
			Axis,
			Flag,
			Status,
			Warning,
			Data,
			Count
		};
		enum class AlertParts : uint8_t 
		{
			Type,
			Device,
			Axis,
			Status,
			Warning,
			Count
		};
		enum class ReplyDataErrorType : uint8_t 
		{
			None,
			Again,
			BadAxis,
			BadData,
			BadMessageID,
			DeviceOnly,
			Full,
			LockStep,
			NoAccess,
			Parked,
			StatusBusy,
			Count
		};
		enum class FlagType : uint8_t 
		{
			Okay,
			Rejected,
			Count
		};
		enum class WarningType : uint8_t 
		{
			None,
			DriverDisabled,
			EncoderError,
			StalledAndStopped,
			ExcessiveTwist,
			StreamBoundErrors,
			InterpolationDeviation,
			LimitError,
			UnexpectedLimit,
			VoltageOutOfRange,
			OverTemperature,
			StationaryDisplacement,
			NoReference,
			ManualControl,
			CommandInterrupted,
			StreamDisconinuity,
			SettingsUpdating,
			JoystickCalibrating,
			Count
		};
		enum class ReplyDataType : uint8_t 
		{
			None,
			Float,
			Integer,
			Error,
			Count
		};
		enum class CommandMessageType : uint8_t 
		{
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
			Warnings,
			Count
		};
		enum class SettingMessageType : uint8_t 
		{
			Accel,
			CLoopCounts,
			CLoopDisplaceTolerance,
			CLoopDurationMax,
			CLoopMode,
			CLoopStallTimeout,
			CLoopSteps,
			CommAddress,
			CommAlert,
			CommCheckSum,
			CommProtocol,
			CommRS232Baud,
			CommRS232Protocol,
			CommRS485Baud,
			CommRS485Enable,
			CommRS485Protocol,
			CommUSBProtocol,
			DeviceID,
			DriverCurrentHold,
			DriverCurrentMax,
			DriverCurrentRun,
			DriverDir,
			DriverTemperature,
			EncoderCount,
			EncoderCountH,
			EncoderDir,
			EncoderError,
			EncoderFilter,
			EncoderIndexCount,
			EncoderIndexPhase,
			EncoderMode,
			EncoderPos,
			JoyDebug,
			KnobDir,
			KnobDistance,
			KnobEnable,
			KnobMaxSpeed,
			KnobMode,
			KnobSpeedProfile,
			LimitApproachMaxSpeed,
			LimitCycleDist,
			LimitDetectDecelOnly,
			LimitDetectMaxSpeed,
			LimitHomeAction,
			LimitHomeEdge,
			LimitHomePos,
			LimitHomePosUpdate,
			LimitHomePreset,
			LimitHomeState,
			LimitHomeTriggered,
			LimitHomeType,
			LimitAwayAction,
			LimitAwayEdge,
			LimitAwayPos,
			LimitAwayPosUpdate,
			LimitAwayPreset,
			LimitAwayState,
			LimitAwayTriggered,
			LimitAwayType,
			LimitCAction,
			LimitCEdge,
			LimitCPos,
			LimitCPosUpdate,
			LimitCPreset,
			LimitCState,
			LimitCTriggered,
			LimitCType,
			LimitDAction,
			LimitDEdge,
			LimitDPos,
			LimitDPosUpdate,
			LimitDPreset,
			LimitDState,
			LimitDTriggered,
			LimitDType,
			LimitMax,
			LimitMin,
			LimitStartPos,
			LimitSwapInputs,
			LockstepNumgroups,
			LockstepTolerance,
			MaxSpeed,
			MotionAccelOnly,
			MotionDecelOnly,
			PeripheralID,
			Pos,
			Resolution,
			StreamNumBufs,
			StreamNumStreams,
			SystemAccess,
			SystemAxisCount,
			SystemCurrent,
			SystemLEDEnable,
			SystemSerial,
			SystemTemperature,
			SystemVoltage,
			Version,
			VersionBuild,
			VirtualNumVirtual,
			Count
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
			Count
		};
		enum class CommandParameterType : uint8_t 
		{
			None,
			Integer,
			Float,
			Parameter,
			Setting,
			Count
		};
		enum class CommandScope : uint8_t 
		{
			Device,
			Axis,
			Either,
			All,
			Count
		};
		struct CommandString
		{
			CommandMessageType Command;
			const char* String;
			size_t Count;
			CommandScope Scope;
			const CommandSentCallback SentCallback;
			const ReplyReceivedCallback ReceivedCallback;
		};
		struct ParameterMessageString
		{
			ParameterMessageType Parameter;
			const char* String;
			size_t Count;
		};
		struct SettingString
		{
			SettingMessageType Setting;
			const char* String;
			size_t Count;
			CommandScope Scope;
			bool Writeable;
		};
		union CommandParameterValue
		{
			float Float;
			int32_t Integer;
			ParameterMessageType Parameter;
			SettingMessageType Setting;
		};
		struct CommandParameter
		{
			CommandParameterValue Value;
			CommandParameterType Type;
		};
		struct CommandMessage
		{
			CommandMessageType Command;
			bool MessageComplete;
			bool MessageStarted;
			uint8_t Axis;
			uint8_t Device;
			uint32_t StartTime;
			CommandParameter Parameters[ZaberParameterMaxLength];
			uint8_t ParameterCount;
		};
		struct ReplyDataError
		{
			ReplyDataErrorType Type;
			const char* Flag;
			size_t FlagLength;
			const char* String;
			size_t StringLength;
			bool TryAgain;
			bool Important;
		};
		struct ReplyFlag
		{
			FlagType Type;
			const char* Flag;
		};
		struct ReplyStatus
		{
			StatusType Type;
			const char* Flag;
		};
		struct Warning
		{
			WarningType Type;
			const char* Flag;
			const char* String;
			size_t StringLength;
			bool NeedsClearing;
			bool Important;
		};
		struct MessageFlag
		{
			MessageType Type;
			char Identifier;
		};
		union ReplyDataPayload
		{
			float Float;
			int32_t Integer;
			ReplyDataErrorType Error;
		};
		struct ReplyData
		{
			ReplyDataType Type;
			ReplyDataPayload Value;
		};
		struct ReplyMessage
		{
			MessageType Type;
			ReplyData Data[ZaberMaxReplyData];
			FlagType Flag;
			StatusType Status;
			WarningType Warning;
			size_t DataLength;
			bool MessageComplete;
			bool MessageStarted;
			uint8_t Axis;
			uint8_t Device;
			uint32_t StartTime;
		};
		ZaberMaster(HardwareSerial* serial); //Invoke with ZaberMaster(&SerialN);
		~ZaberMaster();
		bool Initialize();
		bool SendRenumber();
		bool SendEStop(uint8_t Device, uint8_t Axis);
		bool SendStop(uint8_t Device, uint8_t Axis);
		bool SendHome(uint8_t Device, uint8_t Axis);
		bool SendSystemReset(uint8_t Device);
		bool SendSystemRestore(uint8_t Device);
		bool SendClearWarnings(uint8_t Device);
		bool SendFindRange(uint8_t Device, uint8_t Axis);
		bool SendMoveRel(uint8_t Device, uint8_t Axis, uint32_t Steps);
		bool SendMoveAbs(uint8_t Device, uint8_t Axis, uint32_t Steps);
		bool SendSetAcceleration(uint8_t Device, uint8_t Axis, uint32_t Acceleration);
		bool SendGetAcceleration(uint8_t Device, uint8_t Axis);
		bool SendSetMaxSpeed(uint8_t Device, uint8_t Axis, uint32_t MaxSpeed);
		bool SendGetMaxSpeed(uint8_t Device, uint8_t Axis);
		bool SendSetLimitMax(uint8_t Device, uint8_t Axis, uint32_t LimitMax);
		bool SendGetLimitMax(uint8_t Device, uint8_t Axis);
		bool SendSetLimitMin(uint8_t Device, uint8_t Axis, uint32_t LimitMin);
		bool SendGetLimitMin(uint8_t Device, uint8_t Axis);
		bool SendSetAlertStatus(uint8_t Device, bool Enable);
		bool SendGetAlertStatus(uint8_t Device);
		bool SendSetKnobEnable(uint8_t Device, uint8_t Axis, bool Enable);
		bool SendGetKnobEnable(uint8_t Device, uint8_t Axis);
		bool SendSetParked(uint8_t Device, uint8_t Axis, bool Enable);
		bool SendSetResolution(uint8_t Device, uint8_t Axis, uint8_t Resolution);
		bool SendGetResolution(uint8_t Device, uint8_t Axis);
		bool SendGetPosition(uint8_t Device, uint8_t Axis);
		bool SendGetAxes(uint8_t Device);
		bool GetPosition(uint8_t Device, uint8_t Axis, uint32_t* Position);
		bool GetMaxSpeed(uint8_t Device, uint8_t Axis, uint32_t* Position);
		bool GetIsBusy(uint8_t Device, uint8_t Axis, bool* IsBusy);
		void SetInitializationCompleteCallback(ZaberFinishedListener _InitializationComplete);
		void SetMovementCompleteCallback(ZaberFinishedListenerDeviceAxis _MovementComplete);
		void SetReplyCompleteCallback(ZaberFinishedListener _ReplyComplete);
		void CheckSerial();
	private:
		HardwareSerial* _HardwareSerial;
		bool Initializing;
		bool Initialized;
		uint8_t InitializationStep;
		uint8_t DevicesFound;
		uint8_t AxesFound[ZaberMaxDevices];
		CommandMessage CurrentCommand;
		char CommandBuffer[CommandBufferSize];
		uint32_t LastTransmitionTime;
		bool SendingCommand;
		bool ExpectingReply;
		bool UseAlerts;
		bool ExpectingAlert[ZaberMaxDevices][ZaberMaxAxes];
		uint32_t LastPosition[ZaberMaxDevices][ZaberMaxAxes];
		uint32_t LastMaxSpeed[ZaberMaxDevices][ZaberMaxAxes];
		StatusType LastStatus[ZaberMaxDevices][ZaberMaxAxes];
		ZaberFinishedListenerDeviceAxis MovementComplete;
		ZaberFinishedListener ReplyComplete;
		ZaberFinishedListener InitializationComplete;
		CommandSentCallback CurrentSentCallback;
		ReplyReceivedCallback CurrentReceivedCallback;
		char ReturnBuffer[ReturnBufferSize];
		uint8_t ReturnBufferPosition;
		ReplyMessage ReturnMessage;
		bool ReturnMessageDataTypeDetermined;
		ReplyParts CurrentReplyPart;
		bool RecievingReply;
		uint32_t RecieveStartTime;
		void ParseCharacterForReply(char Character);
		void ParseCharacterForAlert(char Character);
		void ParseType(char Character);
		void ParseDevice(char Character, ReplyParts NextState);
		void ParseAxis(char Character, ReplyParts NextState);
		void ParseFlag(char Character, ReplyParts NextState);
		void ParseStatus(char Character, ReplyParts NextState);
		void ParseWarning(char Character, ReplyParts NextState);
		void ParseData(char Character);
		void ClearReturnBuffer();
		void ResetReturnMessage();
		void FailToParse();
		void ProcessReplyMessage();
		void ProcessAlertMessage();
		void SendCommand();
		void ProcessGetReceived();
		void ProcessRenumberReceived();
		void ProcessMoveStarted();
		void RunNextInitializationStep();
		void SetExpectingAlerts(uint8_t Device, uint8_t Axis);
		uint8_t IntToCharPointer(uint32_t Input, char* Buffer, size_t BufferSize);
		uint8_t FloatToCharPointer(float Input, char* Buffer, size_t BufferSize);
		uint8_t CharPointerToInt(char* Buffer, size_t BufferSize);
		float CharPointerToFloat(char* Buffer, size_t BufferSize);
		static const char CarriageReturnCharacter;
		static const char EndOfLineCharacter;
		static const char SpaceCharacter;
		static const char CommandMarkerCharacter;
		static const char GetCharacter;
		static const uint8_t InitializationStepsCount = ZaberMasterInitializationStepsCount;
		static const CommandMessage InitilizationSteps[];
		static const CommandString CommandIdentifier[];
		static const ParameterMessageString ParameterIdentifier[];
		static const SettingString SettingIdentifier[];
		static const ReplyDataError ReplyDataErrorIdentifier[];
		static const MessageFlag MessageIdentifier[];
		static const ReplyStatus StatusIdentifier[];
		static const ReplyFlag FlagIdentifier[];
		static const Warning WarningIdentifier[];
};
#endif