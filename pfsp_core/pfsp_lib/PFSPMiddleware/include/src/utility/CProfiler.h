#ifndef CPROFILER_H_
#define CPROFILER_H_

#ifndef WIN32
#include "base/CTypedefs.h"
#include <string.h>
#include <sys/time.h>
#include "osal/Debug.h"
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

#define DEFINE_PROFILE_ITEM(sNodePath) \
	static StaticProfileNodePtr __NodePtr ## __LINE__(sNodePath);\
	CProfile __CProfile ## __LINE__(__NodePtr ## __LINE__.pstNode)

struct ProfileNode
{
	int nExecCnt;				// total execution count for class node
	int nMinExecTime[5];			// min-max peak time
	int nMaxExecTime[5];			// min-max peak time
	long nExecTime;				// total execution time for class node
	float nAvgExecTime;
	const char* pcNode;
};

class StaticProfileNodePtr
{
public:
	StaticProfileNodePtr(const char* sNodePath)
	{
		//printf("node ptr : %s\n", sNodePath);
		pstNode = (ProfileNode *)calloc(1, sizeof(ProfileNode));
		pstNode->pcNode = sNodePath;
	}

	ProfileNode *pstNode;
};

/**
*	@class		CProfile
*	@brief
*	@details
*	@see		
*/
class CProfile {
public:
	CProfile(ProfileNode* pstNode);
	~CProfile();

	void ProfilePrint(ProfileNode* pstNode, int nDiff);
	void sort(int* nList, int nCnt, int nOption);

protected:
	ProfileNode* m_poNode;
#ifndef WIN32
	struct timeval m_nStartTime;
#endif
};
#else
#define DEFINE_PROFILE_ITEM(sNodePath)
#endif

#endif  /* CPROFILER_H_ */