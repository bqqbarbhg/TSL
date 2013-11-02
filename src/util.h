#ifndef TSL_UTIL_H
#define TSL_UTIL_H

#ifdef _MSC_VER
#include <stdio.h>
#define TSL_SNPRINTF sprintf_s
#else
#include <stdio.h>
#define TSL_SNPRINTF snprintf
#endif

#endif
