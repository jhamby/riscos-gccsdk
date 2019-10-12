#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdio.h>
#include "swi.h"

#ifndef DEBUG_REPORT_SWI_ERROR
#define DEBUG_REPORT_SWI_ERROR 0
#endif

#ifndef DEBUG_PRINT
#define DEBUG_PRINT 0
#endif

#ifndef DEBUG_REPORT
#define DEBUG_REPORT 0
#endif

#define LOG_PRINT 1
#define LOG_REPORT 2

extern void log_printf(unsigned flags, const char *format, ...);

#if (DEBUG_PRINT || DEBUG_REPORT)
#if DEBUG_PRINT
#define DEBUG_FLAG_PRINT LOG_PRINT
#else
#define DEBUG_FLAG_PRINT 0
#endif
#if DEBUG_REPORT
#define DEBUG_FLAG_REPORT LOG_REPORT
#else
#define DEBUG_FLAG_REPORT 0
#endif
#define DEBUG_OUTPUT(format, ...) \
do {\
log_printf(DEBUG_FLAG_REPORT | DEBUG_FLAG_PRINT, format, ##__VA_ARGS__); \
} while (0)
#else
#define DEBUG_OUTPUT(format, ...)
#endif


#ifdef DEBUG_REPORT_SWI_ERROR
extern void report_swi_error(const char *func, int line, _kernel_oserror *err);
#define DEBUG_OUTPUT_SWI_ERROR(err) report_swi_error(__func__, __LINE__, err)
#else
#define DEBUG_OUTPUT_SWI_ERROR(err)
#endif

#endif
