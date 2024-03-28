#include "tester.h"

int PerformanceTest (
    const char * const test_results_fpath,
    const char * const test_cases_fpath,
    uch                *pixels,
    void (*fptr)(uch *, int, int, float)
)
{
    int ret_code = 0;

    FILE *test_results_file  = fopen (test_results_fpath, "w");
    FILE *test_cases_file    = fopen (test_cases_fpath, "r");

    int n_tests = 0;
    if (fscanf (test_cases_file, "%d\n", &n_tests) != 1)
        return 1;

    ull *test_results = (ull *)  calloc (n_tests, sizeof (ull));
    int **test_cases  = (int **) calloc (n_tests, sizeof (int [3]));

    if (n_tests != ReadTestCases (test_cases_file, test_cases))
    {
        ret_code = 1;
        goto cleanup_and_exit_func;     // toask if it is ok
    }

    for (int n_test = 0; n_test < n_tests; n_test++)
    {
        LOG ("testing [%d/%d]...", n_test + 1, n_tests);

        test_results[n_test] =  CheckPerformanceTicks (
                                    fptr,
                                    pixels,
                                    *(int *)(test_cases + n_test * sizeof (int [3]) + 0 * sizeof (int)),
                                    *(int *)(test_cases + n_test * sizeof (int [3]) + 1 * sizeof (int)),
                                    *(int *)(test_cases + n_test * sizeof (int [3]) + 2 * sizeof (int))
                                );
    }

    // write test results
    for (int n_result = 0; n_result < n_tests; n_result++)
    {
        fprintf (test_results_file, "%lld ", test_results[n_result]);
    }

    fprintf (test_results_file, "\n");

cleanup_and_exit_func:

    free (test_results);
    free (test_cases);
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

int ReadTestCases (FILE *fin, int **testcases)
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
