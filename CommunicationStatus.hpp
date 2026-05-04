#ifndef ZABER_COMMUNICATION_STATUS_HPP //check for multiple inclusions
#define ZABER_COMMUNICATION_STATUS_HPP

#include "CommandMessage.hpp"

namespace Zaber
{
	class CommunicationStatus
	{
	private:
		bool _IsAwaitingReply = false;
		CommandMessage _LastSentCommand = CommandMessage();
		uint32_t _LastSentTime = 0;

	public:
		CommunicationStatus() {};

		void SetCommandSent(const CommandMessage& Command);
		void SetNoLongerWaiting();

		bool IsAwaitingReply() const;
		const CommandMessage& GetLastSentCommand() const;
		uint32_t GetDurationWaiting() const;

		bool HasTimedOut() const;
	};
}

#endif // #define ZABER_COMMUNICATION_STATUS_HPP
