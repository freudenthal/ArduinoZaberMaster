#include "Controller.hpp"

#include <algorithm>

namespace Zaber
{
	// ==============================
	// STATIC CONSTANTS
	// ==============================

	// initialization lists

	const CommandMessage Controller::LinearStandardInitilizationSteps[]=
	{
		CommandMessage::Renumber(),
		CommandMessage::ClearWarnings(0),
		CommandMessage::GetAxes(0),
		CommandMessage::SetAlertStatus(0, true),
		CommandMessage::SetMaxSpeed(0, 0, ZaberDefaultSpeed),
		CommandMessage::Home(2, 0),
		CommandMessage::FindRange(1, 0),
	};
	const uint8_t Controller::LinearStandardInitilizationStepCount = ARRAY_SIZE(LinearStandardInitilizationSteps);

	const CommandMessage Controller::RotationalStandardInitilizationSteps[]=
	{
		CommandMessage::Renumber(),
		CommandMessage::ClearWarnings(0),
		CommandMessage::GetAxes(0),
		CommandMessage::SetAlertStatus(0, true),
		CommandMessage::Home(0, 0),
	};
	const uint8_t Controller::RotationalStandardInitilizationStepCount = ARRAY_SIZE(RotationalStandardInitilizationSteps);

	const CommandMessage Controller::LinearAndRotationalInitilizationSteps[]=
	{
		CommandMessage::Renumber(),
		CommandMessage::ClearWarnings(0),
		CommandMessage::GetAxes(0),
		CommandMessage::SetAlertStatus(0, true),
		CommandMessage::SetMaxSpeed(0, 0, ZaberDefaultSpeed),
		CommandMessage::FindRange(0, 0),
		CommandMessage::Home(1, 1),
	};
	const uint8_t Controller::LinearAndRotationalInitilizationStepCount = ARRAY_SIZE(LinearAndRotationalInitilizationSteps);

	// ==============================
	// PUBLIC METHODS
	// ==============================

	// constructor and destructor

	Controller::Controller(Stream& serial) :
		_StreamPort(serial),
		activeIndexPairMap{ 0, {} },
		ReturnBuffer{} // default all \0
	{
		Initializing = false;
		InitializationStep = 0;
		Active = false;
		LastWarning = WarningType::None;
		OnInitializationComplete = nullptr;
		OnMovementComplete = nullptr;
		OnReplyComplete = nullptr;
		InitializationBehavior = InitializationType::LinearStandard;
		Verbose = false;
	}

	Controller::~Controller() {}

	// setup

	bool Controller::Begin(uint8_t NumberOfDevices, uint8_t* AxisCount)
	{
		bool NoError = true;
		// validate device count and axes count and reduce if necessary
		if (NumberOfDevices >= ZaberMaxDevices)
		{
			NumberOfDevices = ZaberMaxDevices - 1;
			Serial.print("<ZABERDEV>(Device count over max, reducing.)\n");
			NoError = false;
		}
		for (size_t DeviceIndex = 0; DeviceIndex < NumberOfDevices; DeviceIndex++)
		{
			if (AxisCount[DeviceIndex] >= ZaberMaxAxes)
			{
				AxisCount[DeviceIndex] = ZaberMaxAxes - 1;
				Serial.print("<ZABERDEV>(Axis count over max, reducing.)\n");
				NoError = false;
			}
		}
		// set device count and axes count
		activeIndexPairMap.ActiveDevices = NumberOfDevices;
		for (size_t DeviceIndex = 0; DeviceIndex < NumberOfDevices; DeviceIndex++)
		{
			activeIndexPairMap.ActiveAxesByDevice[DeviceIndex] = AxisCount[DeviceIndex];
		}
		// enable serial update checking logic
		Active = true;
		Initializing = false;

		return NoError;
	}

	// verbose configure

	void Controller::SetVerbose(bool VerboseToSet)
	{
		Verbose = VerboseToSet;
	}

	bool Controller::GetVerbose() const
	{
		return Verbose;
	}

	// initialization configuration and invokation

	bool Controller::SetInitializationType(InitializationType TypeToSet)
	{
		InitializationBehavior = TypeToSet;
		return true;
	}

	bool Controller::Initialize()
	{
		CommandQueue.clear();
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
		Active = true;
		CheckInitialization();
		return true;
	}

	// accessors for tracked properties of active axes

	bool Controller::IsAxisSpecificAndActive(uint8_t Device, uint8_t Axis) const
	{
		IndexPair index = { Device, Axis };
		return index.SpecifiesOnlyOne() && activeIndexPairMap.AnyActive(index);
	}

	std::optional<uint32_t> Controller::GetPosition(uint8_t Device, uint8_t Axis) const
	{
		if (IsAxisSpecificAndActive(Device, Axis))
		{
			if (Verbose)
			{
				Serial.print("<ZABERPOSITIONREQ>(");
				Serial.print(AllDeviceProperties[Device-1].AllAxisProperties[Axis-1].Position);
				Serial.print(")\n");
			}
			return AllDeviceProperties[Device-1].AllAxisProperties[Axis-1].Position;
		}
		return std::nullopt;
	}

	std::optional<uint32_t> Controller::GetEncoderPosition(uint8_t Device, uint8_t Axis) const
	{
		if (IsAxisSpecificAndActive(Device, Axis))
		{
			return AllDeviceProperties[Device-1].AllAxisProperties[Axis-1].EncoderPosition;
		}
		return {};
	}

	std::optional<uint32_t> Controller::GetMaxSpeed(uint8_t Device, uint8_t Axis) const
	{
		if (IsAxisSpecificAndActive(Device, Axis))
		{
			return AllDeviceProperties[Device-1].AllAxisProperties[Axis-1].MaxSpeed;
		}
		return {};
	}

	std::optional<StatusType> Controller::GetStatus(uint8_t Device, uint8_t Axis) const
	{
		if (IsAxisSpecificAndActive(Device, Axis))
		{
			return AllDeviceProperties[Device-1].AllAxisProperties[Axis-1].OptStatus;
		}
		return {};
	}

	// queue a command to be sent during check

	bool Controller::EnqueueCommand(const CommandMessage& CommandForEnqueue)
	{
		// report and exit if command does not target any active axes
		if (!activeIndexPairMap.AnyActive(CommandForEnqueue.Device, CommandForEnqueue.Axis))
		{
			ReportWhyCommandTargetsInvalid(CommandForEnqueue);
			return false;
		}

		// add command to queue if queue has space
		bool added = CommandQueue.push_back(CommandForEnqueue);
		if (Verbose)
		{
			if (added)
			{
				Serial.print("<ZABER>(Enqueue ");
				CommandForEnqueue.PrintVerbose(Serial);
				Serial.print(")\n");
			}
			else
			{
				Serial.print("<ZABER>(Enqueue failed because queue is full. lost ");
				CommandForEnqueue.PrintVerbose(Serial);
				Serial.print(")\n");
			}
		}
		return added;
	}

	// set callbacks for various triggers

	void Controller::SetInitializationCompleteCallback(Controller::Listener _InitializationComplete)
	{
		OnInitializationComplete = _InitializationComplete;
	}

