#ifndef PORT_SOCKET_H_
#define PORT_SOCKET_H_

#include "base/CTypedefs.h"

#if defined(PTHREAD)
    #include <pthread.h>
#endif

#if defined(WINCE)
    #include <winsock2.h>
    #include <assert.h>
    #include <stdarg.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

#elif defined(WIN32)
    #include <windows.h>
    #include <assert.h>
    #include <stdarg.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

#elif defined(LINUX)
    #include <stdlib.h>
    #include <unistd.h>
    #include <string.h>
    #include <arpa/inet.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <linux/socket.h>
    #include <netinet/tcp.h>
    #include <netdb.h>

#elif defined(SYS_BIOS6)
#if defined(NETWORK_ENABLED)
    #include "ti/sysbios/knl/Semaphore.h"
    #include "netmain.h"
    #include "_stack.h"
    #include "nettools/netcfg.h"
    #include "nettools/inc/configif.h"
    #include "netctrl/netctrl.h"
    #include "usertype.h"
#endif /* NETWORK_ENABLED */

#endif  // #if defined(WINCE)


#if defined(WINCE)
    typedef SOCKET SOCK_T;

#elif defined(WIN32)
    typedef SOCKET SOCK_T;
    #define MAX_PACKET_SIZE     (256*1024 - 24)

#elif defined(LINUX)
    typedef int SOCK_T;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;

    #define MAX_PACKET_SIZE     (256*1024 - 24)

#elif defined(SYS_BIOS6)
#if defined(NETWORK_ENABLED)
    typedef HANDLE SOCK_T;

    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
#endif /* NETWORK_ENABLED */
#endif  // #if defined(WINCE)

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { 
	SOCKET_NONE = 0,
	SOCKET_TCP, 
	SOCKET_UDP, 
	SOCKET_RAW
} _SOCK_PROTOCOL_TYPE;

typedef int SOCK_PROTOCOL_TYPE;

#if defined(WIN32)
	typedef char OPTVAL_T;
	typedef char OPTLEN_T;
#else
	typedef void	OPTVAL_T;	
	typedef int 	OPTLEN_T;
#endif

	SOCK_T	Port_SOCK_Create(SOCK_PROTOCOL_TYPE eType);

	SOCK_T	Port_SOCK_Accept(SOCK_T sock, SOCKADDR* pSockAddr, int32_t* pnSize);
	int32_t		Port_SOCK_Bind(SOCK_T sock, SOCKADDR* pSockAddr, int32_t nSize);
	int32_t		Port_SOCK_Listen(SOCK_T sock, int32_t nSOMAX );
	int32_t		Port_SOCK_Connect(SOCK_T sock, SOCKADDR* pSockAddr, int32_t nSize);

	int32_t		Port_SOCK_Send(SOCK_T sock, char_t* pcBuf, int32_t nLen, int32_t nFlag);
	int32_t		Port_SOCK_Receive(SOCK_T sock, char_t* pcBuf, int32_t nLen, int32_t nFlag);

	int32_t		Port_SOCK_Sendto(SOCK_T sock, char_t* pcBuf, int32_t nLen, int32_t nFlag, SOCKADDR* pSockAddr, int32_t nSize);
	int32_t		Port_SOCK_ReceiveFrom(SOCK_T sock, char_t* pcBuf, int32_t nLen, int32_t nFlag, SOCKADDR* pSockAddr, int32_t* pnSize);

	int32_t		Port_SOCK_StartUp(void);
	int32_t		Port_SOCK_CleanUp(void);

	int32_t 	Port_SOCK_CloseSocket(SOCK_T sock);

	int32_t		Port_SOCK_GetError(void);

	int32_t		Port_SOCK_GetCurrentIPAddress(char_t* pcBuf, int32_t nLen, int32_t* pnSize);

	int32_t		Port_SOCK_GetSocketOpt(SOCK_T sock, int32_t level, int32_t optname, OPTVAL_T *optval, OPTLEN_T *optlen);
	int32_t		Port_SOCK_SetSocketOpt(SOCK_T sock, int32_t level, int32_t optname,  const  OPTVAL_T  *optval, OPTLEN_T optlen);

#ifdef __cplusplus
}
#endif

#endif // PORT_TASK_H_

