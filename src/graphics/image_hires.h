#pragma once

#include "graphics/color.h"

#include <cstdint>
#include <vector>

// High-resolution mod sprites: a pack group declares `scale:N` (2, 4 or 8), its PNGs are N times the
// sprite's logical size. The loader keeps a 1x copy at logical size in image_t::atlas and the larger
// copies in image_t::hires; the painter draws whichever copy is closest above the on-screen scale.
namespace image_hires {

constexpr int max_scale = 8;
constexpr int max_levels = 3; // 2x, 4x, 8x

bool valid_scale(int scale);
int level_index(int factor); // 2 -> 0, 4 -> 1, 8 -> 2

// Straight-alpha ARGB to premultiplied ARGB, in place (sprites are drawn with a premultiplied blend).
void premultiply(std::vector<color> &pixels);

// 2x2 box filter of premultiplied ARGB; width and height must be even.
std::vector<color> halve(const std::vector<color> &pixels, int width, int height);

// Texture pixels per logical pixel to sample for a sprite drawn at screen_scale screen pixels per logical
// pixel: 1 when not magnified, else the smallest stored factor >= screen_scale, capped at max_factor.
int pick_factor(float screen_scale, int max_factor);

} // namespace image_hires
