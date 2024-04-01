#include <assert.h>
#include <string.h>

#include "./mandelbrot/mandelbrot.h"
#include "./tests/tester.h"

#define NDEBUG

const int MAX_COMMAND_LENGHT            = 200;
const char default_test_results_fpath[] = "tests/testresults/res1.csv";
const char default_test_cases_fpath[]   = "tests/testcases/cases2.txt";

int main (int argc, const char *argv[])
{
    if (argc == 1) MandelbrotUI ();

    for (int n_argv = 1; n_argv < argc; n_argv++)
    {
        if (streq (argv[n_argv], "-tp") ||
            streq (argv[n_argv], "--test-performance"))
        {
            u_char *pixels = (u_char *) calloc (WINDOW_WIDTH * WINDOW_HEIGHT * 4, sizeof (u_char));

            int tests_ret_code = 0;

            const char *test_results_fpath = default_test_results_fpath;
            const char *test_cases_fpath   = default_test_cases_fpath;

            if (argc > 2)
            {
                test_results_fpath = argv[n_argv + 1];
                test_cases_fpath   = argv[n_argv + 2];
            }

            LOG ("test cases: \"%s\"", test_cases_fpath);

            FILE *test_results_temp = fopen (test_results_fpath, "r");
            if (!test_results_temp)
            {
                ERROR ("couldn't open test results: \"%s\"", test_results_fpath);
                test_results_fpath = default_test_results_fpath;
            }
            fclose (test_results_temp);

            // clear remaining results from the test results file
            char clear_file_command[MAX_COMMAND_LENGHT] = {};
            sprintf (clear_file_command, "cat /dev/null > %s", test_results_fpath);
            system (clear_file_command);

            FILE *test_cases_temp = fopen (test_cases_fpath, "r");
            if (!test_cases_temp)
            {
                ERROR ("couldn't open test cases: \"%s\"", test_cases_fpath);
                test_cases_fpath = default_test_cases_fpath;
            }
            fclose (test_cases_temp);

            LOG ("\"BruteForce\" approach testing...");
            tests_ret_code = PerformanceTest (
                    test_results_fpath, test_cases_fpath, "brute force", pixels, &MandelbrotSetBruteForce);

            LOG ("\"Vectorized\" approach testing...");
            tests_ret_code = PerformanceTest (
                    test_results_fpath, test_cases_fpath, "vectorized", pixels, &MandelbrotSetVectorized);

            LOG ("testing finished (%d)\n", tests_ret_code);

            free (pixels);

            // skip file name
            argc--;
            n_argv++;
        }
        else
        {
            ERROR ("Unknown flag \"%s\"!", argv[n_argv + 1]);
        }

        argc--;
    }

    return 0;
}
