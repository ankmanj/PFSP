#ifndef CTYPEDEFS_H_
#define CTYPEDEFS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{

#endif /* _cplusplus */

#define NO_ARGS_COMPILE_WARNING(a) {void* pv = (void*)&a;pv=pv;}

#ifndef TRUE
#define TRUE 	(1U)
#endif /* TRUE */

#ifndef FALSE
#define FALSE 	(0U)
#endif /*FALSE */

#if defined(WIN32)

#define RESTRICT __restrict
#define INLINE __inline

#define PACK(N) __pragma(pack(push, N))
#define UNPACK	__pragma(pack(pop))

#elif defined(ARM) || defined(__GNUC__)	//Linux

#define RESTRICT __restrict__
#define INLINE __inline
// Temporal warning elimination
#define PACK(N) 
#define UNPACK __attribute__ ((__packed__))

#elif defined(TI_DSP_C6000_)

#define RESTRICT __restrict__
#define INLINE inline

#define PACK(N) _Pragma(pack(push, N))
#define UNPACK	_Pragma(pack(pop))

#else
#error Platform Definition Error.
#endif
/**
 *	@def	_INT32_MAX	(2147483647)
 */
#ifndef _INT32_MAX
#define _INT32_MAX	(2147483647)
#endif
/**
 *	@def	_INT16_MAX	(32767)
 */
#ifndef _INT16_MAX
#define _INT16_MAX	(32767)
#endif
/**
 *	@def	_INT8_MAX	(32767)
 */
#ifndef _INT8_MAX
#define _INT8_MAX	(127)
#endif
/**
 *	@def	_UINT32_MAX	(0xFFFFFFFF)
 */
#ifndef _UINT32_MAX
#define _UINT32_MAX	(0xFFFFFFFF)
#endif
/**
 *	@def	_UINT16_MAX	(0xFFFF)
 */
#ifndef _UINT16_MAX
#define _UINT16_MAX	(0xFFFF)
#endif
/**
 *	@def	_UINT8_MAX	(0xFFFF)
 */
#ifndef _UINT8_MAX
#define _UINT8_MAX	(0xFF)
#endif
/**
 *	@def	_INT32_MIN	(-2147483647 - 1)
 */
#ifndef _INT32_MIN
#define _INT32_MIN	(-2147483647 - 1)
#endif
/**
 *	@def	_INT16_MIN	(-32767 - 1)
 */
#ifndef _INT16_MIN
#define _INT16_MIN	(-32767 - 1)
#endif
/**
 *	@def	_INT8_MIN	(-127 - 1)
 */
#ifndef _INT8_MIN
#define _INT8_MIN	(-127 - 1)
#endif
/**
 *	@def	_UINT32_MIN	(0x00000000)
 */
#ifndef _UINT32_MIN
#define _UINT32_MIN	(0x00000000)
#endif
/**
 *	@def	_UINT16_MIN	(0x0000)
 */
#ifndef _UINT16_MIN
#define _UINT16_MIN	(0x0000)
#endif
/**
 *	@def	_UINT8_MIN	(0x00)
 */
#ifndef _UINT8_MIN
#define _UINT8_MIN	(0x00)
#endif


#ifndef	_MIN_
#define _MIN_(x, y)   (((x) < (y)) ? (x) : (y))
#endif

#ifndef	_MAX_
#define _MAX_(x, y)   (((x) > (y)) ? (x) : (y))
#endif

#if !defined(__TI_COMPILER_VERSION__)
#ifndef	_MIN
#define _MIN(x, y)   (((x) < (y)) ? (x) : (y))
#endif

#ifndef	_MAX
#define _MAX(x, y)   (((x) > (y)) ? (x) : (y))
#endif
#endif

#ifndef __GNUC__
#ifndef	min
#define min(x, y)   (((x) < (y)) ? (x) : (y))
#endif

#ifndef	max
#define max(x, y)   (((x) > (y)) ? (x) : (y))
#endif
#endif
#define __MAX_STRING_SIZE (256U)
#define __MAX_STRING_DOUBLESIZE (512U)
#define __MAX_STRING_QUADRAPLESIZE (1024U)

#ifdef _MSC_VER

