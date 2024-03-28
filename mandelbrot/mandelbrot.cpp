#include "./mandelbrot.h"

int MandelbrotUI ()
{
    sf::RenderWindow window (sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot");

    sf::Uint8 *pixels = (sf::Uint8 *) calloc (WINDOW_WIDTH * WINDOW_HEIGHT * 4, sizeof (sf::Uint8));

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

                    LOG ("x_offset has been decreased (%d)", x_offset);
                    break;

                case (sf::Keyboard::Right):
                    x_offset += X_OFFSET_FACTOR;

                    LOG ("x_offset has been increased (%d)", x_offset);
                    break;

                case (sf::Keyboard::Up):
                    y_offset -= Y_OFFSET_FACTOR;

                    LOG ("y_offset has been decreased (%d)", y_offset);
                    break;

                case (sf::Keyboard::Down):
                    y_offset += Y_OFFSET_FACTOR;

                    LOG ("y_offset has been increased (%d)", y_offset);
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

        MandelbrotSetBruteForce (pixels, x_offset, y_offset, scale);

        texture.update (pixels);

        window.clear ();
        window.draw (sprite);
        window.display ();
    }

    free (pixels);

    return 0;
}

void MandelbrotSetBruteForce (sf::Uint8 *pixels, int x_offset, int y_offset, float scale)
{
    #ifdef ASSERTS

    assert (pixels);
    assert (!dbleq (scale, .0f));

    #endif // ASSERTS

    sf::Uint8 color[4] = {}; // current pixel color

    int delta_x = WINDOW_WIDTH  / 2,
        delta_y = WINDOW_HEIGHT / 2;

    for (int y_px = -delta_y; y_px < WINDOW_HEIGHT - delta_y; y_px++)
    {
        for (int x_px = -delta_x; x_px < WINDOW_WIDTH - delta_x; x_px++)
        {
            float xn = 0, yn = 0, x2 = 0, y2 = 0, xy = 0;

            int n_iterations = 0;
            while (n_iterations < MAX_N_ITERATIONS && x2 + y2 < SQR_RADIUS_MAX)
            {
                xn = x2 - y2 + (x_px + x_offset) / scale;
                yn = 2 * xy  + (y_px + y_offset) / scale;

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

                color[0] = (sf::Uint8) 255 - color_coeff * 255;
                color[1] = (sf::Uint8) 1 / (color_coeff) * 255;
                color[2] = (sf::Uint8) sqrtf (sqrtf(1 / color_coeff)) * 50;
                color[3] = 255;
            }
            else
            {
                color[0] = 0;   // r
                color[1] = 0;   // g
                color[2] = 0;   // b
                color[3] = 0;   // a
            }

            int pixels_pos = ((y_px + delta_y) * WINDOW_WIDTH + (x_px + delta_x)) * 4;
            memcpy (pixels + pixels_pos, color, sizeof (color));
        }
    }

    return;     // void
}

void MandelbrotSetVectorized (sf::Uint8 *pixels, int x_offset, int y_offset, float scale)
{
    #ifdef ASSERTS

    assert (pixels);
    assert (!dbleq (scale, .0f));

    #endif // ASSERTS

    sf::Uint8 color[4] = {}; // current pixel color

    int delta_x = WINDOW_WIDTH  / 2,
        delta_y = WINDOW_HEIGHT / 2;

    for (int y_px = -delta_y; y_px < WINDOW_HEIGHT - delta_y; y_px++)
    {
        for (int x_px = -delta_x; x_px < WINDOW_WIDTH - delta_x; x_px++)
        {
        }
    }

    return;     // void
}
