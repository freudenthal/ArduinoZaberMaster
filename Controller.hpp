/*

Zaber master controller. Takes a serial bus and controls a Zaber bus.

*/

#ifndef ZABER_CONTROLLER_HPP //check for multiple inclusions
#define ZABER_CONTROLLER_HPP

#include "Arduino.h"
#include "Types.hpp"
#include "ReplyParser.hpp"
#include "FixedCapacityDeque.hpp"
#include "CommandMessage.hpp"
#include "CommunicationStatus.hpp"
#include "IndexPair.hpp"

#include <vector>
#include <type_traits>

#define ReturnBufferSize 64
#define ZaberDefaultSpeed 250000
#define ZaberQueueCount 10

namespace Zaber
{
	class Controller
	{
	public:
		// inner types and structs
		using Listener = std::function<void()>;
		using ReplyListener = std::function<void(const ReplyParser::ReplyMessage& Response)>;
		using DeviceAxisListener = CommandMessage::DeviceAxisListener;

		enum class InitializationType : uint8_t
		{
			LinearStandard,
			RotationalStandard,
			LinearAndRotational,
		};

		struct AxisPropertiesStruct
		{
			std::optional<StatusType> OptStatus = std::nullopt;
			uint32_t LastStatusUpdateTime = 0;
			uint32_t EncoderPosition = 0;
			uint32_t Position = 0;
			uint32_t MaxSpeed = 0;
			DeviceAxisListener OnIdleCallback = nullptr;
		};

		struct DevicePropertiesStruct
		{
			CommunicationStatus CommStatus;
			AxisPropertiesStruct AllAxisProperties[ZaberMaxAxes];
			bool AlertEnabled = false;
		};

		// public methods

		// constructor and destructor
		Controller(Stream& serial);
		~Controller();

		// setup
		bool Begin(uint8_t NumberOfDevices, uint8_t* AxisCount);

		// verbose
		void SetVerbose(bool VerboseToSet);
		bool GetVerbose() const;

		// initialization configuration and invokation
		bool SetInitializationType(InitializationType TypeToSet);
		bool Initialize();

		// accessors for tracked properties of active axes
		bool IsAxisSpecificAndActive(uint8_t Device, uint8_t Axis) const;
		std::optional<uint32_t> GetPosition(uint8_t Device, uint8_t Axis) const;
		std::optional<uint32_t> GetEncoderPosition(uint8_t Device, uint8_t Axis) const;
		std::optional<uint32_t> GetMaxSpeed(uint8_t Device, uint8_t Axis) const;
		std::optional<StatusType> GetStatus(uint8_t Device, uint8_t Axis) const;

		// queue a command to be sent during check
		bool EnqueueCommand(const CommandMessage& CommandForEnqueue);

		// set callbacks for various triggers
		void SetInitializationCompleteCallback(Listener _InitializationComplete);
		void SetMovementCompleteCallback(DeviceAxisListener _MovementComplete);
		void SetReplyCompleteCallback(ReplyListener _ReplyComplete);

		// update logic for querying communication over serial
		void Check();

	private:

		// private fields
		Stream& _StreamPort;
		bool Active;

		bool Initializing;
		InitializationType InitializationBehavior;
		CommandMessage* InitilizationSteps;
		uint8_t InitializationStep;
		uint8_t InitializationStepsCount;
		
		// active device and axes count and device properties array
		ActiveIndexPairMap activeIndexPairMap;
		DevicePropertiesStruct AllDeviceProperties[ZaberMaxDevices];

		// callbacks
		DeviceAxisListener OnMovementComplete;
		ReplyListener OnReplyComplete;
		Listener OnInitializationComplete;

		// serial buffer
		char ReturnBuffer[ReturnBufferSize];
		uint8_t ReturnBufferCount;

		// print flags
		bool Verbose;
		WarningType LastWarning;

		// command queue
		FixedCapacityDeque<CommandMessage, ZaberQueueCount> CommandQueue;

