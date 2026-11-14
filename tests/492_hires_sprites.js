// High-resolution mod sprites (scale:N pack groups): level generation and the
// on-screen scale -> stored copy selection used by the painter.

var __test492_ok = false

function test492_expect_factor(screen_scale, max_factor, want) {
    var got = __test_hires_pick_factor(screen_scale, max_factor)
    if (got != want) {
        __log_info_native('[test:492] FAIL pick_factor(' + screen_scale + ', ' + max_factor + ')=' + got + ' want ' + want)
        return false
    }
    return true
}

function run_test() {
    __log_info_native('[test:492] hires sprites')

    var failures = __test_hires_selftest()
    if (failures != 0) {
        __log_info_native('[test:492] FAIL selftest failures=' + failures)
        __test_signal_ready()
        return
    }
    __log_marker('hires_levels_ok')

    var cases = [
        [0.5, 4, 1], [1.0, 4, 1], [1.5, 4, 2], [2.0, 4, 2],
        [2.5, 4, 4], [4.0, 4, 4], [6.0, 4, 4], [3.0, 2, 2], [3.0, 1, 1], [5.0, 8, 8]
    ]
    for (var i = 0; i < cases.length; i++) {
        if (!test492_expect_factor(cases[i][0], cases[i][1], cases[i][2])) {
            __test_signal_ready()
            return
        }
    }
    __log_marker('hires_pick_factor_ok')

    __test492_ok = true
    __test_signal_ready()
}

function check_valid() {
    return __test492_ok
        && __test_find_inlog('[test-marker] hires_levels_ok')
        && __test_find_inlog('[test-marker] hires_pick_factor_ok')
}
