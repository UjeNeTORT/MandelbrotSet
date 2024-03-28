#include <assert.h>
#include <string.h>

#include "./mandelbrot/mandelbrot.h"
#include "./tests/tester.h"

const char test_results_fpath[] = "tests/testresults/res1.txt";
const char test_cases_fpath[]   = "tests/testcases/cases1.test";

int main (int argc, char *argv[])
{
    if (argc == 1) MandelbrotUI ();

    int i = 0;
    while (argc > 1)
    {
        if (streq (argv[i + 1], "-tp") ||
            streq (argv[i + 1], "--test-performance"))
        {
            sf::Uint8 *pixels = (sf::Uint8 *) calloc (WINDOW_WIDTH * WINDOW_HEIGHT * 4, sizeof (sf::Uint8));

            PerformanceTest (test_results_fpath, test_cases_fpath, pixels, &MandelbrotSetBruteForce);

            free (pixels);
        }
        else
        {
            ERROR ("Unknown flag \"%s\"!", argv[i + 1]);
        }

        argc--;
    }

    return 0;
}
