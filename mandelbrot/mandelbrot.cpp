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

    while (window.isOpen ()) {
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

    u_char color[4] = {}; // current pixel color

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
            while (n_iterations < MAX_N_ITERATIONS && x2 + y2 < SQR_RADIUS_MAX)
            {
                xn = x2 - y2 + x0;
                yn = 2 * xy  + y0;

                x2 = xn * xn;
                y2 = yn * yn;
                xy = xn * yn;

                n_iterations++;
            }

            // in case of unstable dots behaviour - determine dot color
            // else                               - paint black
            if (n_iterations < MAX_N_ITERATIONS)
            {
                float color_coeff = (float) n_iterations / MAX_N_ITERATIONS;

                color[0] = (u_char) 255 - color_coeff * 255;
                color[1] = (u_char) 1 / (color_coeff) * 255;
                color[2] = (u_char) sqrtf (sqrtf(1 / color_coeff)) * 50;
                color[3] = 255;
            }
            else
            {
                color[0] = 0;   // r
                color[1] = 0;   // g
                color[2] = 0;   // b
                color[3] = 0;   // a
            }

            size_t pixels_pos = ((y_px + delta_y) * WINDOW_WIDTH + (x_px + delta_x)) * 4;
            memcpy (pixels + pixels_pos, color, sizeof (color));
        }
    }

    return;     // void
}

void MandelbrotSetVectorized (u_char *pixels, int x_offset, int y_offset, float scale)
{
    assert (pixels);
    assert (!dbleq (scale, .0f));

    int delta_x = WINDOW_WIDTH  / 2,
        delta_y = WINDOW_HEIGHT / 2;

    float y0 = (-delta_y + y_offset) / scale;

    float dx = 1.0f / scale;
    float dy = 1.0f / scale;

    for (size_t y_px = 0; y_px < WINDOW_HEIGHT; y_px++, y0 += dy)
    {
        float x0 = (-delta_x + x_offset) / scale;

        for (size_t x_px = 0; x_px < WINDOW_WIDTH; x_px += 8, x0 += dx * 8)
        {
            __m256 _01234567   = _mm256_set_ps (7, 6, 5, 4, 3, 2, 1, 0);
            __m256 dx_01234567 = _mm256_set1_ps (dx);
            dx_01234567        = _mm256_mul_ps (_01234567, dx_01234567);

            __m256 r2_vec = _mm256_setzero_ps ();
            __m256 x2_vec = _mm256_setzero_ps ();
            __m256 y2_vec = _mm256_setzero_ps ();
            __m256 xy_vec = _mm256_setzero_ps ();
            __m256 xn_vec = _mm256_setzero_ps ();
            __m256 yn_vec = _mm256_setzero_ps ();

            __m256 x0_vec = _mm256_set1_ps (x0);
            __m256 y0_vec = _mm256_set1_ps (y0);

            x0_vec = _mm256_add_ps (x0_vec, dx_01234567);

            __m256i n_iterations_vec = _mm256_setzero_si256 ();

            int dot_stability_mask = -1;
            int n_iterations       =  0;
            while (n_iterations < MAX_N_ITERATIONS && dot_stability_mask)
            {
                xn_vec = _mm256_add_ps (_mm256_sub_ps (x2_vec, y2_vec), x0_vec);
                yn_vec = _mm256_add_ps (_mm256_mul_ps (xy_vec, _mm256_set1_ps (2)), y0_vec);

                x2_vec = _mm256_mul_ps (xn_vec, xn_vec);
                y2_vec = _mm256_mul_ps (yn_vec, yn_vec);
                xy_vec = _mm256_mul_ps (xn_vec, yn_vec);

                r2_vec = _mm256_add_ps (x2_vec, y2_vec);

                __m256 cmp_vec     = _mm256_cmp_ps (r2_vec, _mm256_set1_ps (SQR_RADIUS_MAX), _CMP_LT_OQ);
                n_iterations_vec   = _mm256_sub_epi32 (n_iterations_vec, _mm256_castps_si256 (cmp_vec));
                dot_stability_mask = _mm256_movemask_ps (cmp_vec);

                n_iterations++;
            }

            int *n_iterations_arr = (int *) &n_iterations_vec;

            for (size_t rel_pixel_n = 0; rel_pixel_n < sizeof (__m256) / sizeof (float); rel_pixel_n++)
            {
                u_char color[4] = {0, 0, 0, 255};

                int n_iter_16 = n_iterations_arr[rel_pixel_n] % 16;

                if (n_iterations_arr[rel_pixel_n] < MAX_N_ITERATIONS)
                {
                    color[0] = MANDELBROT_RGB_GRAD[n_iter_16][0];
                    color[1] = MANDELBROT_RGB_GRAD[n_iter_16][1];
                    color[2] = MANDELBROT_RGB_GRAD[n_iter_16][2];
                }

                size_t pixels_pos = (y_px * WINDOW_WIDTH + x_px + rel_pixel_n) * 4;

                memcpy (pixels + pixels_pos, color, sizeof (color));
            }
        }
    }

    return;     // void
}
