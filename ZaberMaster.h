/*

Zaber master controller. Takes a serial bus and controls a Zaber bus.

*/

#ifndef ZaberMaster_h	//check for multiple inclusions
#define ZaberMaster_h

#include "Arduino.h"

#define ZaberMaxDataString 8
#define ZaberMaxReplyData 4
#define ZaberMaxDevices 3
#define ZaberMaxAxes 3
#define CommandBufferSize 24
#define ReturnBufferSize 64
#define ZaberParameterMaxLength 5
#define ZaberDefaultSpeed 250000
#define ZaberQueueCount 4

typedef void ( *ZaberFinishedListener )();
typedef void ( *ZaberFinishedListenerDeviceAxis )(uint8_t Device, uint8_t Axis);
typedef void ( *ZaberCommandSentCallback )();
typedef void ( *ZaberReplyReceivedCallback )();
typedef void ( *ZaberAlertReceivedCallback )(uint8_t Device, uint8_t Axis);
class ZaberMaster
{
	public:
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
			Count,
			Unknown
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
			Warning,
			Count
		};
		enum class CommandMessageType : uint8_t 
		{
			Status,
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
			Count,
			None
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
		enum class ModeType : uint8_t
		{
			Inactive,
			Idle,
			WaitForCommandReply,
			WaitForAlert,
		};
		struct CommandString
		{
			CommandMessageType Command;
			const char* String;
			size_t Count;
			CommandScope Scope;
			ZaberCommandSentCallback SentCallback;
			ZaberReplyReceivedCallback ReceivedCallback;
			ZaberAlertReceivedCallback AlertCallback;
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
			uint8_t Axis;
			uint8_t Device;
			CommandParameter Parameters[ZaberParameterMaxLength];
			uint8_t ParameterCount;
			ZaberFinishedListener Callback;
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
			WarningType Warning;
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
		enum class InitializationType : uint8_t 
		{
			LinearStandard,
			RotationalStandard,
			LinearAndRotational,
			Count
		};
		struct AxisPropertiesStruct
		{
			bool ExpectingAlert;
			StatusType Status;
			bool Polling;
			bool PollingSent;
			uint32_t LastPollTime;
			uint32_t EncoderPosition;
			uint32_t Position;
			uint32_t MaxSpeed;
			ZaberFinishedListener ExternalCallback;
			ZaberAlertReceivedCallback AlertCallback;
		};
		struct Tok
		{
			const char* p;
			size_t n;
		};
		ZaberMaster(HardwareSerial* serial); //Invoke with ZaberMaster(&SerialN);
		~ZaberMaster();
		static bool SetInitializationType(InitializationType TypeToSet);
		static bool Begin(uint8_t NumberOfDevices, uint8_t* AxisCount);
		static bool Initialize();
		static bool SendGetStatus(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback);
		static bool SendRenumber(ZaberFinishedListener ReturnCallback = NULL);
		static bool SendEStop(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendStop(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendHome(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendSystemReset(uint8_t Device, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendSystemRestore(uint8_t Device, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendClearWarnings(uint8_t Device, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendFindRange(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendMoveRel(uint8_t Device, uint8_t Axis, uint32_t Steps, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendMoveAbs(uint8_t Device, uint8_t Axis, uint32_t Steps, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendSetAcceleration(uint8_t Device, uint8_t Axis, uint32_t Acceleration, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendGetAcceleration(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendSetMaxSpeed(uint8_t Device, uint8_t Axis, uint32_t MaxSpeed, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendGetMaxSpeed(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendSetLimitMax(uint8_t Device, uint8_t Axis, uint32_t LimitMax, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendGetLimitMax(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendSetLimitMin(uint8_t Device, uint8_t Axis, uint32_t LimitMin, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendGetLimitMin(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendSetAlertStatus(uint8_t Device, bool Enable, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendGetAlertStatus(uint8_t Device, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendSetKnobEnable(uint8_t Device, uint8_t Axis, bool Enable, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendGetKnobEnable(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendSetParked(uint8_t Device, uint8_t Axis, bool Enable, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendSetResolution(uint8_t Device, uint8_t Axis, uint8_t Resolution, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendGetResolution(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendGetPosition(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendGetEncoderPosition(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback = NULL);
		static bool SendGetAxes(uint8_t Device, ZaberFinishedListener ReturnCallback = NULL);
		static bool GetPosition(uint8_t Device, uint8_t Axis, uint32_t* Position);
		static bool GetEncoderPosition(uint8_t Device, uint8_t Axis, uint32_t* EncoderPosition);
		static bool GetMaxSpeed(uint8_t Device, uint8_t Axis, uint32_t* MaxSpeed);
		static bool GetIsBusy(uint8_t Device, uint8_t Axis, StatusType* Status);
		static void SetInitializationCompleteCallback(ZaberFinishedListener _InitializationComplete);
		static void SetMovementCompleteCallback(ZaberFinishedListenerDeviceAxis _MovementComplete);
		static void SetReplyCompleteCallback(ZaberFinishedListener _ReplyComplete);
		static void SetVerbose(bool VerboseToSet);
		static void Check();
	private:
		static HardwareSerial* _StreamPort;
		static ModeType Mode;
		static bool Busy;
		static bool Initializing;
		static bool Initialized;
		static InitializationType InitializationBehavior;
		static CommandMessage* InitilizationSteps;
		static uint8_t InitializationStep;
		static uint8_t InitializationStepsCount;
		static uint8_t DevicesFound;
		static uint8_t AxesFound[ZaberMaxDevices];
		static CommandMessage CurrentCommand;
		static char CommandBuffer[CommandBufferSize];
		static AxisPropertiesStruct AxisProperties[ZaberMaxDevices][ZaberMaxAxes];
		static ZaberFinishedListenerDeviceAxis MovementComplete;
		static ZaberFinishedListener ReplyComplete;
		static ZaberFinishedListener InitializationComplete;
		static ZaberCommandSentCallback CurrentSentCallback;
		static ZaberReplyReceivedCallback CurrentReceivedCallback;
		static char ReturnBuffer[ReturnBufferSize];
		static uint8_t ReturnBufferCount;
		static ReplyMessage ReturnMessage;
		static bool ReturnMessageDataTypeDetermined;
		static ReplyParts CurrentReplyPart;
		static bool RecievingReply;
		static bool Verbose;
		static bool PollingAny;
		static bool PollingEnabled;
		static bool AlertEnabled;
		static bool ExpectingAlert;
		static uint32_t PollingTimeMax;
		static WarningType LastWarning;

		static CommandMessage CommandQueue[ZaberQueueCount];
		static CommandMessage CommandForEnqueue;
		static uint8_t CommandQueueHead;
		static uint8_t CommandQueueTail;
		static bool CommandQueueFullFlag;

		static void CheckCommandQueue();
		static void ClearCommandQueue();
		static bool CommandQueueFull();
		static bool CommandQueueEmpty();
		static uint8_t CommandQueueCount();
		static void CommandQueueAdvance();
		static void CommandQueueRetreat();
		static void CommandEnqueue();
		static bool CommandQueuePullToCurrentCommand();
		static void CheckSerial();
		static void CheckPollingStatus();
		static void CheckExpectingAlert();
		static void EnqueStatusForPolling();

		static void ClearReturnBuffer();
		static void ResetReturnMessage();
		static void FailToParse();
		static void ProcessReplyMessage();
		static void ProcessAlertMessage();
		static void SetStatus();
		static void SendCommand();
		static void RunNextInitializationStep();
		static void SetExpectingAlerts(uint8_t Device, uint8_t Axis);
		static uint8_t IntToCharPointer(uint32_t Input, char* Buffer, size_t BufferSize);
		static uint8_t FloatToCharPointer(float Input, char* Buffer, size_t BufferSize);
		static uint8_t CharPointerToInt(char* Buffer, size_t BufferSize);
		static float CharPointerToFloat(char* Buffer, size_t BufferSize);
		static void ClearCommandForEnqueue();
		static void ClearCurrentCommand();
		static void AssignAlertCallbacks(uint8_t CommandIndex);

		static void ProcessMoveEnded(uint8_t Device, uint8_t Axis);
		static void ProcessGetReceived();
		static void ProcessSetReturned();
		static void ProcessRenumberReceived();
		static void ProcessMoveStarted();

		static void ProcessReturnBuffer(char* ReturnBuffer, uint8_t BufferCount);
		static void PrintReplyMessage(ReplyMessage& msg);
		static const char* toString(ReplyDataErrorType v);
		static const char* toString(ReplyDataType v);
		static const char* toString(WarningType v);
		static const char* toString(FlagType v);
		static const char* toString(StatusType v);
		static const char* toString(MessageType v);
		static bool ParseZaberReplyLine(const char* buf, size_t len, ReplyMessage& out);
		static size_t tokenize_space(const char* s, size_t n, Tok out[], size_t outcap);
		static WarningType parse_warning(const Tok& t);
		static StatusType parse_status(const Tok& t);
		static FlagType parse_flag(const Tok& t);
		static MessageType parse_msg_type(char c);
		static float to_float(const Tok& t);
		static long to_int(const Tok& t);
		static int to_uint(const Tok& t);
		static bool tok_is_floatish(const Tok& t);
		static bool tok_is_int(const Tok& t);
		static bool tok_is_uint(const Tok& t);
		static bool tok_eq(const Tok& t, const char* s);

		static const char CarriageReturnCharacter;
		static const char EndOfLineCharacter;
		static const char SpaceCharacter;
		static const char CommandMarkerCharacter;
		static const char GetCharacter;
		static const CommandMessage LinearStandardInitilizationSteps[];
		static const uint8_t LinearStandardInitilizationStepCount;
		static const CommandMessage RotationalStandardInitilizationSteps[];
		static const uint8_t RotationalStandardInitilizationStepCount;
		static const CommandMessage LinearAndRotationalInitilizationSteps[];
		static const uint8_t LinearAndRotationalInitilizationStepCount;
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