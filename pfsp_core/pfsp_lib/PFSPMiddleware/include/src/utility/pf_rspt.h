#ifndef PF_RSPT_H_
#define PF_RSPT_H_

#ifdef TI_DSP_C6000_
//#define __TEST_RSPT__
#endif
//#ifdef _HOST
//#ifdef __CORTEXM3__
//#define __TEST_RSPT__
//#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SYS_BIOS6

#define MAX_NUM_TIME 				(15*10)
#define PF_RSPT_MAX_INFO			24
#define PF_RSPT_MODE_EV_PERIOD		0x01
#define PF_RSPT_MODE_AB_PERIOD		0x02
#define PF_RSPT_MODE_AB_CNT			0x04

#define CVE_RSPT_IDX_OnMessage 		0
#define CVE_RSPT_IDX_Process  		1
#define CVE_RSPT_IDX_CVI_PRE		2
#define CVE_RSPT_IDX_CVI_POST		3
#define CVE_RSPT_IDX_CVI_EXE		4
#define DSP_RSPT_IDX_USER			6

#define CAP_RSPT_IDX_OnRead			0
#define CAP_RSPT_IDX_ReadProc		1
#define CAP_RSPT_IDX_ImgGet			2
#define CAP_RSPT_IDX_ImgPub			3
#define CAP_RSPT_IDX_OvRead			4
#define CAP_RSPT_IDX_OvProc1		5
#define CAP_RSPT_IDX_OvProc2		6

#ifdef __TEST_RSPT__

#define PF_SET_EVT(x)				pf_rspt_setevtt(x)
#define PF_SET_STT(x)				pf_rspt_setstt(x)
#define PF_SET_ETT(x)				pf_rspt_setett(x)
#define PF_SET_ETTCNT(x,y)			pf_rspt_setettcnt(x,y)
#define PF_PRINT_ALL(x)				pf_rspt_printall(x)
#define PF_RSPT_INIT()				pf_rspt_init()

#else

#define PF_SET_EVT(x)
#define PF_SET_STT(x)
#define PF_SET_ETT(x)
#define PF_SET_ETTCNT(x,y)
#define PF_PRINT_ALL(x)
#define PF_RSPT_INIT()

#endif

typedef struct {
	Types_Timestamp64 tsxdc; /* hi, lo */
	union {
		unsigned long long tsll;
		struct {
			unsigned long tslo;
			unsigned long tshi;
		};
	};
} RSPT_TS;

typedef struct
{
	char name[64];
	
	int lock;
	int idx;
	int mode;

	int cnt[MAX_NUM_TIME+1];
	RSPT_TS stt[MAX_NUM_TIME+1];
	RSPT_TS ett[MAX_NUM_TIME+1];
	
}PF_RSPT_INFO, *PPF_RSPT_INFO;

typedef struct
{
	unsigned long long total;
	unsigned long long average;
	unsigned long long totalcnt;

	unsigned long long peak;
	unsigned long long peak_next;
	unsigned long long min;
	unsigned long long min_next;

	int peak_idx;
	int peak_next_idx;
	int min_idx;
	int min_next_idx;

	unsigned long long diff_total;
	unsigned long long diff_average;

	unsigned long long diff_peak;
	unsigned long long diff_peak_next;
	unsigned long long diff_min;
	unsigned long long diff_min_next;

	int diff_peak_idx;
	int diff_peak_next_idx;
	int diff_min_idx;
	int diff_min_next_idx;
} PF_RSPT_RS_INFO, *PPF_RSPT_RS_INFO;

typedef struct
{
	int createidx;
	union {
		unsigned long long cpufreq;
		struct {
			uint32_t cpufreqlo;
			uint32_t cpufreqhi;
		};
	};

	PF_RSPT_INFO createinfos[PF_RSPT_MAX_INFO];
	PF_RSPT_RS_INFO rsinfos[PF_RSPT_MAX_INFO];
}PF_RSPT_MGR_INFO, *PPF_RSPT_MGR_INFO;

typedef struct
{
	int lock;
	PF_RSPT_MGR_INFO info[4];
}PF_RSPT_MGR, *PPF_RSPT_MGR;

extern PPF_RSPT_INFO pMyRsptInfo[PF_RSPT_MAX_INFO];

PPF_RSPT_INFO pf_rspt_create(char* name, int mode);

void pf_rspt_init();
void pf_rspt_reset(PPF_RSPT_INFO pInfo);
void pf_rspt_print(PPF_RSPT_INFO pinfo, int numinfo, int breset, unsigned long long cpufreq);
void pf_rspt_printall(int breset);

void pf_rspt_setstt(PPF_RSPT_INFO pInfo);
void pf_rspt_setevtt(PPF_RSPT_INFO pInfo);
void pf_rspt_setett(PPF_RSPT_INFO pInfo);
void pf_rspt_setettcnt(PPF_RSPT_INFO pInfo, int cnt);

static inline int pf_rspt_lock(PPF_RSPT_INFO pInfo)
{
	if(pInfo->lock > 0) return -1;

	pInfo->lock++;
	if(pInfo->lock > 1) {
		pInfo->lock--;
		return -1;
	}
	
	return 0;
}


static inline void pf_rspt_unlock(PPF_RSPT_INFO pInfo)
{
	pInfo->lock--;
}


#else /* SYS_BIOS6 */

#define PF_SET_EVT(x)
#define PF_SET_STT(x)
#define PF_SET_ETT(x)
#define PF_SET_ETTCNT(x,y)
#define PF_PRINT_ALL(x)
#define PF_RSPT_INIT()

#endif /* SYS_BIOS6 */

#ifdef __cplusplus
}
#endif

#endif /* PF_RSPT_H_ */

