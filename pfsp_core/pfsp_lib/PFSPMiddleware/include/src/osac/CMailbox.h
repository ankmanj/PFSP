#ifndef CMAILBOX_H_
#define CMAILBOX_H_
#include <vector>

#include "base/CTypedefs.h"
#include "osal/Mailbox.h"

using namespace std;

namespace OSAC {

class CMailbox {
public:
	static const int32_t CMAIL_WAIT_INFINITE = MAIL_WAIT_INFINITE;

public:
	CMailbox(void);
	~CMailbox(void);

	int32_t Create(char_t* const pcName, const uint32_t unMaxQueueCount, const uint32_t unMailLength, const uint32_t unTotalMailboxSize);
	int32_t Destroy(void);

	int32_t Open(const char_t* pcName, int32_t* pnTotalMailboxSize);
	int32_t Close(void);

	int32_t Send(char_t* pcBuf, const uint32_t unSize, const int32_t nMsWait  = CMAIL_WAIT_INFINITE) const;
	int32_t Receive(char_t* const pcBuf, const uint32_t unSize, const int32_t nMsWait  = CMAIL_WAIT_INFINITE) const;

	int32_t GetCount(void) const;
	int32_t Flush(void) const;

	int32_t GetMailSize(void) const;
	int32_t GetMailboxSize(void) const;

private:
	MAIL_T m_cContent;
	char_t*  m_pcName;
    std::vector<char_t*> mailBoxName_ {};
    std::vector<MAIL_T> mailBox_ {};
};

}

#endif	/* CMAILBOX_H_ */