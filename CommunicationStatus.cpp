#include "CommunicationStatus.hpp"

namespace Zaber
{
	void CommunicationStatus::SetCommandSent(const CommandMessage& Command)
	{
		_LastSentCommand = Command;
		_IsAwaitingReply = true;
		_LastSentTime = micros();
	};

	void CommunicationStatus::SetNoLongerWaiting()
	{
		// Don't clear last sent command since getLastSentCommand might be needed after the reception is confirmed
		_IsAwaitingReply = false;
		_LastSentTime = 0;
	};

	bool CommunicationStatus::IsAwaitingReply() const
	{
		return _IsAwaitingReply;
	};

	const CommandMessage& CommunicationStatus::GetLastSentCommand() const
	{
		return _LastSentCommand;
	};

	uint32_t CommunicationStatus::GetDurationWaiting() const
	{
		if (_IsAwaitingReply) return micros() - _LastSentTime;
		return 0;
	};

	bool CommunicationStatus::HasTimedOut() const
	{
		// only expect a reply for half a second after sending the command.
		return GetDurationWaiting() > 500'000;
	};
}