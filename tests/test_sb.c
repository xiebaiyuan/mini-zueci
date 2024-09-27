/* Test single-byte stuff and UTF-16/32 */
/*
    libzueci - an open source UTF-8 ECI library adapted from libzint
    Copyright (C) 2022 gitlost
 */
/* SPDX-License-Identifier: BSD-3-Clause */

#include "zueci_testcommon.h"
#ifdef ZUECI_TEST_ICONV
#include <errno.h>
#include <iconv.h>
#endif

static void test_check_args(const testCtx *const p_ctx) {

    int ret;

    unsigned char *src, *dest;
    int src_len, dest_len;

    unsigned char src_buf[16];
    unsigned char dest_buf[16];
    unsigned char dest_cmp_buf[16];

    testStart("test_check_args");

    (void)p_ctx;

    src = src_buf;
    ustrcpy(src, "A");
    src_len = (int) ustrlen(src);
    memset(dest_buf, 0xFF, ZUECI_ASIZE(dest_buf));
    dest = dest_buf;
    assert_equal(ZUECI_ASIZE(dest_buf), ZUECI_ASIZE(dest_cmp_buf), "ZUECI_ASIZE(dest_buf) %d != ZUECI_ASIZE(dest_cmp_buf) %d\n", ZUECI_ASIZE(dest_buf), ZUECI_ASIZE(dest_cmp_buf));
    memset(dest_cmp_buf, 0xFF, ZUECI_ASIZE(dest_buf));
    dest_len = 0;

#ifndef ZUECI_EMBED_NO_TO_ECI
    /* zueci_dest_len_eci() ECI */

    ret = zueci_dest_len_eci(14, src, src_len, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ECI, "zueci_dest_len_eci ECI ret %d != ZUECI_ERROR_INVALID_ECI\n", ret);

    ret = zueci_dest_len_eci(19, src, src_len, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ECI, "zueci_dest_len_eci ECI ret %d != ZUECI_ERROR_INVALID_ECI\n", ret);

    ret = zueci_dest_len_eci(-1, src, src_len, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ECI, "zueci_dest_len_eci ECI ret %d != ZUECI_ERROR_INVALID_ECI\n", ret);

    ret = zueci_dest_len_eci(36, src, src_len, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ECI, "zueci_dest_len_eci ECI ret %d != ZUECI_ERROR_INVALID_ECI\n", ret);

    /* NULL */

    ret = zueci_dest_len_eci(3, NULL, src_len, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ARGS, "zueci_dest_len_eci NULL ret %d != ZUECI_ERROR_INVALID_ARGS\n", ret);

    ret = zueci_dest_len_eci(3, src, src_len, NULL);
    assert_equal(ret, ZUECI_ERROR_INVALID_ARGS, "zueci_dest_len_eci NULL ret %d != ZUECI_ERROR_INVALID_ARGS\n", ret);

    /* zueci_utf8_to_eci() ECI */

    ret = zueci_utf8_to_eci(14, src, src_len, dest, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ECI, "zueci_utf8_to_eci ECI ret %d != ZUECI_ERROR_INVALID_ECI\n", ret);
    assert_zero(memcmp(dest, dest_cmp_buf, ZUECI_ASIZE(dest_cmp_buf)), "zueci_utf8_to_eci ECI memcmp(dest, dest_cmp_buf) != 0\n");
    assert_zero(dest_len, "zueci_utf8_to_eci ECI dest_len %d != 0\n", dest_len);

    ret = zueci_utf8_to_eci(19, src, src_len, dest, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ECI, "zueci_utf8_to_eci ECI ret %d != ZUECI_ERROR_INVALID_ECI\n", ret);

    ret = zueci_utf8_to_eci(-1, src, src_len, dest, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ECI, "zueci_utf8_to_eci ECI ret %d != ZUECI_ERROR_INVALID_ECI\n", ret);

    ret = zueci_utf8_to_eci(36, src, src_len, dest, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ECI, "zueci_utf8_to_eci ECI ret %d != ZUECI_ERROR_INVALID_ECI\n", ret);

    /* NULL */

    ret = zueci_utf8_to_eci(3, NULL, src_len, dest, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ARGS, "zueci_utf8_to_eci NULL ret %d != ZUECI_ERROR_INVALID_ARGS\n", ret);

    ret = zueci_utf8_to_eci(3, src, src_len, NULL, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ARGS, "zueci_utf8_to_eci NULL ret %d != ZUECI_ERROR_INVALID_ARGS\n", ret);

    ret = zueci_utf8_to_eci(3, src, src_len, dest, NULL);
    assert_equal(ret, ZUECI_ERROR_INVALID_ARGS, "zueci_utf8_to_eci NULL ret %d != ZUECI_ERROR_INVALID_ARGS\n", ret);

    /* UTF-8 */

    ret = zueci_utf8_to_eci(3, TU("\x80"), 1, dest, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_UTF8, "zueci_utf8_to_eci UTF-8 ret %d != ZUECI_ERROR_INVALID_UTF8\n", ret);

    ret = zueci_utf8_to_eci(4, TU("\x80"), 1, dest, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_UTF8, "zueci_utf8_to_eci UTF-8 ret %d != ZUECI_ERROR_INVALID_UTF8\n", ret);

    ret = zueci_utf8_to_eci(26, TU("\x80"), 1, dest, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_UTF8, "zueci_utf8_to_eci UTF-8 ret %d != ZUECI_ERROR_INVALID_UTF8\n", ret);
#endif /* ZUECI_EMBED_NO_TO_ECI */

#ifndef ZUECI_EMBED_NO_TO_UTF8
    /* zueci_dest_len_utf8() ECI */

    ret = zueci_dest_len_utf8(14, src, src_len, 0, 0, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ECI, "zueci_dest_len_utf8 ECI ret %d != ZUECI_ERROR_INVALID_ECI\n", ret);

    ret = zueci_dest_len_utf8(19, src, src_len, 0, 0, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ECI, "zueci_dest_len_utf8 ECI ret %d != ZUECI_ERROR_INVALID_ECI\n", ret);

    ret = zueci_dest_len_utf8(-1, src, src_len, 0, 0, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ECI, "zueci_dest_len_utf8 ECI ret %d != ZUECI_ERROR_INVALID_ECI\n", ret);

    ret = zueci_dest_len_utf8(36, src, src_len, 0, 0, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ECI, "zueci_dest_len_utf8 ECI ret %d != ZUECI_ERROR_INVALID_ECI\n", ret);

    /* NULL */

    ret = zueci_dest_len_utf8(3, NULL, src_len, 0, 0, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ARGS, "zueci_dest_len_utf8 NULL ret %d != ZUECI_ERROR_INVALID_ARGS\n", ret);

    ret = zueci_dest_len_utf8(3, src, src_len, 0, 0, NULL);
    assert_equal(ret, ZUECI_ERROR_INVALID_ARGS, "zueci_dest_len_utf8 NULL ret %d != ZUECI_ERROR_INVALID_ARGS\n", ret);

    /* replacement_char */

    ret = zueci_dest_len_utf8(3, src, src_len, 0x10000, 0, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ARGS, "zueci_dest_len_utf8 0x10000 ret %d != ZUECI_ERROR_INVALID_ARGS\n", ret);

    ret = zueci_dest_len_utf8(899, src, src_len, 0x10000, 0, &dest_len); /* Ignored for Binary */
    assert_zero(ret, "zueci_dest_len_utf8 0x10000 Binary ret %d != 0\n", ret);

    /* zueci_eci_to_utf8() ECI */

    ret = zueci_eci_to_utf8(14, src, src_len, 0, 0, dest, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ECI, "zueci_eci_to_utf8 ECI ret %d != ZUECI_ERROR_INVALID_ECI\n", ret);

    ret = zueci_eci_to_utf8(19, src, src_len, 0, 0, dest, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ECI, "zueci_eci_to_utf8 ECI ret %d != ZUECI_ERROR_INVALID_ECI\n", ret);

    ret = zueci_eci_to_utf8(36, src, src_len, 0, 0, dest, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ECI, "zueci_eci_to_utf8 ECI ret %d != ZUECI_ERROR_INVALID_ECI\n", ret);

    /* NULL */

    ret = zueci_eci_to_utf8(3, NULL, src_len, 0, 0, dest, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ARGS, "zueci_eci_to_utf8 NULL ret %d != ZUECI_ERROR_INVALID_ARGS\n", ret);

    ret = zueci_eci_to_utf8(3, src, src_len, 0, 0, NULL, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ARGS, "zueci_eci_to_utf8 NULL ret %d != ZUECI_ERROR_INVALID_ARGS\n", ret);

    ret = zueci_eci_to_utf8(3, src, src_len, 0, 0, dest, NULL);
    assert_equal(ret, ZUECI_ERROR_INVALID_ARGS, "zueci_eci_to_utf8 NULL ret %d != ZUECI_ERROR_INVALID_ARGS\n", ret);

    /* replacement_char */

    ret = zueci_eci_to_utf8(3, src, src_len, 0x10000, 0, dest, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ARGS, "zueci_eci_to_utf8 0x10000 ret %d != ZUECI_ERROR_INVALID_ARGS\n", ret);

    ret = zueci_eci_to_utf8(3, src, src_len, 0xD800, 0, dest, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ARGS, "zueci_eci_to_utf8 0xD800 ret %d != ZUECI_ERROR_INVALID_ARGS\n", ret);

    ret = zueci_eci_to_utf8(4, src, src_len, 0x10000, 0, dest, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ARGS, "zueci_eci_to_utf8 0x10000 ret %d != ZUECI_ERROR_INVALID_ARGS\n", ret);

    ret = zueci_eci_to_utf8(26, src, src_len, 0x10000, 0, dest, &dest_len);
    assert_equal(ret, ZUECI_ERROR_INVALID_ARGS, "zueci_eci_to_utf8 0x10000 ret %d != ZUECI_ERROR_INVALID_ARGS\n", ret);

    ret = zueci_eci_to_utf8(899, src, src_len, 0xD800, 0, dest, &dest_len); /* Ignored for Binary */
    assert_zero(ret, "zueci_eci_to_utf8 0xD800 Binary ret %d != 0\n", ret);
#endif /* ZUECI_EMBED_NO_TO_UTF8 */

    testFinish();
}

/* https://en.wikipedia.org/wiki/Code_page_437 */

static const unsigned short int cp437[] = { /* IBM CP 437 top half */
    0x00c7, 0x00fc, 0x00e9, 0x00e2, 0x00E4, 0x00E0, 0x00E5, 0x00E7, 0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
    0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9, 0x00FF, 0x00D6, 0x00DC, 0x00A2, 0x00A3, 0x00A5, 0x20A7, 0x0192,
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA, 0x00BF, 0x2310, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, 0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
    0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4, 0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229,
    0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248, 0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0,
};

/* Original zint eci.h tables */

static const unsigned short int iso_8859_1[] = {/* Latin alphabet No. 1 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7, 0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7, 0x00b8, 0x00b9, 0x00ba, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00bf,
    0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7, 0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,
    0x00d0, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x00d7, 0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x00de, 0x00df,
    0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7, 0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,
    0x00f0, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x00f7, 0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x00fe, 0x00ff
};

static const unsigned short int iso_8859_2[] = {/* Latin alphabet No. 2 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x00a0, 0x0104, 0x02d8, 0x0141, 0x00a4, 0x013d, 0x015a, 0x00a7, 0x00a8, 0x0160, 0x015e, 0x0164, 0x0179, 0x00ad, 0x017d, 0x017b,
    0x00b0, 0x0105, 0x02db, 0x0142, 0x00b4, 0x013e, 0x015b, 0x02c7, 0x00b8, 0x0161, 0x015f, 0x0165, 0x017a, 0x02dd, 0x017e, 0x017c,
    0x0154, 0x00c1, 0x00c2, 0x0102, 0x00c4, 0x0139, 0x0106, 0x00c7, 0x010c, 0x00c9, 0x0118, 0x00cb, 0x011a, 0x00cd, 0x00ce, 0x010e,
    0x0110, 0x0143, 0x0147, 0x00d3, 0x00d4, 0x0150, 0x00d6, 0x00d7, 0x0158, 0x016e, 0x00da, 0x0170, 0x00dc, 0x00dd, 0x0162, 0x00df,
    0x0155, 0x00e1, 0x00e2, 0x0103, 0x00e4, 0x013a, 0x0107, 0x00e7, 0x010d, 0x00e9, 0x0119, 0x00eb, 0x011b, 0x00ed, 0x00ee, 0x010f,
    0x0111, 0x0144, 0x0148, 0x00f3, 0x00f4, 0x0151, 0x00f6, 0x00f7, 0x0159, 0x016f, 0x00fa, 0x0171, 0x00fc, 0x00fd, 0x0163, 0x02d9
};

static const unsigned short int iso_8859_3[] = {/* Latin alphabet No. 3 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x00a0, 0x0126, 0x02d8, 0x00a3, 0x00a4, 0x0000, 0x0124, 0x00a7, 0x00a8, 0x0130, 0x015e, 0x011e, 0x0134, 0x00ad, 0x0000, 0x017b,
    0x00b0, 0x0127, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x0125, 0x00b7, 0x00b8, 0x0131, 0x015f, 0x011f, 0x0135, 0x00bd, 0x0000, 0x017c,
    0x00c0, 0x00c1, 0x00c2, 0x0000, 0x00c4, 0x010a, 0x0108, 0x00c7, 0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,
    0x0000, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x0120, 0x00d6, 0x00d7, 0x011c, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x016c, 0x015c, 0x00df,
    0x00e0, 0x00e1, 0x00e2, 0x0000, 0x00e4, 0x010b, 0x0109, 0x00e7, 0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,
    0x0000, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x0121, 0x00f6, 0x00f7, 0x011d, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x016d, 0x015d, 0x02d9
};

static const unsigned short int iso_8859_4[] = {/* Latin alphabet No. 4 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x00a0, 0x0104, 0x0138, 0x0156, 0x00a4, 0x0128, 0x013b, 0x00a7, 0x00a8, 0x0160, 0x0112, 0x0122, 0x0166, 0x00ad, 0x017d, 0x00af, /* A5 0x012b -> 0x0128 */
    0x00b0, 0x0105, 0x02db, 0x0157, 0x00b4, 0x0129, 0x013c, 0x02c7, 0x00b8, 0x0161, 0x0113, 0x0123, 0x0167, 0x014a, 0x017e, 0x014b,
    0x0100, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x012e, 0x010c, 0x00c9, 0x0118, 0x00cb, 0x0116, 0x00cd, 0x00ce, 0x012a,
    0x0110, 0x0145, 0x014c, 0x0136, 0x00d4, 0x00d5, 0x00d6, 0x00d7, 0x00d8, 0x0172, 0x00da, 0x00db, 0x00dc, 0x0168, 0x016a, 0x00df,
    0x0101, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x012f, 0x010d, 0x00e9, 0x0119, 0x00eb, 0x0117, 0x00ed, 0x00ee, 0x012b,
    0x0111, 0x0146, 0x014d, 0x0137, 0x00f4, 0x00f5, 0x00f6, 0x00f7, 0x00f8, 0x0173, 0x00fa, 0x00fb, 0x00fc, 0x0169, 0x016b, 0x02d9
};

