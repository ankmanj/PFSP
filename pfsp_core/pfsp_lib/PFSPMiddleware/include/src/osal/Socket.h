#ifndef SOCKET_H_
#define SOCKET_H_

#include "osal/port/Port_Socket.h"
#include "base/CTypedefs.h"

#ifdef __cplusplus
extern "C" {
#endif
	
	SOCK_T	SOCK_Create(SOCK_PROTOCOL_TYPE eType);

	SOCK_T	SOCK_Accept(SOCK_T sock, SOCKADDR* pSockAddr, int32_t* pnSize);
	int32_t	SOCK_Bind(SOCK_T sock, SOCKADDR* pSockaddr, int32_t nSize);
	int32_t	SOCK_Listen(SOCK_T sock, int32_t nSOMAX );
	int32_t	SOCK_Connect(SOCK_T sock, SOCKADDR* pSockAddr, int32_t nSize);

	int32_t	SOCK_Send(SOCK_T sock, char_t* pcBuf, int32_t nLen, int32_t nFlag);
	int32_t	SOCK_Receive(SOCK_T sock, char_t* pcBuf, int32_t nLen, int32_t nFlag);

	int32_t	SOCK_Sendto(SOCK_T sock, char_t* pcBuf, int32_t nLen, int32_t nFlags, SOCKADDR* pSockAddr, int32_t nSize);
	int32_t	SOCK_ReceiveFrom(SOCK_T sock, char_t* pcBuf, int32_t nLen, int32_t nFlags, SOCKADDR* pSockAddr, int32_t* pnSize);

	int32_t	SOCK_Close(SOCK_T sock);

	int32_t	SOCK_StartUp(void);
	int32_t	SOCK_CleanUp(void);

	int32_t	SOCK_GetError(void);

	int32_t	SOCK_GetCurrentIPAddress(char_t* pcBuf, int32_t nLen);

	int32_t	SOCK_GetSocketOpt(SOCK_T sock, int32_t level, int32_t optname, OPTVAL_T *optval, OPTLEN_T *optlen);
	int32_t	SOCK_SetSocketOpt(SOCK_T sock, int32_t level, int32_t optname, const  OPTVAL_T  *optval, OPTLEN_T optlen);

#ifdef __cplusplus
}
#endif

#endif // SOCKET_H_

