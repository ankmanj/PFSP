#ifndef CRC_H_
#define CRC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "base/CTypedefs.h"

uint32_t CRC_GenerateCrcPacket(uint8_t *pucPacket, int32_t nLength);
uint32_t CRC_GenerateCrcPacket2(uint8_t *pucPacket, int32_t nLength, uint8_t *pucPacket2, int32_t nLength2);

#ifdef __cplusplus
}
#endif

#endif // CRC_H_
