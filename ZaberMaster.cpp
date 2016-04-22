#include "ZaberMaster.h"

const char ZaberMaster::CarriageReturnCharacter = '\r';
const char ZaberMaster::EndOfLineCharacter = '\n';
const char ZaberMaster::SpaceCharacter = ' ';
const char ZaberMaster::CommandMarkerCharacter = '/';
const char ZaberMaster::GetCharacter = '?';
const ZaberMaster::CommandString ZaberMaster::CommandIdentifier[]=
{
	{CommandMessageType::EStop,"estop",5,CommandScope::Axis},
	{CommandMessageType::Get,"get",3,CommandScope::Either},
	{CommandMessageType::Home,"home",4,CommandScope::Axis},
	{CommandMessageType::IOInfo,"io info",7,CommandScope::Device},
	{CommandMessageType::IOGet,"io get",6,CommandScope::Device},
	{CommandMessageType::IOSet,"io set",6,CommandScope::Device},
	{CommandMessageType::Joystick,"joystick",8,CommandScope::Device},
	{CommandMessageType::Key,"key",3,CommandScope::Device},
	{CommandMessageType::LockStep,"lockstep",8,CommandScope::Device},
	{CommandMessageType::Move,"move",4,CommandScope::Axis},
	{CommandMessageType::Renumber,"renumber",8,CommandScope::Device},
	{CommandMessageType::Set,"set",3,CommandScope::Either},
	{CommandMessageType::Stop,"stop",4,CommandScope::Axis},
	{CommandMessageType::Stream,"stream",6,CommandScope::Device},
	{CommandMessageType::SystemReset,"system reset",12,CommandScope::Device},
	{CommandMessageType::SystemRestore,"system restore",14,CommandScope::Device},
	{CommandMessageType::ToolsEcho,"tools echo",10,CommandScope::Device},
	{CommandMessageType::ToolsFindRange,"tools findrange",15,CommandScope::Axis},
	{CommandMessageType::ToolsGoToLimit,"tools gotolimit",15,CommandScope::Axis},
	{CommandMessageType::ToolsParking,"tools parking",13,CommandScope::Device},
	{CommandMessageType::ToolsSetComm,"tools setcomm",13,CommandScope::Device},
	{CommandMessageType::ToolsStorePos,"tools storepos",14,CommandScope::Axis},
	{CommandMessageType::Trigger,"trigger",7,CommandScope::Device},
	{CommandMessageType::TriggerDist,"trigger dist",12,CommandScope::Device},
	{CommandMessageType::TriggerTime,"trigger time",12,CommandScope::Device},
	{CommandMessageType::Virtual,"virtual",7,CommandScope::Device},
	{CommandMessageType::Warnings,"warnings",8,CommandScope::Either}
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
	{ParameterMessageType::Info,"info",4},
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
	{ParameterMessageType::Setup,"setup",5},
	{ParameterMessageType::Angle,"angle",5},
	{ParameterMessageType::Ratio,"ratio",5},
	{ParameterMessageType::Info,"info",4},
	{ParameterMessageType::Clear,"clear",5}
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
	Initialized = false;
	InitializationStep = 0;
	DevicesFound = 0;
	memset(AxesFound,0,sizeof(AxesFound));
	CurrentCommand.MessageComplete = false;
	CurrentCommand.MessageStarted = false;
	CurrentCommand.Axis = 0;
	Currentcommand.Device = 0;
	CurrentCommand.StartTime = 0;
	CurrentCommand.ParameterCount = 0;
	for (uint8_t Index; Index < ZaberParameterMaxLength; Index++)
	{
		CurrentCommand.Parameters[Index].Type = ParameterType::None;
		CurrentCommand.Parameters[Index].Value.Integer = 0;
	}
	memset(CommandBuffer,0,sizeof(CommandBuffer));
	LastTransmittionTime = 0;
	SendingCommand = false;
	ExpectingReply = false;
	memset(ExpectingAlert,0,sizeof(ExpectingAlert));
	SupportChaining = true;
	MovementComplete = NULL;
	ReplyComplete = NULL;
	memset(ReturnBuffer,0,sizeof(ReturnBuffer));
	ReturnBufferPosition = 0;
	ReturnMessage.Type = ReturnMessage::Reply;
	for (uint8_t Index; Index < ZaberMaxReplyData; Index++)
	{
		ReturnMessage.Data[Index].Type = ParameterType::None;
		ReturnMessage.Data[Index].Integer.Value = 0;
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
}
ZaberMaster::~ZaberMaster()
{

}
bool Initialize()
{

}
bool ZaberMaster::ShutDown()
{

}
bool ZaberMaster::Renumber()
{

}
bool ZaberMaster::EStop(uint8_t Device, uint8_t Axis)
{

}
bool ZaberMaster::Stop(uint8_t Device, uint8_t Axis)
{

}
bool ZaberMaster::Home(uint8_t Device, uint8_t Axis)
{

}
bool ZaberMaster::SystemReset(uint8_t Device)
{

}
bool ZaberMaster::SystemRestore(uint8_t Device)
{

}
bool ZaberMaster::ClearWarnings(uint8_t Device)
{

}
bool ZaberMaster::FindRange(uint8_t Device, uint8_t Axis)
{

}
bool ZaberMaster::MoveRel(uint8_t Device, uint8_t Axis, uint16_t Steps)
{

}
bool ZaberMaster::MoveAbs(uint8_t Device, uint8_t Axis, uint16_t Steps)
{

}
bool ZaberMaster::SetAcceleration(uint8_t Device, uint8_t Axis, uint16_t Acceleration)
{

}
bool ZaberMaster::GetAcceleration(uint8_t Device, uint8_t Axis, uint16_t* Acceleration)
{

}
bool ZaberMaster::SetMaxSpeed(uint8_t Device, uint8_t Axis, uint16_t MaxSpeed)
{

}
bool ZaberMaster::GetMaxSpeed(uint8_t Device, uint8_t Axis, uint16_t* MaxSpeed)
{

}
bool ZaberMaster::SetLimitMax(uint8_t Device, uint8_t Axis, uint32_t LimitMax)
{

}
bool ZaberMaster::GetLimitMax(uint8_t Device, uint8_t Axis, uint32_t* LimitMax)
{

}
bool ZaberMaster::SetLimitMin(uint8_t Device, uint8_t Axis, uint32_t LimitMin)
{

}
bool ZaberMaster::GetLimitMin(uint8_t Device, uint8_t Axis, uint32_t* LimitMin)
{

}
bool ZaberMaster::SetAlertStatus(uint8_t Device, bool Enable)
{

}
bool ZaberMaster::GetAlertStatus(uint8_t Device, bool* Enable)
{

}
bool ZaberMaster::SetKnobEnable(uint8_t Device, uint8_t Axis, bool Enable)
{

}
bool ZaberMaster::GetKnobEnable(uint8_t Device, uint8_t Axis, bool* Enable)
{

}
bool ZaberMaster::SetParked(uint8_t Device, uint8_t Axis, bool Enable)
{

}
bool ZaberMaster::GetParked(uint8_t Device, uint8_t Axis, bool* Enable)
{

}
bool ZaberMaster::SetResolution(uint8_t Device, uint8_t Axis, uint8_t Resolution)
{

}
bool ZaberMaster::GetResolution(uint8_t Device, uint8_t Axis, uint8_t* Resolution)
{

}
bool ZaberMaster::GetPosition(uint8_t Device, uint8_t Axis, uint16_t* Position)
{

}
void SetMovementCompleteCallback(FinishedListener _MovementComplete)
{
	MovementComplete = _MovementComplete;
}
void SetReplyCompleteCallback(FinishedListener _ReplyComplete)
{
	ReplyComplete = _ReplyComplete;
}
uint8_t ZaberMaster::IntToCharPointer(uint8_t Input, char* Buffer, size_t BufferSize)
{
	memset(Buffer,0,BufferSize);
	return sprintf(Buffer, "%u", Input);
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
	if (ExpectReply)
	{
		uint32_t TimeDifference = micros()  - TransmitTime;
		if(_HardwareSerial->available() > 0)
		{
			char Character = _HardwareSerial->read();
			if (ReturnMessage.Type = MessageType::Info)
			{
				if (Character == EndOfLineCharacter)
				{
					FailToParse();
				}
			}
			else if (ReturnMessage.Type = MessageType::Alert)
			{
				ParseCharacterForAlert(Character);
			}
			else
			{
				ParseCharacterForReply(Character);
			}
		}
	}
	else
	{
		if(_HardwareSerial->available() > 0)
		{
			_HardwareSerial->clear();
		}
	}
}
void ZaberMaster::ParseType(char Character)
{
	bool Found = false;
	for (int Index = 0; Index < MessageType::Count; Index++)
	{
		if (Character == MessageIdentifier[Index].Identifier)
		{
			ReturnMessage.Type = ZaberMessageIdentifier[Index].Type;
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
		uint8_t DeviceNumber = CharPointerToInt(ReturnBuffer);
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
		uint8_t AxisNumber = CharPointerToInt(ReturnBuffer);
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
		for (uint8_t Index; Index < FlagType::Count; Index++)
		{
			if ( strcmp(ReturnBuffer, FlagIdentifier[Index].Flag) == 0)
			{
				ReturnMessage.FlagType = FlagIdentifier[Index].Type;
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
		for (uint8_t Index = 0; Index < StatusType::Count; Index++)
		{
			if ( strcmp(ReturnBuffer, StatusIdentifier[Index].Flag) == 0)
			{
				ReturnMessage.FlagType = StatusIdentifier[Index].Type;
				ClearReturnBuffer();
				CurrentReplyPart = NextState;
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
	if (Character = '-' || isalpha(Character))
	{
		ReturnBuffer[ReturnBufferPosition] = Character;
		if (ReturnBufferPosition < ReturnBufferSize)
		{
			ReturnBufferPosition++;
		}
	}
	else if (Character == SpaceCharacter || Character == EndOfLineCharacter || Character == SpaceCharacter)
	{
		if ( (Character != SpaceCharacter) && (ReturnMessage.Type == MessageType::Reply) )
		{
			FailToParse();
		}
		ReturnBuffer[ReturnBufferPosition] = '\0';
		bool Found = false;
		for (Index = 0; Index < WarningType.Count; Index++)
		{
			if ( strcmp(ReturnBuffer, WarningIdentifier[Index].Flag) == 0 )
			{
				ReturnMessage.WarningType = WarningIdentifier[Index].Type;
				Found = true;
			}
		}
		ClearReturnBuffer();
		if(Found)
		{
			if ( (NextState == ReplyParts::Type) && (ReturnMessage.Type == MessageType::Alert) )
			{
				MessageComplete.MessageComplete = true;
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
	if (ReplyMessage.FlagType == FlagType.Okay)
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
					ReturnMessage.Data[ReturnMessage.DataLength].Value = CharPointerToFloat(ReturnBuffer);
				}
				else
				{
					ReturnBuffer[ReturnBufferPosition] = '\0'
					ReturnMessage.Data[ReturnMessage.DataLength].Type = ReplyDataType::Integer;
					ReturnMessage.Data[ReturnMessage.DataLength].Value = (int32_t)atoi(ReturnBuffer);
				}
				ReturnMessage.DataLength++;
			}
			ReturnMessageDataTypeDetermined = false;
			ClearReturnBuffer();
			if (Character != SpaceCharacter)
			{
				CurrentReplyPart = ReplyParts::Type;
				MessageComplete.MessageComplete = true;
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
			for (Index = 0; Index < ReplyDataErrorType::Count; Index++)
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
				ReturnMessage.Data[0].Value = FoundError;
				CurrentReplyPart = ReplyParts::Type;
				MessageComplete.MessageComplete = true;
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
		case default:
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
		case default:
			break;
	}
}
void ZaberMaster::ProcessAlertMessage()
{
	if (ReplyMessage.Type == MessageType::Alert)
	{
		uint8_t Index = (ReplyMessage.Device * ZaberMaxDevices) + ReplyMessage.Axis;
		if (ExpectingReply[Index] == true)
		{
			ExpectingReply[Index] = false;	
		}
		bool AlertsComplete = true;
		for (uint8_t ExpectingIndex = 0; ExpectingIndex < (ZaberMaxDevices * ZaberMaxAxes); ExpectingIndex++)
		{
			if (ExpectingReply[ExpectingIndex] == true)
			{
				AlertsComplete = false;
				break;
			}
		}
		if (AlertsComplete)
		{
			if (MovementComplete != NULL)
			{
				MovementComplete();
			}
			if (InternalMovementComplete != NULL)
			{
				InternalMovementComplete();
			}
		}
	}
}
void ZaberMaster::ProcessReplyMessage()
{
	if (ReplyMessage.Type == MessageType::Reply)
	{
		if (ExpectingReply)
		{
			ExpectingReply = false;
			if (ReplyComplete != NULL)
			{
				ReplyComplete();
			}
		}
	}
}
void ZaberMaster::SendCommand()
{
	const uint8_t CommandIndex = static_cast<uint8_t>(CurrentCommand.Command);
	const uint8_t* StringToSend = reinterpret_cast<const uint8_t*>(CommandIdentifier[CommandIndex].String);
	const uint8_t CommandSize = CommandIdentifier[CommandIndex].Count;
	const CommandScope CurrentCommandScope = CommandIdentifier[CommandIndex].Scope;
	CurrentCommand.MessageStarted = true;
	CurrentCommand.StartTime = micros();
	memset(CommandBuffer,0,CommandBufferSize);
	_HardwareSerial->write(CommandMarkerCharacter);
	uint8_t CharCount = IntToCharPointer(CurrentCommand.Device, CommandBuffer, CommandBufferSize);
	_HardwareSerial->write(CommandBuffer,CharCount);
	_HardwareSerial->write(SpaceCharacter);
	memset(CommandBuffer,0,CommandBufferSize);
	uint8_t CharCount = IntToCharPointer(CurrentCommand.Axis, CommandBuffer, CommandBufferSize);
	_HardwareSerial->write(CommandBuffer,CharCount);
	_HardwareSerial->write(SpaceCharacter);
	_HardwareSerial->write(StringToSend,CommandSize);
	if (CurrentCommand.ParameterCount > 0)
	{
		for (uint8_t ParameterIndex; ParameterIndex < CurrentCommand.ParameterCount; ParameterIndex++)
		{
			CommandParameterType ParameterType = CurrentCommand[ParameterIndex].Type;
			switch (ParameterType)
			{
				case CommandParameterType::None:
					break;
				case CommandParameterType::Integer:
				{
					uint8_t CharCount = IntToCharPointer(CurrentCommand[ParameterIndex].CommandParameterValue.Integer, CommandBuffer, CommandBufferSize);
					_HardwareSerial->write(CommandBuffer,CharCount);
					break;	
				}
				case CommandParameterType::Float:
				{
					uint8_t CharCount = FloatToCharPointer(CurrentCommand[ParameterIndex].CommandParameterValue.Float, CommandBuffer, CommandBufferSize);
					_HardwareSerial->write(CommandBuffer,CharCount);
					break;	
				}
				case CommandParameterType::Parameter:
				{
					const uint8_t ParameterIdentifierIndex = static_cast<uint8_t>(CurrentCommand[ParameterIndex].Value.Parameter);
					const char* ParameterMessageString = ParameterIdentifier[ParameterIdentifierIndex].String;
					const uint8_t StringCount = ParameterIdentifier[ParameterIdentifierIndex].Count;
					_HardwareSerial->write(ParameterMessageString,StringCount);
					break;	
				}
				case CommandParameterType::Setting:
				{
					const uint8_t SettingIdentifierIndex = static_cast<uint8_t>(CurrentCommand[ParameterIndex].Value.Setting);
					const char* SettingMessageString = SettingIdentifier[SettingIdentifierIndex].String;
					const uint8_t StringCount = SettingIdentifier[ParameterIdentifierIndex].Count;
					_HardwareSerial->write(SettingMessageString,StringCount);
					break;	
				}
				default:
					break;
			}
		}
	}
	_HardwareSerial->write(NewLineCharacter);
}
void ZaberMaster::ClearReturnBuffer()
{
	memset(ReturnBuffer,0,sizeof(ReturnBuffer));
	ReturnBufferPosition = 0;
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
	ClearReturnBuffer();
	ResetReturnMessage();
	CurrentReplyPart = ZaberReplyParts::Type;
}