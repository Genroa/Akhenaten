#include "graphics/image_hires.h"

namespace image_hires {

bool valid_scale(int scale) {
    return scale == 1 || scale == 2 || scale == 4 || scale == 8;
}

int level_index(int factor) {
    switch (factor) {
    case 2: return 0;
    case 4: return 1;
    case 8: return 2;
    default: return -1;
    }
}

void premultiply(std::vector<color> &pixels) {
    for (color &c : pixels) {
        const uint32_t a = (c >> 24) & 0xff;
        if (a == 0xff) {
            continue;
        }
        const uint32_t r = (((c >> 16) & 0xff) * a + 127) / 255;
        const uint32_t g = (((c >> 8) & 0xff) * a + 127) / 255;
        const uint32_t b = ((c & 0xff) * a + 127) / 255;
        c = (a << 24) | (r << 16) | (g << 8) | b;
    }
}

std::vector<color> halve(const std::vector<color> &pixels, int width, int height) {
    const int hw = width / 2;
    const int hh = height / 2;
    std::vector<color> out((size_t)hw * hh);
    for (int y = 0; y < hh; ++y) {
        for (int x = 0; x < hw; ++x) {
            const color quad[4] = {
                pixels[(size_t)(2 * y) * width + 2 * x],
                pixels[(size_t)(2 * y) * width + 2 * x + 1],
                pixels[(size_t)(2 * y + 1) * width + 2 * x],
                pixels[(size_t)(2 * y + 1) * width + 2 * x + 1],
            };
            uint32_t sum[4] = {0, 0, 0, 0};
            for (const color c : quad) {
                sum[0] += (c >> 24) & 0xff;
                sum[1] += (c >> 16) & 0xff;
                sum[2] += (c >> 8) & 0xff;
                sum[3] += c & 0xff;
            }
            out[(size_t)y * hw + x] = (((sum[0] + 2) / 4) << 24) | (((sum[1] + 2) / 4) << 16)
                                      | (((sum[2] + 2) / 4) << 8) | ((sum[3] + 2) / 4);
        }
    }
    return out;
}

int pick_factor(float screen_scale, int max_factor) {
    constexpr float epsilon = 0.001f;
    if (max_factor <= 1 || screen_scale <= 1.0f + epsilon) {
        return 1;
    }
    int factor = 2;
    while (factor < max_factor && (float)factor < screen_scale - epsilon) {
        factor *= 2;
    }
    return factor;
}

} // namespace image_hires
