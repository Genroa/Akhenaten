#include "js/test/js_test_common.h"

#include "core/log.h"
#include "graphics/image_hires.h"

#include <vector>

static int __test_hires_pick_factor(float screen_scale, int max_factor) {
    return image_hires::pick_factor(screen_scale, max_factor);
}
ANK_FUNCTION_2(__test_hires_pick_factor);

// Returns the number of failed checks; each failure is logged.
static int __test_hires_selftest() {
    int failures = 0;
    auto expect = [&failures] (pcstr what, color got, color want) {
        if (got != want) {
            logs::info("[test-hires] FAIL %s: got %08x want %08x", what, got, want);
            ++failures;
        }
    };

    std::vector<color> px = {0xff102030, 0x80ff8040, 0x00ffffff, 0x40000000};
    image_hires::premultiply(px);
    expect("premultiply opaque", px[0], 0xff102030);
    expect("premultiply half", px[1], 0x80804020);
    expect("premultiply transparent clears colour", px[2], 0x00000000);
    expect("premultiply black", px[3], 0x40000000);

    // 4x2 -> 2x1: left quad is uniform, right quad averages opaque white with three transparent pixels.
    const std::vector<color> quad = {0xff204060, 0xff204060, 0xffffffff, 0x00000000,
                                     0xff204060, 0xff204060, 0x00000000, 0x00000000};
    const std::vector<color> half = image_hires::halve(quad, 4, 2);
    if (half.size() != 2) {
        logs::info("[test-hires] FAIL halve size %d", (int)half.size());
        ++failures;
    } else {
        expect("halve uniform", half[0], 0xff204060);
        expect("halve premultiplied edge", half[1], 0x40404040);
    }

    if (!image_hires::valid_scale(4) || image_hires::valid_scale(3) || image_hires::level_index(4) != 1) {
        logs::info("[test-hires] FAIL scale validation");
        ++failures;
    }
    return failures;
}
ANK_FUNCTION(__test_hires_selftest);
