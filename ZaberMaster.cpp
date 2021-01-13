#include "ZaberMaster.h"

const char ZaberMaster::CarriageReturnCharacter = '\r';
const char ZaberMaster::EndOfLineCharacter = '\n';
const char ZaberMaster::SpaceCharacter = ' ';
const char ZaberMaster::CommandMarkerCharacter = '/';
const char ZaberMaster::GetCharacter = '?';
const uint8_t ZaberMaster::InitializationStepsCount = 6;
const ZaberMaster::CommandMessage ZaberMaster::InitilizationSteps[]=
{
	{CommandMessageType::Warnings,0,0,{{{.Parameter=ParameterMessageType::Clear},CommandParameterType::Parameter},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},1,NULL},
	{CommandMessageType::Renumber,0,0,{{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},0,NULL},
	{CommandMessageType::Get,0,0,{{{.Setting=SettingMessageType::SystemAxisCount},CommandParameterType::Setting},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},1,NULL},
	{CommandMessageType::Set,0,0,{{{.Setting=SettingMessageType::MaxSpeed},CommandParameterType::Setting},{{.Integer = ZaberDefaultSpeed},CommandParameterType::Integer},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},2,NULL},
	{CommandMessageType::Set,0,0,{{{.Setting=SettingMessageType::CommAlert},CommandParameterType::Setting},{{.Integer = 1},CommandParameterType::Integer},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},2,NULL},
	{CommandMessageType::ToolsFindRange,0,0,{{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},0,NULL},
};
const ZaberMaster::CommandString ZaberMaster::CommandIdentifier[]=
{
	{CommandMessageType::EStop,"estop",5,CommandScope::Axis, NULL, NULL, NULL},
	{CommandMessageType::Get,"get",3,CommandScope::Either, NULL, &ZaberMaster::ProcessGetReceived, NULL },
	{CommandMessageType::Home,"home",4,CommandScope::Axis, &ZaberMaster::ProcessMoveStarted, NULL, &ZaberMaster::ProcessMoveEnded },
	{CommandMessageType::IOInfo,"io info",7,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::IOGet,"io get",6,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::IOSet,"io set",6,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::Joystick,"joystick",8,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::Key,"key",3,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::LockStep,"lockstep",8,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::Move,"move",4,CommandScope::Axis, &ZaberMaster::ProcessMoveStarted, NULL, &ZaberMaster::ProcessMoveEnded },
	{CommandMessageType::Renumber,"renumber",8,CommandScope::Device, NULL, &ZaberMaster::ProcessRenumberReceived, NULL },
	{CommandMessageType::Set,"set",3,CommandScope::Either, NULL, &ZaberMaster::ProcessSetReturned, NULL },
	{CommandMessageType::Stop,"stop",4,CommandScope::Axis, NULL, NULL, NULL },
	{CommandMessageType::Stream,"stream",6,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::SystemReset,"system reset",12,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::SystemRestore,"system restore",14,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::ToolsEcho,"tools echo",10,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::ToolsFindRange,"tools findrange",15,CommandScope::Axis, &ZaberMaster::ProcessMoveStarted, NULL, &ZaberMaster::ProcessMoveEnded },
	{CommandMessageType::ToolsGoToLimit,"tools gotolimit",15,CommandScope::Axis, &ZaberMaster::ProcessMoveStarted, NULL, &ZaberMaster::ProcessMoveEnded },
	{CommandMessageType::ToolsParking,"tools parking",13,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::ToolsSetComm,"tools setcomm",13,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::ToolsStorePos,"tools storepos",14,CommandScope::Axis, NULL, NULL, NULL },
	{CommandMessageType::Trigger,"trigger",7,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::TriggerDist,"trigger dist",12,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::TriggerTime,"trigger time",12,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::Virtual,"virtual",7,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::Warnings,"warnings",8,CommandScope::Either, NULL, NULL, NULL }
};
const ZaberMaster::ParameterMessageString ZaberMaster::ParameterIdentifier[]=
{
	{ParameterMessageType::Ao,"ao",2},
	{ParameterMessageType::Ai,"ai",2},
	{ParameterMessageType::Do,"do",2},
	{ParameterMessageType::Di,"di",2},
	{ParameterMessageType::Target,"target",6},
	{ParameterMessageType::SpeedProfile,"speedprofile",12},
	{ParameterMessageType::MaxSpeed,"maxspeed",8},
	{ParameterMessageType::Invert,"invert",6},
	{ParameterMessageType::Resolution,"resolution",10},
	{ParameterMessageType::Info,"info",4},
	{ParameterMessageType::Calibrate,"calibrate",9},
	{ParameterMessageType::Limits,"limits",6},
	{ParameterMessageType::Start,"start",5},
	{ParameterMessageType::DeadBands,"deadbands",9},
	{ParameterMessageType::Add,"add",3},
	{ParameterMessageType::Alert,"alert",5},
	{ParameterMessageType::Clear,"clear",5},
	{ParameterMessageType::Stream,"stream",6},
	{ParameterMessageType::Setup,"setup",5},
	{ParameterMessageType::Enable,"enable",6},
	{ParameterMessageType::Disable,"disable",7},
	{ParameterMessageType::Live,"live",4},
	{ParameterMessageType::Call,"call",4},
	{ParameterMessageType::Move,"move",4},
	{ParameterMessageType::Abs,"abs",3},
	{ParameterMessageType::Rel,"rel",3},
	{ParameterMessageType::Vel,"vel",3},
	{ParameterMessageType::Min,"min",3},
	{ParameterMessageType::Max,"max",3},
	{ParameterMessageType::Stored,"stored",6},
	{ParameterMessageType::Store,"store",5},
	{ParameterMessageType::Erase,"erase",5},
	{ParameterMessageType::Print,"print",5},
	{ParameterMessageType::Arc,"arc",3},
	{ParameterMessageType::Cw,"cw",2},
	{ParameterMessageType::Ccw,"ccw",3},
	{ParameterMessageType::Circle,"circle",6},
	{ParameterMessageType::Fifo,"fifo",4},
	{ParameterMessageType::Cork,"cork",4},
	{ParameterMessageType::Uncork,"uncork",6},
	{ParameterMessageType::Io,"io",2},
	{ParameterMessageType::Set,"set",3},
	{ParameterMessageType::Tanaccel,"tanaccel",8},
	{ParameterMessageType::Centripaccel,"centripaccel",12},
	{ParameterMessageType::Wait,"wait",4},
	{ParameterMessageType::Stop,"stop",4},
	{ParameterMessageType::EStop,"estop",5},
	{ParameterMessageType::Home,"home",4},
	{ParameterMessageType::Away,"away",4},
	{ParameterMessageType::A,"a",1},
	{ParameterMessageType::B,"b",1},
	{ParameterMessageType::C,"c",1},
	{ParameterMessageType::D,"d",1},
	{ParameterMessageType::Update,"update",6},
	{ParameterMessageType::Current,"current",7},
	{ParameterMessageType::When,"when",4},
	{ParameterMessageType::Action,"action",6},
	{ParameterMessageType::Equals,"equals",6},
	{ParameterMessageType::Pos,"pos",3},
	{ParameterMessageType::Park,"park",4},
	{ParameterMessageType::Unpark,"unpark",6},
	{ParameterMessageType::Angle,"angle",5},
	{ParameterMessageType::Ratio,"ratio",5}
};
const ZaberMaster::SettingString ZaberMaster::SettingIdentifier[]=
{
	{SettingMessageType::Accel,"accel",5,CommandScope::Axis,true},
	{SettingMessageType::CLoopCounts,"cloop.counts",12,CommandScope::Axis,true},
	{SettingMessageType::CLoopDisplaceTolerance,"cloop.displace.tolerance",24,CommandScope::Axis,true},
	{SettingMessageType::CLoopDurationMax,"cloop.duration.max",18,CommandScope::Axis,true},
	{SettingMessageType::CLoopMode,"cloop.mode",10,CommandScope::Axis,true},
	{SettingMessageType::CLoopStallTimeout,"cloop.stalltimeout",18,CommandScope::Axis,true},
	{SettingMessageType::CLoopSteps,"cloop.steps",11,CommandScope::Axis,true},
	{SettingMessageType::CommAddress,"comm.address",12,CommandScope::Device,true},
	{SettingMessageType::CommAlert,"comm.alert",10,CommandScope::Device,true},
	{SettingMessageType::CommCheckSum,"comm.checksum",13,CommandScope::Device,true},
	{SettingMessageType::CommProtocol,"comm.protocol",13,CommandScope::Device,true},
	{SettingMessageType::CommRS232Baud,"comm.rs232.baud",15,CommandScope::Device,true},
	{SettingMessageType::CommRS232Protocol,"comm.rs232.protocol",19,CommandScope::Device,true},
	{SettingMessageType::CommRS485Baud,"comm.rs485.baud",15,CommandScope::Device,true},
	{SettingMessageType::CommRS485Enable,"comm.rs485.enable",17,CommandScope::Device,true},
	{SettingMessageType::CommRS485Protocol,"comm.rs485.protocol",19,CommandScope::Device,true},
	{SettingMessageType::CommUSBProtocol,"comm.usb.protocol",17,CommandScope::Device,true},
	{SettingMessageType::DeviceID,"deviceid",8,CommandScope::Device,false},
	{SettingMessageType::DriverCurrentHold,"driver.current.hold",19,CommandScope::Axis,true},
	{SettingMessageType::DriverCurrentMax,"driver.current.max",18,CommandScope::Axis,false},
	{SettingMessageType::DriverCurrentRun,"driver.current.run",18,CommandScope::Axis,true},
	{SettingMessageType::DriverDir,"driver.dir",10,CommandScope::Axis,true},
	{SettingMessageType::DriverTemperature,"driver.temperature",18,CommandScope::Axis,false},
	{SettingMessageType::EncoderCount,"encoder.count",13,CommandScope::Axis,true},
	{SettingMessageType::EncoderCountH,"encoder.counth",14,CommandScope::Axis,true},
	{SettingMessageType::EncoderDir,"encoder.dir",11,CommandScope::Axis,true},
	{SettingMessageType::EncoderError,"encoder.error",13,CommandScope::Axis,false},
	{SettingMessageType::EncoderFilter,"encoder.filter",14,CommandScope::Axis,true},
	{SettingMessageType::EncoderIndexCount,"encoder.index.count",19,CommandScope::Axis,true},
	{SettingMessageType::EncoderIndexPhase,"encoder.index.phase",19,CommandScope::Axis,true},
	{SettingMessageType::EncoderMode,"encoder.index.mode",18,CommandScope::Axis,true},
	{SettingMessageType::EncoderPos,"encoder.pos",11,CommandScope::Axis,false},
	{SettingMessageType::JoyDebug,"joy.debug",9,CommandScope::Axis,true},
	{SettingMessageType::KnobDir,"knob.dir",8,CommandScope::Axis,true},
	{SettingMessageType::KnobDistance,"knob.distance",13,CommandScope::Axis,true},
	{SettingMessageType::KnobEnable,"knob.enable",11,CommandScope::Axis,true},
	{SettingMessageType::KnobMaxSpeed,"knob.maxspeed",13,CommandScope::Axis,true},
	{SettingMessageType::KnobMode,"knob.mode",9,CommandScope::Axis,true},
	{SettingMessageType::KnobSpeedProfile,"knob.speedprofile",17,CommandScope::Axis,true},
	{SettingMessageType::LimitApproachMaxSpeed,"limit.approach.maxspeed",23,CommandScope::Axis,true},
	{SettingMessageType::LimitCycleDist,"limit.cycle.dist",16,CommandScope::Axis,true},
	{SettingMessageType::LimitDetectDecelOnly,"limit.detect.decelonly",22,CommandScope::Axis,true},
	{SettingMessageType::LimitDetectMaxSpeed,"limit.detect.maxspeed",21,CommandScope::Axis,true},
	{SettingMessageType::LimitHomeAction,"limit.home.action",17,CommandScope::Axis,true},
	{SettingMessageType::LimitHomeEdge,"limit.home.edge",15,CommandScope::Axis,true},
	{SettingMessageType::LimitHomePos,"limit.home.pos",14,CommandScope::Axis,true},
	{SettingMessageType::LimitHomePosUpdate,"limit.home.posupdate",20,CommandScope::Axis,true},
	{SettingMessageType::LimitHomePreset,"limit.home.preset",17,CommandScope::Axis,true},
	{SettingMessageType::LimitHomeState,"limit.home.state",16,CommandScope::Axis,false},
	{SettingMessageType::LimitHomeTriggered,"limit.home.triggered",20,CommandScope::Axis,false},
	{SettingMessageType::LimitHomeType,"limit.home.type",15,CommandScope::Axis,true},
	{SettingMessageType::LimitAwayAction,"limit.away.action",17,CommandScope::Axis,true},
	{SettingMessageType::LimitAwayEdge,"limit.away.edge",15,CommandScope::Axis,true},
	{SettingMessageType::LimitAwayPos,"limit.away.pos",14,CommandScope::Axis,true},
	{SettingMessageType::LimitAwayPosUpdate,"limit.away.posupdate",20,CommandScope::Axis,true},
	{SettingMessageType::LimitAwayPreset,"limit.away.preset",17,CommandScope::Axis,true},
	{SettingMessageType::LimitAwayState,"limit.away.state",16,CommandScope::Axis,false},
	{SettingMessageType::LimitAwayTriggered,"limit.away.triggered",20,CommandScope::Axis,false},
	{SettingMessageType::LimitAwayType,"limit.away.type",15,CommandScope::Axis,true},
	{SettingMessageType::LimitCAction,"limit.c.action",14,CommandScope::Axis,true},
	{SettingMessageType::LimitCEdge,"limit.c.edge",12,CommandScope::Axis,true},
	{SettingMessageType::LimitCPos,"limit.c.pos",11,CommandScope::Axis,true},
	{SettingMessageType::LimitCPosUpdate,"limit.c.posupdate",17,CommandScope::Axis,true},
	{SettingMessageType::LimitCPreset,"limit.c.preset",14,CommandScope::Axis,true},
	{SettingMessageType::LimitCState,"limit.c.state",13,CommandScope::Axis,false},
	{SettingMessageType::LimitCTriggered,"limit.c.triggered",17,CommandScope::Axis,false},
	{SettingMessageType::LimitCType,"limit.c.type",12,CommandScope::Axis,true},
	{SettingMessageType::LimitDAction,"limit.d.action",14,CommandScope::Axis,true},
	{SettingMessageType::LimitDEdge,"limit.d.edge",12,CommandScope::Axis,true},
	{SettingMessageType::LimitDPos,"limit.d.pos",11,CommandScope::Axis,true},
	{SettingMessageType::LimitDPosUpdate,"limit.d.posupdate",17,CommandScope::Axis,true},
	{SettingMessageType::LimitDPreset,"limit.d.preset",14,CommandScope::Axis,true},
	{SettingMessageType::LimitDState,"limit.d.state",13,CommandScope::Axis,false},
	{SettingMessageType::LimitDTriggered,"limit.d.triggered",17,CommandScope::Axis,false},
	{SettingMessageType::LimitDType,"limit.d.type",12,CommandScope::Axis,true},
	{SettingMessageType::LimitMax,"limit.max",9,CommandScope::Axis,true},
	{SettingMessageType::LimitMin,"limit.min",9,CommandScope::Axis,true},
	{SettingMessageType::LimitStartPos,"limit.start.pos",15,CommandScope::Axis,true},
	{SettingMessageType::LimitSwapInputs,"limit.swapinputs",16,CommandScope::Axis,true},
	{SettingMessageType::LockstepNumgroups,"lockstep.numgroups",18,CommandScope::Axis,false},
	{SettingMessageType::LockstepTolerance,"lockstep.tolerance",18,CommandScope::Axis,true},
	{SettingMessageType::MaxSpeed,"maxspeed",8,CommandScope::Axis,true},
	{SettingMessageType::MotionAccelOnly,"motion.accelonly",16,CommandScope::Axis,true},
	{SettingMessageType::MotionDecelOnly,"motion.decelonly",16,CommandScope::Axis,true},
	{SettingMessageType::PeripheralID,"peripheralid",12,CommandScope::Axis,true},
	{SettingMessageType::Pos,"pos",3,CommandScope::Axis,true},
	{SettingMessageType::Resolution,"resolution",10,CommandScope::Axis,true},
	{SettingMessageType::StreamNumBufs,"stream.numbufs",14,CommandScope::Device,false},
	{SettingMessageType::StreamNumStreams,"stream.numstreams",17,CommandScope::Device,false},
	{SettingMessageType::SystemAccess,"system.access",13,CommandScope::Device,true},
	{SettingMessageType::SystemAxisCount,"system.axiscount",16,CommandScope::Device,false},
	{SettingMessageType::SystemCurrent,"system.current",14,CommandScope::Device,false},
	{SettingMessageType::SystemLEDEnable,"system.led.enable",17,CommandScope::Device,true},
	{SettingMessageType::SystemSerial,"system.serial",13,CommandScope::Device,false},
	{SettingMessageType::SystemTemperature,"system.temperature",18,CommandScope::Device,false},
	{SettingMessageType::SystemVoltage,"system.voltage",14,CommandScope::Device,false},
	{SettingMessageType::Version,"version",7,CommandScope::Device,false},
	{SettingMessageType::VersionBuild,"version.build",13,CommandScope::Device,false},
	{SettingMessageType::VirtualNumVirtual,"virtual.numvirtual",18,CommandScope::Device,false},
};
const ZaberMaster::ReplyDataError ZaberMaster::ReplyDataErrorIdentifier[]=
{
	{ReplyDataErrorType::None, "0", 1, "", 0, false, false},
	{ReplyDataErrorType::Again, "AGAIN", 5, "Send again", 10, true, true},
	{ReplyDataErrorType::BadAxis, "BADAXIS", 7, "Bad axis", 8, false, true},
	{ReplyDataErrorType::BadData, "BADDATA", 7, "Bad data", 8, false, true},
	{ReplyDataErrorType::BadMessageID, "BADMESSAGEID", 12, "Bad message ID", 14, false, false},
	{ReplyDataErrorType::DeviceOnly, "DEVICEONLY", 10, "Device only", 11, false, true},
	{ReplyDataErrorType::Full, "FULL", 4, "Buffer full", 11, false, true},
	{ReplyDataErrorType::LockStep, "LOCKSTEP", 8, "Axis is lock stepped", 20, false, true},
	{ReplyDataErrorType::NoAccess, "NOACCESS", 8, "Command not accessible", 22, false, true},
	{ReplyDataErrorType::Parked, "PARKED", 6, "Device is parked", 16, false, true},
	{ReplyDataErrorType::StatusBusy, "STATUSBUSY", 10, "Device busy", 11, true, true}
};
const ZaberMaster::MessageFlag ZaberMaster::MessageIdentifier[]=
{
	{MessageType::Reply, '@'},
	{MessageType::Info, '#'},
	{MessageType::Alert, '!'}
};
const ZaberMaster::ReplyStatus ZaberMaster::StatusIdentifier[]=
{
	{StatusType::Idle, "IDLE"},
	{StatusType::Busy, "BUSY"}
};
const ZaberMaster::ReplyFlag ZaberMaster::FlagIdentifier[]=
{
	{FlagType::Okay, "OK"},
	{FlagType::Rejected, "RJ"}
};
const ZaberMaster::Warning ZaberMaster::WarningIdentifier[]=
{
	{WarningType::None, "--", "No warnings", 11, false, false},
	{WarningType::DriverDisabled, "FD", "Driver disabled", 15, false, true},
	{WarningType::EncoderError, "FQ", "Encoder error", 13, true, false},
	{WarningType::StalledAndStopped, "FS", "Stalled and stopped", 19, true, true},
	{WarningType::ExcessiveTwist, "FT", "Excessive twist", 15, true, true},
	{WarningType::StreamBoundErrors, "FB", "Stream bound error", 18, true, true},
	{WarningType::InterpolationDeviation, "FP", "Interpolatated path deviation", 29, true, false},
	{WarningType::LimitError, "FE", "Limit error", 11, true, false},
	{WarningType::UnexpectedLimit, "WL", "Unexpected limit trigger", 24, true, false},
	{WarningType::VoltageOutOfRange, "WV", "Voltage out of range", 20, false, false},
	{WarningType::OverTemperature, "WT", "System temperature high", 23, false, false},
	{WarningType::StationaryDisplacement, "WM", "Displaced when stationary", 25, false, false},
	{WarningType::NoReference, "WR", "No reference position", 21, false, true},
	{WarningType::ManualControl, "NC", "Manual control", 14, false, false},
	{WarningType::CommandInterrupted, "NI", "Command interrupted", 19, false, false},
	{WarningType::StreamDisconinuity, "ND", "Stream disconinuity", 19, false, false},
	{WarningType::SettingsUpdating, "NU", "Settings updating", 17, false, false},
	{WarningType::JoystickCalibrating, "NJ", "Joystick calibrating", 20, false, false}
};

HardwareSerial* ZaberMaster::_HardwareSerial;
ZaberMaster::ModeType ZaberMaster::Mode;
bool ZaberMaster::Busy;
bool ZaberMaster::Initializing;
bool ZaberMaster::Initialized;
uint8_t ZaberMaster::InitializationStep;
uint8_t ZaberMaster::DevicesFound;
uint8_t ZaberMaster::AxesFound[ZaberMaxDevices];
ZaberMaster::CommandMessage ZaberMaster::CurrentCommand;
char ZaberMaster::CommandBuffer[CommandBufferSize];
char ZaberMaster::ReturnBuffer[ReturnBufferSize];
char ZaberMaster::VerboseBuffer[ReturnBufferSize];
uint8_t ZaberMaster::VerboseBufferIndex;
uint8_t ZaberMaster::ReturnBufferPosition;
ZaberMaster::ReplyMessage ZaberMaster::ReturnMessage;
bool ZaberMaster::ReturnMessageDataTypeDetermined;
ZaberMaster::ReplyParts ZaberMaster::CurrentReplyPart;
bool ZaberMaster::RecievingReply;
bool ZaberMaster::Verbose;
ZaberMaster::CommandMessage ZaberMaster::CommandQueue[ZaberQueueCount];
ZaberMaster::CommandMessage ZaberMaster::CommandForEnqueue;
uint8_t ZaberMaster::CommandQueueHead;
uint8_t ZaberMaster::CommandQueueTail;
bool ZaberMaster::CommandQueueFullFlag;
ZaberMaster::AxisPropertiesStruct ZaberMaster::AxisProperties[ZaberMaxDevices][ZaberMaxAxes];

ZaberFinishedListenerDeviceAxis ZaberMaster::MovementComplete;
ZaberFinishedListener ZaberMaster::ReplyComplete;
ZaberFinishedListener ZaberMaster::InitializationComplete;
ZaberCommandSentCallback ZaberMaster::CurrentSentCallback;
ZaberReplyReceivedCallback ZaberMaster::CurrentReceivedCallback;

ZaberMaster::ZaberMaster(HardwareSerial* serial)
{
	_HardwareSerial = serial;
	Initialized = false;
	InitializationStep = 0;
	DevicesFound = 0;
	Mode = ModeType::Inactive;
	Busy = false;
	memset(AxesFound,0,sizeof(AxesFound));
	ClearCurrentCommand();
	ClearCommandForEnqueue();
	ClearCommandQueue();
	memset(CommandBuffer,0,sizeof(CommandBuffer));
	//memset(ExpectingAlert,0,sizeof(ExpectingAlert));
	InitializationComplete = NULL;
	MovementComplete = NULL;
	ReplyComplete = NULL;
	memset(ReturnBuffer,0,sizeof(ReturnBuffer));
	ReturnBufferPosition = 0;
	ReturnMessage.Type = MessageType::Reply;
	for (uint8_t Index = 0; Index < ZaberMaxReplyData; Index++)
	{
		ReturnMessage.Data[Index].Type = ReplyDataType::None;
		ReturnMessage.Data[Index].Value.Integer = 0;
	}
	for (size_t DeviceIndex = 0; DeviceIndex < ZaberMaxDevices; DeviceIndex++)
	{
		for (size_t AxisIndex = 0; AxisIndex < ZaberMaxAxes; AxisIndex++)
		{
			AxisProperties[DeviceIndex][AxisIndex].ExpectingAlert = false;
			AxisProperties[DeviceIndex][AxisIndex].Position = 0;
			AxisProperties[DeviceIndex][AxisIndex].MaxSpeed = 0;
			AxisProperties[DeviceIndex][AxisIndex].Status = StatusType::Unknown;
			AxisProperties[DeviceIndex][AxisIndex].ExternalCallback = NULL;
			AxisProperties[DeviceIndex][AxisIndex].AlertCallback = NULL;
		}
	}
	ReturnMessage.Flag = FlagType::Okay;
	ReturnMessage.Status = StatusType::Idle;
	ReturnMessage.Warning = WarningType::None;
	ReturnMessage.DataLength = 0;
	ReturnMessage.Axis = 0;
	ReturnMessage.Device = 0;
	ReturnMessageDataTypeDetermined = false;
	CurrentReplyPart = ReplyParts::Type;
	RecievingReply = false;
	Verbose = false;
}
ZaberMaster::~ZaberMaster()
{

}
void ZaberMaster::SetVerbose(bool VerboseToSet)
{
	Verbose = VerboseToSet;
}
bool ZaberMaster::Initialize()
{
	Initializing = true;
	InitializationStep = 0;
	Initialized = false;
	DevicesFound = 0;
	for (size_t Index = 0; Index < ZaberMaxDevices; Index++)
	{
		AxesFound[Index] = 0;
	}
	Mode = ModeType::Idle;
	RunNextInitializationStep();
	return true;
}
bool ZaberMaster::SendRenumber(ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Renumber;
	CommandForEnqueue.Axis = 0;
	CommandForEnqueue.Device = 0;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.ParameterCount = 0;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendEStop(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::EStop;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.ParameterCount = 0;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendStop(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::EStop;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.ParameterCount = 0;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendHome(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Home;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.ParameterCount = 0;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendSystemReset(uint8_t Device, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::SystemReset;
	CommandForEnqueue.Axis = 0;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.ParameterCount = 0;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendSystemRestore(uint8_t Device, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::SystemRestore;
	CommandForEnqueue.Axis = 0;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.ParameterCount = 0;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendClearWarnings(uint8_t Device, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Warnings;
	CommandForEnqueue.Axis = 0;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Parameter;
	CommandForEnqueue.Parameters[0].Value.Parameter = ParameterMessageType::Clear;
	CommandForEnqueue.ParameterCount = 1;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendFindRange(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::ToolsFindRange;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.ParameterCount = 0;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendMoveRel(uint8_t Device, uint8_t Axis, uint32_t Steps, ZaberFinishedListener ReturnCallback)
{
	if ( (Device > 0) && (Axis > 0) )
	{
		AxisProperties[Device - 1][Axis - 1].ExternalCallback = ReturnCallback;
	}
	else
	{
		CommandForEnqueue.Callback = ReturnCallback;
	}
	CommandForEnqueue.Command = CommandMessageType::Move;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Parameter;
	CommandForEnqueue.Parameters[0].Value.Parameter = ParameterMessageType::Rel;
	CommandForEnqueue.Parameters[1].Type = CommandParameterType::Integer;
	CommandForEnqueue.Parameters[1].Value.Integer = Steps;
	CommandForEnqueue.ParameterCount = 2;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendMoveAbs(uint8_t Device, uint8_t Axis, uint32_t Steps, ZaberFinishedListener ReturnCallback)
{
	if ( (Device > 0) && (Axis > 0) )
	{
		AxisProperties[Device - 1][Axis - 1].ExternalCallback = ReturnCallback;
	}
	else
	{
		CommandForEnqueue.Callback = ReturnCallback;
	}
	CommandForEnqueue.Command = CommandMessageType::Move;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Parameter;
	CommandForEnqueue.Parameters[0].Value.Parameter = ParameterMessageType::Abs;
	CommandForEnqueue.Parameters[1].Type = CommandParameterType::Integer;
	CommandForEnqueue.Parameters[1].Value.Integer = Steps;
	CommandForEnqueue.ParameterCount = 2;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendSetAcceleration(uint8_t Device, uint8_t Axis, uint32_t Acceleration, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Set;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Setting;
	CommandForEnqueue.Parameters[0].Value.Setting = SettingMessageType::Accel;
	CommandForEnqueue.Parameters[1].Type = CommandParameterType::Integer;
	CommandForEnqueue.Parameters[1].Value.Integer = Acceleration;
	CommandForEnqueue.ParameterCount = 2;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendGetAcceleration(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Get;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Setting;
	CommandForEnqueue.Parameters[0].Value.Setting = SettingMessageType::Accel;
	CommandForEnqueue.ParameterCount = 1;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendSetMaxSpeed(uint8_t Device, uint8_t Axis, uint32_t MaxSpeed, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Set;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Setting;
	CommandForEnqueue.Parameters[0].Value.Setting = SettingMessageType::MaxSpeed;
	CommandForEnqueue.Parameters[1].Type = CommandParameterType::Integer;
	CommandForEnqueue.Parameters[1].Value.Integer = MaxSpeed;
	CommandForEnqueue.ParameterCount = 2;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendGetMaxSpeed(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Get;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Setting;
	CommandForEnqueue.Parameters[0].Value.Setting = SettingMessageType::MaxSpeed;
	CommandForEnqueue.ParameterCount = 1;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendSetLimitMax(uint8_t Device, uint8_t Axis, uint32_t LimitMax, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Set;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Setting;
	CommandForEnqueue.Parameters[0].Value.Setting = SettingMessageType::LimitMax;
	CommandForEnqueue.Parameters[1].Type = CommandParameterType::Integer;
	CommandForEnqueue.Parameters[1].Value.Integer = LimitMax;
	CommandForEnqueue.ParameterCount = 2;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendGetLimitMax(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Get;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Setting;
	CommandForEnqueue.Parameters[0].Value.Setting = SettingMessageType::LimitMax;
	CommandForEnqueue.ParameterCount = 1;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendSetLimitMin(uint8_t Device, uint8_t Axis, uint32_t LimitMin, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Set;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Setting;
	CommandForEnqueue.Parameters[0].Value.Setting = SettingMessageType::LimitMin;
	CommandForEnqueue.Parameters[1].Type = CommandParameterType::Integer;
	CommandForEnqueue.Parameters[1].Value.Integer = LimitMin;
	CommandForEnqueue.ParameterCount = 2;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendGetLimitMin(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Get;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Setting;
	CommandForEnqueue.Parameters[0].Value.Setting = SettingMessageType::LimitMin;
	CommandForEnqueue.ParameterCount = 1;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendSetAlertStatus(uint8_t Device, bool Enable, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Set;
	CommandForEnqueue.Axis = 0;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Setting;
	CommandForEnqueue.Parameters[0].Value.Setting = SettingMessageType::CommAlert;
	CommandForEnqueue.Parameters[1].Type = CommandParameterType::Integer;
	CommandForEnqueue.Parameters[1].Value.Integer = (Enable) ? 1 : 0;
	CommandForEnqueue.ParameterCount = 2;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendGetAlertStatus(uint8_t Device, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Get;
	CommandForEnqueue.Axis = 0;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Setting;
	CommandForEnqueue.Parameters[0].Value.Setting = SettingMessageType::CommAlert;
	CommandForEnqueue.ParameterCount = 1;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendSetKnobEnable(uint8_t Device, uint8_t Axis, bool Enable, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Set;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Setting;
	CommandForEnqueue.Parameters[0].Value.Setting = SettingMessageType::KnobEnable;
	CommandForEnqueue.Parameters[1].Type = CommandParameterType::Integer;
	CommandForEnqueue.Parameters[1].Value.Integer = (Enable) ? 1 : 0;
	CommandForEnqueue.ParameterCount = 2;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendGetKnobEnable(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Get;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Setting;
	CommandForEnqueue.Parameters[0].Value.Setting = SettingMessageType::KnobEnable;
	CommandForEnqueue.ParameterCount = 1;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendSetParked(uint8_t Device, uint8_t Axis, bool Enable, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::ToolsParking;
	CommandForEnqueue.Axis = 0;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Parameter;
	CommandForEnqueue.Parameters[0].Value.Parameter = (Enable) ? ParameterMessageType::Park : ParameterMessageType::Unpark;
	CommandForEnqueue.ParameterCount = 1;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendSetResolution(uint8_t Device, uint8_t Axis, uint8_t Resolution, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Set;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Setting;
	CommandForEnqueue.Parameters[0].Value.Setting = SettingMessageType::Resolution;
	CommandForEnqueue.Parameters[1].Type = CommandParameterType::Integer;
	CommandForEnqueue.Parameters[1].Value.Integer = Resolution;
	CommandForEnqueue.ParameterCount = 2;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendGetResolution(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Get;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Setting;
	CommandForEnqueue.Parameters[0].Value.Setting = SettingMessageType::Resolution;
	CommandForEnqueue.ParameterCount = 1;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::SendGetPosition(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Get;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Setting;
	CommandForEnqueue.Parameters[0].Value.Setting = SettingMessageType::Pos;
	CommandForEnqueue.ParameterCount = 1;
	CommandEnqueue();
	return true;
}
bool ZaberMaster::GetPosition(uint8_t Device, uint8_t Axis, uint32_t* Position)
{
	if (Device <= ZaberMaxDevices && Axis <= ZaberMaxAxes)
	{
		if (Device <= DevicesFound)
		{
			if (Axis <= AxesFound[Device-1])
			{
				*Position = AxisProperties[Device-1][Axis-1].Position;
				return true;
			}
		}
	}
	return false;
}
bool ZaberMaster::GetMaxSpeed(uint8_t Device, uint8_t Axis, uint32_t* MaxSpeed)
{
	if (Device <= ZaberMaxDevices && Axis <= ZaberMaxAxes)
	{
		if (Device <= DevicesFound)
		{
			if (Axis <= AxesFound[Device-1])
			{
				*MaxSpeed = AxisProperties[Device-1][Axis-1].MaxSpeed;
				return true;
			}
		}
	}
	return false;
}
bool ZaberMaster::GetIsBusy(uint8_t Device, uint8_t Axis, StatusType* Status)
{
	if ( (Device <= ZaberMaxDevices) && (Axis <= ZaberMaxAxes) )
	{
		if (Device <= DevicesFound)
		{
			if (Axis <= AxesFound[Device-1])
			{
				//Serial.print( StatusIdentifier[static_cast<uint8_t>(LastStatus[Device-1][Axis-1])].Flag );
				//Serial.print("\n");
				*Status = AxisProperties[Device-1][Axis-1].Status;
				return true;
			}
		}
	}
	return false;
}
void ZaberMaster::SetInitializationCompleteCallback(ZaberFinishedListener _InitializationComplete)
{
	InitializationComplete = _InitializationComplete;
}
void ZaberMaster::SetMovementCompleteCallback(ZaberFinishedListenerDeviceAxis _MovementComplete)
{
	MovementComplete = _MovementComplete;
}
void ZaberMaster::SetReplyCompleteCallback(ZaberFinishedListener _ReplyComplete)
{
	ReplyComplete = _ReplyComplete;
}
void ZaberMaster::ClearCurrentCommand()
{
	CurrentCommand.Command = CommandMessageType::None;
	CurrentCommand.Axis = 0;
	CurrentCommand.Device = 0;
	CurrentCommand.ParameterCount = 0;
	CurrentCommand.Callback = NULL;
	for (uint8_t Index = 0; Index < ZaberParameterMaxLength; Index++)
	{
		CurrentCommand.Parameters[Index].Type = CommandParameterType::None;
		CurrentCommand.Parameters[Index].Value.Integer = 0;
	}
}
void ZaberMaster::ClearCommandForEnqueue()
{
	CommandForEnqueue.Command = CommandMessageType::None;
	CommandForEnqueue.Axis = 0;
	CommandForEnqueue.Device = 0;
	CommandForEnqueue.ParameterCount = 0;
	CommandForEnqueue.Callback = NULL;
	for (uint8_t Index = 0; Index < ZaberParameterMaxLength; Index++)
	{
		CommandForEnqueue.Parameters[Index].Type = CommandParameterType::None;
		CommandForEnqueue.Parameters[Index].Value.Integer = 0;
	}
}
void ZaberMaster::ClearCommandQueue()
{
	for (uint8_t QueueIndex = 0; QueueIndex < ZaberQueueCount; QueueIndex++)
	{
		CommandQueue[QueueIndex].Command = CommandMessageType::None;
		CommandQueue[QueueIndex].Axis = 0;
		CommandQueue[QueueIndex].Device = 0;
		CommandQueue[QueueIndex].ParameterCount = 0;
		CommandQueue[QueueIndex].Callback = NULL;
		for (uint8_t Index = 0; Index < ZaberParameterMaxLength; Index++)
		{
			CommandQueue[QueueIndex].Parameters[Index].Type = CommandParameterType::None;
			CommandQueue[QueueIndex].Parameters[Index].Value.Integer = 0;
		}
	}
}
uint8_t ZaberMaster::IntToCharPointer(uint32_t Input, char* Buffer, size_t BufferSize)
{
	memset(Buffer,0,BufferSize);
	return sprintf(Buffer, "%lu", Input);
}
uint8_t ZaberMaster::FloatToCharPointer(float Input, char* Buffer, size_t BufferSize)
{
	memset(Buffer,0,BufferSize);
	return sprintf(Buffer, "%1.3f", Input);
}
uint8_t ZaberMaster::CharPointerToInt(char* Buffer, size_t BufferSize)
{
	Buffer[BufferSize-1] = '\0';
	return (uint8_t) atoi(Buffer);
}
float ZaberMaster::CharPointerToFloat(char* Buffer, size_t BufferSize)
{
	Buffer[BufferSize-1] = '\0';
	float sign = 1.0;
	float value;
	while (isblank(*Buffer) )
	{
		Buffer++;
	}
	if (*Buffer == '-')
	{
		sign = -1.0;
		Buffer++;
	}
	for (value = 0.0; isdigit(*Buffer); Buffer++)
	{
		value = value * 10.0 + (*Buffer - '0');
	}
	if (*Buffer == '.')
	{
		double pow10 = 10.0;
		Buffer++;
		while (isdigit(*Buffer))
		{
			value += (*Buffer - '0') / pow10;
			pow10 *= 10.0;
			Buffer++;
		}
	}
	return sign * value;
}

void ZaberMaster::Check()
{
	switch (Mode)
	{
		case ModeType::Idle:
			CheckCommandQueue();
			break;
		case ModeType::WaitForCommandReply:
			CheckSerial();
			break;
		case ModeType::WaitForAlert:
			CheckSerial();
			CheckCommandQueue();
			break;
		default:
			break;
	}
	if ( Busy && (Mode == ModeType::Idle) && CommandQueueEmpty() )
	{
		Busy = false;
	}
}
void ZaberMaster::CheckCommandQueue()
{
	bool NewCommandPulled = CommandQueuePullToCurrentCommand();
	if (NewCommandPulled)
	{
		Busy = true;
		SendCommand();
	}
}
bool ZaberMaster::CommandQueuePullToCurrentCommand()
{
	bool Status = false;
	if (!CommandQueueEmpty())
	{
		ClearCurrentCommand();
		CurrentCommand.Command = CommandQueue[CommandQueueTail].Command;
		CurrentCommand.Axis = CommandQueue[CommandQueueTail].Axis;
		CurrentCommand.Device = CommandQueue[CommandQueueTail].Device;
		CurrentCommand.ParameterCount = CommandQueue[CommandQueueTail].ParameterCount;
		for (uint8_t Index = 0; Index < CurrentCommand.ParameterCount; ++Index)
		{
			CurrentCommand.Parameters[Index].Type = CommandQueue[CommandQueueTail].Parameters[Index].Type;
			CurrentCommand.Parameters[Index].Value = CommandQueue[CommandQueueTail].Parameters[Index].Value;
		}
		CurrentCommand.Callback = CommandQueue[CommandQueueTail].Callback;
		CommandQueueRetreat();
		Status = true;
		if (Verbose)
		{
			Serial.print("<ZABER>(Dequeue ");
			Serial.print( CommandIdentifier[static_cast<uint8_t>(CurrentCommand.Command)].String );
			Serial.print(",");
			Serial.print(CurrentCommand.Device);
			Serial.print(",");
			Serial.print(CurrentCommand.Axis);
			Serial.print(",");
			Serial.print(CurrentCommand.ParameterCount);
			Serial.print(",");
			Serial.print( ( (CurrentCommand.Callback != NULL) ? 'Y' : 'N' ) );
			Serial.print(")\n");
		}
	}
	return Status;
}
bool ZaberMaster::CommandQueueFull()
{
	return CommandQueueFullFlag;
}
bool ZaberMaster::CommandQueueEmpty()
{
	return ( !CommandQueueFullFlag && (CommandQueueHead == CommandQueueTail) );
}
uint8_t ZaberMaster::CommandQueueCount()
{
	uint8_t Count = ZaberQueueCount;
	if(!CommandQueueFullFlag)
	{
		if(CommandQueueHead >= CommandQueueTail)
		{
			Count = (CommandQueueHead - CommandQueueTail);
		}
		else
		{
			Count = (ZaberQueueCount + CommandQueueHead - CommandQueueTail);
		}
	}
	return Count;
}
void ZaberMaster::CommandQueueAdvance()
{
	if(CommandQueueFullFlag)
	{
		CommandQueueTail = (CommandQueueTail + 1) % ZaberQueueCount;
	}
	CommandQueueHead = (CommandQueueHead + 1) % ZaberQueueCount;
	CommandQueueFullFlag = (CommandQueueHead == CommandQueueTail);
}
void ZaberMaster::CommandQueueRetreat()
{
	CommandQueueFullFlag = false;
	CommandQueueTail = (CommandQueueTail + 1) % ZaberQueueCount;
}
void ZaberMaster::CommandEnqueue()
{
	CommandQueue[CommandQueueHead].Command = CommandForEnqueue.Command;
	CommandQueue[CommandQueueHead].Axis = CommandForEnqueue.Axis;
	CommandQueue[CommandQueueHead].Device = CommandForEnqueue.Device;
	CommandQueue[CommandQueueHead].ParameterCount = CommandForEnqueue.ParameterCount;
	for (uint8_t Index = 0; Index < CommandForEnqueue.ParameterCount; ++Index)
	{
		CommandQueue[CommandQueueHead].Parameters[Index].Type = CommandForEnqueue.Parameters[Index].Type;
		CommandQueue[CommandQueueHead].Parameters[Index].Value = CommandForEnqueue.Parameters[Index].Value;
	}
	CommandQueue[CommandQueueHead].Callback = CommandForEnqueue.Callback;
	if (Verbose)
	{
		Serial.print("<ZABER>(Enqueue ");
		Serial.print( CommandIdentifier[static_cast<uint8_t>(CommandForEnqueue.Command)].String );
		Serial.print(",");
		Serial.print(CommandForEnqueue.Device);
		Serial.print(",");
		Serial.print(CommandForEnqueue.Axis);
		Serial.print(",");
		Serial.print(CommandForEnqueue.ParameterCount);
		Serial.print(",");
		Serial.print( ( (CommandForEnqueue.Callback != NULL) ? 'Y' : 'N' ) );
		Serial.print(")\n");
	}
	CommandQueueAdvance();
}

void ZaberMaster::CheckSerial()
{
	if (_HardwareSerial->available() > 0)
	{
		char Character = _HardwareSerial->read();
		if (Verbose && (Character != EndOfLineCharacter) & (Character != CarriageReturnCharacter) )
		{
			VerboseBuffer[VerboseBufferIndex] = Character;
			VerboseBufferIndex++;
		}
		if (CurrentReplyPart == ReplyParts::Type)
		{
			ParseCharacterForReply(Character);
		}
		else
		{
			if (ReturnMessage.Type == MessageType::Info)
			{
				if (Character == EndOfLineCharacter)
				{
					FailToParse();
				}
			}
			else if (ReturnMessage.Type == MessageType::Alert)
			{
				ParseCharacterForAlert(Character);
			}
			else
			{
				ParseCharacterForReply(Character);
			}
		}
	}
}
void ZaberMaster::ParseCharacterForAlert(char Character)
{
	switch(CurrentReplyPart)
	{
		case ReplyParts::Device:
			ParseDevice(Character, ReplyParts::Axis);
			break;
		case ReplyParts::Axis:
			ParseAxis(Character, ReplyParts::Status);
			break;
		case ReplyParts::Status:
			ParseStatus(Character, ReplyParts::Warning);
			break;
		case ReplyParts::Warning:
			ParseWarning(Character, ReplyParts::Type);
			break;
		default:
			break;
	}
}
void ZaberMaster::ParseCharacterForReply(char Character)
{
	switch(CurrentReplyPart)
	{
		case ReplyParts::Type:
			ParseType(Character);
			break;
		case ReplyParts::Device:
			ParseDevice(Character, ReplyParts::Axis);
			break;
		case ReplyParts::Axis:
			ParseAxis(Character, ReplyParts::Flag);
			break;
		case ReplyParts::Flag:
			ParseFlag(Character, ReplyParts::Status);
			break;
		case ReplyParts::Status:
			ParseStatus(Character, ReplyParts::Warning);
			break;
		case ReplyParts::Warning:
			ParseWarning(Character, ReplyParts::Data);
			break;
		case ReplyParts::Data:
			ParseData(Character);
			break;
		default:
			break;
	}
}
void ZaberMaster::ParseType(char Character)
{
	bool Found = false;
	for (uint8_t Index = 0; Index < static_cast<uint8_t>(MessageType::Count); Index++)
	{
		if (Character == MessageIdentifier[Index].Identifier)
		{
			ReturnMessage.Type = MessageIdentifier[Index].Type;
			Found = true;
			break;
		}
	}
	if (Found)
	{
		ClearReturnBuffer();
		CurrentReplyPart = ReplyParts::Device;
	}
}
void ZaberMaster::ParseDevice(char Character, ReplyParts NextState)
{
	if (isdigit(Character))
	{
		ReturnBuffer[ReturnBufferPosition] = Character;
		if (ReturnBufferPosition < ReturnBufferSize)
		{
			ReturnBufferPosition++;
		}
	}
	else if (Character == SpaceCharacter)
	{
		uint8_t DeviceNumber = CharPointerToInt(ReturnBuffer, ReturnBufferSize);
		if ( (DeviceNumber > 0) && (DeviceNumber < 100) )
		{
			ReturnMessage.Device = DeviceNumber;
			ClearReturnBuffer();
			CurrentReplyPart = NextState;
		}
		else
		{
			FailToParse();
		}
	}
	else
	{
		FailToParse();
	}
}
void ZaberMaster::ParseAxis(char Character, ReplyParts NextState)
{
	if (isdigit(Character))
	{
		ReturnBuffer[ReturnBufferPosition] = Character;
		if (ReturnBufferPosition < ReturnBufferSize)
		{
			ReturnBufferPosition++;
		}
	}
	else if (Character == SpaceCharacter)
	{
		uint8_t AxisNumber = CharPointerToInt(ReturnBuffer, ReturnBufferSize);
		if ( AxisNumber < 4 )
		{
			ReturnMessage.Axis = AxisNumber;
			ClearReturnBuffer();
			CurrentReplyPart = NextState;
		}
		else
		{
			FailToParse();
		}
	}
	else
	{
		FailToParse();
	}
}
void ZaberMaster::ParseFlag(char Character, ReplyParts NextState)
{
	if (isalpha(Character))
	{
		ReturnBuffer[ReturnBufferPosition] = Character;
		if (ReturnBufferPosition < ReturnBufferSize)
		{
			ReturnBufferPosition++;
		}
	}
	else if (Character == SpaceCharacter)
	{
		ReturnBuffer[ReturnBufferPosition] = '\0';
		bool FlagFound = false;
		for (uint8_t Index = 0; Index < static_cast<uint8_t>(FlagType::Count); Index++)
		{
			if ( strcmp(ReturnBuffer, FlagIdentifier[Index].Flag) == 0)
			{
				ReturnMessage.Flag = FlagIdentifier[Index].Type;
				ClearReturnBuffer();
				CurrentReplyPart = NextState;
				FlagFound = true;
			}
		}
		if (!FlagFound)
		{
			FailToParse();
		}
	}
	else
	{
		FailToParse();
	}
}
void ZaberMaster::ParseStatus(char Character, ReplyParts NextState)
{
	if (isalpha(Character))
	{
		ReturnBuffer[ReturnBufferPosition] = Character;
		if (ReturnBufferPosition < ReturnBufferSize)
		{
			ReturnBufferPosition++;
		}
	}
	else if (Character == SpaceCharacter)
	{
		ReturnBuffer[ReturnBufferPosition] = '\0';
		bool StatusFound = false;
		for (uint8_t Index = 0; Index < static_cast<uint8_t>(StatusType::Count); Index++)
		{
			if ( strcmp(ReturnBuffer, StatusIdentifier[Index].Flag) == 0)
			{
				ReturnMessage.Status = StatusIdentifier[Index].Type;
				ClearReturnBuffer();
				CurrentReplyPart = NextState;
				StatusFound = true;
				break;
			}
		}
		if (!StatusFound)
		{
			FailToParse();
		}
	}
	else
	{
		FailToParse();
	}
}
void ZaberMaster::ParseWarning(char Character, ReplyParts NextState)
{
	if (Character == '-' || isalpha(Character))
	{
		ReturnBuffer[ReturnBufferPosition] = Character;
		if (ReturnBufferPosition < ReturnBufferSize)
		{
			ReturnBufferPosition++;
		}
	}
	else if ( (Character == CarriageReturnCharacter) || (Character == SpaceCharacter) || (Character == EndOfLineCharacter) )
	{
		if ( (Character != SpaceCharacter) && (ReturnMessage.Type == MessageType::Reply) )
		{
			FailToParse();
			return;
		}
		ReturnBuffer[ReturnBufferPosition] = '\0';
		bool Found = false;
		for (uint8_t Index = 0; Index < static_cast<uint8_t>(WarningType::Count); Index++)
		{
			if ( strcmp(ReturnBuffer, WarningIdentifier[Index].Flag) == 0 )
			{
				ReturnMessage.Warning = WarningIdentifier[Index].Type;
				Found = true;
				break;
			}
		}
		ClearReturnBuffer();
		if(Found)
		{
			if ( (NextState == ReplyParts::Type) && (ReturnMessage.Type == MessageType::Alert) )
			{
				ReturnMessage.MessageComplete = true;
				CurrentReplyPart = ReplyParts::Type;
				ProcessAlertMessage();
			}
			CurrentReplyPart = NextState;
		}
		else
		{
			FailToParse();
		}
	}
	else
	{
		FailToParse();
	}
}
void ZaberMaster::ParseData(char Character)
{
	if (ReturnMessage.Flag == FlagType::Okay)
	{
		if (CurrentCommand.Command == CommandMessageType::Warnings)
		{
			if (ReturnMessageDataTypeDetermined)
			{
				if (isalpha(Character))
				{
					ReturnBuffer[ReturnBufferPosition] = Character;
					if (ReturnBufferPosition < ReturnBufferSize)
					{
						ReturnBufferPosition++;
					}
				}
				else if (Character == SpaceCharacter)
				{
					ReturnBuffer[ReturnBufferPosition] = '\0';
					bool Found = false;
					for (uint8_t Index = 0; Index < static_cast<uint8_t>(WarningType::Count); Index++)
					{
						if ( strcmp(ReturnBuffer, WarningIdentifier[Index].Flag) == 0 )
						{
							ReturnMessage.Warning = WarningIdentifier[Index].Type;
							Found = true;
							break;
						}
					}
					ClearReturnBuffer();
					if (Found)
					{
						ReturnMessage.Data[ReturnMessage.DataLength].Type = ReplyDataType::Warning;
						ReturnMessage.Data[ReturnMessage.DataLength].Value.Warning = ReturnMessage.Warning;
						if (ReturnMessage.DataLength < (ZaberMaxReplyData-1) )
						{
							ReturnMessage.DataLength++;
						}
					}
				}
				else if (Character == CarriageReturnCharacter || Character == EndOfLineCharacter)
				{
					ReturnMessageDataTypeDetermined = false;
					ClearReturnBuffer();
					CurrentReplyPart = ReplyParts::Type;
					ReturnMessage.MessageComplete = true;
					ProcessReplyMessage();
				}
				else
				{
					FailToParse();
				}
			}
			else
			{
				if (isdigit(Character))
				{
					ReturnBuffer[ReturnBufferPosition] = Character;
					if (ReturnBufferPosition < ReturnBufferSize)
					{
						ReturnBufferPosition++;
					}
				}
				else if (Character == SpaceCharacter)
				{
					ReturnBuffer[ReturnBufferPosition] = '\0';
					ReturnMessageDataTypeDetermined = true;
					ClearReturnBuffer();
				}
				else if (Character == CarriageReturnCharacter || Character == EndOfLineCharacter)
				{
					ReturnMessageDataTypeDetermined = false;
					ClearReturnBuffer();
					CurrentReplyPart = ReplyParts::Type;
					ReturnMessage.MessageComplete = true;
					ProcessReplyMessage();
				}
				else
				{
					FailToParse();
				}
			}
		}
		else
		{
			if (isdigit(Character) || Character == '.')
			{
				if (Character == '.')
				{
					if (ReturnMessageDataTypeDetermined)
					{
						FailToParse();
					}
					else
					{
						ReturnMessageDataTypeDetermined = true;
						ReturnBuffer[ReturnBufferPosition] = Character;
						if (ReturnBufferPosition < ReturnBufferSize)
						{
							ReturnBufferPosition++;
						}
					}
				}
				else
				{
					ReturnBuffer[ReturnBufferPosition] = Character;
					if (ReturnBufferPosition < ReturnBufferSize)
					{
						ReturnBufferPosition++;
					}
				}
			}
			else if (Character == CarriageReturnCharacter || Character == EndOfLineCharacter || Character == SpaceCharacter)
			{
				if (ReturnMessage.DataLength < (ZaberMaxReplyData-1) )
				{
					if (ReturnMessageDataTypeDetermined)
					{
						ReturnMessage.Data[ReturnMessage.DataLength].Type = ReplyDataType::Float;
						ReturnMessage.Data[ReturnMessage.DataLength].Value.Float = CharPointerToFloat(ReturnBuffer, ReturnBufferSize);
					}
					else
					{
						ReturnBuffer[ReturnBufferPosition] = '\0';
						ReturnMessage.Data[ReturnMessage.DataLength].Type = ReplyDataType::Integer;
						ReturnMessage.Data[ReturnMessage.DataLength].Value.Integer = (int32_t)atoi(ReturnBuffer);
					}
					ReturnMessage.DataLength++;
				}
				ReturnMessageDataTypeDetermined = false;
				ClearReturnBuffer();
				if (Character != SpaceCharacter)
				{
					CurrentReplyPart = ReplyParts::Type;
					ReturnMessage.MessageComplete = true;
					ProcessReplyMessage();
				}
			}
			else
			{
				FailToParse();
			}
		}
	}
	else
	{
		if (isalpha(Character))
		{
			ReturnBuffer[ReturnBufferPosition] = Character;
			if (ReturnBufferPosition < ReturnBufferSize)
			{
				ReturnBufferPosition++;
			}
		}
		else if (Character == CarriageReturnCharacter || Character == EndOfLineCharacter || Character == SpaceCharacter)
		{
			ReturnBuffer[ReturnBufferPosition] = '\0';
			bool Found = false;
			ReplyDataErrorType FoundError;
			for (uint8_t Index = 0; Index < static_cast<uint8_t>(ReplyDataErrorType::Count); Index++)
			{
				if ( strcmp(ReturnBuffer, ReplyDataErrorIdentifier[Index].Flag) == 0 )
				{
					FoundError = ReplyDataErrorIdentifier[Index].Type;
					Found = true;
				}
			}
			ClearReturnBuffer();
			if(Found)
			{
				ReturnMessage.Data[0].Type = ReplyDataType::Error;
				ReturnMessage.Data[0].Value.Error = FoundError;
				CurrentReplyPart = ReplyParts::Type;
				ReturnMessage.MessageComplete = true;
				ProcessReplyMessage();
			}
			else
			{
				FailToParse();
			}
		}
		else
		{
			FailToParse();
		}
	}
}
bool ZaberMaster::CheckExpectingAlert()
{
	for (uint8_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
	{
		for (uint8_t AxisIndex = 0; AxisIndex < AxesFound[DeviceIndex]; AxisIndex++)
		{
			if (AxisProperties[DeviceIndex][AxisIndex].ExpectingAlert)
			{
				return true;
			}
		}
	}
	return false;
}
void ZaberMaster::ProcessAlertMessage()
{
	if (Verbose)
	{
		VerboseBuffer[VerboseBufferIndex] = '\0';
		Serial.print("<ZABER>(ALERT: ");
		Serial.print(VerboseBuffer);
		Serial.print(")\n");
		ClearVerboseBuffer();
	}
	if (ReturnMessage.Type == MessageType::Alert)
	{
		if (ReturnMessage.Device > 0)
		{
			if (ReturnMessage.Axis > 0)
			{
				if (AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].ExpectingAlert)
				{
					if (AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].AlertCallback != NULL)
					{
						AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].AlertCallback(ReturnMessage.Device, ReturnMessage.Axis);
					}
					if (AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].ExternalCallback != NULL)
					{
						AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].ExternalCallback();
						AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].ExternalCallback = NULL;
					}
				}
				AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].ExpectingAlert = false;
			}
		}
		SetStatus();
		bool IsExpectingAlert = CheckExpectingAlert();
		if (!IsExpectingAlert)
		{
			if (!Initializing && !Initialized)
			{
				Initialized = true;
				Initializing = false;
				InitializationStep = 0;
				if (InitializationComplete != NULL)
				{
					InitializationComplete();
				}
			}
			else
			{
				if (MovementComplete != NULL)
				{
					MovementComplete(ReturnMessage.Device, ReturnMessage.Axis);
				}
			}
		}
	}
}
void ZaberMaster::RunNextInitializationStep()
{
	if (Initializing)
	{
		if (InitializationStep < InitializationStepsCount)
		{
			CommandForEnqueue.Command = InitilizationSteps[InitializationStep].Command;
			CommandForEnqueue.Axis = InitilizationSteps[InitializationStep].Axis;
			CommandForEnqueue.Device = InitilizationSteps[InitializationStep].Device;
			for (size_t ParameterIndex = 0; ParameterIndex < InitilizationSteps[InitializationStep].ParameterCount; ParameterIndex++)
			{
				CommandForEnqueue.Parameters[ParameterIndex].Type = InitilizationSteps[InitializationStep].Parameters[ParameterIndex].Type;
				switch (CommandForEnqueue.Parameters[ParameterIndex].Type)
				{
				case CommandParameterType::None:
					break;
				case CommandParameterType::Integer:
					CommandForEnqueue.Parameters[ParameterIndex].Value.Integer = InitilizationSteps[InitializationStep].Parameters[ParameterIndex].Value.Integer;
					break;
				case CommandParameterType::Float:
					CommandForEnqueue.Parameters[ParameterIndex].Value.Float = InitilizationSteps[InitializationStep].Parameters[ParameterIndex].Value.Float;
					break;
				case CommandParameterType::Parameter:
					CommandForEnqueue.Parameters[ParameterIndex].Value.Parameter = InitilizationSteps[InitializationStep].Parameters[ParameterIndex].Value.Parameter;
					break;
				case CommandParameterType::Setting:
					CommandForEnqueue.Parameters[ParameterIndex].Value.Setting = InitilizationSteps[InitializationStep].Parameters[ParameterIndex].Value.Setting;
					break;
				default:
					break;
				}
			}
			if (CommandForEnqueue.Command == CommandMessageType::ToolsFindRange)
			{
				for (size_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
				{
					for (size_t AxisIndex = 0; AxisIndex < AxesFound[DeviceIndex]; AxisIndex++)
					{
						AxisProperties[DeviceIndex][AxisIndex].ExpectingAlert = true;
					}
				}
			}
			CommandForEnqueue.ParameterCount = InitilizationSteps[InitializationStep].ParameterCount;
			CommandEnqueue();
		}
		else
		{
			Initializing = false;
		}
	}
}
void ZaberMaster::SetStatus()
{
	if (ReturnMessage.Device == 0)
	{
		if (ReturnMessage.Axis == 0)
		{
			for (size_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
			{
				for (size_t AxisIndex = 0; AxisIndex < AxesFound[DeviceIndex]; AxisIndex++)
				{
					AxisProperties[DeviceIndex][AxisIndex].Status = ReturnMessage.Status;
				}
			}
		}
		else
		{
			for (size_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
			{
				AxisProperties[DeviceIndex][ReturnMessage.Axis - 1].Status = ReturnMessage.Status;
			}
		}
	}
	else
	{
		if (CurrentCommand.Axis == 0)
		{
			for (size_t AxisIndex = 0; AxisIndex < AxesFound[CurrentCommand.Device - 1]; AxisIndex++)
			{
				AxisProperties[ReturnMessage.Device - 1][AxisIndex].Status = ReturnMessage.Status;
			}
		}
		else
		{
			AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].Status = ReturnMessage.Status;
		}
	}
	if (Verbose)
	{
		Serial.print("<ZABER>(Set status ");
		Serial.print(ReturnMessage.Device);
		Serial.print(",");
		Serial.print(ReturnMessage.Axis);
		Serial.print(",");
		Serial.print(static_cast<uint8_t>(ReturnMessage.Status));
		Serial.print(",");
		Serial.print(StatusIdentifier[static_cast<uint8_t>(ReturnMessage.Status)].Flag);
		Serial.print(")\n");
	}
}
void ZaberMaster::ProcessReplyMessage()
{
	if (Verbose)
	{
		VerboseBuffer[VerboseBufferIndex] = '\0';
		Serial.print("<ZABER>(REPLY: ");
		Serial.print(VerboseBuffer);
		Serial.print(")\n");
		ClearVerboseBuffer();
	}
	SetStatus();
	if (ReturnMessage.Type == MessageType::Reply)
	{
		if (Mode == ModeType::WaitForCommandReply)
		{
			if (CurrentReceivedCallback != NULL)
			{
				CurrentReceivedCallback();
			}
			if (Initializing)
			{
				if (InitializationStep < InitializationStepsCount)
				{
					InitializationStep++;
					RunNextInitializationStep();
				}
			}
			else
			{
				if (ReplyComplete != NULL)
				{
					ReplyComplete();
				}
			}
			if (CheckExpectingAlert())
			{
				Mode = ModeType::WaitForAlert;
			}
			else
			{
				Mode = ModeType::Idle;
			}
		}
	}
	if (CurrentCommand.Callback != NULL)
	{
		CurrentCommand.Callback();
	}
	if (ReturnMessage.Warning != WarningType::None)
	{
		Serial.print("<STAGE>(Error: ");
		Serial.print(WarningIdentifier[static_cast<uint8_t>(ReturnMessage.Warning)].String);
		Serial.print(")\n");
		//SendClearWarnings(ReturnMessage.Device);
	}
}
void ZaberMaster::ProcessMoveEnded(uint8_t Device, uint8_t Axis)
{
	SendGetPosition(Device,Axis);
	if (Verbose)
	{
		Serial.print("<ZABER>(Updating position: ");
		Serial.print(Device);
		Serial.print(",");
		Serial.print(Axis);
		Serial.print(")\n");
	}
}
void ZaberMaster::ProcessSetReturned()
{
	if (CurrentCommand.ParameterCount > 0)
	{
		if (CurrentCommand.Parameters[0].Type == CommandParameterType::Setting)
		{
			if (CurrentCommand.Parameters[0].Value.Setting == SettingMessageType::Pos)
			{
				SendGetPosition(ReturnMessage.Device, ReturnMessage.Axis);
			}
			if (CurrentCommand.Parameters[0].Value.Setting == SettingMessageType::MaxSpeed)
			{
				SendGetMaxSpeed(ReturnMessage.Device, ReturnMessage.Axis);
			}
		}
	}
}
void ZaberMaster::ProcessGetReceived()
{
	if (CurrentCommand.ParameterCount > 0)
	{
		if (CurrentCommand.Parameters[0].Type == CommandParameterType::Setting)
		{
			if (CurrentCommand.Parameters[0].Value.Setting == SettingMessageType::Pos)
			{
				if (Verbose)
				{
					Serial.print("<ZABER>(Pos update: ");
					Serial.print(ReturnMessage.Device);
					Serial.print(",");
					Serial.print(ReturnMessage.Axis);
					Serial.print(",");
					Serial.print(ReturnMessage.DataLength);
				}
				if (ReturnMessage.Device == 0)
				{
					Serial.print("<STAGEERROR>(Returned get with 0 device. Invalid.)\n");
				}
				else
				{
					if (ReturnMessage.Axis == 0)
					{
						for (uint8_t Index = 0; Index < ReturnMessage.DataLength; Index++)
						{
							AxisProperties[ReturnMessage.Device - 1][Index].Position = ReturnMessage.Data[Index].Value.Integer;
							if (Verbose)
							{
								Serial.print(",");
								Serial.print(AxisProperties[ReturnMessage.Device - 1][Index].Position);
							}
						}
						if (Verbose)
						{
							Serial.print(")\n");
						}
					}
					else
					{
						AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].Position = ReturnMessage.Data[0].Value.Integer;
						if (Verbose)
						{
							Serial.print(",");
							Serial.print(AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].Position);
							Serial.print(")\n");
						}
					}
				}
			}
			if (CurrentCommand.Parameters[0].Value.Setting == SettingMessageType::SystemAxisCount)
			{
				if (ReturnMessage.DataLength == 1)
				{
					if (ReturnMessage.Device > 0)
					{
						AxesFound[ReturnMessage.Device - 1] = ReturnMessage.Data[0].Value.Integer;
					}
				}
				else if (ReturnMessage.DataLength > 1)
				{
					for (size_t CurrentParameterIndex = 0; CurrentParameterIndex < ReturnMessage.DataLength; CurrentParameterIndex++)
					{
						if (ReturnMessage.Data[CurrentParameterIndex].Type == ReplyDataType::Integer)
						{
							AxesFound[CurrentParameterIndex] = ReturnMessage.Data[CurrentParameterIndex].Value.Integer;
						}
					}
				}
			}
			if (CurrentCommand.Parameters[0].Value.Setting == SettingMessageType::MaxSpeed)
			{
				if (Verbose)
				{
					Serial.print("<ZABER>(Max speed update: ");
					Serial.print(ReturnMessage.Device);
					Serial.print(",");
					Serial.print(ReturnMessage.Axis);
					Serial.print(",");
					Serial.print(ReturnMessage.DataLength);
				}
				if (ReturnMessage.Device == 0)
				{
					Serial.print("<STAGEERROR>(Returned get with 0 device. Invalid.)\n");
				}
				else
				{
					if (ReturnMessage.Axis == 0)
					{
						for (uint8_t Index = 0; Index < ReturnMessage.DataLength; Index++)
						{
							AxisProperties[ReturnMessage.Device - 1][Index].Position = ReturnMessage.Data[Index].Value.Integer;
							if (Verbose)
							{
								Serial.print(",");
								Serial.print(AxisProperties[ReturnMessage.Device - 1][Index].MaxSpeed);
							}
						}
						if (Verbose)
						{
							Serial.print(")\n");
						}
					}
					else
					{
						AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].Position = ReturnMessage.Data[0].Value.Integer;
						if (Verbose)
						{
							Serial.print(",");
							Serial.print(AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].MaxSpeed);
							Serial.print(")\n");
						}
					}
				}
			}
		}
	}
}
void ZaberMaster::ProcessRenumberReceived()
{
	if (DevicesFound < ReturnMessage.Device)
	{
		DevicesFound = ReturnMessage.Device;
	}
}
void ZaberMaster::ProcessMoveStarted()
{
	if (CurrentCommand.Device == 0)
	{
		if (CurrentCommand.Axis == 0)
		{
			for (size_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
			{
				for (size_t AxisIndex = 0; AxisIndex < AxesFound[DeviceIndex]; AxisIndex++)
				{
					AxisProperties[DeviceIndex][AxisIndex].ExpectingAlert = true;
				}
			}
		}
		else
		{
			for (size_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
			{
				AxisProperties[DeviceIndex][CurrentCommand.Axis - 1].ExpectingAlert = true;
			}
		}
	}
	else
	{
		if (CurrentCommand.Axis == 0)
		{
			for (size_t AxisIndex = 0; AxisIndex < AxesFound[CurrentCommand.Device - 1]; AxisIndex++)
			{
				AxisProperties[CurrentCommand.Device - 1][AxisIndex].ExpectingAlert = true;
			}
		}
		else
		{
			AxisProperties[CurrentCommand.Device - 1][CurrentCommand.Axis - 1].ExpectingAlert = true;
		}
	}
	if (Verbose)
	{
		Serial.print("<ZABER>(Expect alert: ");
		Serial.print(CurrentCommand.Device);
		Serial.print(",");
		Serial.print(CurrentCommand.Axis);
		Serial.print(")\n");
	}
}
void ZaberMaster::AssignAlertCallbacks(uint8_t CommandIndex)
{
	if (CurrentCommand.Device == 0)
	{
		if (CurrentCommand.Axis == 0)
		{
			for (size_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
			{
				for (size_t AxisIndex = 0; AxisIndex < AxesFound[DeviceIndex]; AxisIndex++)
				{
					AxisProperties[DeviceIndex][AxisIndex].AlertCallback = CommandIdentifier[CommandIndex].AlertCallback;
				}
			}
		}
		else
		{
			for (size_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
			{
				AxisProperties[DeviceIndex][CurrentCommand.Axis - 1].ExpectingAlert = CommandIdentifier[CommandIndex].AlertCallback;
			}
		}
	}
	else
	{
		if (CurrentCommand.Axis == 0)
		{
			for (size_t AxisIndex = 0; AxisIndex < AxesFound[CurrentCommand.Device - 1]; AxisIndex++)
			{
				AxisProperties[CurrentCommand.Device - 1][AxisIndex].ExpectingAlert = CommandIdentifier[CommandIndex].AlertCallback;
			}
		}
		else
		{
			AxisProperties[CurrentCommand.Device - 1][CurrentCommand.Axis - 1].ExpectingAlert = CommandIdentifier[CommandIndex].AlertCallback;
		}
	}
}
void ZaberMaster::SendCommand()
{
	const uint8_t CommandIndex = static_cast<uint8_t>(CurrentCommand.Command);
	const uint8_t* StringToSend = reinterpret_cast<const uint8_t*>(CommandIdentifier[CommandIndex].String);
	const uint8_t CommandSize = CommandIdentifier[CommandIndex].Count;
	uint8_t CharCount;
	CurrentSentCallback = CommandIdentifier[CommandIndex].SentCallback;
	CurrentReceivedCallback = CommandIdentifier[CommandIndex].ReceivedCallback;
	AssignAlertCallbacks(CommandIndex);
	memset(CommandBuffer,0,CommandBufferSize);
	_HardwareSerial->write(CommandMarkerCharacter);
	CharCount = IntToCharPointer(CurrentCommand.Device, CommandBuffer, CommandBufferSize);
	_HardwareSerial->write(reinterpret_cast<const uint8_t*>(CommandBuffer),CharCount);
	_HardwareSerial->write(SpaceCharacter);
	if (Verbose)
	{
		Serial.print("<ZABER>(OUT: ");
		Serial.print(CommandMarkerCharacter);
		Serial.write(reinterpret_cast<const uint8_t*>(CommandBuffer), CharCount);
		Serial.write(SpaceCharacter);
	}
	memset(CommandBuffer,0,CommandBufferSize);
	CharCount = IntToCharPointer(CurrentCommand.Axis, CommandBuffer, CommandBufferSize);
	_HardwareSerial->write(reinterpret_cast<const uint8_t*>(CommandBuffer),CharCount);
	_HardwareSerial->write(SpaceCharacter);
	_HardwareSerial->write(StringToSend,CommandSize);
	if (Verbose)
	{
		Serial.write(reinterpret_cast<const uint8_t*>(CommandBuffer), CharCount);
		Serial.write(SpaceCharacter);
		Serial.write(StringToSend, CommandSize);
	}
	if (CurrentCommand.ParameterCount > 0)
	{
		for (uint8_t ParameterIndex = 0; ParameterIndex < CurrentCommand.ParameterCount; ParameterIndex++)
		{
			CommandParameterType ParameterType = CurrentCommand.Parameters[ParameterIndex].Type;
			switch (ParameterType)
			{
				case CommandParameterType::None:
					break;
				case CommandParameterType::Integer:
				{
					uint8_t CharCount = IntToCharPointer(CurrentCommand.Parameters[ParameterIndex].Value.Integer, CommandBuffer, CommandBufferSize);
					_HardwareSerial->write(SpaceCharacter);
					_HardwareSerial->write(reinterpret_cast<const uint8_t*>(CommandBuffer),CharCount);
					if (Verbose)
					{
						Serial.write(SpaceCharacter);
						Serial.write(reinterpret_cast<const uint8_t*>(CommandBuffer), CharCount);
					}
					break;
				}
				case CommandParameterType::Float:
				{
					uint8_t CharCount = FloatToCharPointer(CurrentCommand.Parameters[ParameterIndex].Value.Float, CommandBuffer, CommandBufferSize);
					_HardwareSerial->write(SpaceCharacter);
					_HardwareSerial->write(reinterpret_cast<const uint8_t*>(CommandBuffer),CharCount);
					if (Verbose)
					{
						Serial.write(SpaceCharacter);
						Serial.write(reinterpret_cast<const uint8_t*>(CommandBuffer), CharCount);
					}
					break;
				}
				case CommandParameterType::Parameter:
				{
					const uint8_t ParameterIdentifierIndex = static_cast<uint8_t>(CurrentCommand.Parameters[ParameterIndex].Value.Parameter);
					const uint8_t* ParameterMessageString = reinterpret_cast<const uint8_t*>(ParameterIdentifier[ParameterIdentifierIndex].String);
					const uint8_t StringCount = ParameterIdentifier[ParameterIdentifierIndex].Count;
					_HardwareSerial->write(SpaceCharacter);
					_HardwareSerial->write(ParameterMessageString,StringCount);
					if (Verbose)
					{
						Serial.write(SpaceCharacter);
						Serial.write(ParameterMessageString, StringCount);
					}
					break;
				}
				case CommandParameterType::Setting:
				{
					const uint8_t SettingIdentifierIndex = static_cast<uint8_t>(CurrentCommand.Parameters[ParameterIndex].Value.Setting);
					const uint8_t* SettingMessageString = reinterpret_cast<const uint8_t*>(SettingIdentifier[SettingIdentifierIndex].String);
					const uint8_t StringCount = SettingIdentifier[SettingIdentifierIndex].Count;
					_HardwareSerial->write(SpaceCharacter);
					_HardwareSerial->write(SettingMessageString,StringCount);
					if (Verbose)
					{
						Serial.write(SpaceCharacter);
						Serial.write(SettingMessageString, StringCount);
					}
					break;
				}
				default:
					break;
			}
		}
	}
	_HardwareSerial->write(EndOfLineCharacter);
	if (Verbose)
	{
		Serial.print(")\n");
	}
	ResetReturnMessage();
	Mode = ModeType::WaitForCommandReply;
	if (CurrentSentCallback != NULL)
	{
		CurrentSentCallback();
	}
}
void ZaberMaster::ClearReturnBuffer()
{
	memset(ReturnBuffer,0,sizeof(ReturnBuffer));
	ReturnBufferPosition = 0;
}
void ZaberMaster::ClearVerboseBuffer()
{
	memset(VerboseBuffer,0,sizeof(ReturnBuffer));
	VerboseBufferIndex = 0;
}
void ZaberMaster::ResetReturnMessage()
{
	ReturnMessage.DataLength = 0;
	ReturnMessageDataTypeDetermined = false;
}
void ZaberMaster::FailToParse()
{
	if (Verbose)
	{
		VerboseBuffer[VerboseBufferIndex] = '\0';
		Serial.print("<ZABER>(FAIL: ");
		Serial.print(VerboseBuffer);
		Serial.print(")\n");
		ClearVerboseBuffer();
	}
	ClearReturnBuffer();
	ResetReturnMessage();
	Mode = ModeType::Idle;
	CurrentReplyPart = ReplyParts::Type;
}
