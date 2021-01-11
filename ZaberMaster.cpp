#include "ZaberMaster.h"

const char ZaberMaster::CarriageReturnCharacter = '\r';
const char ZaberMaster::EndOfLineCharacter = '\n';
const char ZaberMaster::SpaceCharacter = ' ';
const char ZaberMaster::CommandMarkerCharacter = '/';
const char ZaberMaster::GetCharacter = '?';
const uint8_t ZaberMaster::InitializationStepsCount = 6;
const ZaberMaster::CommandMessage ZaberMaster::InitilizationSteps[]=
{
	{CommandMessageType::Warnings,false,false,0,0,0,{{{.Parameter=ParameterMessageType::Clear},CommandParameterType::Parameter},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},1},
	{CommandMessageType::Renumber,false,false,0,0,0,{{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},0},
	{CommandMessageType::Get,false,false,0,0,0,{{{.Setting=SettingMessageType::SystemAxisCount},CommandParameterType::Setting},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},1 },
	//{CommandMessageType::Set,false,false,0,0,0,{{{.Setting=SettingMessageType::Pos},CommandParameterType::Setting},{{.Integer = 0},CommandParameterType::Integer},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},2},
	{CommandMessageType::Set,false,false,0,0,0,{{{.Setting=SettingMessageType::MaxSpeed},CommandParameterType::Setting},{{.Integer = ZaberDefaultSpeed},CommandParameterType::Integer},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},2},
	{CommandMessageType::Set,false,false,0,0,0,{{{.Setting=SettingMessageType::CommAlert},CommandParameterType::Setting},{{.Integer = 1},CommandParameterType::Integer},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},2},
	{CommandMessageType::ToolsFindRange,false,false,0,0,0,{{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None},{0,CommandParameterType::None}},0},
};
const ZaberMaster::CommandString ZaberMaster::CommandIdentifier[]=
{
	{CommandMessageType::EStop,"estop",5,CommandScope::Axis, &ZaberMaster::ProcessMoveStarted, NULL},
	{CommandMessageType::Get,"get",3,CommandScope::Either, NULL, &ZaberMaster::ProcessGetReceived },
	{CommandMessageType::Home,"home",4,CommandScope::Axis, &ZaberMaster::ProcessMoveStarted, NULL },
	{CommandMessageType::IOInfo,"io info",7,CommandScope::Device, NULL, NULL },
	{CommandMessageType::IOGet,"io get",6,CommandScope::Device, NULL, NULL },
	{CommandMessageType::IOSet,"io set",6,CommandScope::Device, NULL, NULL },
	{CommandMessageType::Joystick,"joystick",8,CommandScope::Device, NULL, NULL },
	{CommandMessageType::Key,"key",3,CommandScope::Device, NULL, NULL },
	{CommandMessageType::LockStep,"lockstep",8,CommandScope::Device, NULL, NULL },
	{CommandMessageType::Move,"move",4,CommandScope::Axis, &ZaberMaster::ProcessMoveStarted, NULL },
	{CommandMessageType::Renumber,"renumber",8,CommandScope::Device, NULL, &ZaberMaster::ProcessRenumberReceived },
	{CommandMessageType::Set,"set",3,CommandScope::Either, NULL, NULL },
	{CommandMessageType::Stop,"stop",4,CommandScope::Axis, &ZaberMaster::ProcessMoveStarted, NULL },
	{CommandMessageType::Stream,"stream",6,CommandScope::Device, NULL, NULL },
	{CommandMessageType::SystemReset,"system reset",12,CommandScope::Device, NULL, NULL },
	{CommandMessageType::SystemRestore,"system restore",14,CommandScope::Device, NULL, NULL },
	{CommandMessageType::ToolsEcho,"tools echo",10,CommandScope::Device, NULL, NULL },
	{CommandMessageType::ToolsFindRange,"tools findrange",15,CommandScope::Axis, &ZaberMaster::ProcessMoveStarted, NULL },
	{CommandMessageType::ToolsGoToLimit,"tools gotolimit",15,CommandScope::Axis, &ZaberMaster::ProcessMoveStarted, NULL },
	{CommandMessageType::ToolsParking,"tools parking",13,CommandScope::Device, NULL, NULL },
	{CommandMessageType::ToolsSetComm,"tools setcomm",13,CommandScope::Device, NULL, NULL },
	{CommandMessageType::ToolsStorePos,"tools storepos",14,CommandScope::Axis, NULL, NULL },
	{CommandMessageType::Trigger,"trigger",7,CommandScope::Device, NULL, NULL },
	{CommandMessageType::TriggerDist,"trigger dist",12,CommandScope::Device, NULL, NULL },
	{CommandMessageType::TriggerTime,"trigger time",12,CommandScope::Device, NULL, NULL },
	{CommandMessageType::Virtual,"virtual",7,CommandScope::Device, NULL, NULL },
	{CommandMessageType::Warnings,"warnings",8,CommandScope::Either, NULL, NULL }
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

ZaberMaster::ZaberMaster(HardwareSerial* serial)
{
	_HardwareSerial = serial;
	Initialized = false;
	InitializationStep = 0;
	DevicesFound = 0;
	memset(AxesFound,0,sizeof(AxesFound));
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = 0;
	CurrentCommand.Device = 0;
	CurrentCommand.StartTime = 0;
	CurrentCommand.ParameterCount = 0;
	for (uint8_t Index = 0; Index < ZaberParameterMaxLength; Index++)
	{
		CurrentCommand.Parameters[Index].Type = CommandParameterType::None;
		CurrentCommand.Parameters[Index].Value.Integer = 0;
	}
	memset(CommandBuffer,0,sizeof(CommandBuffer));
	LastTransmitionTime = 0;
	SendingCommand = false;
	ExpectingReply = false;
	memset(ExpectingAlert,0,sizeof(ExpectingAlert));
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
			LastPosition[DeviceIndex][AxisIndex] = 0;
			LastMaxSpeed[DeviceIndex][AxisIndex] = 0;
			LastStatus[DeviceIndex][AxisIndex] = StatusType::Unknown;
		}
	}
	ReturnMessage.Flag = FlagType::Okay;
	ReturnMessage.Status = StatusType::Idle;
	ReturnMessage.Warning = WarningType::None;
	ReturnMessage.DataLength = 0;
	ReturnMessage.MessageComplete = false;
	ReturnMessage.MessageStarted = false;
	ReturnMessage.Axis = 0;
	ReturnMessage.Device = 0;
	ReturnMessage.StartTime = 0;
	ReturnMessageDataTypeDetermined = false;
	CurrentReplyPart = ReplyParts::Type;
	RecievingReply = false;
	RecieveStartTime = 0;
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
	//Serial.print("Begin initializing.");
	if (!Initializing)
	{
		Initializing = true;
		InitializationStep = 0;
		Initialized = false;
		DevicesFound = 0;
		for (size_t Index = 0; Index < ZaberMaxDevices; Index++)
		{
			AxesFound[Index] = 0;
		}
		RunNextInitializationStep();
	}
	return true;
}
bool ZaberMaster::SendRenumber()
{
	CurrentCommand.Command = CommandMessageType::Renumber;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = 0;
	CurrentCommand.Device = 0;
	CurrentCommand.ParameterCount = 0;
	SendCommand();
	return true;
}
bool ZaberMaster::SendEStop(uint8_t Device, uint8_t Axis)
{
	CurrentCommand.Command = CommandMessageType::EStop;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.ParameterCount = 0;
	SendCommand();
	return true;
}
bool ZaberMaster::SendStop(uint8_t Device, uint8_t Axis)
{
	CurrentCommand.Command = CommandMessageType::EStop;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.ParameterCount = 0;
	SendCommand();
	return true;
}
bool ZaberMaster::SendHome(uint8_t Device, uint8_t Axis)
{
	CurrentCommand.Command = CommandMessageType::Home;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.ParameterCount = 0;
	SetExpectingAlerts(Device, Axis);
	SendCommand();
	return true;
}
bool ZaberMaster::SendSystemReset(uint8_t Device)
{
	CurrentCommand.Command = CommandMessageType::SystemReset;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = 0;
	CurrentCommand.Device = Device;
	CurrentCommand.ParameterCount = 0;
	SendCommand();
	return true;
}
bool ZaberMaster::SendSystemRestore(uint8_t Device)
{
	CurrentCommand.Command = CommandMessageType::SystemRestore;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = 0;
	CurrentCommand.Device = Device;
	CurrentCommand.ParameterCount = 0;
	SendCommand();
	return true;
}
bool ZaberMaster::SendClearWarnings(uint8_t Device)
{
	CurrentCommand.Command = CommandMessageType::Warnings;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = 0;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Parameter;
	CurrentCommand.Parameters[0].Value.Parameter = ParameterMessageType::Clear;
	CurrentCommand.ParameterCount = 1;
	SendCommand();
	return true;
}
bool ZaberMaster::SendFindRange(uint8_t Device, uint8_t Axis)
{
	CurrentCommand.Command = CommandMessageType::ToolsFindRange;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.ParameterCount = 0;
	SetExpectingAlerts(Device, Axis);
	SendCommand();
	return true;
}
bool ZaberMaster::SendMoveRel(uint8_t Device, uint8_t Axis, uint32_t Steps)
{
	CurrentCommand.Command = CommandMessageType::Move;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Parameter;
	CurrentCommand.Parameters[0].Value.Parameter = ParameterMessageType::Rel;
	CurrentCommand.Parameters[1].Type = CommandParameterType::Integer;
	CurrentCommand.Parameters[1].Value.Integer = Steps;
	CurrentCommand.ParameterCount = 2;
	SetExpectingAlerts(Device, Axis);
	SendCommand();
	return true;
}
bool ZaberMaster::SendMoveAbs(uint8_t Device, uint8_t Axis, uint32_t Steps)
{
	CurrentCommand.Command = CommandMessageType::Move;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Parameter;
	CurrentCommand.Parameters[0].Value.Parameter = ParameterMessageType::Abs;
	CurrentCommand.Parameters[1].Type = CommandParameterType::Integer;
	CurrentCommand.Parameters[1].Value.Integer = Steps;
	CurrentCommand.ParameterCount = 2;
	SetExpectingAlerts(Device, Axis);
	//Serial.print("mi");
	//Serial.print(Steps);
	//Serial.print("mi");
	SendCommand();
	return true;
}
void ZaberMaster::SetExpectingAlerts(uint8_t Device, uint8_t Axis)
{
	if (Device > 0)
	{
		if (Axis > 0)
		{
			ExpectingAlert[Device - 1][Axis - 1] = true;
		}
	}
	else
	{
		if (Axis > 0)
		{
			for (size_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
			{
				ExpectingAlert[DeviceIndex][Axis - 1] = true;
			}
		}
		else
		{
			for (size_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
			{
				for (size_t AxisIndex = 0; AxisIndex < AxesFound[DeviceIndex]; AxisIndex++)
				{
					ExpectingAlert[DeviceIndex][Axis - 1] = true;
				}
				ExpectingAlert[DeviceIndex][Axis - 1] = true;
			}
		}
	}
}
bool ZaberMaster::SendSetAcceleration(uint8_t Device, uint8_t Axis, uint32_t Acceleration)
{
	CurrentCommand.Command = CommandMessageType::Set;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Setting;
	CurrentCommand.Parameters[0].Value.Setting = SettingMessageType::Accel;
	CurrentCommand.Parameters[1].Type = CommandParameterType::Integer;
	CurrentCommand.Parameters[1].Value.Integer = Acceleration;
	CurrentCommand.ParameterCount = 2;
	SendCommand();
	return true;
}
bool ZaberMaster::SendGetAcceleration(uint8_t Device, uint8_t Axis)
{
	CurrentCommand.Command = CommandMessageType::Get;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Setting;
	CurrentCommand.Parameters[0].Value.Setting = SettingMessageType::Accel;
	CurrentCommand.ParameterCount = 1;
	SendCommand();
	return true;
}
bool ZaberMaster::SendSetMaxSpeed(uint8_t Device, uint8_t Axis, uint32_t MaxSpeed)
{
	CurrentCommand.Command = CommandMessageType::Set;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Setting;
	CurrentCommand.Parameters[0].Value.Setting = SettingMessageType::MaxSpeed;
	CurrentCommand.Parameters[1].Type = CommandParameterType::Integer;
	CurrentCommand.Parameters[1].Value.Integer = MaxSpeed;
	CurrentCommand.ParameterCount = 2;
	SendCommand();
	return true;
}
bool ZaberMaster::SendGetMaxSpeed(uint8_t Device, uint8_t Axis)
{
	CurrentCommand.Command = CommandMessageType::Get;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Setting;
	CurrentCommand.Parameters[0].Value.Setting = SettingMessageType::MaxSpeed;
	CurrentCommand.ParameterCount = 1;
	SendCommand();
	return true;
}
bool ZaberMaster::SendSetLimitMax(uint8_t Device, uint8_t Axis, uint32_t LimitMax)
{
	CurrentCommand.Command = CommandMessageType::Set;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Setting;
	CurrentCommand.Parameters[0].Value.Setting = SettingMessageType::LimitMax;
	CurrentCommand.Parameters[1].Type = CommandParameterType::Integer;
	CurrentCommand.Parameters[1].Value.Integer = LimitMax;
	CurrentCommand.ParameterCount = 2;
	SendCommand();
	return true;
}
bool ZaberMaster::SendGetLimitMax(uint8_t Device, uint8_t Axis)
{
	CurrentCommand.Command = CommandMessageType::Get;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Setting;
	CurrentCommand.Parameters[0].Value.Setting = SettingMessageType::LimitMax;
	CurrentCommand.ParameterCount = 1;
	SendCommand();
	return true;
}
bool ZaberMaster::SendSetLimitMin(uint8_t Device, uint8_t Axis, uint32_t LimitMin)
{
	CurrentCommand.Command = CommandMessageType::Set;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Setting;
	CurrentCommand.Parameters[0].Value.Setting = SettingMessageType::LimitMin;
	CurrentCommand.Parameters[1].Type = CommandParameterType::Integer;
	CurrentCommand.Parameters[1].Value.Integer = LimitMin;
	CurrentCommand.ParameterCount = 2;
	SendCommand();
	return true;
}
bool ZaberMaster::SendGetLimitMin(uint8_t Device, uint8_t Axis)
{
	CurrentCommand.Command = CommandMessageType::Get;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Setting;
	CurrentCommand.Parameters[0].Value.Setting = SettingMessageType::LimitMin;
	CurrentCommand.ParameterCount = 1;
	SendCommand();
	return true;
}
bool ZaberMaster::SendSetAlertStatus(uint8_t Device, bool Enable)
{
	CurrentCommand.Command = CommandMessageType::Set;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = 0;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Setting;
	CurrentCommand.Parameters[0].Value.Setting = SettingMessageType::CommAlert;
	CurrentCommand.Parameters[1].Type = CommandParameterType::Integer;
	CurrentCommand.Parameters[1].Value.Integer = (Enable) ? 1 : 0;
	CurrentCommand.ParameterCount = 2;
	SendCommand();
	return true;
}
bool ZaberMaster::SendGetAlertStatus(uint8_t Device)
{
	CurrentCommand.Command = CommandMessageType::Get;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = 0;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Setting;
	CurrentCommand.Parameters[0].Value.Setting = SettingMessageType::CommAlert;
	CurrentCommand.ParameterCount = 1;
	SendCommand();
	return true;
}
bool ZaberMaster::SendSetKnobEnable(uint8_t Device, uint8_t Axis, bool Enable)
{
	CurrentCommand.Command = CommandMessageType::Set;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Setting;
	CurrentCommand.Parameters[0].Value.Setting = SettingMessageType::KnobEnable;
	CurrentCommand.Parameters[1].Type = CommandParameterType::Integer;
	CurrentCommand.Parameters[1].Value.Integer = (Enable) ? 1 : 0;
	CurrentCommand.ParameterCount = 2;
	SendCommand();
	return true;
}
bool ZaberMaster::SendGetKnobEnable(uint8_t Device, uint8_t Axis)
{
	CurrentCommand.Command = CommandMessageType::Get;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Setting;
	CurrentCommand.Parameters[0].Value.Setting = SettingMessageType::KnobEnable;
	CurrentCommand.ParameterCount = 1;
	SendCommand();
	return true;
}
bool ZaberMaster::SendSetParked(uint8_t Device, uint8_t Axis, bool Enable)
{
	CurrentCommand.Command = CommandMessageType::ToolsParking;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = 0;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Parameter;
	CurrentCommand.Parameters[0].Value.Parameter = (Enable) ? ParameterMessageType::Park : ParameterMessageType::Unpark;
	CurrentCommand.ParameterCount = 1;
	SendCommand();
	return true;
}
bool ZaberMaster::SendSetResolution(uint8_t Device, uint8_t Axis, uint8_t Resolution)
{
	CurrentCommand.Command = CommandMessageType::Set;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Setting;
	CurrentCommand.Parameters[0].Value.Setting = SettingMessageType::Resolution;
	CurrentCommand.Parameters[1].Type = CommandParameterType::Integer;
	CurrentCommand.Parameters[1].Value.Integer = Resolution;
	CurrentCommand.ParameterCount = 2;
	SendCommand();
	return true;
}
bool ZaberMaster::SendGetResolution(uint8_t Device, uint8_t Axis)
{
	CurrentCommand.Command = CommandMessageType::Get;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Setting;
	CurrentCommand.Parameters[0].Value.Setting = SettingMessageType::Resolution;
	CurrentCommand.ParameterCount = 1;
	SendCommand();
	return true;
}
bool ZaberMaster::SendGetPosition(uint8_t Device, uint8_t Axis)
{
	CurrentCommand.Command = CommandMessageType::Get;
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = Axis;
	CurrentCommand.Device = Device;
	CurrentCommand.Parameters[0].Type = CommandParameterType::Setting;
	CurrentCommand.Parameters[0].Value.Setting = SettingMessageType::Pos;
	CurrentCommand.ParameterCount = 1;
	SendCommand();
	return true;
}
bool ZaberMaster::GetPosition(uint8_t Device, uint8_t Axis, uint32_t* Position)
{
	if (Device <= ZaberMaxDevices && Axis <= ZaberMaxAxes)
	{
		if (Device < DevicesFound)
		{
			if (Axis < AxesFound[Device-1])
			{
				*Position = LastPosition[Device-1][Axis-1];
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
				*MaxSpeed = LastMaxSpeed[Device-1][Axis-1];
				return true;
			}
		}
	}
	return false;
}
bool ZaberMaster::GetIsBusy(uint8_t Device, uint8_t Axis, StatusType* Status)
{
	Serial.print("Z:");
	Serial.print(Device);
	Serial.print(",");
	Serial.print(Axis);
	Serial.print(",");
	Serial.print(ZaberMaxDevices);
	Serial.print(",");
	Serial.print(ZaberMaxAxes);
	Serial.print(",");
	if ( (Device <= ZaberMaxDevices) && (Axis <= ZaberMaxAxes) )
	{
		Serial.print(DevicesFound);
		Serial.print(",");
		if (Device <= DevicesFound)
		{
			Serial.print(AxesFound[Device-1]);
			Serial.print(",");
			if (Axis <= AxesFound[Device-1])
			{
				Serial.print( StatusIdentifier[static_cast<uint8_t>(LastStatus[Device-1][Axis-1])].Flag );
				Serial.print("\n");
				*Status = LastStatus[Device-1][Axis-1];
				//if (LastStatus[Device-1][Axis-1] == StatusType::Busy)
				//if (StatusType::Busy == StatusType::Busy)
				//{
				//	return true;
				//}
				//else
				//{
				//	return false;
				//}
				return true;
			}
		}
	}
	Serial.print("<STAGE>(Get status error device axis not found.)\n");
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
void ZaberMaster::CheckSerial()
{
	if (_HardwareSerial->available() > 0)
	{
		char Character = _HardwareSerial->read();
		//Serial.write(Character);
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
void ZaberMaster::ParseType(char Character)
{
	bool Found = false;
	for (uint8_t Index = 0; Index < static_cast<uint8_t>(MessageType::Count); Index++)
	{
		if (Character == MessageIdentifier[Index].Identifier)
		{
			//Serial.print('f');
			//Serial.print(MessageIdentifier[Index].Identifier);
			//Serial.print('f');
			ReturnMessage.Type = MessageIdentifier[Index].Type;
			ReturnMessage.StartTime = micros();
			ReturnMessage.MessageStarted = true;
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
			//Serial.write('d');
			//Serial.print(DeviceNumber);
			//Serial.write('d');
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
			//Serial.write('a');
			//Serial.print(AxisNumber);
			//Serial.write('a');
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
				//Serial.write('f');
				//Serial.print(FlagIdentifier[Index].Flag);
				//Serial.write('f');
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
				//Serial.write('s');
				//Serial.print(StatusIdentifier[Index].Flag);
				//Serial.write('s');
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
				//Serial.write('w');
				//Serial.print(WarningIdentifier[Index].Flag);
				//Serial.write('w');
				Found = true;
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
			if (ReturnMessage.DataLength < 3)
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
					//Serial.write('d');
					//Serial.print(ReturnMessage.Data[ReturnMessage.DataLength].Value.Integer);
					//Serial.write('d');
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
void ZaberMaster::ParseCharacterForAlert(char Character)
{
	//Serial.print("a");
	switch(CurrentReplyPart)
	{
		case ReplyParts::Device:
			//Serial.print("d");
			ParseDevice(Character, ReplyParts::Axis);
			break;
		case ReplyParts::Axis:
			//Serial.print("b");
			ParseAxis(Character, ReplyParts::Status);
			break;
		case ReplyParts::Status:
			//Serial.print("s");
			ParseStatus(Character, ReplyParts::Warning);
			break;
		case ReplyParts::Warning:
			//Serial.print("w");
			ParseWarning(Character, ReplyParts::Type);
			break;
		default:
			break;
	}
}
void ZaberMaster::ParseCharacterForReply(char Character)
{
	//Serial.print('r');
	switch(CurrentReplyPart)
	{
		case ReplyParts::Type:
			//Serial.print('t');
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
		//Serial.print("pa");
		if (ReturnMessage.Device > 0)
		{
			if (ReturnMessage.Axis > 0)
			{
				//Serial.print("D:");
				//Serial.print(ReturnMessage.Device);
				//Serial.print(":");
				//Serial.print(ReturnMessage.Axis);
				//Serial.print(";\n");
				ExpectingAlert[ReturnMessage.Device - 1][ReturnMessage.Axis - 1] = false;
			}
		}
		SetLastStatus();
		bool AlertsComplete = true;
		for (uint8_t DeviceIndex = 0; DeviceIndex < ZaberMaxDevices; DeviceIndex++)
		{
			for (uint8_t AxisIndex = 0; AxisIndex < ZaberMaxAxes; AxisIndex++)
			{
				if (ExpectingAlert[DeviceIndex][AxisIndex] == true)
				{
					AlertsComplete = false;
					break;
				}
			}
			if (!AlertsComplete)
			{
				break;
			}
		}
		if (AlertsComplete)
		{
			if (Initializing)
			{
				//Serial.print("initd");
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
			//Serial.print(" On step: ");
			//Serial.println(InitializationStep);
			CurrentCommand.Command = InitilizationSteps[InitializationStep].Command;
			CurrentCommand.MessageComplete = InitilizationSteps[InitializationStep].MessageComplete;
			CurrentCommand.MessageStarted = InitilizationSteps[InitializationStep].MessageStarted;
			CurrentCommand.Axis = InitilizationSteps[InitializationStep].Axis;
			CurrentCommand.Device = InitilizationSteps[InitializationStep].Device;
			for (size_t ParameterIndex = 0; ParameterIndex < InitilizationSteps[InitializationStep].ParameterCount; ParameterIndex++)
			{
				CurrentCommand.Parameters[ParameterIndex].Type = InitilizationSteps[InitializationStep].Parameters[ParameterIndex].Type;
				switch (CurrentCommand.Parameters[ParameterIndex].Type)
				{
				case CommandParameterType::None:
					break;
				case CommandParameterType::Integer:
					CurrentCommand.Parameters[ParameterIndex].Value.Integer = InitilizationSteps[InitializationStep].Parameters[ParameterIndex].Value.Integer;
					break;
				case CommandParameterType::Float:
					CurrentCommand.Parameters[ParameterIndex].Value.Float = InitilizationSteps[InitializationStep].Parameters[ParameterIndex].Value.Float;
					break;
				case CommandParameterType::Parameter:
					CurrentCommand.Parameters[ParameterIndex].Value.Parameter = InitilizationSteps[InitializationStep].Parameters[ParameterIndex].Value.Parameter;
					break;
				case CommandParameterType::Setting:
					CurrentCommand.Parameters[ParameterIndex].Value.Setting = InitilizationSteps[InitializationStep].Parameters[ParameterIndex].Value.Setting;
					break;
				default:
					break;
				}

			}
			if (CurrentCommand.Command == CommandMessageType::ToolsFindRange)
			{
				//Serial.print("ea");
				for (size_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
				{
					for (size_t AxisIndex = 0; AxisIndex < AxesFound[DeviceIndex]; AxisIndex++)
					{
						ExpectingAlert[DeviceIndex][AxisIndex] = true;
					}
				}
			}
			CurrentCommand.ParameterCount = InitilizationSteps[InitializationStep].ParameterCount;
			SendCommand();
		}
		else
		{
			Initializing = false;
			Initialized = true;
			if (InitializationComplete != NULL)
			{
				InitializationComplete();
			}
		}
	}
}
void ZaberMaster::SetLastStatus()
{
	if (ReturnMessage.Device == 0)
	{
		if (ReturnMessage.Axis == 0)
		{
			for (size_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
			{
				for (size_t AxisIndex = 0; AxisIndex < AxesFound[DeviceIndex]; AxisIndex++)
				{
					LastStatus[DeviceIndex][AxisIndex] = ReturnMessage.Status;
				}
			}
		}
		else
		{
			for (size_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
			{
				LastStatus[DeviceIndex][ReturnMessage.Axis - 1] = ReturnMessage.Status;
			}
		}
	}
	else
	{
		if (CurrentCommand.Axis == 0)
		{
			for (size_t AxisIndex = 0; AxisIndex < AxesFound[CurrentCommand.Device - 1]; AxisIndex++)
			{
				LastStatus[ReturnMessage.Device - 1][AxisIndex] = ReturnMessage.Status;
			}
		}
		else
		{
			LastStatus[ReturnMessage.Device - 1][ReturnMessage.Axis - 1] = ReturnMessage.Status;
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
	SetLastStatus();
	if (ReturnMessage.Type == MessageType::Reply)
	{
		if (ExpectingReply)
		{
			ExpectingReply = false;
			if (CurrentReceivedCallback != NULL)
			{
				(this->*CurrentReceivedCallback)();
			}
			if (Initializing)
			{
				if (InitializationStep < InitializationStepsCount)
				{
					//Serial.print("rn");
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
		}
	}
	if (ReturnMessage.Warning != WarningType::None)
	{
		Serial.print("<STAGE>(Error: ");
		Serial.print(WarningIdentifier[static_cast<uint8_t>(ReturnMessage.Warning)].String);
		Serial.print(")\n");
	}
}
void ZaberMaster::ProcessGetReceived()
{
	//Serial.print("gcb");
	//Serial.print(CurrentCommand.ParameterCount);
	if (CurrentCommand.ParameterCount > 0)
	{
		if (CurrentCommand.Parameters[0].Type == CommandParameterType::Parameter)
		{
			//Serial.print("pt");
			if (CurrentCommand.Parameters[0].Value.Parameter == ParameterMessageType::Pos)
			{
				if (ReturnMessage.DataLength == 1)
				{
					if (ReturnMessage.Device > 0)
					{
						LastPosition[ReturnMessage.Device - 1][ReturnMessage.Axis - 1] = ReturnMessage.Data[0].Value.Integer;
					}
				}
				else if (ReturnMessage.DataLength > 1)
				{
					for (size_t CurrentParameterIndex = 0; CurrentParameterIndex < ReturnMessage.DataLength; CurrentParameterIndex++)
					{
						if (ReturnMessage.Data[CurrentParameterIndex].Type == ReplyDataType::Integer)
						{
							if (ReturnMessage.Device > 0)
							{
								LastPosition[ReturnMessage.Device - 1][CurrentParameterIndex] = ReturnMessage.Data[CurrentParameterIndex].Value.Integer;
							}
						}
					}
				}
			}
		}
		else if (CurrentCommand.Parameters[0].Type == CommandParameterType::Setting)
		{
			//Serial.print("st");
			if (CurrentCommand.Parameters[0].Value.Setting == SettingMessageType::SystemAxisCount)
			{
				//Serial.print("ac");
				//Serial.print(ReturnMessage.DataLength);
				if (ReturnMessage.DataLength == 1)
				{
					if (ReturnMessage.Device > 0)
					{
						AxesFound[ReturnMessage.Device - 1] = ReturnMessage.Data[0].Value.Integer;
						//Serial.print("af");
						//Serial.print(AxesFound[ReturnMessage.Device - 1]);
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
				if (ReturnMessage.DataLength == 1)
				{
					if (ReturnMessage.Device > 0)
					{
						LastMaxSpeed[ReturnMessage.Device - 1][ReturnMessage.Axis - 1] = ReturnMessage.Data[0].Value.Integer;
					}
				}
				else if (ReturnMessage.DataLength > 1)
				{
					for (size_t CurrentParameterIndex = 0; CurrentParameterIndex < ReturnMessage.DataLength; CurrentParameterIndex++)
					{
						if (ReturnMessage.Data[CurrentParameterIndex].Type == ReplyDataType::Integer)
						{
							if (ReturnMessage.Device > 0)
							{
								LastMaxSpeed[ReturnMessage.Device - 1][CurrentParameterIndex] = ReturnMessage.Data[CurrentParameterIndex].Value.Integer;
							}
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
		//Serial.print("df");
		//Serial.print(DevicesFound);
	}
}
void ZaberMaster::ProcessMoveStarted()
{
	if (UseAlerts)
	{
		if (CurrentCommand.Device == 0)
		{
			if (CurrentCommand.Axis == 0)
			{
				for (size_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
				{
					for (size_t AxisIndex = 0; AxisIndex < AxesFound[DeviceIndex]; AxisIndex++)
					{
						ExpectingAlert[DeviceIndex][AxisIndex] = true;
					}
				}
			}
			else
			{
				for (size_t DeviceIndex = 0; DeviceIndex < DevicesFound; DeviceIndex++)
				{
					ExpectingAlert[DeviceIndex][CurrentCommand.Axis - 1] = true;
				}
			}
		}
		else
		{
			if (CurrentCommand.Axis == 0)
			{
				for (size_t AxisIndex = 0; AxisIndex < AxesFound[CurrentCommand.Device - 1]; AxisIndex++)
				{
					ExpectingAlert[CurrentCommand.Device - 1][AxisIndex] = true;
				}
			}
			else
			{
				ExpectingAlert[CurrentCommand.Device - 1][CurrentCommand.Axis - 1] = true;
			}
		}
	}
}
void ZaberMaster::SendCommand()
{
	const uint8_t CommandIndex = static_cast<uint8_t>(CurrentCommand.Command);
	const uint8_t* StringToSend = reinterpret_cast<const uint8_t*>(CommandIdentifier[CommandIndex].String);
	const uint8_t CommandSize = CommandIdentifier[CommandIndex].Count;
	//const CommandScope CurrentCommandScope = CommandIdentifier[CommandIndex].Scope;
	uint8_t CharCount;
	CurrentSentCallback = CommandIdentifier[CommandIndex].SentCallback;
	CurrentReceivedCallback = CommandIdentifier[CommandIndex].ReceivedCallback;
	CurrentCommand.MessageStarted = true;
	CurrentCommand.StartTime = micros();
	memset(CommandBuffer,0,CommandBufferSize);
	_HardwareSerial->write(CommandMarkerCharacter);
	//Serial.print("Z:");
	//Serial.print(CommandMarkerCharacter);
	CharCount = IntToCharPointer(CurrentCommand.Device, CommandBuffer, CommandBufferSize);
	_HardwareSerial->write(reinterpret_cast<const uint8_t*>(CommandBuffer),CharCount);
	//Serial.write(reinterpret_cast<const uint8_t*>(CommandBuffer), CharCount);
	_HardwareSerial->write(SpaceCharacter);
	//Serial.write(SpaceCharacter);
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
	//Serial.write(reinterpret_cast<const uint8_t*>(CommandBuffer), CharCount);
	_HardwareSerial->write(SpaceCharacter);
	//Serial.write(SpaceCharacter);
	_HardwareSerial->write(StringToSend,CommandSize);
	//Serial.write(StringToSend, CommandSize);
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
					//Serial.write(SpaceCharacter);
					_HardwareSerial->write(reinterpret_cast<const uint8_t*>(CommandBuffer),CharCount);
					//Serial.write(reinterpret_cast<const uint8_t*>(CommandBuffer), CharCount);
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
					//Serial.write(SpaceCharacter);
					_HardwareSerial->write(reinterpret_cast<const uint8_t*>(CommandBuffer),CharCount);
					//Serial.write(reinterpret_cast<const uint8_t*>(CommandBuffer), CharCount);
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
					//Serial.write(SpaceCharacter);
					_HardwareSerial->write(ParameterMessageString,StringCount);
					//Serial.write(ParameterMessageString, StringCount);
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
					//Serial.write(SpaceCharacter);
					_HardwareSerial->write(SettingMessageString,StringCount);
					//Serial.write(SettingMessageString, StringCount);
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
	//Serial.write('\n');
	if (Verbose)
	{
		Serial.print(")\n");
	}
	ExpectingReply = true;
	ResetReturnMessage();
	if (CurrentSentCallback != NULL)
	{
		(this->*CurrentSentCallback)();
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
	ReturnMessage.MessageStarted = false;
	ReturnMessage.MessageComplete = false;
	ReturnMessage.DataLength = 0;
	ReturnMessage.StartTime = 0;
	ReturnMessageDataTypeDetermined = false;
}
void ZaberMaster::FailToParse()
{
	//Serial.print("!fp!");
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
	CurrentReplyPart = ReplyParts::Type;
}
