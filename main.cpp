#include <assert.h>
#include <string.h>

#include "./mandelbrot/mandelbrot.h"
#include "./tests/tester.h"

const char default_test_results_fpath[] = "tests/testresults/res1.csv";
const char default_test_cases_fpath[]   = "tests/testcases/cases1.test";

int main (int argc, char *argv[])
{
    if (argc == 1) MandelbrotUI ();

    for (int n_argv = 1; n_argv < argc; n_argv++)
    {
        if (streq (argv[n_argv], "-tp") ||
            streq (argv[n_argv], "--test-performance"))
        {
            sf::Uint8 *pixels = (sf::Uint8 *) calloc (WINDOW_WIDTH * WINDOW_HEIGHT * 4, sizeof (sf::Uint8));

            int tests_ret_code = 0;
            if (argc > 2)
            {
                tests_ret_code = PerformanceTest (
                    argv[n_argv + 1], argv[n_argv + 2], pixels, &MandelbrotSetBruteForce);
            }

            else
            {
                tests_ret_code = PerformanceTest (
                    default_test_results_fpath, default_test_cases_fpath, pixels, &MandelbrotSetBruteForce
                );
            }

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