static const unsigned short int iso_8859_5[] = {/* Latin/Cyrillic alphabet */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x00a0, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0406, 0x0407, 0x0408, 0x0409, 0x040a, 0x040b, 0x040c, 0x00ad, 0x040e, 0x040f,
    0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417, 0x0418, 0x0419, 0x041a, 0x041b, 0x041c, 0x041d, 0x041e, 0x041f,
    0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427, 0x0428, 0x0429, 0x042a, 0x042b, 0x042c, 0x042d, 0x042e, 0x042f,
    0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437, 0x0438, 0x0439, 0x043a, 0x043b, 0x043c, 0x043d, 0x043e, 0x043f,
    0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447, 0x0448, 0x0449, 0x044a, 0x044b, 0x044c, 0x044d, 0x044e, 0x044f,
    0x2116, 0x0451, 0x0452, 0x0453, 0x0454, 0x0455, 0x0456, 0x0457, 0x0458, 0x0459, 0x045a, 0x045b, 0x045c, 0x00a7, 0x045e, 0x045f
};

static const unsigned short int iso_8859_6[] = {/* Latin/Arabic alphabet */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x00a0, 0x0000, 0x0000, 0x0000, 0x00a4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x060c, 0x00ad, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x061b, 0x0000, 0x0000, 0x0000, 0x061f,
    0x0000, 0x0621, 0x0622, 0x0623, 0x0624, 0x0625, 0x0626, 0x0627, 0x0628, 0x0629, 0x062a, 0x062b, 0x062c, 0x062d, 0x062e, 0x062f,
    0x0630, 0x0631, 0x0632, 0x0633, 0x0634, 0x0635, 0x0636, 0x0637, 0x0638, 0x0639, 0x063a, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0640, 0x0641, 0x0642, 0x0643, 0x0644, 0x0645, 0x0646, 0x0647, 0x0648, 0x0649, 0x064a, 0x064b, 0x064c, 0x064d, 0x064e, 0x064f,
    0x0650, 0x0651, 0x0652, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

static const unsigned short int iso_8859_7[] = {/* Latin/Greek alphabet */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x00a0, 0x2018, 0x2019, 0x00a3, 0x20ac, 0x20af, 0x00a6, 0x00a7, 0x00a8, 0x00a9, 0x037a, 0x00ab, 0x00ac, 0x00ad, 0x0000, 0x2015,
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x0384, 0x0385, 0x0386, 0x00b7, 0x0388, 0x0389, 0x038a, 0x00bb, 0x038c, 0x00bd, 0x038e, 0x038f,
    0x0390, 0x0391, 0x0392, 0x0393, 0x0394, 0x0395, 0x0396, 0x0397, 0x0398, 0x0399, 0x039a, 0x039b, 0x039c, 0x039d, 0x039e, 0x039f,
    0x03a0, 0x03a1, 0x0000, 0x03a3, 0x03a4, 0x03a5, 0x03a6, 0x03a7, 0x03a8, 0x03a9, 0x03aa, 0x03ab, 0x03ac, 0x03ad, 0x03ae, 0x03af,
    0x03b0, 0x03b1, 0x03b2, 0x03b3, 0x03b4, 0x03b5, 0x03b6, 0x03b7, 0x03b8, 0x03b9, 0x03ba, 0x03bb, 0x03bc, 0x03bd, 0x03be, 0x03bf,
    0x03c0, 0x03c1, 0x03c2, 0x03c3, 0x03c4, 0x03c5, 0x03c6, 0x03c7, 0x03c8, 0x03c9, 0x03ca, 0x03cb, 0x03cc, 0x03cd, 0x03ce, 0x0000
};

static const unsigned short int iso_8859_8[] = {/* Latin/Hebrew alphabet */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x00a0, 0x0000, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7, 0x00a8, 0x00a9, 0x00d7, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7, 0x00b8, 0x00b9, 0x00f7, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2017,
    0x05d0, 0x05d1, 0x05d2, 0x05d3, 0x05d4, 0x05d5, 0x05d6, 0x05d7, 0x05d8, 0x05d9, 0x05da, 0x05db, 0x05dc, 0x05dd, 0x05de, 0x05df,
    0x05e0, 0x05e1, 0x05e2, 0x05e3, 0x05e4, 0x05e5, 0x05e6, 0x05e7, 0x05e8, 0x05e9, 0x05ea, 0x0000, 0x0000, 0x200e, 0x200f, 0x0000
};

static const unsigned short int iso_8859_9[] = {/* Latin alphabet No. 5 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7, 0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7, 0x00b8, 0x00b9, 0x00ba, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00bf,
    0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7, 0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,
    0x011e, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x00d7, 0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x0130, 0x015e, 0x00df,
    0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7, 0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,
    0x011f, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x00f7, 0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x0131, 0x015f, 0x00ff
};

static const unsigned short int iso_8859_10[] = {/* Latin alphabet No. 6 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x00a0, 0x0104, 0x0112, 0x0122, 0x012a, 0x0128, 0x0136, 0x00a7, 0x013b, 0x0110, 0x0160, 0x0166, 0x017d, 0x00ad, 0x016a, 0x014a, /* A5 0x012b -> 0x0128 */
    0x00b0, 0x0105, 0x0113, 0x0123, 0x012b, 0x0129, 0x0137, 0x00b7, 0x013c, 0x0111, 0x0161, 0x0167, 0x017e, 0x2015, 0x016b, 0x014b,
    0x0100, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x012e, 0x010c, 0x00c9, 0x0118, 0x00cb, 0x0116, 0x00cd, 0x00ce, 0x00cf,
    0x00d0, 0x0145, 0x014c, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x0168, 0x00d8, 0x0172, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x00de, 0x00df,
    0x0101, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x012f, 0x010d, 0x00e9, 0x0119, 0x00eb, 0x0117, 0x00ed, 0x00ee, 0x00ef,
    0x00f0, 0x0146, 0x014d, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x0169, 0x00f8, 0x0173, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x00fe, 0x0138
};

static const unsigned short int iso_8859_11[] = {/* Latin/Thai alphabet */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x00a0, 0x0e01, 0x0e02, 0x0e03, 0x0e04, 0x0e05, 0x0e06, 0x0e07, 0x0e08, 0x0e09, 0x0e0a, 0x0e0b, 0x0e0c, 0x0e0d, 0x0e0e, 0x0e0f,
    0x0e10, 0x0e11, 0x0e12, 0x0e13, 0x0e14, 0x0e15, 0x0e16, 0x0e17, 0x0e18, 0x0e19, 0x0e1a, 0x0e1b, 0x0e1c, 0x0e1d, 0x0e1e, 0x0e1f,
    0x0e20, 0x0e21, 0x0e22, 0x0e23, 0x0e24, 0x0e25, 0x0e26, 0x0e27, 0x0e28, 0x0e29, 0x0e2a, 0x0e2b, 0x0e2c, 0x0e2d, 0x0e2e, 0x0e2f,
    0x0e30, 0x0e31, 0x0e32, 0x0e33, 0x0e34, 0x0e35, 0x0e36, 0x0e37, 0x0e38, 0x0e39, 0x0e3a, 0x0000, 0x0000, 0x0000, 0x0000, 0x0e3f, /* D5 0x0e36 -> 0x0e35 */
    0x0e40, 0x0e41, 0x0e42, 0x0e43, 0x0e44, 0x0e45, 0x0e46, 0x0e47, 0x0e48, 0x0e49, 0x0e4a, 0x0e4b, 0x0e4c, 0x0e4d, 0x0e4e, 0x0e4f,
    0x0e50, 0x0e51, 0x0e52, 0x0e53, 0x0e54, 0x0e55, 0x0e56, 0x0e57, 0x0e58, 0x0e59, 0x0e5a, 0x0e5b, 0x0000, 0x0000, 0x0000, 0x0000
};

static const unsigned short int iso_8859_13[] = {/* Latin alphabet No. 7 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x00a0, 0x201d, 0x00a2, 0x00a3, 0x00a4, 0x201e, 0x00a6, 0x00a7, 0x00d8, 0x00a9, 0x0156, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00c6,
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x201c, 0x00b5, 0x00b6, 0x00b7, 0x00f8, 0x00b9, 0x0157, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00e6,
    0x0104, 0x012e, 0x0100, 0x0106, 0x00c4, 0x00c5, 0x0118, 0x0112, 0x010c, 0x00c9, 0x0179, 0x0116, 0x0122, 0x0136, 0x012a, 0x013b,
    0x0160, 0x0143, 0x0145, 0x00d3, 0x014c, 0x00d5, 0x00d6, 0x00d7, 0x0172, 0x0141, 0x015a, 0x016a, 0x00dc, 0x017b, 0x017d, 0x00df,
    0x0105, 0x012f, 0x0101, 0x0107, 0x00e4, 0x00e5, 0x0119, 0x0113, 0x010d, 0x00e9, 0x017a, 0x0117, 0x0123, 0x0137, 0x012b, 0x013c,
    0x0161, 0x0144, 0x0146, 0x00f3, 0x014d, 0x00f5, 0x00f6, 0x00f7, 0x0173, 0x0142, 0x015b, 0x016b, 0x00fc, 0x017c, 0x017e, 0x2019
};

static const unsigned short int iso_8859_14[] = {/* Latin alphabet No. 8 (Celtic) */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x00a0, 0x1e02, 0x1e03, 0x00a3, 0x010a, 0x010b, 0x1e0a, 0x00a7, 0x1e80, 0x00a9, 0x1e82, 0x1e0b, 0x1ef2, 0x00ad, 0x00ae, 0x0178,
    0x1e1e, 0x1e1f, 0x0120, 0x0121, 0x1e40, 0x1e41, 0x00b6, 0x1e56, 0x1e81, 0x1e57, 0x1e83, 0x1e60, 0x1ef3, 0x1e84, 0x1e85, 0x1e61,
    0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7, 0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,
    0x0174, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x1e6a, 0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x0176, 0x00df,
    0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7, 0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,
    0x0175, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x1e6b, 0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x0177, 0x00ff
};

static const unsigned short int iso_8859_15[] = {/* Latin alphabet No. 9 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x20ac, 0x00a5, 0x0160, 0x00a7, 0x0161, 0x00a9, 0x00aa, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x017d, 0x00b5, 0x00b6, 0x00b7, 0x017e, 0x00b9, 0x00ba, 0x00bb, 0x0152, 0x0153, 0x0178, 0x00bf,
    0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7, 0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,
    0x00d0, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x00d7, 0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x00de, 0x00df,
    0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7, 0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,
    0x00f0, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x00f7, 0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x00fe, 0x00ff
};

static const unsigned short int iso_8859_16[] = {/* Latin alphabet No. 10 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x00a0, 0x0104, 0x0105, 0x0141, 0x20ac, 0x201e, 0x0160, 0x00a7, 0x0161, 0x00a9, 0x0218, 0x00ab, 0x0179, 0x00ad, 0x017a, 0x017b,
    0x00b0, 0x00b1, 0x010c, 0x0142, 0x017d, 0x201d, 0x00b6, 0x00b7, 0x017e, 0x010d, 0x0219, 0x00bb, 0x0152, 0x0153, 0x0178, 0x017c,
    0x00c0, 0x00c1, 0x00c2, 0x0102, 0x00c4, 0x0106, 0x00c6, 0x00c7, 0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,
    0x0110, 0x0143, 0x00d2, 0x00d3, 0x00d4, 0x0150, 0x00d6, 0x015a, 0x0170, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x0118, 0x021a, 0x00df,
    0x00e0, 0x00e1, 0x00e2, 0x0103, 0x00e4, 0x0107, 0x00e6, 0x00e7, 0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,
    0x0111, 0x0144, 0x00f2, 0x00f3, 0x00f4, 0x0151, 0x00f6, 0x015b, 0x0171, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x0119, 0x021b, 0x00ff
};

static const unsigned short int windows_1250[] = {
    0x20ac, 0x0000, 0x201a, 0x0000, 0x201e, 0x2026, 0x2020, 0x2021, 0x0000, 0x2030, 0x0160, 0x2039, 0x015a, 0x0164, 0x017d, 0x0179,
    0x0000, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014, 0x0000, 0x2122, 0x0161, 0x203a, 0x015b, 0x0165, 0x017e, 0x017a,
    0x00a0, 0x02c7, 0x02d8, 0x0141, 0x00a4, 0x0104, 0x00a6, 0x00a7, 0x00a8, 0x00a9, 0x015e, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x017b, /* A2 0x02db -> 0x02d8 */
    0x00b0, 0x00b1, 0x02db, 0x0142, 0x00b4, 0x00b5, 0x00b6, 0x00b7, 0x00b8, 0x0105, 0x015f, 0x00bb, 0x013d, 0x02dd, 0x013e, 0x017c,
    0x0154, 0x00c1, 0x00c2, 0x0102, 0x00c4, 0x0139, 0x0106, 0x00c7, 0x010c, 0x00c9, 0x0118, 0x00cb, 0x011a, 0x00cd, 0x00ce, 0x010e,
    0x0110, 0x0143, 0x0147, 0x00d3, 0x00d4, 0x0150, 0x00d6, 0x00d7, 0x0158, 0x016e, 0x00da, 0x0170, 0x00dc, 0x00dd, 0x0162, 0x00df,
    0x0155, 0x00e1, 0x00e2, 0x0103, 0x00e4, 0x013a, 0x0107, 0x00e7, 0x010d, 0x00e9, 0x0119, 0x00eb, 0x011b, 0x00ed, 0x00ee, 0x010f,
    0x0111, 0x0144, 0x0148, 0x00f3, 0x00f4, 0x0151, 0x00f6, 0x00f7, 0x0159, 0x016f, 0x00fa, 0x0171, 0x00fc, 0x00fd, 0x0163, 0x02d9
};

static const unsigned short int windows_1251[] = {
    0x0402, 0x0403, 0x201a, 0x0453, 0x201e, 0x2026, 0x2020, 0x2021, 0x20ac, 0x2030, 0x0409, 0x2039, 0x040a, 0x040c, 0x040b, 0x040f,
    0x0452, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014, 0x0000, 0x2122, 0x0459, 0x203a, 0x045a, 0x045c, 0x045b, 0x045f,
    0x00a0, 0x040e, 0x045e, 0x0408, 0x00a4, 0x0490, 0x00a6, 0x00a7, 0x0401, 0x00a9, 0x0404, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x0407,
    0x00b0, 0x00b1, 0x0406, 0x0456, 0x0491, 0x00b5, 0x00b6, 0x00b7, 0x0451, 0x2116, 0x0454, 0x00bb, 0x0458, 0x0405, 0x0455, 0x0457,
    0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417, 0x0418, 0x0419, 0x041a, 0x041b, 0x041c, 0x041d, 0x041e, 0x041f,
    0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427, 0x0428, 0x0429, 0x042a, 0x042b, 0x042c, 0x042d, 0x042e, 0x042f,
    0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437, 0x0438, 0x0439, 0x043a, 0x043b, 0x043c, 0x043d, 0x043e, 0x043f,
    0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447, 0x0448, 0x0449, 0x044a, 0x044b, 0x044c, 0x044d, 0x044e, 0x044f
};

static const unsigned short int windows_1252[] = {
    0x20ac, 0x0000, 0x201a, 0x0192, 0x201e, 0x2026, 0x2020, 0x2021, 0x02c6, 0x2030, 0x0160, 0x2039, 0x0152, 0x0000, 0x017d, 0x0000,
    0x0000, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014, 0x02dc, 0x2122, 0x0161, 0x203a, 0x0153, 0x0000, 0x017e, 0x0178,
    0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7, 0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7, 0x00b8, 0x00b9, 0x00ba, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00bf,
    0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7, 0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,
    0x00d0, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x00d7, 0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x00de, 0x00df,
    0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7, 0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,
    0x00f0, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x00f7, 0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x00fe, 0x00ff
};

static const unsigned short int windows_1256[] = {
    0x20ac, 0x067e, 0x201a, 0x0192, 0x201e, 0x2026, 0x2020, 0x2021, 0x02c6, 0x2030, 0x0679, 0x2039, 0x0152, 0x0686, 0x0698, 0x0688,
    0x06af, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014, 0x06a9, 0x2122, 0x0691, 0x203a, 0x0153, 0x200c, 0x200d, 0x06ba,
    0x00a0, 0x060c, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7, 0x00a8, 0x00a9, 0x06be, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7, 0x00b8, 0x00b9, 0x061b, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x061f,
    0x06c1, 0x0621, 0x0622, 0x0623, 0x0624, 0x0625, 0x0626, 0x0627, 0x0628, 0x0629, 0x062a, 0x062b, 0x062c, 0x062d, 0x062e, 0x062f,
    0x0630, 0x0631, 0x0632, 0x0633, 0x0634, 0x0635, 0x0636, 0x00d7, 0x0637, 0x0638, 0x0639, 0x063a, 0x0640, 0x0641, 0x0642, 0x0643,
    0x00e0, 0x0644, 0x00e2, 0x0645, 0x0646, 0x0647, 0x0648, 0x00e7, 0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x0649, 0x064a, 0x00ee, 0x00ef,
    0x064b, 0x064c, 0x064d, 0x064e, 0x00f4, 0x064f, 0x0650, 0x00f7, 0x0651, 0x00f9, 0x0652, 0x00fb, 0x00fc, 0x200e, 0x200f, 0x06d2
};

static void test_sb(const testCtx *const p_ctx) {

    struct item {
        int eci;
        const unsigned short *tab;
        const char *iconv;
    };
    /* s/\/\*[ 0-9]*\*\//\=printf("\/\*%3d*\/", line(".") - line("'<")): */
    static const struct item data[] = {
        /*  0*/ { 0, cp437, "CP437" },
        /*  1*/ { 1, iso_8859_1, "ISO-8859-1" },
        /*  2*/ { 2, cp437, "CP437" },
        /*  3*/ { 3, iso_8859_1, "ISO-8859-1" },
        /*  4*/ { 4, iso_8859_2, "ISO-8859-2" },
        /*  5*/ { 5, iso_8859_3, "ISO-8859-3" },
        /*  6*/ { 6, iso_8859_4, "ISO-8859-4" },
        /*  7*/ { 7, iso_8859_5, "ISO-8859-5" },
        /*  8*/ { 8, iso_8859_6, "ISO-8859-6" },
        /*  9*/ { 9, iso_8859_7, "ISO-8859-7" },
        /* 10*/ { 10, iso_8859_8, "ISO-8859-8" },
        /* 11*/ { 11, iso_8859_9, "ISO-8859-9" },
        /* 12*/ { 12, iso_8859_10, "ISO-8859-10" },
        /* 13*/ { 13, iso_8859_11, "ISO-8859-11" },
        /* 14*/ { 15, iso_8859_13, "ISO-8859-13" },
        /* 15*/ { 16, iso_8859_14, "ISO-8859-14" },
        /* 16*/ { 17, iso_8859_15, "ISO-8859-15" },
        /* 17*/ { 18, iso_8859_16, "ISO-8859-16" },
        /* 18*/ { 21, windows_1250, "CP1250" },
        /* 19*/ { 22, windows_1251, "CP1251" },
        /* 20*/ { 23, windows_1252, "CP1252" },
        /* 21*/ { 24, windows_1256, "CP1256" },
    };
    const int data_size = ZUECI_ASIZE(data);
    int i, src_len, ret;

    unsigned char source[5];
    unsigned char dest[2];
    unsigned char dest2[5];

    testStart("test_sb");

    for (i = 0; i < data_size; i++) {
        int j;
#ifdef ZUECI_TEST_ICONV
        iconv_t cd_from_u, cd_to_u;
        cd_from_u = iconv_open(data[i].iconv /*to*/, "UTF-8" /*from*/);
        assert_zero(cd_from_u == (iconv_t) -1, "iconv_open cd_from_u == -1\n");
        cd_to_u = iconv_open("UTF-8" /*to*/, data[i].iconv /*from*/);
        assert_zero(cd_to_u == (iconv_t) -1, "iconv_open cd_to_u == -1\n");
#endif

        if (testContinue(p_ctx, i)) continue;

        for (j = 0; j < 128; j++) {
            int k = j + 128;
            int dest_len = 0;
            int dest2_len = 0;
            memset(dest, 0, ZUECI_ASIZE(dest));
            if (data[i].tab[j]) {
                src_len = testConvertUnicodeToUtf8(data[i].tab[j], source);
                assert_nonzero(src_len, "i:%d testConvertUnicodeToUtf8 src_len %d == 0\n", i, src_len);
#ifndef ZUECI_EMBED_NO_TO_ECI
                ret = zueci_utf8_to_eci(data[i].eci, source, src_len, dest, &dest_len);
                assert_zero(ret, "i:%d zueci_utf8_to_eci ret %d != 0\n", i, ret);
                assert_equal(*dest, k, "i:%d j:%d eci:%d codep:0x%x *dest 0x%X (%d) != 0x%X (%d)\n",
                    i, j, data[i].eci, data[i].tab[j], *dest, *dest, k, k);
                assert_nonzero(dest_len, "i:%d testConvertUnicodeToUtf8 dest_len %d == 0\n", i, dest_len);

#ifdef ZUECI_TEST_ICONV
                {
                    char out_from_u_buf[5] = {0};
                    char *in_from_u = (char *) source, *out_from_u = out_from_u_buf;
                    size_t in_from_u_bytes_left = (size_t) src_len;
                    size_t out_from_u_bytes_left = ZUECI_ASIZE(out_from_u_buf);

                    while (in_from_u_bytes_left) {
                        size_t from_u_ret = iconv(cd_from_u, &in_from_u, &in_from_u_bytes_left,
                            &out_from_u, &out_from_u_bytes_left);
                        assert_zero(from_u_ret, "i:%d j:%d from_u_ret %d != 0 (%d: %s)\n",
                            i, j, (int) from_u_ret, errno, strerror(errno));
                    }
                    assert_equal(dest_len + (int) out_from_u_bytes_left, ZUECI_ASIZE(out_from_u_buf),
                        "i:%d k:%d 0x%02X dest_len %d + out_from_u_bytes_left %d != %d\n",
                        i, k, k, dest_len, (int) out_from_u_bytes_left, ZUECI_ASIZE(out_from_u_buf));
                    assert_zero(in_from_u_bytes_left, "i:%d in_from_u_bytes_left %d != 0\n",
                        i, (int) in_from_u_bytes_left);
                    assert_zero(memcmp(out_from_u_buf, dest, dest_len),
                        "i:%d memcmp(out_from_u_buf, dest, %d) != 0\n", i, dest_len);
                }
#endif

#ifndef ZUECI_EMBED_NO_TO_UTF8
                /* Check reverse */
                memset(dest2, 0, ZUECI_ASIZE(dest2));
                ret = zueci_eci_to_utf8(data[i].eci, dest, dest_len, 0, 0, dest2, &dest2_len);
                assert_zero(ret, "i:%d j:%d zueci_eci_to_utf8 ret %d != 0\n", i, j, ret);
                assert_equal(src_len, dest2_len, "i:%d j:%d zueci_eci_to_utf8 src_len %d != dest2_len %d\n",
                    i, j, src_len, dest2_len);
                assert_zero(memcmp(source, dest2, src_len), "i:%d j:%d memcmp(source, dest2) != 0\n", i, j);

#ifdef ZUECI_TEST_ICONV
                {
                    char out_to_u_buf[5] = {0};
                    char *in_to_u = (char *) dest, *out_to_u = out_to_u_buf;
                    size_t in_to_u_bytes_left = (size_t) dest_len;
                    size_t out_to_u_bytes_left = ZUECI_ASIZE(out_to_u_buf);

                    while (in_to_u_bytes_left) {
                        size_t to_u_ret = iconv(cd_to_u, &in_to_u, &in_to_u_bytes_left,
                            &out_to_u, &out_to_u_bytes_left);
                        assert_zero(to_u_ret, "i:%d to_u_ret %d != 0 (%d: %s)\n",
                            i, (int) to_u_ret, errno, strerror(errno));
                    }
                    assert_equal(src_len + (int) out_to_u_bytes_left, ZUECI_ASIZE(out_to_u_buf),
                        "i:%d src_len %d + out_to_u_bytes_left %d != %d\n",
                        i, src_len, (int) out_to_u_bytes_left, ZUECI_ASIZE(out_to_u_buf));
                    assert_zero(in_to_u_bytes_left,
                        "i:%d in_to_u_bytes_left %d != 0\n", i, (int) in_to_u_bytes_left);
                    assert_zero(memcmp(out_to_u_buf, dest2, src_len),
                        "i:%d memcmp(out_to_u_buf, dest2, %d) != 0\n", i, src_len);
                }
#endif
#else
                (void)dest2; (void)dest2_len;
#endif /* ZUECI_EMBED_NO_TO_UTF8 */
#endif /* ZUECI_EMBED_NO_TO_ECI */
            } else {
                src_len = testConvertUnicodeToUtf8(k, source);
                assert_nonzero(src_len, "i:%d testConvertUnicodeToUtf8 src_len %d == 0\n", i, src_len);
#ifndef ZUECI_EMBED_NO_TO_ECI
                ret = zueci_utf8_to_eci(data[i].eci, source, src_len, dest, &dest_len);
                if (ret == 0) { /* Should be mapping for this codepoint in another entry */
                    int found = 0;
                    int m;
                    for (m = 0; m < 128; m++) {
                        if (data[i].tab[m] == k) {
                            found = 1;
                            break;
                        }
                    }
                    assert_nonzero(found,
                        "i:%d j:%d eci:%d codep:0x%x source:%s not found zueci_utf8_to_eci ret %d == 0\n",
                        i, j, data[i].eci, k, source, ret);
                } else {
                    assert_equal(ret, ZUECI_ERROR_INVALID_DATA,
                        "i:%d j:%d eci:%d codep:0x%x source:%s zueci_utf8_to_eci ret %d != ZUECI_ERROR_INVALID_DATA\n",
                        i, j, data[i].eci, k, source, ret);
                }
#endif
            }

            /* Check ASCII */
            dest_len = 0;
            memset(dest, 0, ZUECI_ASIZE(dest));
            src_len = testConvertUnicodeToUtf8(j, source);
            assert_equal(src_len, 1, "i:%d testConvertUnicodeToUtf8 src_len %d != 1\n", i, src_len);
#ifndef ZUECI_EMBED_NO_TO_ECI
            ret = zueci_utf8_to_eci(data[i].eci, source, src_len, dest, &dest_len);
            assert_zero(ret, "i:%d zueci_utf8_to_eci ret %d != 0\n", i, ret);
            assert_equal(dest_len, 1, "i:%d testConvertUnicodeToUtf8 dest_len %d != 1\n", i, dest_len);
            assert_equal(*dest, j, "i:%d j:%d eci:%d codep:0x%x *dest 0x%X (%d) != 0x%X (%d)\n",
                i, j, data[i].eci, j, *dest, *dest, j, j);

#ifndef ZUECI_EMBED_NO_TO_UTF8
            /* Check reverse */
            memset(dest2, 0, ZUECI_ASIZE(dest2));
            ret = zueci_eci_to_utf8(data[i].eci, dest, dest_len, 0, 0, dest2, &dest2_len);
            assert_zero(ret, "i:%d j:%d zueci_eci_to_utf8 ret %d != 0\n", i, j, ret);
            assert_equal(src_len, dest2_len, "i:%d j:%d zueci_eci_to_utf8 src_len %d != dest2_len %d\n",
                i, j, src_len, dest2_len);
            assert_zero(memcmp(source, dest2, src_len), "i:%d j:%d memcmp(source, dest2) != 0\n", i, j);
#endif
#else
            (void)ret; (void)dest2; (void)dest_len; (void)dest2_len;
#endif /* ZUECI_EMBED_NO_TO_ECI */
        }

#ifdef ZUECI_TEST_ICONV
        assert_zero(iconv_close(cd_from_u), "iconv_close cd_from_u fail\n");
        assert_zero(iconv_close(cd_to_u), "iconv_close cd_to_u fail\n");
#endif
    }

    testFinish();
}

#ifndef ZUECI_EMBED_NO_TO_ECI
static void test_ascii(const testCtx *const p_ctx) {

    struct item {
        int eci;
        unsigned char *data;
        int length;
        int ret;
    };
    /* s/\/\*[ 0-9]*\*\//\=printf("\/\*%3d*\/", line(".") - line("'<")): */
    static const struct item data[] = {
        /*  0*/ { 27, TU("\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"), 32, 0 },
        /*  1*/ { 27, TU(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\x7F"), 96, 0 },
        /*  2*/ { 27, TU("\xC2\x80"), -1, ZUECI_ERROR_INVALID_DATA },
        /*  3*/ { 170, TU("\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"), 32, 0 },
        /*  4*/ { 170, TU(" !\"  %&'()*+,-./0123456789:;<=>? ABCDEFGHIJKLMNOPQRSTUVWXYZ    _ abcdefghijklmnopqrstuvwxyz    \x7F"), 96, 0 },
        /*  5*/ { 170, TU("#"), -1, ZUECI_ERROR_INVALID_DATA },
        /*  6*/ { 170, TU("$"), -1, ZUECI_ERROR_INVALID_DATA },
        /*  7*/ { 170, TU("@"), -1, ZUECI_ERROR_INVALID_DATA },
        /*  8*/ { 170, TU("["), -1, ZUECI_ERROR_INVALID_DATA },
        /*  9*/ { 170, TU("\\"), -1, ZUECI_ERROR_INVALID_DATA },
        /* 10*/ { 170, TU("]"), -1, ZUECI_ERROR_INVALID_DATA },
        /* 11*/ { 170, TU("^"), -1, ZUECI_ERROR_INVALID_DATA },
        /* 12*/ { 170, TU("`"), -1, ZUECI_ERROR_INVALID_DATA },
        /* 13*/ { 170, TU("{"), -1, ZUECI_ERROR_INVALID_DATA },
        /* 14*/ { 170, TU("|"), -1, ZUECI_ERROR_INVALID_DATA },
        /* 15*/ { 170, TU("}"), -1, ZUECI_ERROR_INVALID_DATA },
        /* 16*/ { 170, TU("~"), -1, ZUECI_ERROR_INVALID_DATA },
        /* 17*/ { 170, TU("\xC2\x80"), -1, ZUECI_ERROR_INVALID_DATA },
        /* 18*/ { 170, TU("~"), -1, ZUECI_ERROR_INVALID_DATA },
        /* 19*/ { 0, TU("\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"), 32, 0 },
        /* 20*/ { 0, TU(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\x7F"), 96, 0 },
        /* 21*/ { 0, TU("\xC4\x80"), -1, ZUECI_ERROR_INVALID_DATA }, /* U+0100 */
        /* 22*/ { 26, TU("\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"), 32, 0 },
        /* 23*/ { 26, TU(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\x7F"), 96, 0 },
        /* 24*/ { 899, TU("\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"), 32, 0 },
        /* 25*/ { 899, TU(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\x7F"), 96, 0 },
        /* 26*/ { 899, TU("\xC4\x80"), -1, ZUECI_ERROR_INVALID_DATA }, /* U+0100 */
    };
    const int data_size = ZUECI_ASIZE(data);
    int i, src_len, ret;

    unsigned char dest[1024];
    unsigned char dest2[1024];

    testStart("test_ascii");

    for (i = 0; i < data_size; i++) {
        int dest_len = 0, eci_dest_len = 0;
        int dest2_len = 0, utf8_dest_len = 0;

        if (testContinue(p_ctx, i)) continue;

        /* zueci_dest_len_eci() */

        src_len = data[i].length != -1 ? data[i].length : (int) ustrlen(data[i].data);
        ret = zueci_dest_len_eci(data[i].eci, data[i].data, src_len, &eci_dest_len);
        assert_zero(ret, "i:%d zueci_dest_len_eci ret %d != 0\n", i, ret);
        assert_nonzero(eci_dest_len + 1 <= ZUECI_ASIZE(dest),
            "i:%d eci_dest_len %d + 1 > %d\n", i, eci_dest_len, ZUECI_ASIZE(dest));

        /* zueci_utf8_to_eci() */

        ret = zueci_utf8_to_eci(data[i].eci, data[i].data, src_len, dest, &dest_len);
        assert_equal(ret, data[i].ret, "i:%d zueci_utf8_to_eci ret %d != %d\n", i, ret, data[i].ret);
        if (ret == 0) {
            assert_equal(src_len, dest_len, "i:%d src_len %d != %d\n", i, src_len, dest_len);
            assert_zero(memcmp(data[i].data, dest, src_len), "i:%d memcmp != 0\n", i);

#ifndef ZUECI_EMBED_NO_TO_UTF8
            /* Check round-trip */

            ret = zueci_dest_len_utf8(data[i].eci, dest, dest_len, 0, 0, &utf8_dest_len);
            assert_zero(ret, "i:%d zueci_dest_len_utf8 ret %d != 0\n", i, ret);
            assert_nonzero(utf8_dest_len + 1 <= ZUECI_ASIZE(dest2), "i:%d utf8_dest_len %d + 1 > %d\n",
                i, utf8_dest_len, ZUECI_ASIZE(dest2));

            ret = zueci_eci_to_utf8(data[i].eci, dest, dest_len, 0, 0, dest2, &dest2_len);
            assert_equal(ret, data[i].ret, "i:%d zueci_eci_to_utf8 ret %d != %d\n", i, ret, data[i].ret);
            assert_equal(dest2_len, src_len, "i:%d dest2_len %d != src_len %d\n", i, dest2_len, src_len);
            assert_zero(memcmp(dest2, data[i].data, src_len), "i:%d memcmp() != 0\n", i);
            assert_nonzero(dest2_len <= utf8_dest_len, "i:%d dest2_len %d > utf8_dest_len %d\n",
                i, dest2_len, utf8_dest_len);
#else
            (void)dest2; (void)dest2_len; (void)utf8_dest_len;
#endif /* ZUECI_EMBED_NO_TO_UTF8 */
        }
    }

    testFinish();
}
#endif /* ZUECI_EMBED_NO_TO_ECI */

#ifndef ZUECI_EMBED_NO_TO_ECI
static void test_utf16be(const testCtx *const p_ctx) {

    struct item {
        unsigned char *data;
        int length;
        int ret;
        int expected_length;
        char *expected;
    };
    /* s/\/\*[ 0-9]*\*\//\=printf("\/\*%3d*\/", line(".") - line("'<")): */
    static const struct item data[] = {
        /*  0*/ { TU("\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"), 32, 0, 32 * 2, NULL },
        /*  1*/ { TU(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\x7F"), 96, 0, 96 * 2, NULL },
        /*  2*/ { TU("\xC2\x80\xEF\xBF\xBF"), -1, 0, 4, "\000\x80\xFF\xFF" }, /* U+0080 U+FFFF */
        /*  3*/ { TU("\xEF\xBF\xBE"), -1, 0, 2, "\xFF\xFE" }, /* U+FFFE (reversed BOM) allowed */
        /*  4*/ { TU("\xEF\xBB\xBF"), -1, 0, 2, "\xFE\xFF" }, /* U+FEFF (BOM) allowed */
        /*  5*/ { TU("\xED\x9F\xBF"), -1, 0, 2, "\xD7\xFF" }, /* U+D7FF (ed9fbf) */
        /*  6*/ { TU("\xED\xA0\x80"), -1, ZUECI_ERROR_INVALID_UTF8, -1, NULL }, /* U+D800 (eda080) surrogate half not allowed */
        /*  7*/ { TU("\xED\xBF\xBF"), -1, ZUECI_ERROR_INVALID_UTF8, -1, NULL }, /* U+DFFF (edbfbf) surrogate half not allowed */
        /*  8*/ { TU("\xEE\x80\x80"), -1, 0, 2, "\xE0\000" }, /* U+E000 (ee8080) */
        /*  9*/ { TU("\xF0\x90\x80\x80"), -1, 0, 4, "\xD8\000\xDC\000" }, /* U+10000 maps to surrogate pair */
        /* 10*/ { TU("\xF0\x90\x8F\xBF"), -1, 0, 4, "\xD8\000\xDF\xFF" }, /* U+103FF maps to surrogate pair */
        /* 11*/ { TU("\xF4\x80\x80\x80"), -1, 0, 4, "\xDB\xC0\xDC\000" }, /* U+100000 maps to surrogate pair */
        /* 12*/ { TU("\xF4\x8F\xB0\x80"), -1, 0, 4, "\xDB\xFF\xDC\000" }, /* U+10FC00 maps to surrogate pair */
        /* 13*/ { TU("\xF4\x8F\xBF\xBF"), -1, 0, 4, "\xDB\xFF\xDF\xFF" }, /* U+10FFFF maps to surrogate pair */
        /* 14*/ { TU("\xF4\x90\x80\x80"), -1, ZUECI_ERROR_INVALID_UTF8, -1, NULL }, /* Non-Unicode 0x110000 not allowed */
        /* 15*/ { TU("A\xED\x9F\xBF\xC2\xA1\xEE\x80\x80\xF0\xA0\x80\x80G"), -1, 0, 14, "\000A\xD7\xFF\000\xA1\xE0\000\xD8\x40\xDC\000\000G" }, /* 1, 2, 3, 4-byte mix */
    };
    const int data_size = ZUECI_ASIZE(data);
    int i, src_len, ret;
    const int eci = 25;

    unsigned char dest[1024];
    unsigned char dest2[1024];
#ifdef ZUECI_TEST_ICONV
    iconv_t cd_from_u, cd_to_u;
#endif

    testStart("test_utf16be");

#ifdef ZUECI_TEST_ICONV
    cd_from_u = iconv_open("UTF-16BE" /*to*/, "UTF-8" /*from*/);
    assert_zero(cd_from_u == (iconv_t) -1, "iconv_open cd_from_u == -1\n");
    cd_to_u = iconv_open("UTF-8" /*to*/, "UTF-16BE" /*from*/);
    assert_zero(cd_to_u == (iconv_t) -1, "iconv_open cd_to_u == -1\n");
#endif

    for (i = 0; i < data_size; i++) {
        int dest_len = 0, eci_dest_len = 0;
        int dest2_len = 0, utf8_dest_len = 0;

        if (testContinue(p_ctx, i)) continue;

        /* zueci_dest_len_eci() */

        src_len = data[i].length != -1 ? data[i].length : (int) ustrlen(data[i].data);
        ret = zueci_dest_len_eci(eci, data[i].data, src_len, &eci_dest_len);
        assert_zero(ret, "i:%d zueci_dest_len_eci ret %d != 0\n", i, ret);
        assert_nonzero(eci_dest_len + 1 <= ZUECI_ASIZE(dest),
            "i:%d eci_dest_len %d + 1 > %d\n", i, eci_dest_len, ZUECI_ASIZE(dest));

        /* zueci_utf8_to_eci() */

        ret = zueci_utf8_to_eci(eci, data[i].data, src_len, dest, &dest_len);
        assert_equal(ret, data[i].ret, "i:%d zueci_utf8_to_eci ret %d != %d\n", i, ret, data[i].ret);
        if (ret == 0) {
            assert_equal(dest_len, data[i].expected_length, "i:%d dest_len %d != %d\n",
                i, dest_len, data[i].expected_length);
            assert_nonzero(dest_len <= eci_dest_len, "i:%d dest_len %d > eci_dest_len %d\n",
                i, dest_len, eci_dest_len);
            if (data[i].expected) {
                assert_zero(memcmp(dest, data[i].expected, data[i].expected_length), "i:%d memcmp() != 0\n", i);
            } else {
                int j;
                for (j = 0; j < src_len; j++) {
                    assert_zero(dest[j * 2], "i:%d dest[%d] %d != 0\n", i, j * 2, dest[j * 2]);
                    assert_equal(dest[j * 2 + 1], data[i].data[j], "i:%d dest[%d] %d != data[%d] %d\n",
                        i, j * 2 + 1, dest[j * 2 + 1], j, data[i].data[j]);
                }
            }

#ifdef ZUECI_TEST_ICONV
            {
                char out_from_u_buf[1024] = {0};
                char *in_from_u = (char *) data[i].data, *out_from_u = out_from_u_buf;
                size_t in_from_u_bytes_left = (size_t) src_len;
                size_t out_from_u_bytes_left = ZUECI_ASIZE(out_from_u_buf);

                while (in_from_u_bytes_left) {
                    size_t from_u_ret = iconv(cd_from_u, &in_from_u, &in_from_u_bytes_left,
                        &out_from_u, &out_from_u_bytes_left);
                    assert_zero(from_u_ret, "i:%d from_u_ret %d != 0 (%d: %s)\n",
                        i, (int) from_u_ret, errno, strerror(errno));
                }
                assert_equal(data[i].expected_length + (int) out_from_u_bytes_left, ZUECI_ASIZE(out_from_u_buf),
                    "i:%d data[i].expected_length %d + out_from_u_bytes_left %d != %d\n",
                    i, data[i].expected_length, (int) out_from_u_bytes_left, ZUECI_ASIZE(out_from_u_buf));
                assert_zero(in_from_u_bytes_left, "i:%d in_from_u_bytes_left %d != 0\n",
                    i, (int) in_from_u_bytes_left);
                assert_zero(memcmp(out_from_u_buf, dest, dest_len),
                    "i:%d memcmp(out_from_u_buf, dest, %d) != 0\n", i, dest_len);
            }
#endif

#ifndef ZUECI_EMBED_NO_TO_UTF8
            /* Check round-trip */

            ret = zueci_dest_len_utf8(eci, dest, dest_len, 0, 0, &utf8_dest_len);
            assert_zero(ret, "i:%d zueci_dest_len_utf8 ret %d != 0\n", i, ret);
            assert_nonzero(utf8_dest_len + 1 <= ZUECI_ASIZE(dest2), "i:%d utf8_dest_len %d + 1 > %d\n",
                i, utf8_dest_len, ZUECI_ASIZE(dest2));

            ret = zueci_eci_to_utf8(eci, dest, dest_len, 0, 0, dest2, &dest2_len);
            assert_equal(ret, data[i].ret, "i:%d zueci_eci_to_utf8 ret %d != %d\n", i, ret, data[i].ret);
            assert_equal(dest2_len, src_len, "i:%d dest2_len %d != src_len %d\n", i, dest2_len, src_len);
            assert_zero(memcmp(dest2, data[i].data, src_len), "i:%d memcmp() != 0\n", i);
            assert_nonzero(dest2_len <= utf8_dest_len, "i:%d dest2_len %d > utf8_dest_len %d\n",
                i, dest2_len, utf8_dest_len);
#else
            (void)dest2; (void)dest2_len; (void)utf8_dest_len;
#endif

#ifdef ZUECI_TEST_ICONV
            {
                char out_to_u_buf[1024] = {0};
                char *in_to_u = (char *) dest, *out_to_u = out_to_u_buf;
                size_t in_to_u_bytes_left = (size_t) dest_len;
                size_t out_to_u_bytes_left = ZUECI_ASIZE(out_to_u_buf);

                while (in_to_u_bytes_left) {
                    size_t to_u_ret = iconv(cd_to_u, &in_to_u, &in_to_u_bytes_left,
                        &out_to_u, &out_to_u_bytes_left);
                    assert_zero(to_u_ret, "i:%d to_u_ret %d != 0 (%d: %s)\n",
                        i, (int) to_u_ret, errno, strerror(errno));
                }
                assert_equal(src_len + (int) out_to_u_bytes_left, ZUECI_ASIZE(out_to_u_buf),
                    "i:%d src_len %d + out_to_u_bytes_left %d != %d\n",
                    i, src_len, (int) out_to_u_bytes_left, ZUECI_ASIZE(out_to_u_buf));
                assert_zero(in_to_u_bytes_left,
                    "i:%d in_to_u_bytes_left %d != 0\n", i, (int) in_to_u_bytes_left);
                assert_zero(memcmp(out_to_u_buf, data[i].data, src_len),
                    "i:%d memcmp(out_to_u_buf, data[i].data, %d) != 0\n", i, src_len);
            }
#endif
        }
    }

#ifdef ZUECI_TEST_ICONV
    assert_zero(iconv_close(cd_from_u), "iconv_close cd_from_u fail\n");
    assert_zero(iconv_close(cd_to_u), "iconv_close cd_to_u fail\n");
#endif

    testFinish();
}
#endif /* ZUECI_EMBED_NO_TO_ECI */

#ifndef ZUECI_EMBED_NO_TO_ECI
static void test_utf16le(const testCtx *const p_ctx) {

    struct item {
        unsigned char *data;
        int length;
        int ret;
        int expected_length;
        char *expected;
    };
    /* s/\/\*[ 0-9]*\*\//\=printf("\/\*%3d*\/", line(".") - line("'<")): */
    static const struct item data[] = {
        /*  0*/ { TU("\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"), 32, 0, 32 * 2, NULL },
        /*  1*/ { TU(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\x7F"), 96, 0, 96 * 2, NULL },
        /*  2*/ { TU("\xC2\x80\xEF\xBF\xBF"), -1, 0, 4, "\x80\000\xFF\xFF" }, /* U+0080 U+FFFF */
        /*  3*/ { TU("\xEF\xBF\xBE"), -1, 0, 2, "\xFE\xFF" }, /* U+FFFE (reversed BOM) allowed */
        /*  4*/ { TU("\xEF\xBB\xBF"), -1, 0, 2, "\xFF\xFE" }, /* U+FEFF (BOM) allowed */
        /*  5*/ { TU("\xED\x9F\xBF"), -1, 0, 2, "\xFF\xD7" }, /* U+D7FF (ed9fbf) */
        /*  6*/ { TU("\xED\xA0\x80"), -1, ZUECI_ERROR_INVALID_UTF8, -1, NULL }, /* U+D800 (eda080) surrogate half not allowed */
        /*  7*/ { TU("\xED\xBF\xBF"), -1, ZUECI_ERROR_INVALID_UTF8, -1, NULL }, /* U+DFFF (edbfbf) surrogate half not allowed */
        /*  8*/ { TU("\xEE\x80\x80"), -1, 0, 2, "\000\xE0" }, /* U+E000 (ee8080) */
        /*  9*/ { TU("\xF0\x90\x80\x80"), -1, 0, 4, "\000\xD8\000\xDC" }, /* U+10000 maps to surrogate pair */
        /* 10*/ { TU("\xF0\x90\x8F\xBF"), -1, 0, 4, "\000\xD8\xFF\xDF" }, /* U+103FF maps to surrogate pair */
        /* 11*/ { TU("\xF4\x80\x80\x80"), -1, 0, 4, "\xC0\xDB\000\xDC" }, /* U+100000 maps to surrogate pair */
        /* 12*/ { TU("\xF4\x8F\xB0\x80"), -1, 0, 4, "\xFF\xDB\000\xDC" }, /* U+10FC00 maps to surrogate pair */
        /* 13*/ { TU("\xF4\x8F\xBF\xBF"), -1, 0, 4, "\xFF\xDB\xFF\xDF" }, /* U+10FFFF maps to surrogate pair */
        /* 14*/ { TU("\xF4\x90\x80\x80"), -1, ZUECI_ERROR_INVALID_UTF8, -1, NULL }, /* Non-Unicode 0x110000 not allowed */
        /* 15*/ { TU("A\xED\x9F\xBF\xC2\xA1\xEE\x80\x80\xF0\xA0\x80\x80G"), -1, 0, 14, "A\000\xFF\xD7\xA1\000\000\xE0\x40\xD8\000\xDCG\000" }, /* 1, 2, 3, 4-byte mix */
    };
    const int data_size = ZUECI_ASIZE(data);
    int i, src_len, ret;
    const int eci = 33;

    unsigned char dest[1024];
    unsigned char dest2[1024];
#ifdef ZUECI_TEST_ICONV
    iconv_t cd_from_u, cd_to_u;
#endif

    testStart("test_utf16le");

#ifdef ZUECI_TEST_ICONV
    cd_from_u = iconv_open("UTF-16LE" /*to*/, "UTF-8" /*from*/);
    assert_zero(cd_from_u == (iconv_t) -1, "iconv_open cd_from_u == -1\n");
    cd_to_u = iconv_open("UTF-8" /*to*/, "UTF-16LE" /*from*/);
    assert_zero(cd_to_u == (iconv_t) -1, "iconv_open cd_to_u == -1\n");
#endif

    for (i = 0; i < data_size; i++) {
        int dest_len = 0, eci_dest_len = 0;
        int dest2_len = 0, utf8_dest_len = 0;

        if (testContinue(p_ctx, i)) continue;

        /* zueci_dest_len_eci() */

        src_len = data[i].length != -1 ? data[i].length : (int) ustrlen(data[i].data);
        ret = zueci_dest_len_eci(eci, data[i].data, src_len, &eci_dest_len);
        assert_zero(ret, "i:%d zueci_dest_len_eci ret %d != 0\n", i, ret);
        assert_nonzero(eci_dest_len + 1 <= ZUECI_ASIZE(dest),
            "i:%d eci_dest_len %d + 1 > %d\n", i, eci_dest_len, ZUECI_ASIZE(dest));

        /* zueci_utf8_to_eci() */

        ret = zueci_utf8_to_eci(eci, (const unsigned char *) data[i].data, src_len, (unsigned char *) dest, &dest_len);
        assert_equal(ret, data[i].ret, "i:%d zueci_utf8_to_eci ret %d != %d\n", i, ret, data[i].ret);
        if (ret == 0) {
            assert_equal(dest_len, data[i].expected_length, "i:%d dest_len %d != %d\n",
                i, dest_len, data[i].expected_length);
            assert_nonzero(dest_len <= eci_dest_len, "i:%d dest_len %d > eci_dest_len %d\n",
                i, dest_len, eci_dest_len);
            if (data[i].expected) {
                assert_zero(memcmp(dest, data[i].expected, data[i].expected_length), "i:%d memcmp() != 0\n", i);
            } else {
                int j;
                for (j = 0; j < src_len; j++) {
                    assert_equal(dest[j * 2], data[i].data[j], "i:%d dest[%d] %d != data[%d] %d\n",
                        i, j * 2, dest[j * 2], j, data[i].data[j]);
                    assert_zero(dest[j * 2 + 1], "i:%d dest[%d] %d != 0\n", i, j * 2 + 1, dest[j * 2 + 1]);
                }
            }

#ifdef ZUECI_TEST_ICONV
            {
                char out_from_u_buf[1024] = {0};
                char *in_from_u = (char *) data[i].data, *out_from_u = out_from_u_buf;
                size_t in_from_u_bytes_left = (size_t) src_len;
                size_t out_from_u_bytes_left = ZUECI_ASIZE(out_from_u_buf);

                while (in_from_u_bytes_left) {
                    size_t from_u_ret = iconv(cd_from_u, &in_from_u, &in_from_u_bytes_left,
                        &out_from_u, &out_from_u_bytes_left);
                    assert_zero(from_u_ret, "i:%d from_u_ret %d != 0 (%d: %s)\n",
                        i, (int) from_u_ret, errno, strerror(errno));
                }
                assert_equal(data[i].expected_length + (int) out_from_u_bytes_left, ZUECI_ASIZE(out_from_u_buf),
                    "i:%d data[i].expected_length %d + out_from_u_bytes_left %d != %d\n",
                    i, data[i].expected_length, (int) out_from_u_bytes_left, ZUECI_ASIZE(out_from_u_buf));
                assert_zero(in_from_u_bytes_left, "i:%d in_from_u_bytes_left %d != 0\n",
                    i, (int) in_from_u_bytes_left);
                assert_zero(memcmp(out_from_u_buf, dest, dest_len),
                    "i:%d memcmp(out_from_u_buf, dest, %d) != 0\n", i, dest_len);
            }
#endif

#ifndef ZUECI_EMBED_NO_TO_UTF8
            /* Check round-trip */

            ret = zueci_dest_len_utf8(eci, dest, dest_len, 0, 0, &utf8_dest_len);
            assert_zero(ret, "i:%d zueci_dest_len_utf8 ret %d != 0\n", i, ret);
            assert_nonzero(utf8_dest_len + 1 <= ZUECI_ASIZE(dest2), "i:%d utf8_dest_len %d + 1 > %d\n",
                i, utf8_dest_len, ZUECI_ASIZE(dest2));

            ret = zueci_eci_to_utf8(eci, dest, dest_len, 0, 0, dest2, &dest2_len);
            assert_equal(ret, data[i].ret, "i:%d zueci_eci_to_utf8 ret %d != %d\n", i, ret, data[i].ret);
            assert_equal(dest2_len, src_len, "i:%d dest2_len %d != src_len %d\n", i, dest2_len, src_len);
            assert_zero(memcmp(dest2, data[i].data, src_len), "i:%d memcmp() != 0\n", i);
            assert_nonzero(dest2_len <= utf8_dest_len, "i:%d dest2_len %d > utf8_dest_len %d\n",
                i, dest2_len, utf8_dest_len);
#else
            (void)dest2; (void)dest2_len; (void)utf8_dest_len;
#endif /* ZUECI_EMBED_NO_TO_UTF8 */

#ifdef ZUECI_TEST_ICONV
            {
                char out_to_u_buf[1024] = {0};
                char *in_to_u = (char *) dest, *out_to_u = out_to_u_buf;
                size_t in_to_u_bytes_left = (size_t) dest_len;
                size_t out_to_u_bytes_left = ZUECI_ASIZE(out_to_u_buf);

                while (in_to_u_bytes_left) {
                    size_t to_u_ret = iconv(cd_to_u, &in_to_u, &in_to_u_bytes_left,
                        &out_to_u, &out_to_u_bytes_left);
                    assert_zero(to_u_ret, "i:%d to_u_ret %d != 0 (%d: %s)\n",
                        i, (int) to_u_ret, errno, strerror(errno));
                }
                assert_equal(src_len + (int) out_to_u_bytes_left, ZUECI_ASIZE(out_to_u_buf),
                    "i:%d src_len %d + out_to_u_bytes_left %d != %d\n",
                    i, src_len, (int) out_to_u_bytes_left, ZUECI_ASIZE(out_to_u_buf));
                assert_zero(in_to_u_bytes_left,
                    "i:%d in_to_u_bytes_left %d != 0\n", i, (int) in_to_u_bytes_left);
                assert_zero(memcmp(out_to_u_buf, data[i].data, src_len),
                    "i:%d memcmp(out_to_u_buf, data[i].data, %d) != 0\n", i, src_len);
            }
#endif
        }
    }

#ifdef ZUECI_TEST_ICONV
    assert_zero(iconv_close(cd_from_u), "iconv_close cd_from_u fail\n");
    assert_zero(iconv_close(cd_to_u), "iconv_close cd_to_u fail\n");
#endif

    testFinish();
}
#endif /* ZUECI_EMBED_NO_TO_ECI */

#ifndef ZUECI_EMBED_NO_TO_ECI
static void test_utf32be(const testCtx *const p_ctx) {

    struct item {
        unsigned char *data;
        int length;
        int ret;
        int expected_length;
        char *expected;
    };
    /* s/\/\*[ 0-9]*\*\//\=printf("\/\*%3d*\/", line(".") - line("'<")): */
    static const struct item data[] = {
        /*  0*/ { TU("\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"), 32, 0, 32 * 4, NULL },
        /*  1*/ { TU(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\x7F"), 96, 0, 96 * 4, NULL },
        /*  2*/ { TU("\xC2\x80\xEF\xBF\xBF"), -1, 0, 8, "\000\000\000\x80\000\000\xFF\xFF" }, /* U+0080 U+FFFF */
        /*  3*/ { TU("\xEF\xBF\xBE"), -1, 0, 4, "\000\000\xFF\xFE" }, /* U+FFFE (reversed BOM) allowed */
        /*  4*/ { TU("\xEF\xBB\xBF"), -1, 0, 4, "\000\000\xFE\xFF" }, /* U+FEFF (BOM) allowed */
        /*  5*/ { TU("\xED\x9F\xBF"), -1, 0, 4, "\000\000\xD7\xFF" }, /* U+D7FF (ed9fbf) */
        /*  6*/ { TU("\xED\xA0\x80"), -1, ZUECI_ERROR_INVALID_UTF8, -1, NULL }, /* U+D800 (eda080) surrogate half not allowed */
        /*  7*/ { TU("\xED\xBF\xBF"), -1, ZUECI_ERROR_INVALID_UTF8, -1, NULL }, /* U+DFFF (edbfbf) surrogate half not allowed */
        /*  8*/ { TU("\xEE\x80\x80"), -1, 0, 4, "\000\000\xE0\000" }, /* U+E000 (ee8080) */
        /*  9*/ { TU("\xF0\x90\x80\x80"), -1, 0, 4, "\000\x01\000\000" }, /* U+10000 */
        /* 10*/ { TU("\xF4\x8F\xBF\xBF"), -1, 0, 4, "\000\020\xFF\xFF" }, /* U+10FFFF */
        /* 11*/ { TU("\xF4\x90\x80\x80"), -1, ZUECI_ERROR_INVALID_UTF8, -1, NULL }, /* Non-Unicode 0x110000 not allowed */
        /* 12*/ { TU("A\xED\x9F\xBF\xC2\xA1\xEE\x80\x80\xF0\xA0\x80\x80G"), -1, 0, 24, "\000\000\000A\000\000\xD7\xFF\000\000\000\xA1\000\000\xE0\000\000\x02\000\000\000\000\000G" }, /* 1, 2, 3, 4-byte mix */
    };
    const int data_size = ZUECI_ASIZE(data);
    int i, src_len, ret;
    const int eci = 34;

    unsigned char dest[1024];
    unsigned char dest2[1024];
#ifdef ZUECI_TEST_ICONV
    iconv_t cd_from_u, cd_to_u;
#endif

    testStart("test_utf32be");

#ifdef ZUECI_TEST_ICONV
    cd_from_u = iconv_open("UTF-32BE" /*to*/, "UTF-8" /*from*/);
    assert_zero(cd_from_u == (iconv_t) -1, "iconv_open cd_from_u == -1\n");
    cd_to_u = iconv_open("UTF-8" /*to*/, "UTF-32BE" /*from*/);
    assert_zero(cd_to_u == (iconv_t) -1, "iconv_open cd_to_u == -1\n");
#endif

    for (i = 0; i < data_size; i++) {
        int dest_len = 0, eci_dest_len = 0;
        int dest2_len = 0, utf8_dest_len = 0;

        if (testContinue(p_ctx, i)) continue;

        /* zueci_dest_len_eci() */

        src_len = data[i].length != -1 ? data[i].length : (int) ustrlen(data[i].data);
        ret = zueci_dest_len_eci(eci, data[i].data, src_len, &eci_dest_len);
        assert_zero(ret, "i:%d zueci_dest_len_eci ret %d != 0\n", i, ret);
        assert_nonzero(eci_dest_len + 1 <= ZUECI_ASIZE(dest),
            "i:%d eci_dest_len %d + 1 > %d\n", i, eci_dest_len, ZUECI_ASIZE(dest));

        /* zueci_utf8_to_eci() */

        ret = zueci_utf8_to_eci(eci, data[i].data, src_len, dest, &dest_len);
        assert_equal(ret, data[i].ret, "i:%d zueci_utf8_to_eci ret %d != %d\n", i, ret, data[i].ret);
        if (ret == 0) {
            assert_equal(dest_len, data[i].expected_length, "i:%d dest_len %d != %d\n", i, dest_len, data[i].expected_length);
            assert_nonzero(dest_len <= eci_dest_len, "i:%d dest_len %d > eci_dest_len %d\n", i, dest_len, eci_dest_len);
            if (data[i].expected) {
                assert_zero(memcmp(dest, data[i].expected, data[i].expected_length), "i:%d memcmp() != 0\n", i);
            } else {
                int j;
                for (j = 0; j < src_len; j++) {
                    assert_zero(dest[j * 4], "i:%d dest[%d] %d != 0\n", i, j * 4, dest[j * 4]);
                    assert_zero(dest[j * 4 + 1], "i:%d dest[%d] %d != 0\n", i, j * 4 + 1, dest[j * 4 + 1]);
                    assert_zero(dest[j * 4 + 2], "i:%d dest[%d] %d != 0\n", i, j * 4 + 2, dest[j * 4 + 2]);
                    assert_equal(dest[j * 4 + 3], data[i].data[j], "i:%d dest[%d] %d != data[%d] %d\n", i, j * 4 + 3, dest[j * 4 + 3], j, data[i].data[j]);
                }
            }

#ifdef ZUECI_TEST_ICONV
            {
                char out_from_u_buf[1024] = {0};
                char *in_from_u = (char *) data[i].data, *out_from_u = out_from_u_buf;
                size_t in_from_u_bytes_left = (size_t) src_len;
                size_t out_from_u_bytes_left = ZUECI_ASIZE(out_from_u_buf);

                while (in_from_u_bytes_left) {
                    size_t from_u_ret = iconv(cd_from_u, &in_from_u, &in_from_u_bytes_left,
                        &out_from_u, &out_from_u_bytes_left);
                    assert_zero(from_u_ret, "i:%d from_u_ret %d != 0 (%d: %s)\n",
                        i, (int) from_u_ret, errno, strerror(errno));
                }
                assert_equal(data[i].expected_length + (int) out_from_u_bytes_left, ZUECI_ASIZE(out_from_u_buf),
                    "i:%d data[i].expected_length %d + out_from_u_bytes_left %d != %d\n",
                    i, data[i].expected_length, (int) out_from_u_bytes_left, ZUECI_ASIZE(out_from_u_buf));
                assert_zero(in_from_u_bytes_left, "i:%d in_from_u_bytes_left %d != 0\n",
                    i, (int) in_from_u_bytes_left);
                assert_zero(memcmp(out_from_u_buf, dest, dest_len),
                    "i:%d memcmp(out_from_u_buf, dest, %d) != 0\n", i, dest_len);
            }
#endif

#ifndef ZUECI_EMBED_NO_TO_UTF8
            /* Check round-trip */

            ret = zueci_dest_len_utf8(eci, dest, dest_len, 0, 0, &utf8_dest_len);
            assert_zero(ret, "i:%d zueci_dest_len_utf8 ret %d != 0\n", i, ret);
            assert_nonzero(utf8_dest_len + 1 <= ZUECI_ASIZE(dest2), "i:%d utf8_dest_len %d + 1 > %d\n",
                i, utf8_dest_len, ZUECI_ASIZE(dest2));

            ret = zueci_eci_to_utf8(eci, dest, dest_len, 0, 0, dest2, &dest2_len);
            assert_equal(ret, data[i].ret, "i:%d zueci_eci_to_utf8 ret %d != %d\n", i, ret, data[i].ret);
            assert_equal(dest2_len, src_len, "i:%d dest2_len %d != src_len %d\n", i, dest2_len, src_len);
            assert_zero(memcmp(dest2, data[i].data, src_len), "i:%d memcmp() != 0\n", i);
            assert_nonzero(dest2_len <= utf8_dest_len, "i:%d dest2_len %d > utf8_dest_len %d\n",
                i, dest2_len, utf8_dest_len);
#else
            (void)dest2; (void)dest2_len; (void)utf8_dest_len;
#endif /* ZUECI_EMBED_NO_TO_UTF8 */

#ifdef ZUECI_TEST_ICONV
            {
                char out_to_u_buf[1024] = {0};
                char *in_to_u = (char *) dest, *out_to_u = out_to_u_buf;
                size_t in_to_u_bytes_left = (size_t) dest_len;
                size_t out_to_u_bytes_left = ZUECI_ASIZE(out_to_u_buf);

                while (in_to_u_bytes_left) {
                    size_t to_u_ret = iconv(cd_to_u, &in_to_u, &in_to_u_bytes_left,
                        &out_to_u, &out_to_u_bytes_left);
                    assert_zero(to_u_ret, "i:%d to_u_ret %d != 0 (%d: %s)\n",
                        i, (int) to_u_ret, errno, strerror(errno));
                }
                assert_equal(src_len + (int) out_to_u_bytes_left, ZUECI_ASIZE(out_to_u_buf),
                    "i:%d src_len %d + out_to_u_bytes_left %d != %d\n",
                    i, src_len, (int) out_to_u_bytes_left, ZUECI_ASIZE(out_to_u_buf));
                assert_zero(in_to_u_bytes_left,
                    "i:%d in_to_u_bytes_left %d != 0\n", i, (int) in_to_u_bytes_left);
                assert_zero(memcmp(out_to_u_buf, data[i].data, src_len),
                    "i:%d memcmp(out_to_u_buf, data[i].data, %d) != 0\n", i, src_len);
            }
#endif
        }
    }

#ifdef ZUECI_TEST_ICONV
    assert_zero(iconv_close(cd_from_u), "iconv_close cd_from_u fail\n");
    assert_zero(iconv_close(cd_to_u), "iconv_close cd_to_u fail\n");
#endif

    testFinish();
}
#endif /* ZUECI_EMBED_NO_TO_ECI */

#ifndef ZUECI_EMBED_NO_TO_ECI
static void test_utf32le(const testCtx *const p_ctx) {

    struct item {
        unsigned char *data;
        int length;
        int ret;
        int expected_length;
        char *expected;
    };
    /* s/\/\*[ 0-9]*\*\//\=printf("\/\*%3d*\/", line(".") - line("'<")): */
    static const struct item data[] = {
        /*  0*/ { TU("\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"), 32, 0, 32 * 4, NULL },
        /*  1*/ { TU(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\x7F"), 96, 0, 96 * 4, NULL },
        /*  2*/ { TU("\xC2\x80\xEF\xBF\xBF"), -1, 0, 8, "\x80\000\000\000\xFF\xFF\000\000" }, /* U+0080 U+FFFF */
        /*  3*/ { TU("\xEF\xBF\xBE"), -1, 0, 4, "\xFE\xFF\000\000" }, /* U+FFFE (reversed BOM) allowed */
        /*  4*/ { TU("\xEF\xBB\xBF"), -1, 0, 4, "\xFF\xFE\000\000" }, /* U+FEFF (BOM) allowed */
        /*  5*/ { TU("\xED\x9F\xBF"), -1, 0, 4, "\xFF\xD7\000\000" }, /* U+D7FF (ed9fbf) */
        /*  6*/ { TU("\xED\xA0\x80"), -1, ZUECI_ERROR_INVALID_UTF8, -1, NULL }, /* U+D800 (eda080) surrogate half not allowed */
        /*  7*/ { TU("\xED\xBF\xBF"), -1, ZUECI_ERROR_INVALID_UTF8, -1, NULL }, /* U+DFFF (edbfbf) surrogate half not allowed */
        /*  8*/ { TU("\xEE\x80\x80"), -1, 0, 4, "\000\xE0\000\000" }, /* U+E000 (ee8080) */
        /*  9*/ { TU("\xF0\x90\x80\x80"), -1, 0, 4, "\000\000\x01\000" }, /* U+10000 */
        /* 10*/ { TU("\xF4\x8F\xBF\xBF"), -1, 0, 4, "\xFF\xFF\020\000" }, /* U+10FFFF */
        /* 11*/ { TU("\xF4\x90\x80\x80"), -1, ZUECI_ERROR_INVALID_UTF8, -1, NULL }, /* Non-Unicode 0x110000 not allowed */
        /* 12*/ { TU("A\xED\x9F\xBF\xC2\xA1\xEE\x80\x80\xF0\xA0\x80\x80G"), -1, 0, 24, "A\000\000\000\xFF\xD7\000\000\xA1\000\000\000\000\xE0\000\000\000\000\x02\000G\000\000\000" }, /* 1, 2, 3, 4-byte mix */
    };
    const int data_size = ZUECI_ASIZE(data);
    int i, src_len, ret;
    const int eci = 35;

    unsigned char dest[1024];
    unsigned char dest2[1024];
#ifdef ZUECI_TEST_ICONV
    iconv_t cd_from_u, cd_to_u;
#endif

    testStart("test_utf32le");

#ifdef ZUECI_TEST_ICONV
    cd_from_u = iconv_open("UTF-32LE" /*to*/, "UTF-8" /*from*/);
    assert_zero(cd_from_u == (iconv_t) -1, "iconv_open cd_from_u == -1\n");
    cd_to_u = iconv_open("UTF-8" /*to*/, "UTF-32LE" /*from*/);
    assert_zero(cd_to_u == (iconv_t) -1, "iconv_open cd_to_u == -1\n");
#endif

    for (i = 0; i < data_size; i++) {
        int dest_len = 0, eci_dest_len = 0;
        int dest2_len = 0, utf8_dest_len = 0;

        if (testContinue(p_ctx, i)) continue;

        /* zueci_dest_len_eci() */

        src_len = data[i].length != -1 ? data[i].length : (int) ustrlen(data[i].data);
        ret = zueci_dest_len_eci(eci, data[i].data, src_len, &eci_dest_len);
        assert_zero(ret, "i:%d zueci_dest_len_eci ret %d != 0\n", i, ret);
        assert_nonzero(eci_dest_len + 1 <= ZUECI_ASIZE(dest),
            "i:%d eci_dest_len %d + 1 > %d\n", i, eci_dest_len, ZUECI_ASIZE(dest));

        /* zueci_utf8_to_eci() */

        ret = zueci_utf8_to_eci(eci, data[i].data, src_len, dest, &dest_len);
        assert_equal(ret, data[i].ret, "i:%d zueci_utf8_to_eci ret %d != %d\n", i, ret, data[i].ret);
        if (ret == 0) {
            assert_equal(dest_len, data[i].expected_length, "i:%d dest_len %d != %d\n", i, dest_len, data[i].expected_length);
            assert_nonzero(dest_len <= eci_dest_len, "i:%d dest_len %d > eci_dest_len %d\n", i, dest_len, eci_dest_len);
            if (data[i].expected) {
                assert_zero(memcmp(dest, data[i].expected, data[i].expected_length), "i:%d memcmp() != 0\n", i);
            } else {
                int j;
                for (j = 0; j < src_len; j++) {
                    assert_equal(dest[j * 4], data[i].data[j], "i:%d dest[%d] %d != data[%d] %d\n", i, j * 4, dest[j * 4], j, data[i].data[j]);
                    assert_zero(dest[j * 4 + 1], "i:%d dest[%d] %d != 0\n", i, j * 4 + 1, dest[j * 4 + 1]);
                    assert_zero(dest[j * 4 + 2], "i:%d dest[%d] %d != 0\n", i, j * 4 + 2, dest[j * 4 + 2]);
                    assert_zero(dest[j * 4 + 3], "i:%d dest[%d] %d != 0\n", i, j * 4 + 3, dest[j * 4 + 3]);
                }
            }

#ifdef ZUECI_TEST_ICONV
            {
                char out_from_u_buf[1024] = {0};
                char *in_from_u = (char *) data[i].data, *out_from_u = out_from_u_buf;
                size_t in_from_u_bytes_left = (size_t) src_len;
                size_t out_from_u_bytes_left = ZUECI_ASIZE(out_from_u_buf);

                while (in_from_u_bytes_left) {
                    size_t from_u_ret = iconv(cd_from_u, &in_from_u, &in_from_u_bytes_left,
                        &out_from_u, &out_from_u_bytes_left);
                    assert_zero(from_u_ret, "i:%d from_u_ret %d != 0 (%d: %s)\n",
                        i, (int) from_u_ret, errno, strerror(errno));
                }
                assert_equal(data[i].expected_length + (int) out_from_u_bytes_left, ZUECI_ASIZE(out_from_u_buf),
                    "i:%d data[i].expected_length %d + out_from_u_bytes_left %d != %d\n",
                    i, data[i].expected_length, (int) out_from_u_bytes_left, ZUECI_ASIZE(out_from_u_buf));
                assert_zero(in_from_u_bytes_left, "i:%d in_from_u_bytes_left %d != 0\n",
                    i, (int) in_from_u_bytes_left);
                assert_zero(memcmp(out_from_u_buf, dest, dest_len),
                    "i:%d memcmp(out_from_u_buf, dest, %d) != 0\n", i, dest_len);
            }
#endif

#ifndef ZUECI_EMBED_NO_TO_UTF8
            /* Check round-trip */

            ret = zueci_dest_len_utf8(eci, dest, dest_len, 0, 0, &utf8_dest_len);
            assert_zero(ret, "i:%d zueci_dest_len_utf8 ret %d != 0\n", i, ret);
            assert_nonzero(utf8_dest_len + 1 <= ZUECI_ASIZE(dest2), "i:%d utf8_dest_len %d + 1 > %d\n",
                i, utf8_dest_len, ZUECI_ASIZE(dest2));

            ret = zueci_eci_to_utf8(eci, dest, dest_len, 0, 0, dest2, &dest2_len);
            assert_equal(ret, data[i].ret, "i:%d zueci_eci_to_utf8 ret %d != %d\n", i, ret, data[i].ret);
            assert_equal(dest2_len, src_len, "i:%d dest2_len %d != src_len %d\n", i, dest2_len, src_len);
            assert_zero(memcmp(dest2, data[i].data, src_len), "i:%d memcmp() != 0\n", i);
            assert_nonzero(dest2_len <= utf8_dest_len, "i:%d dest2_len %d > utf8_dest_len %d\n",
                i, dest2_len, utf8_dest_len);
#else
            (void)dest2; (void)dest2_len; (void)utf8_dest_len;
#endif /* ZUECI_EMBED_NO_TO_UTF8 */

#ifdef ZUECI_TEST_ICONV
            {
                char out_to_u_buf[1024] = {0};
                char *in_to_u = (char *) dest, *out_to_u = out_to_u_buf;
                size_t in_to_u_bytes_left = (size_t) dest_len;
                size_t out_to_u_bytes_left = ZUECI_ASIZE(out_to_u_buf);

                while (in_to_u_bytes_left) {
                    size_t to_u_ret = iconv(cd_to_u, &in_to_u, &in_to_u_bytes_left,
                        &out_to_u, &out_to_u_bytes_left);
                    assert_zero(to_u_ret, "i:%d to_u_ret %d != 0 (%d: %s)\n",
                        i, (int) to_u_ret, errno, strerror(errno));
                }
                assert_equal(src_len + (int) out_to_u_bytes_left, ZUECI_ASIZE(out_to_u_buf),
                    "i:%d src_len %d + out_to_u_bytes_left %d != %d\n",
                    i, src_len, (int) out_to_u_bytes_left, ZUECI_ASIZE(out_to_u_buf));
                assert_zero(in_to_u_bytes_left,
                    "i:%d in_to_u_bytes_left %d != 0\n", i, (int) in_to_u_bytes_left);
                assert_zero(memcmp(out_to_u_buf, data[i].data, src_len),
                    "i:%d memcmp(out_to_u_buf, data[i].data, %d) != 0\n", i, src_len);
            }
#endif
        }
    }

#ifdef ZUECI_TEST_ICONV
    assert_zero(iconv_close(cd_from_u), "iconv_close cd_from_u fail\n");
    assert_zero(iconv_close(cd_to_u), "iconv_close cd_to_u fail\n");
#endif

    testFinish();
}
#endif /* ZUECI_EMBED_NO_TO_ECI */

#ifndef ZUECI_EMBED_NO_TO_UTF8
/* Test zueci_eci_to_utf8() `replacement_char` & `flags` */
static void test_replacement_char_flags(const testCtx *const p_ctx) {

    struct item {
        int eci;
        unsigned char *data;
        int length;
        unsigned int replacement_char;
        unsigned int flags;
        const char *iconv;
        int ret;
        int expected_length;
    };
    /* s/\/\*[ 0-9]*\*\//\=printf("\/\*%3d*\/", line(".") - line("'<")): */
    static const struct item data[] = {
        /*  0*/ { 1, TU("\x80"), -1, 0, 0, "ISO-8859-1", ZUECI_ERROR_INVALID_DATA, -1 },
        /*  1*/ { 1, TU("\x80"), -1, '?', 0, "ISO-8859-1", ZUECI_WARN_INVALID_DATA, 1 },
        /*  2*/ { 1, TU("\x80"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "ISO-8859-1", 0, 2 }, /* Converted to U+0080 */
        /*  3*/ { 3, TU("A\x80GH\x9F\xA0"), -1, 0, 0, "ISO-8859-1", ZUECI_ERROR_INVALID_DATA, -1 },
        /*  4*/ { 3, TU("A\x80GH\x9F\xA0"), -1, 0xFFFD, 0, "ISO-8859-1", ZUECI_WARN_INVALID_DATA, 11 },
        /*  5*/ { 3, TU("A\x80GH\x9F\xA0"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "ISO-8859-1", 0, 9 },
        /*  6*/ { 4, TU("\x80"), -1, 0, 0, "ISO-8859-2", ZUECI_ERROR_INVALID_DATA, -1 },
        /*  7*/ { 4, TU("\x80"), -1, '?', 0, "ISO-8859-2", ZUECI_WARN_INVALID_DATA, 1 },
        /*  8*/ { 4, TU("\x80"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "ISO-8859-2", 0, 2 },
        /*  9*/ { 5, TU("\xA5GH\xA4I\xF0"), -1, 0, 0, "ISO-8859-3", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 10*/ { 5, TU("\xA5GH\xA4I\xF0"), -1, 0x00BF, 0, "ISO-8859-3", ZUECI_WARN_INVALID_DATA, 9 }, /* U+00BF INVERTED QUESTION MARK */
        /* 11*/ { 5, TU("\xA5GH\xA4I\xF0"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "ISO-8859-3", 0, 9 },
        /* 12*/ { 6, TU("\x80"), -1, 0, 0, "ISO-8859-4", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 13*/ { 6, TU("\x80"), -1, 0xFFFD, 0, "ISO-8859-4", ZUECI_WARN_INVALID_DATA, 3 },
        /* 14*/ { 6, TU("\x80"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "ISO-8859-4", 0, 2 },
        /* 15*/ { 7, TU("\x9F"), -1, 0, 0, "ISO-8859-5", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 16*/ { 7, TU("\x9F"), -1, 0xFFFD, 0, "ISO-8859-5", ZUECI_WARN_INVALID_DATA, 3 },
        /* 17*/ { 7, TU("\x9F"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "ISO-8859-5", 0, 2 },
        /* 18*/ { 8, TU("\xA1"), -1, 0, 0, "ISO-8859-6", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 19*/ { 8, TU("\xA1"), -1, 0xFFFD, 0, "ISO-8859-6", ZUECI_WARN_INVALID_DATA, 3 },
        /* 20*/ { 8, TU("\xA1"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "ISO-8859-6", 0, 2 },
        /* 21*/ { 9, TU("\xFF"), -1, 0, 0, "ISO-8859-7", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 22*/ { 9, TU("\xFF"), -1, 0xFFFD, 0, "ISO-8859-7", ZUECI_WARN_INVALID_DATA, 3 },
        /* 23*/ { 9, TU("\xFF"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "ISO-8859-7", 0, 2 },
        /* 24*/ { 10, TU("\xC0"), -1, 0, 0, "ISO-8859-8", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 25*/ { 10, TU("\xC0"), -1, 0xFFFD, 0, "ISO-8859-8", ZUECI_WARN_INVALID_DATA, 3 },
        /* 26*/ { 10, TU("\xC0"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "ISO-8859-8", 0, 2 },
        /* 27*/ { 11, TU("\x80"), -1, 0, 0, "ISO-8859-9", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 28*/ { 11, TU("\x80"), -1, 0xFFFD, 0, "ISO-8859-9", ZUECI_WARN_INVALID_DATA, 3 },
        /* 29*/ { 11, TU("\x80"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "ISO-8859-9", 0, 2 },
        /* 30*/ { 12, TU("\x80"), -1, 0, 0, "ISO-8859-10", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 31*/ { 12, TU("\x80"), -1, 0xFFFD, 0, "ISO-8859-10", ZUECI_WARN_INVALID_DATA, 3 },
        /* 32*/ { 12, TU("\x80"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "ISO-8859-10", 0, 2 },
        /* 33*/ { 13, TU("\xDB"), -1, 0, 0, "ISO-8859-11", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 34*/ { 13, TU("\xDB"), -1, 0xFFFD, 0, "ISO-8859-11", ZUECI_WARN_INVALID_DATA, 3 },
        /* 35*/ { 13, TU("\xDB"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "ISO-8859-11", 0, 2 },
        /* 36*/ { 15, TU("\x80"), -1, 0, 0, "ISO-8859-13", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 37*/ { 15, TU("\x80"), -1, 0xFFFD, 0, "ISO-8859-13", ZUECI_WARN_INVALID_DATA, 3 },
        /* 38*/ { 15, TU("\x80"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "ISO-8859-13", 0, 2 },
        /* 39*/ { 16, TU("\x80"), -1, 0, 0, "ISO-8859-14", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 40*/ { 16, TU("\x80"), -1, 0xFFFD, 0, "ISO-8859-14", ZUECI_WARN_INVALID_DATA, 3 },
        /* 41*/ { 16, TU("\x80"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "ISO-8859-14", 0, 2 },
        /* 42*/ { 17, TU("\x80"), -1, 0, 0, "ISO-8859-15", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 43*/ { 17, TU("\x80"), -1, 0xFFFD, 0, "ISO-8859-15", ZUECI_WARN_INVALID_DATA, 3 },
        /* 44*/ { 17, TU("\x80"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "ISO-8859-15", 0, 2 },
        /* 45*/ { 18, TU("\x80"), -1, 0, 0, "ISO-8859-16", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 46*/ { 18, TU("\x80"), -1, 0xFFFD, 0, "ISO-8859-16", ZUECI_WARN_INVALID_DATA, 3 },
        /* 47*/ { 18, TU("\x80"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "ISO-8859-16", 0, 2 },
        /* 48*/ { 21, TU("\x90"), -1, 0, 0, "CP1250", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 49*/ { 21, TU("\x90"), -1, 0xFFFD, 0, "CP1250", ZUECI_WARN_INVALID_DATA, 3 },
        /* 50*/ { 21, TU("\x90"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "CP1250", 0, 2 },
        /* 51*/ { 22, TU("\x98"), -1, 0, 0, "CP1251", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 52*/ { 22, TU("\x98"), -1, 0xFFFD, 0, "CP1251", ZUECI_WARN_INVALID_DATA, 3 },
        /* 53*/ { 22, TU("\x98"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "CP1251", 0, 2 },
        /* 54*/ { 23, TU("\x9D"), -1, 0, 0, "CP1252", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 55*/ { 23, TU("\x9D"), -1, 0xFFFD, 0, "CP1252", ZUECI_WARN_INVALID_DATA, 3 },
        /* 56*/ { 23, TU("\x9D"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "CP1252", 0, 2 },
        /* 57*/ { 25, TU("\xD8\000"), 2, 0, 0, "UTF-16BE", ZUECI_ERROR_INVALID_DATA, -1 }, /* U+D800 */
        /* 58*/ { 25, TU("\xD8\000"), 2, 0xFFFD, 0, "UTF-16BE", ZUECI_WARN_INVALID_DATA, 3 }, /* -> U+FFFD */
        /* 59*/ { 25, TU("\xD8\000"), 2, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "UTF-16BE", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 60*/ { 25, TU("\xD8\000\xDB\xFF"), 4, 0, 0, "UTF-16BE", ZUECI_ERROR_INVALID_DATA, -1 }, /* U+D800 U+DBFF (2 high surrogates) */
        /* 61*/ { 25, TU("\xD8\000\xDB\xFF"), 4, 0xFFFD, 0, "UTF-16BE", ZUECI_WARN_INVALID_DATA, 6 }, /* -> U+FFFD U+FFFD */
        /* 62*/ { 25, TU("\xD8\000\xDB\xFF"), 4, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "UTF-16BE", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 63*/ { 25, TU("\000\000\000"), 3, 0, 0, "UTF-16BE", ZUECI_ERROR_INVALID_DATA, -1 }, /* U+0000 0x00 */
        /* 64*/ { 25, TU("\000\000\000"), 3, 0, 0xFFFD, "UTF-16BE", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 65*/ { 25, TU("\000\000\000"), 3, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "UTF-16BE", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 66*/ { 25, TU("\xDC\000\xDB\xFF\xDC\000"), 6, 0, 0, "UTF-16BE", ZUECI_ERROR_INVALID_DATA, -1 }, /* U+DC00 U+DBFF U+DC00 */
        /* 67*/ { 25, TU("\xDC\000\xDB\xFF\xDC\000"), 6, 0xFFFD, 0, "UTF-16BE", ZUECI_WARN_INVALID_DATA, 7 }, /* -> U+FFFD U+10FC00 */
        /* 68*/ { 25, TU("\xDC\000\xDB\xFF\xDC\000"), 6, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "UTF-16BE", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 69*/ { 26, TU("\x80"), -1, 0, 0, "UTF-8", ZUECI_ERROR_INVALID_UTF8, -1 },
        /* 70*/ { 26, TU("\x80"), -1, 0xFFFD, 0, "UTF-8", ZUECI_WARN_INVALID_DATA, 3 }, /* -> U+FFFD */
        /* 71*/ { 26, TU("\x80"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "UTF-8", ZUECI_ERROR_INVALID_UTF8, -1 },
        /* 72*/ { 26, TU("\x80G"), -1, 0, 0, "UTF-8", ZUECI_ERROR_INVALID_UTF8, -1 }, /* 0x80 G */
        /* 73*/ { 26, TU("\x80G"), -1, 0xFFFD, 0, "UTF-8", ZUECI_WARN_INVALID_DATA, 4 }, /* -> U+FFFD G */
        /* 74*/ { 26, TU("\x80G"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "UTF-8", ZUECI_ERROR_INVALID_UTF8, -1 },
        /* 75*/ { 26, TU("\x80G\xC2H"), -1, 0, 0, "UTF-8", ZUECI_ERROR_INVALID_UTF8, -1 }, /* 0x80 G 0xC2 H */
        /* 76*/ { 26, TU("\x80G\xC2H"), -1, 0xFFFD, 0, "UTF-8", ZUECI_WARN_INVALID_DATA, 8 }, /* -> U+FFFD G U+FFFD H */
        /* 77*/ { 26, TU("\x80G\xC2H"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "UTF-8", ZUECI_ERROR_INVALID_UTF8, -1 },
        /* 78*/ { 26, TU("\x80G\xC2\x80H"), -1, 0, 0, "UTF-8", ZUECI_ERROR_INVALID_UTF8, -1 }, /* 0x80 G U+0080 H */
        /* 79*/ { 26, TU("\x80G\xC2\x80H"), -1, 0xFFFD, 0, "UTF-8", ZUECI_WARN_INVALID_DATA, 7 }, /* -> U+FFFD G U+0080 H */
        /* 80*/ { 26, TU("\x80G\xC2\x80H"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "UTF-8", ZUECI_ERROR_INVALID_UTF8, -1 },
        /* 81*/ { 26, TU("\x80G\xC2\x80\x80\xB0H"), -1, 0, 0, "UTF-8", ZUECI_ERROR_INVALID_UTF8, -1 }, /* 0x80 G U+0080 0x80 0xB0 H */
        /* 82*/ { 26, TU("\x80G\xC2\x80\x80\xB0H"), -1, 0xFFFD, 0, "UTF-8", ZUECI_WARN_INVALID_DATA, 10 }, /* -> U+FFFD G U+0080 U+FFFD H */
        /* 83*/ { 26, TU("\x80G\xC2\x80\x80\xB0H"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "UTF-8", ZUECI_ERROR_INVALID_UTF8, -1 },
        /* 84*/ { 26, TU("\x80G\xC2\x80\x80\xB0\xC2\x81H"), -1, 0, 0, "UTF-8", ZUECI_ERROR_INVALID_UTF8, -1 }, /* 0x80 G U+0080 0x80 0xB0 U+0081 H */
        /* 85*/ { 26, TU("\x80G\xC2\x80\x80\xB0\xC2\x81H"), -1, 0xFFFD, 0, "UTF-8", ZUECI_WARN_INVALID_DATA, 12 }, /* -> U+FFFD G U+0080 U+FFFD U+0081 H */
        /* 86*/ { 26, TU("\x80G\xC2\x80\x80\xB0\xC2\x81H"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "UTF-8", ZUECI_ERROR_INVALID_UTF8, -1 },
        /* 87*/ { 27, TU("\x80\x81\x82"), -1, 0, 0, "ASCII", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 88*/ { 27, TU("\x80\x81\x82"), -1, 0xFFFD, 0, "ASCII", ZUECI_WARN_INVALID_DATA, 9 }, /* -> U+FFFD U+FFFD U+FFFD */
        /* 89*/ { 27, TU("\x80\x81\x82"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "ASCII", 0, 6 }, /* -> U+0080 U+0081 U+0082 */
        /* 90*/ { 33, TU("\xFF\xD7\000\xD8\000\xC0\xFF\xDF"), 8, 0, 0, "UTF-16LE", ZUECI_ERROR_INVALID_DATA, -1 }, /* U+D7FF U+D800 U+C000 U+DFFF */
        /* 91*/ { 33, TU("\xFF\xD7\000\xD8\000\xC0\xFF\xDF"), 8, 0x00BF, 0, "UTF-16LE", ZUECI_WARN_INVALID_DATA, 10 },  /* -> U+D7FF U+00BF U+C000 U+00BF */
        /* 92*/ { 33, TU("\xFF\xD7\000\xD8\000\xC0\xFF\xDF"), 8, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "UTF-16LE", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 93*/ { 33, TU("\000"), 1, 0, 0, "UTF-16LE", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 94*/ { 33, TU("\000"), 1, 0xFFFD, 0, "UTF-16LE", ZUECI_WARN_INVALID_DATA, 3 }, /* -> U+FFFD */
        /* 95*/ { 33, TU("\000"), 1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "UTF-16LE", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 96*/ { 34, TU("\000\000\xD8\000"), 4, 0, 0, "UTF-32BE", ZUECI_ERROR_INVALID_DATA, -1 }, /* U+D800 */
        /* 97*/ { 34, TU("\000\000\xD8\000"), 4, 0xFFFD, 0, "UTF-32BE", ZUECI_WARN_INVALID_DATA, 3 }, /* -> U+FFFD */
        /* 98*/ { 34, TU("\000\000\xD8\000"), 4, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "UTF-32BE", ZUECI_ERROR_INVALID_DATA, -1 },
        /* 99*/ { 34, TU("\000\000"), 2, 0, 0, "UTF-32BE", ZUECI_ERROR_INVALID_DATA, -1 },
        /*100*/ { 34, TU("\000\000"), 2, 0xFFFD, 0, "UTF-32BE", ZUECI_WARN_INVALID_DATA, 3 }, /* -> U+FFFD */
        /*101*/ { 34, TU("\000\000"), 2, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "UTF-32BE", ZUECI_ERROR_INVALID_DATA, -1 },
        /*102*/ { 34, TU("\000\000\xD8\000\000\000\xD7\xFF"), 8, 0, 0, "UTF-32BE", ZUECI_ERROR_INVALID_DATA, -1 }, /* U+D800 U+D7FF */
        /*103*/ { 34, TU("\000\000\xD8\000\000\000\xD7\xFF"), 8, 0x00BF, 0, "UTF-32BE", ZUECI_WARN_INVALID_DATA, 5 }, /* -> U+00BF U+D7FF */
        /*104*/ { 34, TU("\000\000\xD8\000\000\000\xD7\xFF"), 8, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "UTF-32BE", ZUECI_ERROR_INVALID_DATA, -1 },
        /*105*/ { 35, TU("\000\xD8\000\000"), 4, 0, 0, "UTF-32LE", ZUECI_ERROR_INVALID_DATA, -1 }, /* U+D800 */
        /*106*/ { 35, TU("\000\xD8\000\000"), 4, '?', 0, "UTF-32LE", ZUECI_WARN_INVALID_DATA, 1 }, /* -> ? */
        /*107*/ { 35, TU("\000\xD8\000\000"), 4, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "UTF-32LE", ZUECI_ERROR_INVALID_DATA, -1 },
        /*108*/ { 35, TU("\000\000\000"), 3, 0, 0, "UTF-32LE", ZUECI_ERROR_INVALID_DATA, -1 },
        /*109*/ { 35, TU("\000\000\000"), 3, 0xFFFD, 0, "UTF-32LE", ZUECI_WARN_INVALID_DATA, 3 }, /* -> U+FFFD */
        /*110*/ { 35, TU("\000\000\000"), 3, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "UTF-32LE", ZUECI_ERROR_INVALID_DATA, -1 },
        /*111*/ { 35, TU("\xFF\xFF\x01\000\000\xD8\000\000"), 8, 0, 0, "UTF-32LE", ZUECI_ERROR_INVALID_DATA, -1 }, /* U+1FFFF U+D800 */
        /*112*/ { 35, TU("\xFF\xFF\x01\000\000\xD8\000\000"), 8, 0xFFDD, 0, "UTF-32LE", ZUECI_WARN_INVALID_DATA, 7 }, /* -> U+1FFFF U+FFDD */
        /*113*/ { 35, TU("\xFF\xFF\x01\000\000\xD8\000\000"), 8, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "UTF-32LE", ZUECI_ERROR_INVALID_DATA, -1 },
        /*114*/ { 170, TU("#"), -1, 0, 0, "", ZUECI_ERROR_INVALID_DATA, -1 },
        /*115*/ { 170, TU("#"), -1, 0xFFFD, 0, "", ZUECI_WARN_INVALID_DATA, 3 }, /* -> U+FFFD */
        /*116*/ { 170, TU("#"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "", 0, 1 }, /* -> # */
        /*117*/ { 170, TU("A#B$C@D"), -1, 0, 0, "", ZUECI_ERROR_INVALID_DATA, -1 },
        /*118*/ { 170, TU("A#B$C@D"), -1, 0xFFFD, 0, "", ZUECI_WARN_INVALID_DATA, 13 }, /* -> A U+FFFD B U+FFFD C U+FFFD D */
        /*119*/ { 170, TU("A#B$C@D"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, "", 0, 7 }, /* -> A # B $ C @ D */
        /*120*/ { 899, TU("\x80\xFF"), -1, 0, 0, NULL, 0, 4 },
        /*121*/ { 899, TU("\x80\xFF"), -1, 0, ZUECI_FLAG_SB_STRAIGHT_THRU, NULL, 0, 4 },
    };
    int data_size = ZUECI_ASIZE(data);
    int i, src_len, ret;

    unsigned char dest[1024];

    testStart("test_replacement_char");

    for (i = 0; i < data_size; i++) {
        int dest_len = 0, utf8_dest_len = 0;

        if (testContinue(p_ctx, i)) continue;

        /* zueci_dest_len_utf8() */

        src_len = data[i].length != -1 ? data[i].length : (int) ustrlen(data[i].data);
        ret = zueci_dest_len_utf8(data[i].eci, data[i].data, src_len, data[i].replacement_char, data[i].flags, &utf8_dest_len);
        assert_equal(ret, data[i].ret, "i:%d zueci_dest_len_utf8 ret %d != %d\n", i, ret, data[i].ret);
        if (ret < ZUECI_ERROR) {
            assert_nonzero(utf8_dest_len + 1 <= ZUECI_ASIZE(dest),
                "i:%d utf8_dest_len %d + 1 > %d\n", i, utf8_dest_len, ZUECI_ASIZE(dest));
        }

        /* zueci_eci_to_utf8() */

        ret = zueci_eci_to_utf8(data[i].eci, data[i].data, src_len, data[i].replacement_char, data[i].flags, dest, &dest_len);
        assert_equal(ret, data[i].ret, "i:%d zueci_eci_to_utf8 ret %d != %d\n", i, ret, data[i].ret);
        if (ret < ZUECI_ERROR) {
            assert_equal(dest_len, data[i].expected_length, "i:%d dest_len %d != %d, dest %.*s\n",
                i, dest_len, data[i].expected_length, dest_len, dest);
            assert_equal(dest_len, utf8_dest_len,  "i:%d dest_len %d != utf8_dest_len %d\n",
                i, dest_len, utf8_dest_len);
        }
    }

    testFinish();
}
#endif /* ZUECI_EMBED_NO_TO_UTF8 */

int main(int argc, char *argv[]) {

    testFunction funcs[] = { /* name, func */
        { "test_check_args", test_check_args },
        { "test_sb", test_sb },
#ifndef ZUECI_EMBED_NO_TO_ECI
        { "test_ascii", test_ascii },
        { "test_utf16be", test_utf16be },
        { "test_utf16le", test_utf16le },
        { "test_utf32be", test_utf32be },
        { "test_utf32le", test_utf32le },
#endif
#ifndef ZUECI_EMBED_NO_TO_UTF8
        { "test_replacement_char_flags", test_replacement_char_flags },
#endif
    };

    testRun(argc, argv, funcs, ZUECI_ASIZE(funcs));

    testReport();

    return 0;
}

/* vim: set ts=4 sw=4 et norl : */