	void Controller::SetMovementCompleteCallback(Controller::DeviceAxisListener _MovementComplete)
	{
		OnMovementComplete = _MovementComplete;
	}

	void Controller::SetReplyCompleteCallback(Controller::ReplyListener _ReplyComplete)
	{
		OnReplyComplete = _ReplyComplete;
	}

	// update logic for querying communication over serial

	void Controller::Check()
	{
		if (Active)
		{
			CheckSerial();
			CheckCommandQueue();
			CheckInitialization();
			PollBusyAxesAndCheckForTimeouts();
		}
	}

	// ==============================
	// PRIVATE METHODS
	// ==============================

	// check various parts of the controller from the check command

	void Controller::CheckCommandQueue()
	{
		// loop until empty or until the front of the queue is valid and has to wait
		while (!CommandQueue.empty())
		{
			CommandMessage NewCommand = CommandQueue.peek_front();

			// report and exit if command does not target any active axes
			if (!activeIndexPairMap.AnyActive(NewCommand.Device, NewCommand.Axis))
			{
				ReportWhyCommandTargetsInvalid(NewCommand);
				// remove command from queue
				CommandQueue.pop_front();
			}
			else if (CanCommandSendNow(NewCommand))
			{
				// remove command from queue
				CommandQueue.pop_front();
				if (Verbose)
				{
					Serial.print("<ZABER>(Dequeue ");
					NewCommand.PrintVerbose(Serial);
					Serial.print(")\n");
				}
				// send command
				SendCommandNow(NewCommand);
			}
			else // command targets active axes (is valid) but cannot send now
			{
				// exit loop early because the command at the front of the queue needs to wait.
				break;
			}
		}
	}

