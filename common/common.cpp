/*************************************************************************
 * (c) 2023 Tikhonov Yaroslav (aka UjeNeTORT)
 *
 * email: tikhonovty@gmail.com
 * telegram: https://t.me/netortofficial
 * GitHub:   https://github.com/UjeNeTORT
 * repo:     https://github.com/UjeNeTORT/MandelbrotSet
 *************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

#include "common.h"

int PrintfDebug (const char * funcname, int line, const char * filename, const char * format, ...)
{
    assert (funcname);
    assert (filename);
    assert (format);

    #ifdef DEBUG_PRINTFS_DETAILED
        fprintf (stderr, GREEN_CLR "[DEBUG | %s:%d %s]\n<< ", funcname, line, filename);
    #else
        fprintf (stderr, GREEN_CLR "[DEBUG] ");
    #endif // DEBUG_PRINTFS_DETAILED



    va_list ptr;

    va_start (ptr, format);

    int res = vfprintf (stderr, format, ptr);

    va_end (ptr);

    fprintf (stdout, RST_CLR "\n" RST_CLR);

    return res;
}

int PrintfError (const char * funcname, int line, const char * filename, const char * format, ...)
{
    assert (funcname);
    assert (filename);
    assert (format);

    #ifdef ERROR_PRINTFS_DETAILED
        fprintf (stderr, RED_CLR "[%s:%d %s]\nERROR! ", funcname, line, filename);
    #else
        fprintf (stderr, RED_CLR "[ERROR] ");
    #endif // ERROR_PRINTFS_DETAILED

    va_list ptr;

    va_start (ptr, format);

    int res = vfprintf (stderr, format, ptr);

    va_end (ptr);

    fprintf (stdout, RST_CLR "\n" );

    return res;
}

int PrintfLog (const char * funcname, int line, const char * filename, const char * format, ...)
{
    assert (funcname);
    assert (filename);
    assert (format);

    #ifdef LOGS_DETAILED
        fprintf (stderr, CYAN_CLR "[log from %s:%d %s] ", funcname, line, filename);
    #else
        fprintf (stderr, CYAN_CLR "[LOG] ");
    #endif // LOGS_DETAILED

    va_list ptr;

    va_start (ptr, format);

    int res = vfprintf (stderr, format, ptr);

    va_end (ptr);

    fprintf (stdout, RST_CLR "\n" );

    return res;
}

// unfinished
int PrintProgressBar (unsigned curr_progress, unsigned max_progress)
{
    assert (curr_progress <= max_progress);
    // assert (max != 0);

    const char prefix[]    = "---[";
    const char suffix[]    = "]---";

    const int  prefix_size = sizeof (prefix) - 1;
    const int  suffix_size = sizeof (suffix) - 1;

    char progress_bar[max_progress] = {};

    for (unsigned i = 0; i < max_progress; i++)
    {
        progress_bar[i] = (i <= curr_progress) ? '#' : '_';
    }

    fprintf (stderr, "\r%s%s%s", prefix, progress_bar, suffix);

    return 0;
}