#define CCALL __cdecl
#pragma section(".CRT$XCU",read)
#define INITIALIZER(f) \
	static void __cdecl f(void); \
	__declspec(allocate(".CRT$XCU")) void (__cdecl*f##_)(void) = f; \
	static void __cdecl f(void)

#elif defined(__GNUC__)

#define CCALL
#define INITIALIZER(f) \
	static __attribute__((constructor, used)) void f(void)

#endif

#if defined(WIN32)

	typedef char				char_t;
	typedef unsigned char		uchar_t;
	typedef float				float32_t;
	typedef double			float64_t;

	typedef long signed int		long_t;
	typedef uintptr_t			uaddr_t;
	typedef intptr_t			addr_t;

	#define snprintf _snprintf

	#define RP_THREAD_LOCAL __declspec(thread)
	#ifndef COROUTINE
	#define _TLS_SUPPORT_
	#endif
	#ifdef _USRDLL
//j		#include "stdafx.h"
	#endif

#elif defined(__linux)

	#define __int8_t_defined

	typedef char				char_t;
	typedef unsigned char		uchar_t;
	typedef signed char			int8_t;
	typedef unsigned char		uint8_t;
	typedef signed short		int16_t;
	typedef unsigned short		uint16_t;
	typedef signed int			int32_t;
	typedef unsigned int		uint32_t;
	typedef long signed int		long_t;
	typedef long unsigned int	ulong_t;
#if 0
#if !defined(__linux64__)
	typedef signed long long	int64_t;
	typedef unsigned long long	uint64_t;
#else
	typedef long signed int 		intl64_t;
	typedef long unsigned int 	uint64_t;

#endif
#endif
	typedef float				float32_t;
	typedef double			float64_t;

	typedef intptr_t			addr_t;
	typedef uintptr_t			uaddr_t;

	#ifdef __GNUC__
		#define RP_THREAD_LOCAL __thread
	#else
		#define RP_THREAD_LOCAL __declspec(thread)
	#endif
	#define _TLS_SUPPORT_

#elif defined(_TMS320C6740)


	typedef char			char_t;
	typedef unsigned char		uchar_t;
#if 0
	typedef signed char			int8_t;
	typedef unsigned char		uint8_t;
	typedef signed short		int16_t;
	typedef unsigned short		uint16_t;
	typedef signed int			int32_t;
	typedef unsigned int		uint32_t;
	typedef signed long			int40_t;
	typedef unsigned long		uint40_t;
	typedef signed long long	int64_t;
	typedef unsigned long long	uint64_t;
#else
	#include <stdint.h>
#endif
#ifndef _VECT
	typedef float				float32_t;
#endif
	typedef double				float64_t;

	#define RP_THREAD_LOCAL

#elif defined(STM32F2XX)

	typedef char				char_t;
	typedef unsigned char		uchar_t;

	#if !defined(__STM32F2xx_H)
		typedef signed char			int8_t;
		typedef unsigned char		uint8_t;
		typedef signed short		int16_t;
		typedef unsigned short		uint16_t;
		typedef signed int			int32_t;
		typedef unsigned int		uint32_t;
		typedef signed long long	int64_t;
		typedef unsigned long long	uint64_t;
	#endif

	typedef float				float32_t;
	typedef long double			float64_t;

	// ignore warnings for constant/interface
	#pragma diag_suppress=Pa050
	#pragma diag_suppress=Pa096
	#pragma diag_suppress=Pe815

	#define RP_THREAD_LOCAL

#elif defined(__TI_TMS470_V7__)

	typedef char				char_t;
	typedef unsigned char		uchar_t;
#if 0
	typedef signed char			int8_t;
	typedef unsigned char		uint8_t;
	typedef signed short		int16_t;
	typedef unsigned short		uint16_t;
	typedef signed int			int32_t;
	typedef unsigned int		uint32_t;
	typedef signed long long	int64_t;
	typedef unsigned long long	uint64_t;
#else
	#include <stdint.h>
#endif
	typedef float				float32_t;
	typedef long double			float64_t;

	#define RP_THREAD_LOCAL

#else

	#error There is no platform definition.

#endif


typedef struct {
	uint8_t* punVirtualAddress;
	uaddr_t   unPhysicalAddress;
	uint32_t unSize;
} MemoryObject_t;

#ifdef __cplusplus
}
#endif /* _cplusplus */

#endif	/* CTYPEDEFS_H_ */