	void Controller::CheckSerial()
	{
		while (_StreamPort.available() > 0)
		{
			char NewChar = _StreamPort.read();
			if (NewChar == '\n')
			{
				// do nothing if buffer is empty when EOL arrives
				if (ReturnBufferCount > 0)
				{
					ReturnBuffer[ReturnBufferCount] = NewChar;
					ReturnBufferCount++;
					ProcessReturnBuffer();
					ClearReturnBuffer();
				}
			}
			else if (NewChar == '\r')
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
					Serial.write(ReturnBuffer, ReturnBufferCount);
					Serial.print(")\n");
					ClearReturnBuffer();
				}
			}
		}
	}

	void Controller::PollBusyAxesAndCheckForTimeouts()
	{
		// poll busy axes on active devices
		uint32_t now = micros();
		for (uint8_t DeviceIndex = 1; DeviceIndex <= activeIndexPairMap.ActiveDevices; DeviceIndex++)
		{
			auto& deviceProperties = AllDeviceProperties[DeviceIndex-1];
			auto& commStatus = deviceProperties.CommStatus;
			uint32_t deviceSelectedPollingDelay = deviceProperties.AlertEnabled ? POLLING_DELAY_MICROS_ALERTS_ON : POLLING_DELAY_MICROS_NO_ALERTS;

			if (!commStatus.IsAwaitingReply())
			{
				for (uint8_t AxisIndex = 1; AxisIndex <= activeIndexPairMap.ActiveAxesByDevice[DeviceIndex-1]; AxisIndex++)
				{
					auto& axisProperties = deviceProperties.AllAxisProperties[AxisIndex-1];
					if (axisProperties.OptStatus == StatusType::Busy)
					{
						if (now - axisProperties.LastStatusUpdateTime > deviceSelectedPollingDelay)
						{
							if (Verbose)
							{
								Serial.printf("<ZABER>(Polling for device #%d Axis #%d.)\n", DeviceIndex, AxisIndex);
							}
							// all the preconditions for being able to send a command without breaking stuff have already been checked
							SendCommandNow(CommandMessage::GetStatus(DeviceIndex, AxisIndex));
						}
					}
				}
			}
		}

		// check for timeouts across active and inactive devices
		for (uint8_t DeviceIndex = 1; DeviceIndex <= ZaberMaxDevices; DeviceIndex++)
		{
			auto& commStatus = AllDeviceProperties[DeviceIndex-1].CommStatus;

			if (commStatus.IsAwaitingReply())
			{
				// check for timed out missing replies
				if (commStatus.HasTimedOut())
				{
					commStatus.SetNoLongerWaiting();
					const auto& lastSentCommand = commStatus.GetLastSentCommand();

					// don't report timeouts for inactive devices when verbose is off
					if (Verbose || DeviceIndex <= activeIndexPairMap.ActiveDevices)
					{
						Serial.printf("<STAGEERROR>(Device #%d did not respond to the previous command \"", DeviceIndex);
						lastSentCommand.PrintWithNoNewline(Serial);
						Serial.print("\")\n");
					}

					if (lastSentCommand.Type == CommandMessage::CommandType::Renumber && lastSentCommand.Device == 0)
					{
						// assert this device is inactive
						if (activeIndexPairMap.ActiveDevices >= DeviceIndex)
						{
							activeIndexPairMap.ActiveDevices = DeviceIndex-1;
							Serial.printf("<STAGEWARN>(Active device #%d did not respond to the renumber all command. Reducing known active device count to %d.)\n",
								DeviceIndex, activeIndexPairMap.ActiveDevices);
						}
					}

					// call the callback now or else it never gets called.
					if (lastSentCommand.Callback)
					{
						lastSentCommand.Callback(DeviceIndex, lastSentCommand.Axis);
					}
				}
			}
		}
	}

	void Controller::CheckInitialization()
	{
		if (Initializing)
		{
			// run as many commands as possible without colliding/overlapping the commands
			while (InitializationStep < InitializationStepsCount && CanCommandSendNowWithoutInterrupting(InitilizationSteps[InitializationStep]))
			{
				SendCommandNow(InitilizationSteps[InitializationStep++]);
			}
			// if all commands have been sent, initialization is done once all replies are received and all axes are idle.
			if (InitializationStep >= InitializationStepsCount && AreAllAxesNotAwaitingReplyAndNotBusy())
			{
				Initializing = false;
				if (OnInitializationComplete != nullptr)
				{
					OnInitializationComplete();
				}
			}
		}
	}

	// assess status of queue

	bool Controller::CanCommandSendNow(const CommandMessage& Command) const
	{
		// check all devices in array, including inactive ones (in case of awaiting renumber commands on inactive devices)
		const std::vector<uint8_t> deviceList = (Command.Device == 0) ? ActiveIndexPairMap::AllDevicesVector() : std::vector<uint8_t>{ Command.Device };
		for (const auto& DeviceIndex : deviceList)
		{
			if (AllDeviceProperties[DeviceIndex-1].CommStatus.IsAwaitingReply())
			{
				return false;
			}
		}
		// none are awaiting replies
		return true;
	}

	bool Controller::CanCommandSendNowWithoutInterrupting(const CommandMessage& Command) const
	{
		// first check that no specified devices are awaiting replies
		if (!CanCommandSendNow(Command))
		{
			return false;
		}
		// then check all specified _active_ axes for whether they're busy
		for (const auto& IndexPair : activeIndexPairMap.GetSpecifiedDeviceAxisList(Command.Device, Command.Axis))
		{
			auto& axisProperties = AllDeviceProperties[IndexPair.Device-1].AllAxisProperties[IndexPair.Axis-1];
			if (axisProperties.OptStatus == StatusType::Busy)
			{
				return false;
			}
		}
		// none are busy
		return true;
	}

	bool Controller::AreAllAxesNotAwaitingReplyAndNotBusy() const
	{
		// check whether all devices (inactive or active) are awaiting a response
		// and check whether any active axes on any active devices are busy
		for (uint8_t Device = 1; Device <= ZaberMaxDevices; Device++)
		{
			auto& deviceProperties = AllDeviceProperties[Device-1];
			if (Device <= activeIndexPairMap.ActiveDevices)
			{
				for (uint8_t Axis = 1; Axis <= activeIndexPairMap.ActiveAxesByDevice[Device-1]; Axis++)
				{
					auto& axisProperties = deviceProperties.AllAxisProperties[Axis-1];
					if (axisProperties.OptStatus == StatusType::Busy)
					{
						return false;
					}
				}
			}
			if (deviceProperties.CommStatus.IsAwaitingReply())
			{
				return false;
			}
		}
		return true;
	}

	bool Controller::WillQueueCheckAxisStatusSoon(uint8_t device, uint8_t axis) const
	{
		// false if out of bounds
		if (device == 0 || device > ZaberMaxDevices || axis == 0 || axis > ZaberMaxAxes)
			return false;

		// true if the current sent command is checking that axis
		auto& commStatus = AllDeviceProperties[device-1].CommStatus;
		auto& lastSentCommand = commStatus.GetLastSentCommand();
		if (commStatus.IsAwaitingReply() && IndexPair{ lastSentCommand.Device, lastSentCommand.Axis }.TargetsSingleAxisOf({ device, axis }))
		{
			return true;
		}

		// true if any command in the queue targets that specific axis
		for (const auto& command : CommandQueue)
		{
			if (IndexPair{ command.Device, command.Axis }.TargetsSingleAxisOf({ device, axis }))
			{
				return true;
			}
		}

		// otherwise false
		return false;
	}

	// feedback for command filtering

	void Controller::ReportWhyCommandTargetsInvalid(const CommandMessage& Command) const
	{
		// cannot target active axes if device target is inactive
		if (Command.Device > activeIndexPairMap.ActiveDevices)
		{
			Serial.printf("<STAGEERROR>(Command rejected: targeted inactive device #%d.)\n",
				Command.Device);
			return;
		}

		if (Command.Device != 0)
		{
			// if it targets only one device, the targeted axis must be active on that device
			bool targetsActiveAxis = Command.Axis <= activeIndexPairMap.ActiveAxesByDevice[Command.Device-1];
			if (!targetsActiveAxis)
			{
				Serial.printf("<STAGEERROR>(Command rejected: targeted axis #%d is inactive on targeted device #%d.)\n",
					Command.Axis, Command.Device);
				return;
			}
		}
		else // Device == 0
		{
			// if it targets multiple devices, then the targeted axis must be active on at least one of the active devices
			int maxActiveAxes = 0;
			for (int deviceIndex = 1; deviceIndex < activeIndexPairMap.ActiveDevices && Command.Axis > maxActiveAxes; deviceIndex++)
			{
				if (activeIndexPairMap.ActiveAxesByDevice[deviceIndex-1] > maxActiveAxes)
				{
					maxActiveAxes = activeIndexPairMap.ActiveAxesByDevice[deviceIndex-1];
				}
			}
			bool targetsActiveAxis = Command.Axis <= maxActiveAxes;
			if (!targetsActiveAxis)
			{
				Serial.printf("<STAGEERROR>(Command rejected: targeted axis #%d is inactive on all targeted active devices.)\n",
					Command.Axis);
				return;
			}
		}

		// default
		Serial.printf("<STAGEERROR>(Command rejected for unspecified reason.)\n");
	}

	// sendnow or queue

	void Controller::SendCommandNow(const CommandMessage& Command)
	{
		// print command out to zaber connection
		Command.PrintWithNoNewline(_StreamPort);
		_StreamPort.print('\n');

		if (Verbose)
		{
			// log command to output
			Serial.print("<ZABER>(OUT: ");
			Command.PrintWithNoNewline(Serial);
			Serial.print(")\n");
		}

		// by default, expect a response on all active devices if device index is 0, BUT
		// there's a special case for command "/0 0 renumber"
		// then expect a response on all devices (incuding inactive ones) since this is meant to activate or deactivate devices based on who responds.
		auto deviceList = (Command.Type == CommandMessage::CommandType::Renumber && Command.Device == 0) ?
			ActiveIndexPairMap::AllDevicesVector() : activeIndexPairMap.GetSpecifiedDeviceList(Command.Device);

		for (uint8_t DeviceIndex : deviceList)
		{
			// for each device specified by the command, expect a command sent
			if (Verbose)
			{
				Serial.printf("<ZABER>(Device #%d now awaiting reply.)\n", DeviceIndex);
			}
			AllDeviceProperties[DeviceIndex-1].CommStatus.SetCommandSent(Command);
		}
	}

	bool Controller::SendCommandNowOrPrequeueIfBusy(const CommandMessage& Command)
	{
		// don't send or prequeue if command is invalid
		if (!activeIndexPairMap.AnyActive(Command.Device, Command.Axis))
		{
			return false;
		}

		// send if target device is not busy with commmand, otherwise prequeue in front
		if (CanCommandSendNow(Command))
		{
			SendCommandNow(Command);
			return true;
		}
		else
		{
			return CommandQueue.push_front(Command);
		}
	}

	// process serial response from connected Zaber

	void Controller::ClearReturnBuffer()
	{
		memset(ReturnBuffer, 0, sizeof(ReturnBuffer));
		ReturnBufferCount = 0;
	}

	void Controller::ProcessReturnBuffer()
	{
		if (Verbose)
		{
			Serial.print("<ZABERRET>(");
			PrintReturnBuffer();
			Serial.print(")\n");
		}
		if (auto optionalParsedResponse = ReplyParser::ParseLine(std::string_view(ReturnBuffer, ReturnBufferCount)))
		{
			ProcessResponse(optionalParsedResponse.value());
		}
		else
		{
			Serial.print("<ZABERPARSE>(Failed to parse: \"");
			PrintReturnBuffer();
			Serial.print("\")\n");
		}
	}

	void Controller::PrintReturnBuffer() const
	{
		// don't print newline at end of buffer to keep verbose print in one line.
		char lastChar = ReturnBuffer[ReturnBufferCount-1];
		if (lastChar == '\r' || lastChar == '\n')
		{
			Serial.write(ReturnBuffer, ReturnBufferCount-1);
			if (lastChar == '\r')
			{
				Serial.print("\\r");
			}
			else // lastChar == '\n'
			{
				Serial.print("\\n");
			}
		}
		else
		{
			Serial.write(ReturnBuffer, ReturnBufferCount);
		}
	}

	// process response based on type

	void Controller::ProcessResponse(const ReplyParser::ReplyMessage& Response)
	{
		if (Verbose)
		{
			Response.PrintVerboseMultiLine(Serial);
		}

		// filter out invalid responses
		if (Response.Device == 0) // no responses should specify device == 0
		{
			Serial.print("<STAGEERROR>(Response specified device #0. Invalid.)\n");
			return;
		}

		if (Response.Device > ZaberMaxDevices)
		{
			Serial.printf("<STAGEERROR>(Response specified device #%d, beyond max device count of %d. Invalid.)\n",
			Response.Device, ZaberMaxDevices);
			return;
		}

		// these checks do not evaluate whether the specified device or axes for the response are active or not.
		// that responsibility it left up to the response handlers

		// process response based on type
		if (Response.Type == ResponseType::Reply)
		{
			HandleReply(Response);
		}
		else if (Response.Type == ResponseType::Alert)
		{
			HandleAlert(Response);
		}
		else
		{
			// report unprocessed responses
			Serial.printf("<STAGEWARN>(Response received with type \"%s\" which is not Reply or Alert. Response will not be processed.)\n",
				ResponseTypeProperties::Of(Response.Type).ReadableName);
		}
	}

	/**
	 * Call when the Response type is reply and the device index is within the max device range (not the active device range, this method will filter for that).
	 */
	void Controller::HandleReply(const ReplyParser::ReplyMessage& Response)
	{
		// assert response is a reply type
		if (Response.Type != ResponseType::Reply)
		{
			Serial.print("<STAGEERROR>(Called to process non-reply message with reply logic. Invalid use.)\n");
			return;
		}

		// assert response device is within bounds
		if (Response.Device < 1 || ZaberMaxDevices < Response.Device)
		{
			Serial.printf("<STAGEERROR>(Cannot process reply with device index #%d outside of range [1, %d].)\n",
				Response.Device, ZaberMaxDevices);
		}

		// device index may be outside the active device limit, but that's okay since some commands are sent on inactive devices (such as /renumber)
		// the axis may be out of bounds
		auto& deviceProperties = AllDeviceProperties[Response.Device-1];
		auto& commStatus = deviceProperties.CommStatus;
		bool hasLastSentCommand = commStatus.IsAwaitingReply();
		// copy value and do not reference because this address may be overwritten by HandleResponseStatus before the callback is referenced
		const auto LastSentCommand = commStatus.GetLastSentCommand();
		// track whether to expect a new move for HandleResponseStatus
		bool newMoveStarted = false;

		// process meaning of reply if possible
		if (hasLastSentCommand)
		{
			// any reply on the same device means the command response was received and should no longer be waited for
			commStatus.SetNoLongerWaiting();
			if (LastSentCommand.Device == 0 || LastSentCommand.Device == Response.Device)
			{
				// if we have the correct sent command, we evaluate it only if it wasn't rejected
				if (Response.Flag == FlagType::Okay)
				{
					// check that the axis matches only if it is OK, because sometimes it can be rejected for BADAXIS and return with a different axis
					if (Response.Axis == LastSentCommand.Axis)
					{
						// process commands by type
						if (Response.Axis <= ZaberMaxAxes)
						{
							switch (LastSentCommand.Type)
							{
								case CommandMessage::CommandType::Renumber: HandleRenumberReply(Response); break;
								case CommandMessage::CommandType::Get: HandleGetReply(Response); break;
								case CommandMessage::CommandType::Set: HandleSetReply(Response); break;
								default: /* do nothing */ break; // keep this in to keep the compiler from warning about unhandled cases.
							}

							// process all movement causing commands
							if (CommandMessage::CommandTypeProperties::Of(LastSentCommand.Type).CausesBusy)
							{
								HandleCausesBusyReply(Response, newMoveStarted);
							}
						}
						else // axis > ZaberMaxAxes
						{
							Serial.printf("<STAGEWARN>(Response axis #%d is out of bounds of max axis count [%d].)\n",
								Response.Axis, ZaberMaxAxes);
						}
					}
					else // response axis does not match axis specified by last sent command
					{
						Serial.printf("<STAGEERROR>(The Last sent command for Device #%d specified axis #%d but response specified axis #%d.)\n",
							Response.Device, LastSentCommand.Axis, Response.Axis);
					}
				}
				else // reply was rejected
				{
					if (Response.DataLength > 0 && Response.Data[0].Type == ReplyParser::ReplyDataType::Error)
					{
						Serial.printf("<STAGEERROR>(\"%s\" command was rejected with reason: \"%s\".)\n",
							CommandMessage::CommandTypeProperties::Of(LastSentCommand.Type).code, RejectionError::Of(Response.Data[0].Value.Error).ReadableDescription);
					}
					else // reply data did not contain a rejection error
					{
						Serial.printf("<STAGEERROR>(\"%s\" command was rejected for an unspecified reason.)\n",
							CommandMessage::CommandTypeProperties::Of(LastSentCommand.Type).code);
					}

					// print out command that was rejected
					Serial.print("<STAGEERROR>[REJECTED COMMAND](");
					LastSentCommand.PrintVerbose(Serial);
					Serial.print(")\n");

					// print out rejection response to rejected command
					Serial.print("<STAGEERROR>[REJECTION REPLY](");
					Response.print(Serial);
					Serial.print(")\n");
				}
			}
			else // response device does not match device of last sent command
			{
				Serial.printf("<STAGEERROR>(When checking for last sent command to device #%d to get context of response for same device, found command sent to device #%d instead.)\n",
					Response.Device, LastSentCommand.Device);
			}
		}
		else // reply was not expected
		{
			Serial.printf("<STAGEERROR>(Unanticipated reply message for device #%d and axis #%d has no contextual previous command from which to infer its meaning.)\n",
				Response.Device, Response.Axis);
		}

		// Update Statuses if necessary and warn about or investigate unknowns.
		std::vector<uint8_t> checkStatusList{};
		// track whether the callback should not be called at the end of this function because it was forwarded to be called at a later event.
		HandleResponseStatus(Response, newMoveStarted, checkStatusList);

		// we only have a callback if there was a command associated with the reply,
		// and we don't want to notify the global onReplyComplete callback if we don't know the command associated with the reply.
		if (hasLastSentCommand)
		{
			// If a new move started, that callback was delayed to be called after movement is done and the new position is acquired.
			// so don't call it now because the movement isn't done, it will get called later.
			if (!newMoveStarted)
			{
				if (LastSentCommand.Callback)
				{
					LastSentCommand.Callback(Response.Device, Response.Axis);
				}
			}
			// do not call the global callbacks when initializing
			if (!Initializing && OnReplyComplete)
			{
				OnReplyComplete(Response);
			}
		}

		// send get status commands last
		QueryStatusesOfAxes(Response.Device, checkStatusList);

		// report warnings from replies only
		if (Response.Warning != WarningType::None)
		{
			if (LastWarning != Response.Warning)
			{
				Serial.print("<STAGE>(Error:");
				Serial.print(static_cast<uint8_t>(Response.Warning));
				Serial.print(',');
				Serial.print(static_cast<uint8_t>(LastWarning));
				Serial.print(',');
				Serial.print(WarningTypeProperties::Of(Response.Warning).ReadableDescription);
				Serial.print(")\n");
			}
			//SendClearWarnings(Response.Device);
		}
		LastWarning = Response.Warning;
	}

	/**
	 * Call when the Response type is alert and the device index is within the max device range (not the active device range, this method will filter for that).
	 */
	void Controller::HandleAlert(const ReplyParser::ReplyMessage& Response)
	{
		// only process alert responses
		if (Response.Type != ResponseType::Alert)
		{
			Serial.print("<STAGEERROR>(Called to process non-alert message with alert logic. Invalid use.)\n");
			return;
		}

		// response device index must be within max device range
		if (Response.Device < 1 || ZaberMaxDevices < Response.Device)
		{
			Serial.printf("<STAGEERROR>(Cannot process alert with device index #%d outside of range [1, %d].)\n",
				Response.Device, ZaberMaxDevices);
		}

		// alerts specified axis cannot be 0
		if (Response.Axis == 0)
		{
			Serial.print("<STAGEERROR>(Returned alert message with 0 axis. Invalid.)\n");
			return;
		}

		// alerts are always idle
		if (Response.Status != StatusType::Idle)
		{
			Serial.print("<STAGEERROR>(An alert response indicated a busy status. Invalid.)\n");
			return;
		}

		// Any alert response means alerts are enabled
		// only update if all preconditions for a valid alert are passed
		auto& deviceProperties = AllDeviceProperties[Response.Device-1];
		if (!deviceProperties.AlertEnabled)
		{
			// warn about incorrectly tracked state of alerts on device
			Serial.printf("<STAGEWARN>(Device #%d received an alert but tracked state believed alerts were disabled. Updating.)\n");
			deviceProperties.AlertEnabled = true;
		}

		if (Response.Axis > ZaberMaxAxes)
		{
			Serial.printf("<STAGEERROR>(Cannot process status of alert with axis #%d beyond max axes count [%d].)\n",
				Response.Axis, ZaberMaxAxes);
			return;
		}

		// skip HandleResponseStatus and call SetStatus directly because checkStatusList will always be empty.
		SetStatusAccordingToResponse(Response, false);

		// don't call OnMovementComplete at the end of this because it will be called by the OnIdleCallback when the Cmd::getPos gets a reply which was sent by SetStatus
	}

	// process status of response

	/**
	 * Either sets the status of the specified axes to the response status or identifies axes to check the status of.
	 */
	void Controller::HandleResponseStatus(const ReplyParser::ReplyMessage& Response, bool NewMoveStarted, std::vector<uint8_t>& CheckStatusList)
	{
		if (Response.Axis > ZaberMaxAxes)
		{
			Serial.printf("<STAGEERROR>(Cannot process status of response with axis #%d beyond max axes count [%d].)\n",
				Response.Axis, ZaberMaxAxes);
			return;
		}

		if (Response.Axis != 0 || Response.Status == StatusType::Idle || NewMoveStarted)
		{
			// if the axis is specified, then whether the status is idle or busy, we know the axis's status now.
			// if the status is idle, whether the axis is 0 or specific, we know they are idle now.
			// if axis is 0 and the status is busy, then we know they're all busy if the command caused new movement.
			SetStatusAccordingToResponse(Response, NewMoveStarted);
		}
		else // response axis == 0 && Response.Status == BUSY && !NewMoveStarted
		{
			auto& deviceProperties = AllDeviceProperties[Response.Device-1];
			auto specifiedAxes = activeIndexPairMap.GetSpecifiedAxisListForDevice(Response.Device, Response.Axis);

			// if all are idle, warn about unexpected busy status
			if (std::all_of(
				specifiedAxes.begin(), specifiedAxes.end(),
				[&deviceProperties](uint8_t axis)
				{
					return deviceProperties.AllAxisProperties[axis-1].OptStatus == StatusType::Idle;
				}
			))
			{
				// warn about unexpected BUSY status when all active axes were previously IDLE
				Serial.printf("<STAGEWARN>(Unexpected busy status among all axes on device #%d when all axes were previously idle.)\n", Response.Device);
			}

			// if none were busy, check status of all axes,
			// otherwise, just check the status of any that are unknown
			if (std::all_of(
				specifiedAxes.begin(), specifiedAxes.end(),
				[&deviceProperties](uint8_t axis)
				{
					return deviceProperties.AllAxisProperties[axis-1].OptStatus != StatusType::Busy;
				}
			))
			{
				for (uint8_t axis : activeIndexPairMap.GetSpecifiedAxisListForDevice(Response.Device, Response.Axis))
				{
					CheckStatusList.push_back(axis);
				}
			}
			else
			{
				auto& deviceProperties = AllDeviceProperties[Response.Device-1];
				for (uint8_t axis : activeIndexPairMap.GetSpecifiedAxisListForDevice(Response.Device, Response.Axis))
				{
					auto& axisProperties = deviceProperties.AllAxisProperties[axis-1];
					if (!axisProperties.OptStatus.has_value())
					{
						CheckStatusList.push_back(axis);
					}
				}
			}
		}
	}

	/**
	 * Set the status of all axes specified by the response device and axis indexes according to the response status.
	 * Any axes that have become idle will pass on their OnIdle callbacks to the responding behavior to be called once the position has been updated.
	 */
	void Controller::SetStatusAccordingToResponse(const ReplyParser::ReplyMessage& Response, bool NewMoveStarted)
	{
		// if NewMoveStarted, response status must be busy
		if (NewMoveStarted && Response.Status != StatusType::Busy)
		{
			Serial.print("<STAGEERROR>(Invalid use of SetStatus: NewMoveStarted is true but response status is not busy.)\n");
			return;
		}
		// Response Device and Axis must be in bounds of active devices and axes
		if (!activeIndexPairMap.AnyActive(Response.Device, Response.Axis))
		{
			Serial.printf("<STAGEERROR>(Cannot SetStatus because axis/axes tartgeted by device #%d and axis #%d specified by Response is inactive)\n",
				Response.Device, Response.Axis);
		}

		// either we know for certain that an interruption occurred or didn't occur or we're not certain (both false)
		bool interruptionOccured = NewMoveStarted && Response.Status == StatusType::Busy && Response.Warning == WarningType::CommandInterrupted;
		bool interruptionCouldntOccur = !NewMoveStarted || Response.Status != StatusType::Busy || Response.Warning == WarningType::None;
		// both are false when NewMoveStarted and Response.Status is Busy and Response.Warning is not CommandInterrupted nor None

		auto& deviceProperties = AllDeviceProperties[Response.Device-1];

		if (interruptionOccured)
		{
			// warn about interrupted command
			Serial.print("<STAGEWARN>(Previous movement was interrupted by new move command.)\n");

			// if all specified axes were idle, warn about incorrectly previous tracked status of axis/axes
			auto specifiedAxes = activeIndexPairMap.GetSpecifiedAxisListForDevice(Response.Device, Response.Axis);
			if (std::all_of(
				specifiedAxes.begin(), specifiedAxes.end(),
				[&deviceProperties](uint8_t axis)
				{
					return deviceProperties.AllAxisProperties[axis-1].OptStatus == StatusType::Idle;
				}
			))
			{
				Serial.print("<STAGEWARN>(An interruption implies an axis was busy, but all specified axes were tracked as idle.)\n");
			}
		}

		std::vector<uint8_t> becameIdleList{};
		auto now = micros();
		for (uint8_t axis : activeIndexPairMap.GetSpecifiedAxisListForDevice(Response.Device, Response.Axis))
		{
			auto& axisProperties = deviceProperties.AllAxisProperties[axis-1];
			auto previousStatus = axisProperties.OptStatus;
			axisProperties.OptStatus = Response.Status;
			axisProperties.LastStatusUpdateTime = now;

			// track which axes transitioned to idle to send getPos commands in response.
			bool transitionedToIdle = Response.Status == StatusType::Idle && (previousStatus == StatusType::Busy || Response.Type == ResponseType::Alert);
			if (transitionedToIdle)
			{
				becameIdleList.push_back(axis);
			}

			// warnings
			if (transitionedToIdle)
			{
				if (deviceProperties.AlertEnabled && Response.Type != ResponseType::Alert)
				{
					// warn about missing expected alert
					Serial.printf("<STAGEWARN>(Expected an alert to notify axis #%d going idle, but no alert fired before this reply showed an idle status.)\n", axis);
				}
				if (Response.Warning == WarningType::ManualControl)
				{
					// warn about manually controlled stop
					Serial.printf("<STAGEWARN>(Axis #%d was manually stopped.)\n", axis);
				}
			}
			if (previousStatus == StatusType::Busy)
			{
				if (NewMoveStarted)
				{
					// a new move was started while the tracked axis may have previously been busy. That tracked previous status may be out of date.
					// whether out of date or not, the old callback for when it goes idle won't get called now because new movement has started.
					// warn about missed chance to call callback for axis transitioning from busy to idle.
					Serial.printf("<STAGEWARN>(Axis #%d was previously tracked as busy and a new movement has started, so old callback will be missed.)\n", axis);

					// if the previous tracked state was busy and the new state is busy, then we expect an interruption,
					// but if an interruption couldn't have happened because the warning flag is None, then that means it wasn't actually busy and the
					// tracked previous state was not correct. This is an expectable outcome if alerts are disabled and it went idle since the last poll.
					// but if alerts are enabled, then there was a missing alert that should have been received when it went idle.
					if (deviceProperties.AlertEnabled && interruptionCouldntOccur)
					{
						// warn about missing alert message that should have been received before now if there was no interruption
						Serial.printf("<STAGEWARN>(Axis #%d was previously tracked as busy and a new movement should have interrupted it, but there was no interruption, "
							"which means it was not busy. An alert should have notified the system about the axis going to idle, but it was not received.)\n",
							axis);
					}
				}
			}
			else if (previousStatus == StatusType::Idle)
			{
				if (Response.Status == StatusType::Busy && !NewMoveStarted)
				{
					// warn about unknown cause of new BUSY status.
					Serial.printf("<STAGEWARN>(Axis #%d transitioned from idle to busy, but the previous command should not have started new movement.)\n", axis);
				}
				if (Response.Type == ResponseType::Alert)
				{
					// warn about outdated tracked status
					Serial.printf("<STAGEWARN>(An alert indicated axis #%d transitioned from busy to idle, but the previously tracked status was idle.)\n", axis);
				}
			}
		}

		if (Verbose)
		{
			Serial.print("<ZABER>(Set status ");
			Serial.print(Response.Device);
			Serial.print(',');
			Serial.print(Response.Axis);
			Serial.print(',');
			Serial.print(StatusTypeProperties::Of(Response.Status).ReadableName);
			Serial.print(',');
			Serial.print(Response.Status == StatusType::Busy); // was IsBusy global state
			Serial.print(',');
			Serial.print(NewMoveStarted && deviceProperties.AlertEnabled); // was NeedToExpectAlert variable
			Serial.print(',');
			Serial.print(NewMoveStarted); // was TransitionToBusy variable
			Serial.print(")\n");
		}

		// process all axes that went idle
		for (uint8_t axis : becameIdleList)
		{
			ProcessAxisBecameIdle(Response.Device, axis);
		}
	}

	/**
	 * If an axis became idle, a get pos command needs to be sent and the on movement complete callback needs to be called when that command replies.
	 * A forwarded callback from a move command needs to be called as well when that get pos command replies.
	 */
	void Controller::ProcessAxisBecameIdle(uint8_t Device, uint8_t Axis)
	{
		auto& deviceProperties = AllDeviceProperties[Device-1];
		auto& axisProperties = deviceProperties.AllAxisProperties[Axis-1];
		auto callback = axisProperties.OnIdleCallback;

		// forward the callback from the move command to the get command whenever the axis goes idle.
		SendCommandNowOrPrequeueIfBusy(
			CommandMessage::GetPosition(Device, Axis,
				[callback, this](uint8_t dev, uint8_t ax)
				{
					if (callback)
					{
						callback(dev, ax);
					}
					// do not call the global callbacks when initializing
					if (!Initializing && this->OnMovementComplete)
					{
						this->OnMovementComplete(dev, ax);
					}
				}
			)
		);

		// clear this callback from this specified axis to guarantee it only gets called once.
		axisProperties.OnIdleCallback = nullptr;
	}

	void Controller::QueryStatusesOfAxes(uint8_t device, const std::vector<uint8_t>& axes)
	{
		for (uint8_t axis : axes)
		{
			if (!WillQueueCheckAxisStatusSoon(device, axis))
			{
				SendCommandNowOrPrequeueIfBusy(CommandMessage::GetStatus(device, axis));
			}
		}
	}

	// process reply based on type

	void Controller::HandleGetReply(const ReplyParser::ReplyMessage& Response)
	{
		const auto& LastSentCommand = AllDeviceProperties[Response.Device-1].CommStatus.GetLastSentCommand();

		// assert the sent command had a parameter for the get <setting>
		if (LastSentCommand.ParameterCount < 1)
		{
			Serial.print("<STAGEERROR>(Last sent Get command had no parameters. Invalid.)\n");
			return;
		}

		// assert the sent command's first parameter was a setting
		if (LastSentCommand.Parameters[0].Type != CommandMessage::CommandParameterType::Setting)
		{
			Serial.print("<STAGEERROR>(Last sent Get command's first parameter was not a setting. Invalid.)\n");
			return;
		}

		switch (LastSentCommand.Parameters[0].Value.Setting)
		{
			case SettingType::Pos:             HandleGetPosReply(Response); break;
			case SettingType::EncoderPos:      HandleGetEncoderPosReply(Response); break;
			case SettingType::SystemAxisCount: HandleGetSystemAxisCountReply(Response); break;
			case SettingType::MaxSpeed:        HandleGetMaxSpeedReply(Response); break;
			case SettingType::CommAlert:       HandleGetCommAlertReply(Response); break;
			default: break;
		}
	}

	void Controller::HandleSetReply(const ReplyParser::ReplyMessage& Response)
	{
		const auto& LastSentCommand = AllDeviceProperties[Response.Device-1].CommStatus.GetLastSentCommand();

		// assert the sent command has parameters for the set <setting> <value>
		if (LastSentCommand.ParameterCount < 2)
		{
			Serial.print("<STAGEERROR>(Last sent Set command had no parameters. Invalid.)\n");
			return;
		}

		// assert the sent command's first parameter was a setting
		if (LastSentCommand.Parameters[0].Type != CommandMessage::CommandParameterType::Setting)
		{
			Serial.print("<STAGEERROR>(Last sent Set command's first parameter was not a setting. Invalid.)\n");
			return;
		}

		switch (LastSentCommand.Parameters[0].Value.Setting)
		{
			case SettingType::MaxSpeed:  HandleSetMaxSpeedReply(Response); break;
			case SettingType::CommAlert: HandleSetCommAlertReply(Response); break;
			default: break;
		}
	}

	void Controller::HandleRenumberReply(const ReplyParser::ReplyMessage& Response)
	{
		if (Verbose)
		{
			Serial.printf("<ZABERDEVICEFOUND>(%d)\n", Response.Device);
		}
		if (activeIndexPairMap.ActiveDevices < Response.Device)
		{
			if (Verbose)
			{
				Serial.printf("<ZABERDEVICEACTIVATED>(%d)\n", Response.Device);
			}
			activeIndexPairMap.ActiveDevices = Response.Device;
		}
	}

	void Controller::HandleCausesBusyReply(const ReplyParser::ReplyMessage& Response, bool& NewMoveStarted)
	{
		if (Response.Status == StatusType::Busy)
		{
			// tell the update status function to expect a transiton to busy
			// this also informs that the callback has been forwarded
			NewMoveStarted = true;

			auto& deviceProperties = AllDeviceProperties[Response.Device-1];
			for (uint8_t axis : activeIndexPairMap.GetSpecifiedAxisListForDevice(Response.Device, Response.Axis))
			{
				// save the command callback for later if this status was from reply to a move command that succeeded.
				auto& axisProperties = deviceProperties.AllAxisProperties[axis-1];
				axisProperties.OnIdleCallback = deviceProperties.CommStatus.GetLastSentCommand().Callback;
				if (Verbose)
				{
					Serial.printf("<ZABER>(Reassign callback %d,%d)\n", Response.Device, axis);
				}
			}
		}
		else // command causes busy and was not rejected but the reply status was not busy.
		{
			// warn about unexpected idle status when movement command response indicated success via OK reply.
			Serial.printf("<STAGEWARN>(Expected that OK response to movement command would cause movement for axis #%d on device #%d but the response status was not busy.)\n",
				Response.Axis, Response.Device);
		}
	}

	// process set reply based on command setting
	
	void Controller::HandleSetMaxSpeedReply(const ReplyParser::ReplyMessage& Response)
	{
		const auto& LastSentCommand = AllDeviceProperties[Response.Device-1].CommStatus.GetLastSentCommand();

		// assert the sent command has parameter value for the setting is an integer
		if (LastSentCommand.Parameters[1].Type != CommandMessage::CommandParameterType::Integer)
		{
			Serial.print("<STAGEERROR>(Last sent Set Max Speed command's value parameter was not an integer. Invalid.)\n");
			return;
		}

		// update the maxspeed on all the targeted axes
		auto& deviceProperties = AllDeviceProperties[Response.Device-1];
		for (uint8_t axis : activeIndexPairMap.GetSpecifiedAxisListForDevice(Response.Device, Response.Axis))
		{
			deviceProperties.AllAxisProperties[axis-1].MaxSpeed = LastSentCommand.Parameters[1].Value.Integer;
		}
	}

	void Controller::HandleSetCommAlertReply(const ReplyParser::ReplyMessage& Response)
	{
		const auto& LastSentCommand = AllDeviceProperties[Response.Device-1].CommStatus.GetLastSentCommand();

		// assert the sent command has parameter value for the setting is an integer
		if (LastSentCommand.Parameters[1].Type != CommandMessage::CommandParameterType::Integer)
		{
			Serial.print("<STAGEERROR>(Last sent Set Max Speed command's value parameter was not an integer. Invalid.)\n");
			return;
		}

		// update the alert enabled setting on the device
		AllDeviceProperties[Response.Device-1].AlertEnabled = LastSentCommand.Parameters[1].Value.Integer;
	}
	
	// process get reply based on command setting

	void Controller::HandleGetPosReply(const ReplyParser::ReplyMessage& Response)
	{
		if (AreGetReplyPacketsValidForPerAxis(Response, ReplyParser::ReplyDataType::Integer))
		{
			// save the positions to the tracked properties
			auto& deviceProperties = AllDeviceProperties[Response.Device - 1];
			if (Response.Axis == 0)
			{
				// for however many packets there are that can be processed, save them to position.
				for (uint8_t AxisIndex = 0; AxisIndex < Response.DataLength && AxisIndex < ZaberMaxAxes; AxisIndex++)
				{
					deviceProperties.AllAxisProperties[AxisIndex].Position = Response.Data[AxisIndex].Value.Integer;
				}
			}
			else
			{
				// save only the one to the specified axis.
				deviceProperties.AllAxisProperties[Response.Axis-1].Position = Response.Data[0].Value.Integer;
			}

			// report the updated position
			if (Verbose)
			{
				Serial.print("<ZABER>(Pos update: ");
				Serial.print(Response.Device);
				Serial.print(',');
				Serial.print(Response.Axis);
				Serial.print(',');
				Serial.print(Response.DataLength);
				for (uint8_t AxisIndex = 0; AxisIndex < Response.DataLength && AxisIndex < ZaberMaxAxes; AxisIndex++)
				{
					Serial.print(',');
					Response.Data[AxisIndex].print(Serial);
				}
				Serial.print(")\n");
			}
		}
	}

	void Controller::HandleGetEncoderPosReply(const ReplyParser::ReplyMessage& Response)
	{
		if (AreGetReplyPacketsValidForPerAxis(Response, ReplyParser::ReplyDataType::Integer))
		{
			// save the positions to the tracked properties
			auto& deviceProperties = AllDeviceProperties[Response.Device - 1];
			if (Response.Axis == 0)
			{
				for (uint8_t AxisIndex = 0; AxisIndex < Response.DataLength && AxisIndex < ZaberMaxAxes; AxisIndex++)
				{
					deviceProperties.AllAxisProperties[AxisIndex].EncoderPosition = Response.Data[AxisIndex].Value.Integer;
				}
			}
			else
			{
				deviceProperties.AllAxisProperties[Response.Axis - 1].EncoderPosition = Response.Data[0].Value.Integer;
			}

			// report the updated encoded position
			if (Verbose)
			{
				Serial.print("<ZABER>(Encoder position update: ");
				Serial.print(Response.Device);
				Serial.print(',');
				Serial.print(Response.Axis);
				Serial.print(',');
				Serial.print(Response.DataLength);
				for (uint8_t AxisIndex = 0; AxisIndex < Response.DataLength && AxisIndex < ZaberMaxAxes; AxisIndex++)
				{
					Serial.print(',');
					Response.Data[AxisIndex].print(Serial);
				}
				Serial.print(")\n");
			}
		}
	}

	void Controller::HandleGetSystemAxisCountReply(const ReplyParser::ReplyMessage& Response)
	{
		if (AreGetReplyPacketsValidForPerDevice(Response, ReplyParser::ReplyDataType::Integer))
		{
			// save the updated active axis count to the tracked properties
			auto previousActiveAxisCount = activeIndexPairMap.ActiveAxesByDevice[Response.Device-1];
			activeIndexPairMap.ActiveAxesByDevice[Response.Device-1] = Response.Data[0].Value.Integer;
			// keep the active axis count below or equal to the max axes limit
			if (activeIndexPairMap.ActiveAxesByDevice[Response.Device-1] > ZaberMaxAxes)
			{
				activeIndexPairMap.ActiveAxesByDevice[Response.Device-1] = ZaberMaxAxes;
			}

			// report the received get response
			if (Verbose)
			{
				Serial.printf("<ZABER>(axis count update: device #%d -> %d axes.)\n", Response.Device, Response.Data[0].Value.Integer);
			}

			// report the updated axisCount if it changed
			if (previousActiveAxisCount != activeIndexPairMap.ActiveAxesByDevice[Response.Device-1])
			{
				Serial.printf("<ZABER>(Active axis count for device #%d updated from %d to %d.)\n",
					Response.Device, previousActiveAxisCount, activeIndexPairMap.ActiveAxesByDevice[Response.Device-1]);
			}
		}
	}

	void Controller::HandleGetMaxSpeedReply(const ReplyParser::ReplyMessage& Response)
	{
		if (AreGetReplyPacketsValidForPerAxis(Response, ReplyParser::ReplyDataType::Integer))
		{
			// save the speeds to the tracked properties
			auto& deviceProperties = AllDeviceProperties[Response.Device - 1];
			if (Response.Axis == 0)
			{
				for (uint8_t AxisIndex = 0; AxisIndex < Response.DataLength && AxisIndex < ZaberMaxAxes; AxisIndex++)
				{
					deviceProperties.AllAxisProperties[AxisIndex].MaxSpeed = Response.Data[AxisIndex].Value.Integer;
				}
			}
			else
			{
				deviceProperties.AllAxisProperties[Response.Axis - 1].MaxSpeed = Response.Data[0].Value.Integer;
			}

			// report the updated max speed
			if (Verbose)
			{
				Serial.print("<ZABER>(Max speed update: ");
				Serial.print(Response.Device);
				Serial.print(',');
				Serial.print(Response.Axis);
				Serial.print(',');
				Serial.print(Response.DataLength);
				for (uint8_t AxisIndex = 0; AxisIndex < Response.DataLength && AxisIndex < ZaberMaxAxes; AxisIndex++)
				{
					Serial.print(',');
					Response.Data[AxisIndex].print(Serial);
				}
				Serial.print(")\n");
			}
		}
	}

	void Controller::HandleGetCommAlertReply(const ReplyParser::ReplyMessage& Response)
	{
		// expect 1 integer packet
		if (AreGetReplyPacketsValidForPerDevice(Response, ReplyParser::ReplyDataType::Integer))
		{
			// validate that integer is a boolean
			auto enabled = Response.Data[0].Value.Integer;
			if (enabled == 0 || enabled == 1)
			{
				// save updated comm alert
				AllDeviceProperties[Response.Device - 1].AlertEnabled = enabled;

				// report updated comm alert
				if (Verbose)
				{
					Serial.print("<ZABER>(Alert Enabled update: ");
					Serial.print(Response.Device);
					Serial.print(',');
					Serial.print(Response.Axis);
					Serial.print(',');
					Serial.print(Response.DataLength);
					Serial.print(',');
					Response.Data[0].print(Serial);
					Serial.print(")\n");
				}
			}
			else
			{
				Serial.print("<STAGEERROR>(CommAlert Get request packet integer value was not 1 or 0. Invalid.)\n");
			}
		}
	}

	// handle get helpers

	// validate response data packets when the expected amount is the axisCount
	bool Controller::AreGetReplyPacketsValidForPerAxis(const ReplyParser::ReplyMessage& Response, ReplyParser::ReplyDataType ExpectedDataType)
	{
		// there needs to be at least one packet
		if (Response.DataLength == 0)
		{
			Serial.printf("<STAGEERROR>(returned reply for get command for axis #%d on device #%d returned 0 packets.)\n",
				Response.Axis, Response.Device);
			// this is not recoverable. exit early.
			return false;
		}

		// the number of packets returned should equal the number of active axes if axis is 0,
		// otherwise the number of packets returned should be 1
		uint8_t expectedPacketCount = (Response.Axis == 0) ? activeIndexPairMap.ActiveAxesByDevice[Response.Device-1] : 1;
		if (Response.DataLength != expectedPacketCount)
		{
			Serial.printf("<STAGEWARN>(returned reply for get command for axis #%d on device #%d returned %d packets instead of expected %d packets.)\n",
				Response.Axis, Response.Device, Response.DataLength, expectedPacketCount);
				// this is recoverable since there's at least one packet
		}

		// check that all packets are expected type
		for (uint8_t i = 0; i < Response.DataLength; i++)
		{
			if (Response.Data[i].Type != ExpectedDataType)
			{
				Serial.printf("<STAGEERROR>(returned reply for get command for axis #%d on device #%d returned packet #%d which was \"%s\" and not \"%s\".)\n",
					Response.Axis, Response.Device, i, ReplyParser::toString(Response.Data[i].Type), ReplyParser::toString(ExpectedDataType));
				// this is not recoverable. exit early.
				return false;
			}
		}

		// response has been validated for being a get reply
		return true;
	}

	// validate response data packets when the expected amount is 1
	bool Controller::AreGetReplyPacketsValidForPerDevice(const ReplyParser::ReplyMessage& Response, ReplyParser::ReplyDataType ExpectedDataType)
	{
		// there needs to be at least one packet
		if (Response.DataLength == 0)
		{
			Serial.printf("<STAGEERROR>(returned reply for get command for axis #%d on device #%d returned 0 packets.)\n",
				Response.Axis, Response.Device);
			// this is not recoverable. exit early.
			return false;
		}

		// the number of packets returned should be 1
		if (Response.DataLength != 1)
		{
			Serial.printf("<STAGEWARN>(returned reply for get command for axis #%d on device #%d returned %d packets instead of expected 1 packet.)\n",
				Response.Axis, Response.Device, Response.DataLength);
				// this is recoverable since there's at least one packet
		}

		// check that the first packet is of the expected type
		if (Response.Data[0].Type != ExpectedDataType)
		{
			Serial.printf("<STAGEERROR>(returned reply for get command for axis #%d on device #%d returned packet #0 which was \"%s\" and not \"%s\".)\n",
				Response.Axis, Response.Device, ReplyParser::toString(Response.Data[0].Type), ReplyParser::toString(ExpectedDataType));
			// this is not recoverable. exit early.
			return false;
		}

		// response has been validated for being a get reply
		return true;
	}
}
