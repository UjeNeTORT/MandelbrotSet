#include "./mandelbrot.h"

int MandelbrotUI ()
{
    sf::RenderWindow window (sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot");

    u_char *pixels = (u_char *) calloc (WINDOW_WIDTH * WINDOW_HEIGHT * 4, sizeof (u_char));

    int x_offset = DFLT_X_OFFSET,
        y_offset = DFLT_Y_OFFSET;
    float scale  = DFLT_SCALE;

    sf::Texture texture;
    texture.create (WINDOW_WIDTH, WINDOW_HEIGHT);
    texture.update (pixels);

    sf::Sprite sprite (texture);

    while (window.isOpen ())
    {
        sf::Event event;

        while (window.pollEvent (event))

            switch (event.type)
            {
            case (sf::Event::Closed):

exit:
                LOG("quit...");
                window.close ();
                break;

            case (sf::Event::KeyPressed):

                switch (event.key.code)
                {
                case (sf::Keyboard::Left):
                    x_offset -= X_OFFSET_FACTOR;

                    LOG ("x_offset has been decreased (%lg)", (float) x_offset / scale);
                    break;

                case (sf::Keyboard::Right):
                    x_offset += X_OFFSET_FACTOR;

                    LOG ("x_offset has been increased (%lg)", (float) x_offset / scale);
                    break;

                case (sf::Keyboard::Up):
                    y_offset -= Y_OFFSET_FACTOR;

                    LOG ("y_offset has been decreased (%lg)", (float) y_offset / scale);
                    break;

                case (sf::Keyboard::Down):
                    y_offset += Y_OFFSET_FACTOR;

                    LOG ("y_offset has been increased (%lg)", (float) y_offset / scale);
                    break;

                case (sf::Keyboard::Comma):
                    scale *= LEGENDARY_THREE_QUARTERS;
                    x_offset *= LEGENDARY_THREE_QUARTERS;
                    y_offset *= LEGENDARY_THREE_QUARTERS;

                    LOG ("scale has been decreased (%lg)", scale);
                    break;

                case (sf::Keyboard::Period):
                    scale /= LEGENDARY_THREE_QUARTERS;
                    x_offset /= LEGENDARY_THREE_QUARTERS;
                    y_offset /= LEGENDARY_THREE_QUARTERS;

                    LOG ("scale has been increased (%lg)", scale);
                    break;

                case (sf::Keyboard::Delete):
                    x_offset = DFLT_X_OFFSET; y_offset = DFLT_Y_OFFSET;
                    scale = DFLT_SCALE;

                    LOG ("offsets and scaling was set to default");
                    break;

                case (sf::Keyboard::Q):

                    LOG ("forsing quit...");
                    goto exit;

                default:
                    break;
                }

                break;

            default:
                // display fps?
                break;
            }

        // MandelbrotSetBruteForce (pixels, x_offset, y_offset, scale);
        MandelbrotSetVectorized (pixels, x_offset, y_offset, scale);

        texture.update (pixels);

        window.clear ();
        window.draw (sprite);
        window.display ();
    }

    free (pixels);

    return 0;
}

void MandelbrotSetBruteForce (u_char *pixels, int x_offset, int y_offset, float scale)
{
    assert (pixels);
    assert (!dbleq (scale, .0f));

    int delta_x = WINDOW_WIDTH  / 2,
        delta_y = WINDOW_HEIGHT / 2;

    float y0 = (-delta_y + y_offset) / scale;

    float dx = 1.0f / scale;
    float dy = 1.0f / scale;

    for (int y_px = -delta_y; y_px < WINDOW_HEIGHT - delta_y; y_px++, y0 += dy)
    {
        float x0 = (-delta_x + x_offset) / scale;

        for (int x_px = -delta_x; x_px < WINDOW_WIDTH - delta_x; x_px++, x0 += dx)
        {
            float xn = 0, yn = 0, x2 = 0, y2 = 0, xy = 0;

            size_t n_iterations = 0;
            for (n_iterations = 0; n_iterations < MAX_N_ITERATIONS && x2 + y2 < SQR_RADIUS_MAX; n_iterations++)
            {
                xn = x2 - y2 + x0;
                yn = 2 * xy  + y0;

                x2 = xn * xn;
                y2 = yn * yn;
                xy = xn * yn;
            }

            u_char color[4] = {0, 0, 0, 255}; // current pixel color

            // in case of unstable dots behaviour - determine dot color
            // else                               - paint black

            float clr_coeff = n_iterations / MAX_N_ITERATIONS;

            if (!dbleq (clr_coeff, 1))
                {
                    if (clr_coeff > 0.3)
                    {
                        color[0] = (u_char) (clr_coeff * 0);
                        color[1] = (u_char) (clr_coeff * 500);
                        color[2] = (u_char) (clr_coeff * 0);
                    }
                    else
                    {
                        color[0] = (u_char) (clr_coeff * 200);
                        color[1] = (u_char) (clr_coeff * 600);
                        color[2] = (u_char) (clr_coeff * 200);
                    }
                }

            size_t pixels_pos = ((y_px + delta_y) * WINDOW_WIDTH + (x_px + delta_x)) * 4;
            memcpy (pixels + pixels_pos, color, sizeof (color));
        }
    }

    return;     // void
}

void MandelbrotSetVectorized (u_char *pixels, int x_offset, int y_offset, float scale)
{
    int delta_x = WINDOW_WIDTH  / 2,
        delta_y = WINDOW_HEIGHT / 2;

    float y0 = (-delta_y + y_offset) / scale;

    float dx = 1.0f / scale;
    float dy = 1.0f / scale;

    __m256 _01234567   = _mm256_set_ps (7, 6, 5, 4, 3, 2, 1, 0);
    __m256 _2_vec          = _mm256_set1_ps (2);
    __m256 sqr_rad_vec     = _mm256_set1_ps (SQR_RADIUS_MAX);
    __m256 max_n_iters_vec = _mm256_set1_ps (MAX_N_ITERATIONS);

    for (size_t y_px = 0; y_px < WINDOW_HEIGHT; y_px++, y0 += dy)
    {
        float x0 = (-delta_x + x_offset) / scale;

        for (size_t x_px = 0; x_px < WINDOW_WIDTH; x_px += 8, x0 += dx * 8)
        {
            __m256 dx_01234567 = _mm256_mul_ps (_01234567, _mm256_set1_ps (dx));

            __m256 r2_vec = _mm256_setzero_ps ();
            __m256 x2_vec = _mm256_setzero_ps ();
            __m256 y2_vec = _mm256_setzero_ps ();
            __m256 xy_vec = _mm256_setzero_ps ();

            __m256 x0_vec = _mm256_set1_ps (x0);
            __m256 y0_vec = _mm256_set1_ps (y0);

            x0_vec = _mm256_add_ps (x0_vec, dx_01234567);

            volatile __m256i n_iterations_vec = _mm256_setzero_si256 ();

            int dot_stability_mask = -1;
            for (int n_iterations = 0; n_iterations < MAX_N_ITERATIONS && dot_stability_mask; n_iterations++)
            {
                __m256 xn_vec = _mm256_add_ps (_mm256_sub_ps (x2_vec, y2_vec), x0_vec);
                __m256 yn_vec = _mm256_add_ps (_mm256_mul_ps (xy_vec, _2_vec), y0_vec);

                x2_vec = _mm256_mul_ps (xn_vec, xn_vec);
                y2_vec = _mm256_mul_ps (yn_vec, yn_vec);
                xy_vec = _mm256_mul_ps (xn_vec, yn_vec);

                r2_vec = _mm256_add_ps (x2_vec, y2_vec);

                __m256 cmp_vec     = _mm256_cmp_ps (r2_vec, sqr_rad_vec, _CMP_LT_OQ);
                n_iterations_vec   = _mm256_sub_epi32 (n_iterations_vec, _mm256_castps_si256 (cmp_vec));
                dot_stability_mask = _mm256_movemask_ps (cmp_vec);
            }

            __m256 clr_quotients_vec = _mm256_div_ps (_mm256_cvtepi32_ps (n_iterations_vec), max_n_iters_vec);
            float *clr_quotients = (float *) &clr_quotients_vec;

            for (size_t rel_pixel_n = 0; rel_pixel_n < sizeof (__m256) / sizeof (float); rel_pixel_n++)
            {
                u_char color[4] = {0, 0, 0, 255};

                float clr_coeff = clr_quotients[rel_pixel_n];

                if (clr_coeff != 1)
                {
                    if (clr_coeff > 0.3)
                    {
                        color[0] = (u_char) (clr_coeff * 0);
                        color[1] = (u_char) (clr_coeff * 500);
                        color[2] = (u_char) (clr_coeff * 0);
                    }
                    else
                    {
                        color[0] = (u_char) (clr_coeff * 200);
                        color[1] = (u_char) (clr_coeff * 600);
                        color[2] = (u_char) (clr_coeff * 200);
                    }
                }

                size_t pixels_pos = (y_px * WINDOW_WIDTH + x_px + rel_pixel_n) * 4;

                memcpy (pixels + pixels_pos, color, sizeof (color));
            }
        }
    }

    return;     // void
}

/**
 * get i % (2 ^ n) for each i in A
 *
*/
static inline __m256i _mm256_mod_base_2_epi32 (__m256i A, const int n)
{
    return _mm256_sub_epi32 (A, _mm256_slli_epi32 (_mm256_srai_epi32 (A, n), n));
}