#include <SFML/Graphics.hpp>

#include <assert.h>
#include <math.h>
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/common.h"

const int   WINDOW_WIDTH             = 1920;   // window width
const int   WINDOW_HEIGHT            = 1080;   // window height
const int   MAX_N_ITERATIONS         = 500;    // max n iterations
const int   SQR_RADIUS_MAX           = 100;    // max radius^2

const int   DFLT_X_OFFSET            = 0;      // default x axis offset
const int   DFLT_Y_OFFSET            = 0;      // default y axis offset
const float DFLT_SCALE               = 333.0f; // default scale (fit image in the window)

const int   X_OFFSET_FACTOR          = WINDOW_WIDTH  * 0.1; // n pixels to shift on shift-button press (x axis)
const int   Y_OFFSET_FACTOR          = WINDOW_HEIGHT * 0.1; // n pixels to shift on shift-button press (y axis)
const float LEGENDARY_THREE_QUARTERS = 0.75f;               // scaling factor

/**
 * WIKIPEDIA-LIKE COLORS
 * CREDIT
 * these gradient colors were taken from q9f's (https://stackoverflow.com/users/1260906/q9f)
 * message on stackoverflow (https://stackoverflow.com/questions/16500656/which-color-gradient-is-used-to-color-mandelbrot-in-wikipedia)
*/
const int MANDELBROT_RGB_GRAD[][3] =
{
    { 66,  30,  15}, // brown 3
    { 25,   7,  26}, // dark violett
    {  9,   1,  47}, // darkest blue
    {  4,   4,  73}, // blue 5
    {  0,   7, 100}, // blue 4
    { 12,  44, 138}, // blue 3
    { 24,  82, 177}, // blue 2
    { 57, 125, 209}, // blue 1
    {134, 181, 229}, // blue 0
    {211, 236, 248}, // lightest blue
    {241, 233, 191}, // lightest yellow
    {248, 201,  95}, // light yellow
    {255, 170,   0}, // dirty yellow
    {204, 128,   0}, // brown 0
    {153,  87,   0}, // brown 1
    {106,  52,   3}, // brown 2
};

/**
 * @brief show user mandelbrot image and provide navigation and scaling interface
 *
 * @return always 0
*/
int  MandelbrotUI ();

/**
 * @brief fill pixels array with the mandelbrot set image
 *
 * @param pixels   pixels buffer
 * @param x_offset x axis offset
 * @param y_offset y axis offset
 * @param scale    scale factor
*/
void MandelbrotSetBruteForce (u_char *pixels, int x_offset, int y_offset, float scale);

/**
 * @brief fill pixels array with the mandelbrot set image
 *
 * @param pixels   pixels buffer
 * @param x_offset x axis offset
 * @param y_offset y axis offset
 * @param scale    scale factor
 *
 * @note uses intrinsic functions
*/
void MandelbrotSetVectorized (u_char *pixels, int x_offset, int y_offset, float scale);

/**
 *
*/
static inline __m256i _mm256_mod_base_2_epi32 (__m256i A, const int n);