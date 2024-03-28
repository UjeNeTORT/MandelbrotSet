#include <SFML/Graphics.hpp>

#include <assert.h>
#include <math.h>
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/common.h"

#define ASSERTS

typedef unsigned char uch; // should i use this instead of sf::uint8?

const int   WINDOW_WIDTH             = 1920;
const int   WINDOW_HEIGHT            = 1080;
const float MAX_N_ITERATIONS         = 200;
const float SQR_RADIUS_MAX           = 100;

const int   DFLT_X_OFFSET            = 0;
const int   DFLT_Y_OFFSET            = 0;
const float DFLT_SCALE               = 333.0f;

const int   X_OFFSET_FACTOR          = WINDOW_WIDTH  * 0.1;
const int   Y_OFFSET_FACTOR          = WINDOW_HEIGHT * 0.1;
const float LEGENDARY_THREE_QUARTERS = 0.75f;

int  MandelbrotUI ();

void MandelbrotSetBruteForce (sf::Uint8 *pixels, int x_offset, int y_offset, float scale);
void MandelbrotSetVectorized (sf::Uint8 *pixels, int x_offset, int y_offset, float scale);
