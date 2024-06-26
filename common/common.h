/*************************************************************************
 * (c) 2023 Tikhonov Yaroslav (aka UjeNeTORT)
 *
 * email:    tikhonovty@gmail.com
 * telegram: https://t.me/netortofficial
 * GitHub:   https://github.com/UjeNeTORT
 * repo:     https://github.com/UjeNeTORT/language
 *************************************************************************/

#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "colors.h"

const double EPS = 1e-7;

const int PROGRESS_BAR_LENGTH = 50;

#define LOGS
// #define LOGS_DETAILED
#define DEBUG_PRINTFS_DETAILED
#define ERROR_PRINTFS_DETAILED

#define streq(s1, s2) (!strcmp ((s1), (s2)))
#define dbleq(d1, d2) (fabs((d1) - (d2)) < EPS)
#define sizearr(arr) sizeof(arr) / sizeof((arr)[0])

// todo ask ded how to not destroy PC
#define ABORT()                                              \
{                                                            \
    for (int i = 0; i < 100000; i++)                         \
    {                                                        \
        printf("hehe aborting your computer and your nerves right now for free!))))))\n"); \
        abort ();                                            \
    }                                                        \
}                                                            \
    // system("shutdown -P now");

int PrintfDebug   (const char* funcname, int line, const char* filename, const char* format, ...);
int PrintfError   (const char* funcname, int line, const char* filename, const char* format, ...);
int PrintfLog     (const char* funcname, int line, const char* filename, const char* format, ...);

#define PRINTF_DEBUG(format, ...) \
    PrintfDebug (__FUNCTION__, __LINE__, __FILE__, format __VA_OPT__(,) __VA_ARGS__)

#ifdef LOGS
    #define LOG(format, ...) \
    PrintfLog (__FUNCTION__, __LINE__, __FILE__, format __VA_OPT__(,) __VA_ARGS__)
#else // LOGS
    #define LOG(format, ...) ;
#endif // LOGS

#define ERROR(format, ...) \
    PrintfError (__FUNCTION__, __LINE__, __FILE__, format __VA_OPT__(,) __VA_ARGS__)

#define RET_ERROR(ret_val, format, ...) \
    { PrintfError (__FUNCTION__, __LINE__, __FILE__, format __VA_OPT__(,) __VA_ARGS__); \
      return ret_val;}

int CheckExtension (char* filename);
int DelExtension   (char* filename);

int PrintProgressBar (unsigned count, unsigned max);

#endif // TOOLS_H
