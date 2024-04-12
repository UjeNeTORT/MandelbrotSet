#include <assert.h>
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/common.h"

const int MAX_N_TESTS        = 100; // max test cases per file allowed
const int N_TEST_CASE_PARAMS = 3;   // n of parameters passed to fucntion being tested from file

typedef unsigned long long ull;

typedef enum {
    TEST_SUCCESSFUL        = 0, // nothing unexpected happened
    TEST_WRNG_INPUT_FILE   = 1, // cant open input file
    TEST_WRNG_OUTPUT_FILE  = 2, // cant open test results file
    TEST_WRNG_DATA         = 3, // number of tests readen doesnt match
    TEST_OUTPUT_ERR        = 4, // couldnt write to file
} PerformanseTestResult;

/**
 * @brief measure performance of function and load results to file
 *
 * @param test_results_fpath  path to test results file
 * @param test_cases_fpath    path to test cases file
 * @param tests_name          test results row name
 * @param pixels              dummy buffer of pixels
 * @param mandelbrot_func_ptr mandelbrot-calculator function pointer
 *
 * @return ret code PerformanceTestResult (0 on success)
*/
PerformanseTestResult PerformanceTest (
    const char * const test_results_fpath,
    const char * const test_cases_fpath,
    const char * const tests_name,
    u_char             *pixels,
    void (*mandelbrot_func_ptr)(u_char *, int, int, float)
);

/**
 * @brief measucre time it takes to run function (calc difference between cpu clock cycles before and after func launch)
 *
 * @param mandelbrot_func_ptr mandelbrot-calculator function pointer
 * @param pixels              dummy buffer of pixels
 * @param x_offset            x axis offset
 * @param y_offset            y axis offset
 * @param scale               image scale
 *
 * @return delta t (long long)
*/
int64_t CheckPerformanceTicks (
    void (*mandelbrot_func_ptr) (u_char *, int, int, float),
    u_char   *pixels,
    int      x_offset,
    int      y_offset,
    float    scale
);

/**
 * @brief read test cases from file
 *
 * @param fin       input stream
 * @param testcases test cases array
 *
 * @return number of testcases readen
*/
int ReadTestCases (FILE *fin, int *testcases);

/**
 * @brief get current cpu clock cycles
 *
 * @return current cpu clock cycles
*/
ull GetTicks ();