		// private helper methods
		
		// check various parts of the controller from the check command
		void CheckCommandQueue();
		void CheckSerial();
		void PollBusyAxesAndCheckForTimeouts();
		void CheckInitialization();

		// assess status of queue
		bool CanCommandSendNow(const CommandMessage& Command) const;
		bool CanCommandSendNowWithoutInterrupting(const CommandMessage& Command) const;
		bool AreAllAxesNotAwaitingReplyAndNotBusy() const;
		bool WillQueueCheckAxisStatusSoon(uint8_t device, uint8_t axis) const;

		// assess validity of commands or axes
		void ReportWhyCommandTargetsInvalid(const CommandMessage& command) const;

		// sendnow or queue
		void SendCommandNow(const CommandMessage& Command);
		bool SendCommandNowOrPrequeueIfBusy(const CommandMessage& Command);

		// process serial response from connected Zaber
		void ProcessReturnBuffer();
		void ClearReturnBuffer();
		void PrintReturnBuffer() const;

		// process response based on type
		void ProcessResponse(const ReplyParser::ReplyMessage& Response);
		void HandleReply(const ReplyParser::ReplyMessage& Response);
		void HandleAlert(const ReplyParser::ReplyMessage& Response);

		// process status of response
		void HandleResponseStatus(const ReplyParser::ReplyMessage& Response, bool NewMoveStarted, std::vector<uint8_t>& CheckStatusList);
		void SetStatusAccordingToResponse(const ReplyParser::ReplyMessage& Response, bool NewMoveStarted);
		void ProcessAxisBecameIdle(uint8_t Device, uint8_t Axis);
		void QueryStatusesOfAxes(uint8_t device, const std::vector<uint8_t>& axes);

		// process reply based on type
		void HandleGetReply(const ReplyParser::ReplyMessage& Response);
		void HandleSetReply(const ReplyParser::ReplyMessage& Response);
		void HandleRenumberReply(const ReplyParser::ReplyMessage& Response);
		void HandleCausesBusyReply(const ReplyParser::ReplyMessage& Response, bool& NewMoveStarted);

		// process set reply based on command setting
		void HandleSetMaxSpeedReply(const ReplyParser::ReplyMessage& Response);
		void HandleSetCommAlertReply(const ReplyParser::ReplyMessage& Response);

		// process get reply based on command setting
		void HandleGetPosReply(const ReplyParser::ReplyMessage& Response);
		void HandleGetEncoderPosReply(const ReplyParser::ReplyMessage& Response);
		void HandleGetSystemAxisCountReply(const ReplyParser::ReplyMessage& Response);
		void HandleGetMaxSpeedReply(const ReplyParser::ReplyMessage& Response);
		void HandleGetCommAlertReply(const ReplyParser::ReplyMessage& Response);

		// handle get helpers
		bool AreGetReplyPacketsValidForPerAxis(const ReplyParser::ReplyMessage& Response, ReplyParser::ReplyDataType ExpectedDataType);
		static bool AreGetReplyPacketsValidForPerDevice(const ReplyParser::ReplyMessage& Response, ReplyParser::ReplyDataType ExpectedDataType);

		// static constants
		static constexpr uint32_t POLLING_DELAY_MICROS_NO_ALERTS = 100'000; // 1/10th second
		static constexpr uint32_t POLLING_DELAY_MICROS_ALERTS_ON = 10'000'000; // 10 seconds

		static const CommandMessage LinearStandardInitilizationSteps[];
		static const uint8_t LinearStandardInitilizationStepCount;
		static const CommandMessage RotationalStandardInitilizationSteps[];
		static const uint8_t RotationalStandardInitilizationStepCount;
		static const CommandMessage LinearAndRotationalInitilizationSteps[];
		static const uint8_t LinearAndRotationalInitilizationStepCount;
	};
}

#endif // ZABER_CONTROLLER_HPP
