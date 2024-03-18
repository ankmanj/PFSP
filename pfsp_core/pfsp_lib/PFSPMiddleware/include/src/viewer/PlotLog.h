#ifndef PLOTLOG_H_
#define PLOTLOG_H_

#include "base/CTypedefs.h"
#include "interface/ILoggable.h"

#define MAX_PLOTDATANAME (32)

typedef enum
{
	eDATA_TYPE_INT16T = 20,
	eDATA_TYPE_INT16T_PARRAY,
	eDATA_TYPE_INT16T_SARRAY,
	eDATA_TYPE_INT32T,
	eDATA_TYPE_INT32T_PARRAY,
	eDATA_TYPE_INT32T_SARRAY,
	eDATA_TYPE_FLOAT32T,
	eDATA_TYPE_FLOAT32T_PARRAY,
	eDATA_TYPE_FLOAT32T_SARRAY,
	eDATA_TYPE_FLOAT64T,
	eDATA_TYPE_FLOAT64T_PARRAY,
	eDATA_TYPE_FLOAT64T_SARRAY
}DataType_t;


class CPlotLog : public ILoggable
{
public :
	CPlotLog(void);
	CPlotLog(char_t* pData, int32_t nSize);
	CPlotLog(char_t* pszDataName, DataType_t nDataType, int32_t nSize=1);
	virtual ~CPlotLog(void);

	virtual char_t* Serialize(int32_t* nLength);
	virtual void Unserialize(char_t* pszSerializedStream, int32_t nLength);

	char_t*  	m_pszDataName;
	DataType_t*	m_peDataType;
	int32_t*	m_pnSizeOfData;
	int8_t*		m_parData;

	void SetData(int16_t nData);
	void SetData(int16_t* pnData);
	void SetData(int32_t nData);
	void SetData(int32_t* pnData);
	void SetData(float32_t fData);
	void SetData(float32_t* pfData);
	void SetData(float64_t fData);
	void SetData(float64_t* pfData);

	// 변수 추가 필요 

	virtual const char_t* GetLogMsgName(void);

	char_t* m_pSerializeBuffer;
};

#endif  /* PLOTLOG_H_ */