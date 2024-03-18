#ifndef STRNLEN_H_
#define STRNLEN_H_

#if defined(__TI_COMPILER_VERSION__)

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t strnlen(const char *s, size_t maxlen);

#ifdef __cplusplus
}
#endif

#endif

#endif	/* STRNLEN_H_ */