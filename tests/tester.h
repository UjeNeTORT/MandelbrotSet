#include <stdio.h>
#include <stdlib.h>

#include "../common/common.h"

const int MAX_N_TESTS = 100; // max test cases per file allowed

typedef unsigned long long ull;
typedef unsigned char      uch;

typedef enum {
    TEST_SUCCESSFUL        = 0,
    TEST_WRNG_INPUT_FILE   = 1,
    TEST_WRNG_OUTPUT_FILE  = 2,
    TEST_WRNG_DATA         = 3,
    TEST_OUTPUT_ERR        = 4,
} PerformanseTestResult;

PerformanseTestResult PerformanceTest (
    const char * const test_results_fname,
    const char * const test_cases_fname,
    uch                *pixels,
    void (*fptr)(uch *, int, int, float)
);

ull CheckPerformanceTicks (
    void (*mandelbrot_func_ptr) (uch *, int, int, float),
    uch     *pixels,
    int      x_offset,
    int      y_offset,
    float    scale
);

int ReadTestCases (FILE *fin, int testcases[][3]);

ull GetTicks ();