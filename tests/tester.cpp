#include "tester.h"

PerformanseTestResult PerformanceTest (
    const char * const test_results_fpath,
    const char * const test_cases_fpath,
    uch                *pixels,
    void (*fptr)(uch *, int, int, float)
)
{
    PerformanseTestResult ret_code = TEST_SUCCESSFUL;

    FILE *test_results_file  = fopen (test_results_fpath, "w");
    FILE *test_cases_file    = fopen (test_cases_fpath, "r");

    ull test_results[MAX_N_TESTS]    = {};
    int test_cases  [MAX_N_TESTS][3] = {};

    int n_tests = 0;

    if (!test_results_file)
    {
        ret_code = TEST_WRNG_OUTPUT_FILE;
        goto cleanup_and_return;
    }

    if (!test_cases_file)
    {
        ret_code = TEST_WRNG_INPUT_FILE;
        goto cleanup_and_return;
    }

    if (fscanf (test_cases_file, "%d\n", &n_tests) != 1)
    {
        ret_code = TEST_WRNG_DATA;
        goto cleanup_and_return;
    }

    if (n_tests != ReadTestCases (test_cases_file, test_cases))
    {
        ret_code = TEST_WRNG_DATA;
        goto cleanup_and_return;     // toask if it is ok
    }

    LOG ("performance testing is on...");
    for (int n_test = 0; n_test < n_tests; n_test++)
    {
        test_results[n_test] =  CheckPerformanceTicks (
                                    fptr,
                                    pixels,
                                    *(int *)(test_cases + n_test * sizeof (int [3]) + 0 * sizeof (int)),
                                    *(int *)(test_cases + n_test * sizeof (int [3]) + 1 * sizeof (int)),
                                    *(int *)(test_cases + n_test * sizeof (int [3]) + 2 * sizeof (int))
                                );

        PrintProgressBar (n_test + 1, n_tests);
    }

    fprintf (stderr, "\n");

    // write test results

    fprintf (test_results_file, "test_result\n");

    for (int n_result = 0; n_result < n_tests; n_result++)
    {
        int ret_val_printf = fprintf (test_results_file, "%lld \n", test_results[n_result]);
        if (ret_val_printf == 0)
        {
            ret_code = TEST_OUTPUT_ERR;
            goto cleanup_and_return;
        }
    }

    LOG ("test results stored at \"%s\"\n", test_results_fpath);

cleanup_and_return:

    fclose (test_cases_file);
    fclose (test_results_file);

    return ret_code;
}

ull CheckPerformanceTicks (void (*mandelbrot_func_ptr)(uch *, int, int, float), uch * pixels, int x_offset, int y_offset, float scale)
{
    ull delta_t = -GetTicks ();

    mandelbrot_func_ptr (pixels, x_offset, y_offset, scale);

    delta_t += GetTicks ();

    return delta_t;
}

int ReadTestCases (FILE *fin, int testcases[][3])
{
    int n_tests = 0;

    while (fscanf (fin, "%d %d %d\n", (int *)(testcases + n_tests * sizeof (int [3]) + 0 * sizeof (int)),
                                      (int *)(testcases + n_tests * sizeof (int [3]) + 1 * sizeof (int)),
                                      (int *)(testcases + n_tests * sizeof (int [3]) + 2 * sizeof (int))) == 3)
    {
        n_tests++;
    }

    return n_tests;
}

ull GetTicks ()
{
    ull ticks_rax = 0;
    ull ticks_rdx = 0;

    asm volatile (
        "rdtsc\n\t"
        : "=a" (ticks_rax), "=d" (ticks_rdx)
    );

    return (ticks_rdx << 32) | ticks_rax;
}
