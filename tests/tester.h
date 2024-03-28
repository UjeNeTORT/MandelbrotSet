#include <stdio.h>
#include <stdlib.h>

#include "../common/common.h"

typedef unsigned long long ull;
typedef unsigned char      uch;

int PerformanceTest (
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

int ReadTestCases (FILE *fin, int **testcases);

ull GetTicks ();