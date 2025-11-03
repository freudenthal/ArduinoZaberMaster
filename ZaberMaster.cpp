#include "ZaberMaster.h"

const char ZaberMaster::CarriageReturnCharacter = '\r';
const char ZaberMaster::EndOfLineCharacter = '\n';
const char ZaberMaster::SpaceCharacter = ' ';
const char ZaberMaster::CommandMarkerCharacter = '/';
const char ZaberMaster::GetCharacter = '?';
const uint8_t ZaberMaster::LinearStandardInitilizationStepCount = 6;
const ZaberMaster::CommandMessage ZaberMaster::LinearStandardInitilizationSteps[]=
{
	{CommandMessageType::Warnings,0,0,{{{.Parameter=ParameterMessageType::Clear},CommandParameterType::Parameter},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},1,NULL},
	{CommandMessageType::Renumber,0,0,{{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},0,NULL},
	{CommandMessageType::Get,0,0,{{{.Setting=SettingMessageType::SystemAxisCount},CommandParameterType::Setting},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},1,NULL},
	{CommandMessageType::Set,0,0,{{{.Setting=SettingMessageType::MaxSpeed},CommandParameterType::Setting},{{.Integer = ZaberDefaultSpeed},CommandParameterType::Integer},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},2,NULL},
	{CommandMessageType::Set,0,0,{{{.Setting=SettingMessageType::CommAlert},CommandParameterType::Setting},{{.Integer = 1},CommandParameterType::Integer},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},2,NULL},
	{CommandMessageType::ToolsFindRange,0,0,{{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},0,NULL},
};
const uint8_t ZaberMaster::RotationalStandardInitilizationStepCount = 4;
const ZaberMaster::CommandMessage ZaberMaster::RotationalStandardInitilizationSteps[]=
{
	{CommandMessageType::Warnings,0,0,{{{.Parameter=ParameterMessageType::Clear},CommandParameterType::Parameter},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},1,NULL},
	{CommandMessageType::Home,1,1,{{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},0,NULL},
	{CommandMessageType::Home,1,2,{{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},0,NULL},
	{CommandMessageType::Set,0,0,{{{.Setting=SettingMessageType::CommAlert},CommandParameterType::Setting},{{.Integer = 1},CommandParameterType::Integer},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},2,NULL},
};
const uint8_t ZaberMaster::LinearAndRotationalInitilizationStepCount = 8;
const ZaberMaster::CommandMessage ZaberMaster::LinearAndRotationalInitilizationSteps[]=
{
	{CommandMessageType::Warnings,0,0,{{{.Parameter=ParameterMessageType::Clear},CommandParameterType::Parameter},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},1,NULL},
	{CommandMessageType::Renumber,0,0,{{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},0,NULL},
	{CommandMessageType::Get,0,0,{{{.Setting=SettingMessageType::SystemAxisCount},CommandParameterType::Setting},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},1,NULL},
	{CommandMessageType::Set,0,0,{{{.Setting=SettingMessageType::MaxSpeed},CommandParameterType::Setting},{{.Integer = ZaberDefaultSpeed},CommandParameterType::Integer},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},2,NULL},
	{CommandMessageType::Set,0,0,{{{.Setting=SettingMessageType::CommAlert},CommandParameterType::Setting},{{.Integer = 1},CommandParameterType::Integer},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},2,NULL},
	{CommandMessageType::ToolsFindRange,0,0,{{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},0,NULL},
	{CommandMessageType::ToolsFindRange,0,1,{{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},0,NULL},
	{CommandMessageType::Home,1,1,{{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},0,NULL},
};
const ZaberMaster::CommandString ZaberMaster::CommandIdentifier[]=
{
	{CommandMessageType::Status,"",0,CommandScope::Axis, NULL, NULL, NULL},
	{CommandMessageType::EStop,"estop",5,CommandScope::Axis, NULL, NULL, NULL},
	{CommandMessageType::Get,"get",3,CommandScope::Either, NULL, &ZaberMaster::ProcessGetReceived, NULL },
	{CommandMessageType::Home,"home",4,CommandScope::Axis, NULL, NULL, NULL },
	{CommandMessageType::IOInfo,"io info",7,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::IOGet,"io get",6,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::IOSet,"io set",6,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::Joystick,"joystick",8,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::Key,"key",3,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::LockStep,"lockstep",8,CommandScope::Device, NULL, NULL, NULL },
	{CommandMessageType::Move,"move",4,CommandScope::Axis, NULL, NULL, NULL },
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

HardwareSerial* ZaberMaster::_StreamPort;
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
uint8_t ZaberMaster::ReturnBufferCount;
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
ZaberMaster::InitializationType ZaberMaster::InitializationBehavior;
ZaberMaster::CommandMessage* ZaberMaster::InitilizationSteps;
uint8_t ZaberMaster::InitializationStepsCount;
bool ZaberMaster::PollingAny;
uint32_t ZaberMaster::PollingTimeMax;
bool ZaberMaster::PollingEnabled;
bool ZaberMaster::AlertEnabled;
bool ZaberMaster::ExpectingAlert;
ZaberMaster::WarningType ZaberMaster::LastWarning;

ZaberFinishedListenerDeviceAxis ZaberMaster::MovementComplete;
ZaberFinishedListener ZaberMaster::ReplyComplete;
ZaberFinishedListener ZaberMaster::InitializationComplete;
ZaberCommandSentCallback ZaberMaster::CurrentSentCallback;
ZaberReplyReceivedCallback ZaberMaster::CurrentReceivedCallback;

ZaberMaster::ZaberMaster(HardwareSerial* serial)
{
	_StreamPort = serial;
	Initialized = false;
	InitializationStep = 0;
	DevicesFound = 0;
	Mode = ModeType::Inactive;
	Busy = false;
	AlertEnabled = false;
	PollingAny = false;
	PollingTimeMax = 100000;
	PollingEnabled = true;
	ExpectingAlert = false;
	LastWarning = WarningType::None;
	memset(AxesFound,0,sizeof(AxesFound));
	ClearCurrentCommand();
	ClearCommandForEnqueue();
	ClearCommandQueue();
	memset(CommandBuffer,0,sizeof(CommandBuffer));
	InitializationComplete = NULL;
	MovementComplete = NULL;
	ReplyComplete = NULL;
	memset(ReturnBuffer,0,sizeof(ReturnBuffer));
	ReturnMessage.Type = MessageType::Reply;
	InitializationBehavior = InitializationType::LinearStandard;
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
			AxisProperties[DeviceIndex][AxisIndex].EncoderPosition = 0;
			AxisProperties[DeviceIndex][AxisIndex].Position = 0;
			AxisProperties[DeviceIndex][AxisIndex].MaxSpeed = 0;
			AxisProperties[DeviceIndex][AxisIndex].Status = StatusType::Unknown;
			AxisProperties[DeviceIndex][AxisIndex].ExternalCallback = NULL;
			AxisProperties[DeviceIndex][AxisIndex].AlertCallback = NULL;
			AxisProperties[DeviceIndex][AxisIndex].Polling = false;
			AxisProperties[DeviceIndex][AxisIndex].PollingSent = false;
			AxisProperties[DeviceIndex][AxisIndex].LastPollTime = 0;
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
bool ZaberMaster::Begin(uint8_t NumberOfDevices, uint8_t* AxisCount)
{
	bool Status = true;
	if (NumberOfDevices >= ZaberMaxDevices)
	{
		NumberOfDevices = ZaberMaxDevices - 1;
		Serial.print("<ZABERDEV>(Device count over max, reducing.)\n");
		Status = false;
	}
	for (size_t DeviceIndex = 0; DeviceIndex < NumberOfDevices; DeviceIndex++)
	{
		if (AxisCount[DeviceIndex] >= ZaberMaxAxes)
		{
			AxisCount[DeviceIndex] = ZaberMaxAxes - 1;
			Serial.print("<ZABERDEV>(Axis count over max, reducing.)\n");
			Status = false;
		}
	}
	DevicesFound = NumberOfDevices;
	for (size_t DeviceIndex = 0; DeviceIndex < NumberOfDevices; DeviceIndex++)
	{
		AxesFound[DeviceIndex] = AxisCount[DeviceIndex];
	}
	Mode = ModeType::Idle;
	Initializing = false;
	Initialized = true;
	return Status;
}
void ZaberMaster::SetVerbose(bool VerboseToSet)
{
	Verbose = VerboseToSet;
}
bool ZaberMaster::SetInitializationType(InitializationType TypeToSet)
{
	InitializationBehavior = TypeToSet;
	return true;
}
bool ZaberMaster::Initialize()
{
	switch (InitializationBehavior)
	{
		case InitializationType::LinearStandard:
			InitilizationSteps = const_cast<CommandMessage*>(LinearStandardInitilizationSteps);
			InitializationStepsCount = LinearStandardInitilizationStepCount;
			break;
		case InitializationType::RotationalStandard:
			InitilizationSteps = const_cast<CommandMessage*>(RotationalStandardInitilizationSteps);
			InitializationStepsCount = RotationalStandardInitilizationStepCount;
			break;
		case InitializationType::LinearAndRotational:
			InitilizationSteps = const_cast<CommandMessage*>(LinearAndRotationalInitilizationSteps);
			InitializationStepsCount = LinearAndRotationalInitilizationStepCount;
			break;
		default:
			InitilizationSteps = const_cast<CommandMessage*>(LinearStandardInitilizationSteps);
			InitializationStepsCount = LinearStandardInitilizationStepCount;
			break;
	}
	Initializing = true;
	InitializationStep = 0;
	Initialized = false;
	//DevicesFound = 0;
	//for (size_t Index = 0; Index < ZaberMaxDevices; Index++)
	//{
	//	AxesFound[Index] = 0;
	//}
	Mode = ModeType::Idle;
	RunNextInitializationStep();
	return true;
}
bool ZaberMaster::SendGetStatus(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Status;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.ParameterCount = 0;
	CommandEnqueue();
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
	//if ( (Device > 0) && (Axis > 0) )
	//{
	//	AxisProperties[Device - 1][Axis - 1].ExternalCallback = ReturnCallback;
	//}
	//else
	//{
		CommandForEnqueue.Callback = ReturnCallback;
	//}
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
	//if ( (Device > 0) && (Axis > 0) )
	//{
	//	AxisProperties[Device - 1][Axis - 1].ExternalCallback = ReturnCallback;
	//}
	//else
	//{
		CommandForEnqueue.Callback = ReturnCallback;
	//}
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
	if (Enable)
	{
		AlertEnabled = true;
	}
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
bool ZaberMaster::SendGetEncoderPosition(uint8_t Device, uint8_t Axis, ZaberFinishedListener ReturnCallback)
{
	CommandForEnqueue.Command = CommandMessageType::Get;
	CommandForEnqueue.Axis = Axis;
	CommandForEnqueue.Device = Device;
	CommandForEnqueue.Callback = ReturnCallback;
	CommandForEnqueue.Parameters[0].Type = CommandParameterType::Setting;
	CommandForEnqueue.Parameters[0].Value.Setting = SettingMessageType::EncoderPos;
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
				if (Verbose)
				{
					Serial.print("<ZABERPOSITIONREQ>(");
					Serial.print(AxisProperties[Device-1][Axis-1].Position);
					Serial.print(")\n");
				}
				*Position = AxisProperties[Device-1][Axis-1].Position;
				return true;
			}
		}
	}
	return false;
}
bool ZaberMaster::GetEncoderPosition(uint8_t Device, uint8_t Axis, uint32_t* EncoderPosition)
{
	if (Device <= ZaberMaxDevices && Axis <= ZaberMaxAxes)
	{
		if (Device <= DevicesFound)
		{
			if (Axis <= AxesFound[Device-1])
			{
				*EncoderPosition = AxisProperties[Device-1][Axis-1].EncoderPosition;
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
		case ModeType::WaitForAlert:
			CheckSerial();
			CheckCommandQueue();
			break;
		case ModeType::WaitForCommandReply:
			CheckSerial();
			break;
		default:
			break;
	}
	if (PollingAny && PollingEnabled)
	{
		EnqueStatusForPolling();
	}
	if ( Busy && (Mode == ModeType::Idle) && CommandQueueEmpty() )
	{
		Busy = false;
	}
}
void ZaberMaster::EnqueStatusForPolling()
{
	bool PollingActive = false;
	bool PollingSent = false;
	bool LastPollTime = 0;
	for (uint8_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
	{
		for (uint8_t AxisIndex = 0; AxisIndex < AxesFound[DeviceIndex]; AxisIndex++)
		{
			PollingActive = AxisProperties[DeviceIndex][AxisIndex].Polling;
			PollingSent = AxisProperties[DeviceIndex][AxisIndex].PollingSent;
			LastPollTime = AxisProperties[DeviceIndex][AxisIndex].LastPollTime;
			if (PollingActive && !PollingSent)
			{
				if (micros() - LastPollTime > PollingTimeMax)
				{
					AxisProperties[DeviceIndex][AxisIndex].PollingSent = true;
					AxisProperties[DeviceIndex][AxisIndex].LastPollTime = micros();
					SendGetStatus(DeviceIndex+1,AxisIndex+1,NULL);
				}
			}
		}
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
bool ZaberMaster::tok_eq(const Tok& t, const char* s)
{
	size_t i=0;
	for(; s[i]; ++i)
	{
		if (i>=t.n || t.p[i]!=s[i])
		{
			return false;
		}
	}
	return i==t.n;
}
bool ZaberMaster::tok_is_uint(const Tok& t)
{
	if (t.n==0)
	{
		return false;
	}
	for (size_t i=0;i<t.n;i++)
	{
		if (t.p[i]<'0'||t.p[i]>'9')
		{
			return false;
		}
	}
	return true;
}
bool ZaberMaster::tok_is_int(const Tok& t)
{
	if (t.n==0)
	{
		return false;
	}
	size_t i=0;
	if (t.p[0]=='-'||t.p[0]=='+')
	{
		if (t.n==1)
		{
			return false;
		}
		i=1;
	}
	for (; i<t.n; i++)
	{
		if (t.p[i]<'0'||t.p[i]>'9')
		{
			return false;
		}
	}
	return true;
}
bool ZaberMaster::tok_is_floatish(const Tok& t)
{
	// integer or decimal or sci notation
	bool dot=false;
	bool exp=false;
	bool digit=false;
	for (size_t i=0;i<t.n;i++)
	{
		char c=t.p[i];
		if ((c>='0'&&c<='9'))
		{
			digit=true;
			continue;
		}
		if ((c=='-'||c=='+') && (i==0 || (exp && (t.p[i-1]=='e'||t.p[i-1]=='E'))))
		{
			continue;
		}
		if (c=='.')
		{
			if (dot)
			{
				return false;
			}
			dot=true;
			continue;
		}
		if (c=='e'||c=='E')
		{
			if (exp)
			{
				return false;
			}
			exp=true;
			continue;
		}
		return false;
	}
	return digit;
}
int ZaberMaster::to_uint(const Tok& t)
{
	int v=0;
	for (size_t i=0;i<t.n;i++)
	{
		v = v*10 + (t.p[i]-'0');
	}
	return v;
}
long ZaberMaster::to_int(const Tok& t)
{
	bool neg=(t.p[0]=='-');
	long v=0;
	size_t i=neg?1:0;
	for (; i<t.n;i++)
	{
		v = v*10 + (t.p[i]-'0');
	}
	return neg ? -v : v;
}
float ZaberMaster::to_float(const Tok& t)
{
	// strtod respecting non-null-terminated slice
	char buf[48];
	size_t n = t.n<sizeof(buf)-1 ? t.n : sizeof(buf)-1;
	memcpy(buf, t.p, n); buf[n]=0;
	return strtof(buf,nullptr);
}
ZaberMaster::MessageType ZaberMaster::parse_msg_type(char c)
{
	if (c=='@') return MessageType::Reply;
	if (c=='#') return MessageType::Info;
	if (c=='!') return MessageType::Alert;
	return MessageType::Reply; // default
}
ZaberMaster::FlagType ZaberMaster::parse_flag(const Tok& t)
{
	if (tok_eq(t,"OK")) return FlagType::Okay;
	if (tok_eq(t,"RJ")) return FlagType::Rejected;
	return FlagType::Okay;
}
ZaberMaster::StatusType ZaberMaster::parse_status(const Tok& t)
{
	if (tok_eq(t,"IDLE")) return StatusType::Idle;
	if (tok_eq(t,"BUSY")) return StatusType::Busy;
	return StatusType::Unknown;
}
ZaberMaster::WarningType ZaberMaster::parse_warning(const Tok& t)
{
	// No active warnings
	if (tok_eq(t,"--")) return WarningType::None;
	// Driver / hardware faults
	if (tok_eq(t,"FO")) return WarningType::DriverDisabled;      // Driver Disabled :contentReference[oaicite:0]{index=0}
	if (tok_eq(t,"FV")) return WarningType::VoltageOutOfRange;    // Over/undervoltage driver disabled :contentReference[oaicite:1]{index=1}
	if (tok_eq(t,"FM")) return WarningType::OverTemperature;      // Motor temperature error (driver disabled) :contentReference[oaicite:2]{index=2}
	if (tok_eq(t,"FQ")) return WarningType::EncoderError;         // Encoder Error :contentReference[oaicite:3]{index=3}
	// Motion/limits
	if (tok_eq(t,"FS")) return WarningType::StalledAndStopped;    // Stalled and Stopped (a.k.a. “Stalled”) :contentReference[oaicite:4]{index=4}
	if (tok_eq(t,"FT")) return WarningType::ExcessiveTwist;       // Excessive Twist (lockstep) :contentReference[oaicite:5]{index=5}
	if (tok_eq(t,"FE")) return WarningType::LimitError;           // Limit Error (sensor/problem/out-of-range) :contentReference[oaicite:6]{index=6}
	if (tok_eq(t,"WL")) return WarningType::UnexpectedLimit;      // Unexpected Limit Trigger :contentReference[oaicite:7]{index=7}
	if (tok_eq(t,"WM")) return WarningType::StationaryDisplacement;// Displaced When Stationary :contentReference[oaicite:8]{index=8}
	// Streaming / interpolation
	if (tok_eq(t,"FP")) return WarningType::InterpolationDeviation; // Interpolated Path Deviation :contentReference[oaicite:9]{index=9}
	if (tok_eq(t,"ND")) return WarningType::StreamDisconinuity;     // Stream/PVT sequence discontinuity (queue underrun) :contentReference[oaicite:10]{index=10}
	// Referencing / homing / notes
	if (tok_eq(t,"WR")) return WarningType::NoReference;         // No Reference Position :contentReference[oaicite:11]{index=11}
	if (tok_eq(t,"NC")) return WarningType::ManualControl;       // Manual Control (knob) :contentReference[oaicite:12]{index=12}
	if (tok_eq(t,"NI")) return WarningType::CommandInterrupted;  // Movement Interrupted :contentReference[oaicite:13]{index=13}
	// Unknown / not mapped in enum (e.g., WT, WH, etc.) → treat as None for now.
	return WarningType::None;
}
size_t ZaberMaster::tokenize_space(const char* s, size_t n, Tok out[], size_t outcap)
{
	// Split on ASCII spaces; skip CR/LF. Keeps slices into original buffer.
	size_t k=0, i=0;
	while (i<n && (s[i]=='\r'||s[i]=='\n'))
	{
		i++;
	}
	while (i<n)
	{
		while (i<n && s[i]==' ')
		{
			i++;
		}
		if (i>=n)
		{
			break;
		}
		size_t j=i;
		while (j<n && s[j]!=' ' && s[j]!='\r' && s[j]!='\n')
		{
			j++;
		}
		if (k<outcap)
		{
			out[k].p=&s[i];
			out[k].n=j-i;
			k++;
		}
		i=j;
		while (i<n && (s[i]==' '))
		{
			i++;
		}
		if (i<n && (s[i]=='\r'||s[i]=='\n'))
		{
			break;
		}
	}
	return k;
}
bool ZaberMaster::ParseZaberReplyLine(const char* buf, size_t len, ReplyMessage& out)
{
	// Main parser. Returns true if it looks like a valid Zaber line.
	// Initialize
	out.DataLength=0;
	out.Axis=0;
	out.Device=0;
	out.Flag=FlagType::Okay;
	out.Status=StatusType::Unknown;
	out.Warning=WarningType::None;
	out.MessageStarted = (len>0 && (buf[0]=='@'||buf[0]=='#'||buf[0]=='!'));
	out.MessageComplete = (len>0 && (buf[len-1]=='\n' || buf[len-1]=='\r'));
	out.Type = (len>0) ? parse_msg_type(buf[0]) : MessageType::Reply;
	if (!out.MessageStarted) return false;
	if (out.Type == MessageType::Info) return false;
	// Tokenize after the first char (message type)
	Tok tok[32];
	size_t ntok = (len>0) ? tokenize_space(buf+1, len-1, tok, 32) : 0;
	if (ntok < 2)
	{
		return false; // need at least device, axis
	}
	// Device (2 digits), Axis (1 digit)
	if (!tok_is_uint(tok[0]) || tok[0].n!=2)
	{
		return false;
	}
	out.Device = (uint8_t)to_uint(tok[0]);
	if (!tok_is_uint(tok[1]))
	{
		return false;
	}
	out.Axis = (uint8_t)to_uint(tok[1]);
	// Optional Message ID token (2 digits); if present, shift indices by +1
	size_t idx = 2;
	bool hasMsgId = (idx < ntok && tok[idx].n==2 && tok_is_uint(tok[idx]));
	if (hasMsgId)
	{
		idx++;
	}
	if (out.Type == MessageType::Reply)
	{
		// Flag
		out.Flag = parse_flag(tok[idx++]);
		if (idx>=ntok)
		{
			return false;
		}
		// Status
		out.Status = parse_status(tok[idx++]);
		if (idx>=ntok)
		{
			return false;
		}
		// Warning
		out.Warning = parse_warning(tok[idx++]);
		// Remaining tokens are data (zero or more)
		while (idx < ntok && out.DataLength < (sizeof(out.Data)/sizeof(out.Data[0])))
		{
			const Tok& t = tok[idx++];
			bool IsInt = tok_is_int(t);
			bool IsFloatIsh = tok_is_floatish(t);
			if (IsInt)
			{
				out.Data[out.DataLength].Type = ReplyDataType::Integer;
				out.Data[out.DataLength].Value.Integer = (int32_t)to_int(t);
				out.DataLength++;
			}
			else if (IsFloatIsh)
			{
				out.Data[out.DataLength].Type = ReplyDataType::Float;
				out.Data[out.DataLength].Value.Float = to_float(t);
				out.DataLength++;
			}
			else if (tok_eq(t,"--"))
			{
				// ignore explicit "no data"
			}
			else
			{
				Serial.println("<ZABERLINE>(Bad data parse.)");
			}
		}
		return true;
	}
	else if (out.Type == MessageType::Alert)
	{
		// Alerts do NOT include a flag. Format is: STATUS WARNING
		// Minimum tokens after (dev,axis[,msgid]) is 1 (status); warning may follow.
		if (idx >= ntok) return false;
		// STATUS
		out.Status = parse_status(tok[idx++]);

		// WARNING (optional but typical)
		if (idx < ntok)
		{
			out.Warning = parse_warning(tok[idx++]);
		}
		// Alerts usually have no trailing data; ignore anything extra but warn once.
		if (idx < ntok)
		{
			Serial.println("<ZABERLINE>(Extra tokens after alert; ignoring.)");
		}
		// No data array for alerts
		out.DataLength = 0;
		return true;
	}
	return false;
}
const char* ZaberMaster::toString(MessageType v)
{
	switch (v)
	{
		case MessageType::Reply: return "Reply";
		case MessageType::Info: return "Info";
		case MessageType::Alert: return "Alert";
		case MessageType::Count: return "Count";
		default: return "Unknown";
	}
}
const char* ZaberMaster::toString(StatusType v)
{
	switch (v)
	{
		case StatusType::Idle: return "Idle";
		case StatusType::Busy: return "Busy";
		case StatusType::Count: return "Count";
		case StatusType::Unknown: return "Unknown";
		default: return "Unknown";
	}
}
const char* ZaberMaster::toString(FlagType v)
{
	switch (v)
	{
		case FlagType::Okay: return "Okay";
		case FlagType::Rejected: return "Rejected";
		case FlagType::Count: return "Count";
		default: return "Unknown";
	}
}
const char* ZaberMaster::toString(WarningType v)
{
	switch (v)
	{
		case WarningType::None: return "None";
		case WarningType::DriverDisabled: return "DriverDisabled";
		case WarningType::EncoderError: return "EncoderError";
		case WarningType::StalledAndStopped: return "StalledAndStopped";
		case WarningType::ExcessiveTwist: return "ExcessiveTwist";
		case WarningType::StreamBoundErrors: return "StreamBoundErrors";
		case WarningType::InterpolationDeviation: return "InterpolationDeviation";
		case WarningType::LimitError: return "LimitError";
		case WarningType::UnexpectedLimit: return "UnexpectedLimit";
		case WarningType::VoltageOutOfRange: return "VoltageOutOfRange";
		case WarningType::OverTemperature: return "OverTemperature";
		case WarningType::StationaryDisplacement: return "StationaryDisplacement";
		case WarningType::NoReference: return "NoReference";
		case WarningType::ManualControl: return "ManualControl";
		case WarningType::CommandInterrupted: return "CommandInterrupted";
		case WarningType::StreamDisconinuity: return "StreamDisconinuity";
		case WarningType::SettingsUpdating: return "SettingsUpdating";
		case WarningType::JoystickCalibrating: return "JoystickCalibrating";
		case WarningType::Count: return "Count";
		default: return "Unknown";
	}
}
const char* ZaberMaster::toString(ReplyDataType v)
{
	switch (v)
	{
		case ReplyDataType::None: return "None";
		case ReplyDataType::Float: return "Float";
		case ReplyDataType::Integer: return "Integer";
		case ReplyDataType::Error: return "Error";
		case ReplyDataType::Warning: return "Warning";
		case ReplyDataType::Count: return "Count";
		default: return "Unknown";
	}
}
const char* ZaberMaster::toString(ReplyDataErrorType v)
{
	switch (v)
	{
		case ReplyDataErrorType::None:return "None";
		case ReplyDataErrorType::Again: return "Again";
		case ReplyDataErrorType::BadAxis: return "BadAxis";
		case ReplyDataErrorType::BadData: return "BadData";
		case ReplyDataErrorType::BadMessageID: return "BadMessageID";
		case ReplyDataErrorType::DeviceOnly: return "DeviceOnly";
		case ReplyDataErrorType::Full: return "Full";
		case ReplyDataErrorType::LockStep: return "LockStep";
		case ReplyDataErrorType::NoAccess: return "NoAccess";
		case ReplyDataErrorType::Parked: return "Parked";
		case ReplyDataErrorType::StatusBusy: return "StatusBusy";
		case ReplyDataErrorType::Count: return "Count";
		default: return "Unknown";
	}
}
void ZaberMaster::PrintReplyMessage(ReplyMessage& msg)
{
	Serial.print(F("---- ReplyMessage ----\n"));
	Serial.print(F("MessageType: "));
	Serial.print(toString(msg.Type));
	Serial.print("\n");
	Serial.print(F("Flag: "));
	Serial.print(toString(msg.Flag));
	Serial.print("\n");
	Serial.print(F("Status: "));
	Serial.print(toString(msg.Status));
	Serial.print("\n");
	Serial.print(F("Warning: "));
	Serial.print(toString(msg.Warning));
	Serial.print("\n");
	Serial.print(F("DataLength: "));
	Serial.print(msg.DataLength);
	Serial.print("\n");
	Serial.print(F("MessageComplete: "));
	Serial.print(msg.MessageComplete ? "true" : "false");
	Serial.print("\n");
	Serial.print(F("MessageStarted: "));
	Serial.print(msg.MessageStarted ? "true" : "false");
	Serial.print("\n");
	Serial.print(F("Axis: "));
	Serial.print(msg.Axis);
	Serial.print("\n");
	Serial.print(F("Device: "));
	Serial.print(msg.Device);
	Serial.print("\n");
	Serial.print(F("StartTime: "));
	Serial.print(msg.StartTime);
	Serial.print("\n");
	Serial.print(F("---- Data ----"));
	Serial.print("\n");
	for (size_t i = 0; i < msg.DataLength; i++)
	{
		const ReplyData& d = msg.Data[i];
		Serial.print(F("  ["));
		Serial.print(i);
		Serial.print(F("]"));
		Serial.print(F(" Type: "));
		Serial.print(toString(d.Type));
		Serial.print("\n");
		switch (d.Type)
		{
			case ReplyDataType::Float:
				Serial.print(F(" Value (float): "));
				Serial.print(d.Value.Float, 6);
				Serial.print("\n");
				break;
			case ReplyDataType::Integer:
				Serial.print(F(" Value (int32): "));
				Serial.print(d.Value.Integer);
				Serial.print("\n");
				break;
			case ReplyDataType::Error:
				Serial.print(F(" Value (error): "));
				Serial.print(toString(d.Value.Error));
				Serial.print("\n");
				break;
			case ReplyDataType::Warning:
				Serial.print(F(" Value (warning): "));
				Serial.print(toString(d.Value.Warning));
				Serial.print("\n");
				break;
			default:
				Serial.print(F("    (no data)"));
				Serial.print("\n");
				break;
		}
	}
	Serial.print(F("-----------------------"));
	Serial.print("\n");
}
void ZaberMaster::ProcessReturnBuffer(char* ReturnBuffer, uint8_t BufferCount)
{
	if (Verbose)
	{
		Serial.print("<ZABERRET>(");
		Serial.print(ReturnBuffer);
		Serial.print(")\n");
	}
	bool Status = ParseZaberReplyLine(ReturnBuffer, BufferCount, ReturnMessage);
	if (Status)
	{
		if (Verbose)
		{
			PrintReplyMessage(ReturnMessage);
		}
		if (ReturnMessage.Type == MessageType::Alert)
		{
			AlertEnabled = true;
			PollingEnabled = false;
			ProcessAlertMessage();
		}
		else if (ReturnMessage.Type == MessageType::Reply)
		{
			ProcessReplyMessage();
		}
	}
	else
	{
		Serial.print("<ZABERPARSE>(Failed to parse: ");
		Serial.print(ReturnBuffer);
		Serial.print(")\n");
		FailToParse();
	}
}
void ZaberMaster::CheckSerial()
{
	while (_StreamPort->available() > 0)
	{
		char NewChar = _StreamPort->read();
		//Serial.print(NewChar);
		if ( (NewChar == EndOfLineCharacter) && (ReturnBufferCount > 0) )
		{
			ReturnBuffer[ReturnBufferCount] = EndOfLineCharacter;
			ReturnBufferCount++;
			ProcessReturnBuffer(ReturnBuffer, ReturnBufferCount);
			ClearReturnBuffer();
		}
		else if (NewChar == CarriageReturnCharacter)
		{
			//Do nothing
		}
		else
		{
			ReturnBuffer[ReturnBufferCount] = NewChar;
			ReturnBufferCount++;
			if (ReturnBufferCount >= ReturnBufferSize)
			{
				Serial.print("<ZABERBUFOVERFLOW>(");
				Serial.print(ReturnBuffer);
				Serial.print(")\n");
				ClearReturnBuffer();
			}
		}
	}
}
void ZaberMaster::CheckExpectingAlert()
{
	bool UpdateExpectingAlert = false;
	for (uint8_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
	{
		for (uint8_t AxisIndex = 0; AxisIndex < AxesFound[DeviceIndex]; AxisIndex++)
		{
			if (AxisProperties[DeviceIndex][AxisIndex].ExpectingAlert)
			{
				UpdateExpectingAlert = true;
				break;
			}
			if (UpdateExpectingAlert)
			{
				break;
			}
		}
	}
	ExpectingAlert = UpdateExpectingAlert;
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
						if (AlertEnabled)
						{
							AxisProperties[DeviceIndex][AxisIndex].ExpectingAlert = true;
						}
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
void ZaberMaster::CheckPollingStatus()
{
	bool AnyPollingUpdate = false;
	for (size_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
	{
		for (size_t AxisIndex = 0; AxisIndex < AxesFound[DeviceIndex]; AxisIndex++)
		{
			if (AxisProperties[DeviceIndex][ReturnMessage.Axis - 1].Polling)
			{
				AnyPollingUpdate = true;
				break;
			}
		}
		if (AnyPollingUpdate)
		{
			break;
		}
	}
	PollingAny = AnyPollingUpdate;
}
void ZaberMaster::SetStatus()
{
	bool IsBusy = ReturnMessage.Status == StatusType::Busy;
	bool TransitionToBusy = !ExpectingAlert && !PollingAny && IsBusy;
	bool NeedToExpectAlert = IsBusy && AlertEnabled;
	bool StartPolling = IsBusy && PollingEnabled;
	bool NeedToSetExternalCallback = (CurrentCommand.Callback != NULL) && (NeedToExpectAlert);
	if (ReturnMessage.Device == 0)
	{
		if (ReturnMessage.Axis == 0)
		{
			for (size_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
			{
				for (size_t AxisIndex = 0; AxisIndex < AxesFound[DeviceIndex]; AxisIndex++)
				{
					AxisProperties[DeviceIndex][AxisIndex].Status = ReturnMessage.Status;
					AxisProperties[DeviceIndex][AxisIndex].Polling = StartPolling;
					AxisProperties[DeviceIndex][AxisIndex].PollingSent = false;
					AxisProperties[DeviceIndex][AxisIndex].LastPollTime = micros();
					if (NeedToExpectAlert)
					{
						AxisProperties[DeviceIndex][AxisIndex].ExpectingAlert = true;
					}
				}
			}
		}
		else
		{
			for (size_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
			{
				AxisProperties[DeviceIndex][ReturnMessage.Axis - 1].Status = ReturnMessage.Status;
				AxisProperties[DeviceIndex][ReturnMessage.Axis - 1].Polling = StartPolling;
				AxisProperties[DeviceIndex][ReturnMessage.Axis - 1].PollingSent = false;
				AxisProperties[DeviceIndex][ReturnMessage.Axis - 1].LastPollTime = micros();
				if (NeedToExpectAlert)
				{
					AxisProperties[DeviceIndex][ReturnMessage.Axis - 1].ExpectingAlert = true;
				}
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
				AxisProperties[ReturnMessage.Device - 1][AxisIndex].Polling = StartPolling;
				AxisProperties[ReturnMessage.Device - 1][AxisIndex].PollingSent = false;
				AxisProperties[ReturnMessage.Device - 1][AxisIndex].LastPollTime = micros();
				if (NeedToExpectAlert)
				{
					AxisProperties[ReturnMessage.Device - 1][AxisIndex].ExpectingAlert = true;
				}
			}
		}
		else
		{
			AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].Status = ReturnMessage.Status;
			AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].Polling = StartPolling;
			AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].PollingSent = false;
			AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].LastPollTime = micros();
			if (NeedToExpectAlert)
			{
				AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].ExpectingAlert = true;
			}
			if (NeedToSetExternalCallback && (AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].ExternalCallback == NULL) )
			{
				AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].ExternalCallback = CurrentCommand.Callback;
				CurrentCommand.Callback = NULL;
				if (Verbose)
				{
					Serial.print("<ZABER>(Reassign callback ");
					Serial.print(ReturnMessage.Device);
					Serial.print(",");
					Serial.print(ReturnMessage.Axis);
					Serial.print(")\n");
				}
			}
		}
	}
	CheckPollingStatus();
	CheckExpectingAlert();
	if (Verbose)
	{
		Serial.print("<ZABER>(Set status ");
		Serial.print(ReturnMessage.Device);
		Serial.print(",");
		Serial.print(ReturnMessage.Axis);
		Serial.print(",");
		Serial.print(toString(ReturnMessage.Status));
		Serial.print(",");
		Serial.print(IsBusy);
		Serial.print(",");
		Serial.print(NeedToExpectAlert);
		Serial.print(",");
		Serial.print(TransitionToBusy);
		Serial.print(")\n");
	}
}
void ZaberMaster::ProcessReplyMessage()
{
	if (Verbose)
	{
		
	}
	SetStatus();
	if (ReturnMessage.Type == MessageType::Reply)
	{
		if (Mode == ModeType::WaitForCommandReply)
		{
			if (CurrentReceivedCallback != NULL)
			{
				CurrentReceivedCallback();
				CurrentReceivedCallback = NULL;
			}
			if (Initializing && !PollingAny && !ExpectingAlert)
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
			}
			if (ExpectingAlert)
			{
				Mode = ModeType::WaitForAlert;
			}
			else
			{
				Mode = ModeType::Idle;
			}
		}
	}
	if ( (ReturnMessage.Status == StatusType::Idle) )
	{
		if (CurrentCommand.Callback != NULL)
		{
			CurrentCommand.Callback();
			CurrentCommand.Callback = NULL;
		}
		if ( (ReturnMessage.Device > 0) && (ReturnMessage.Axis > 0) && (AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].ExternalCallback != NULL) )
		{
			AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].ExternalCallback();
			AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].ExternalCallback = NULL;
		}
	}
	if (ReturnMessage.Warning != WarningType::None)
	{
		if (LastWarning != ReturnMessage.Warning)
		{
			Serial.print("<STAGE>(Error:");
			Serial.print(static_cast<uint8_t>(ReturnMessage.Warning));
			Serial.print(",");
			Serial.print(static_cast<uint8_t>(LastWarning));
			Serial.print(",");
			Serial.print(WarningIdentifier[static_cast<uint8_t>(ReturnMessage.Warning)].String);
			Serial.print(")\n");
			LastWarning = ReturnMessage.Warning;
		}
		//SendClearWarnings(ReturnMessage.Device);
	}
	else
	{
		LastWarning = WarningType::None;
	}
}
void ZaberMaster::ProcessAlertMessage()
{
	if (Verbose)
	{
		
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
				AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].Polling = false;
			}
		}
		SetStatus();
		if (!ExpectingAlert && !PollingAny)
		{
			if (Initializing)
			{
				if (InitializationStep < InitializationStepsCount)
				{
					InitializationStep++;
					RunNextInitializationStep();
				}
			}
			else if (!Initializing && !Initialized)
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
			if (CurrentCommand.Parameters[0].Value.Setting == SettingMessageType::EncoderPos)
			{
				if (Verbose)
				{
					Serial.print("<ZABER>(Encoder position update: ");
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
							AxisProperties[ReturnMessage.Device - 1][Index].EncoderPosition = ReturnMessage.Data[Index].Value.Integer;
							if (Verbose)
							{
								Serial.print(",");
								Serial.print(AxisProperties[ReturnMessage.Device - 1][Index].EncoderPosition);
							}
						}
						if (Verbose)
						{
							Serial.print(")\n");
						}
					}
					else
					{
						AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].EncoderPosition = ReturnMessage.Data[0].Value.Integer;
						if (Verbose)
						{
							Serial.print(",");
							Serial.print(AxisProperties[ReturnMessage.Device - 1][ReturnMessage.Axis - 1].EncoderPosition);
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
		if (Verbose)
		{
			Serial.print("<ZABERDEVICEFOUND>");
			Serial.print(ReturnMessage.Device);
			Serial.print(")\n");
		}
		DevicesFound = ReturnMessage.Device;
	}
}
void ZaberMaster::ProcessMoveStarted()
{
	if (AlertEnabled)
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
	_StreamPort->write(CommandMarkerCharacter);
	CharCount = IntToCharPointer(CurrentCommand.Device, CommandBuffer, CommandBufferSize);
	_StreamPort->write(reinterpret_cast<const uint8_t*>(CommandBuffer),CharCount);
	_StreamPort->write(SpaceCharacter);
	if (Verbose)
	{
		Serial.print("<ZABER>(OUT: ");
		Serial.print(CommandMarkerCharacter);
		Serial.print(reinterpret_cast<const char*>(CommandBuffer));
		Serial.print(SpaceCharacter);
	}
	memset(CommandBuffer,0,CommandBufferSize);
	CharCount = IntToCharPointer(CurrentCommand.Axis, CommandBuffer, CommandBufferSize);
	_StreamPort->write(reinterpret_cast<const uint8_t*>(CommandBuffer),CharCount);
	_StreamPort->write(SpaceCharacter);
	_StreamPort->write(StringToSend,CommandSize);
	if (Verbose)
	{
		Serial.print(reinterpret_cast<const char*>(CommandBuffer));
		Serial.print(SpaceCharacter);
		Serial.print(reinterpret_cast<const char*>(StringToSend));
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
					_StreamPort->write(SpaceCharacter);
					_StreamPort->write(reinterpret_cast<const uint8_t*>(CommandBuffer),CharCount);
					if (Verbose)
					{
						Serial.print(SpaceCharacter);
						Serial.print(reinterpret_cast<const char*>(CommandBuffer));
					}
					break;
				}
				case CommandParameterType::Float:
				{
					uint8_t CharCount = FloatToCharPointer(CurrentCommand.Parameters[ParameterIndex].Value.Float, CommandBuffer, CommandBufferSize);
					_StreamPort->write(SpaceCharacter);
					_StreamPort->write(reinterpret_cast<const uint8_t*>(CommandBuffer),CharCount);
					if (Verbose)
					{
						Serial.print(SpaceCharacter);
						Serial.print(reinterpret_cast<const char*>(CommandBuffer));
					}
					break;
				}
				case CommandParameterType::Parameter:
				{
					const uint8_t ParameterIdentifierIndex = static_cast<uint8_t>(CurrentCommand.Parameters[ParameterIndex].Value.Parameter);
					const uint8_t* ParameterMessageString = reinterpret_cast<const uint8_t*>(ParameterIdentifier[ParameterIdentifierIndex].String);
					const uint8_t StringCount = ParameterIdentifier[ParameterIdentifierIndex].Count;
					_StreamPort->write(SpaceCharacter);
					_StreamPort->write(ParameterMessageString,StringCount);
					if (Verbose)
					{
						Serial.print(SpaceCharacter);
						Serial.print(reinterpret_cast<const char*>(ParameterMessageString));
					}
					break;
				}
				case CommandParameterType::Setting:
				{
					const uint8_t SettingIdentifierIndex = static_cast<uint8_t>(CurrentCommand.Parameters[ParameterIndex].Value.Setting);
					const uint8_t* SettingMessageString = reinterpret_cast<const uint8_t*>(SettingIdentifier[SettingIdentifierIndex].String);
					const uint8_t StringCount = SettingIdentifier[SettingIdentifierIndex].Count;
					_StreamPort->write(SpaceCharacter);
					_StreamPort->write(SettingMessageString,StringCount);
					if (Verbose)
					{
						Serial.print(SpaceCharacter);
						Serial.print(reinterpret_cast<const char*>(SettingMessageString));
					}
					break;
				}
				default:
					break;
			}
		}
	}
	_StreamPort->write(EndOfLineCharacter);
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
	ReturnBufferCount = 0;
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
		
	}
	ClearReturnBuffer();
	ResetReturnMessage();
	Mode = ModeType::Idle;
	CurrentReplyPart = ReplyParts::Type;
}
