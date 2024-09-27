/* Test ECI 32 GB 18030 Chinese */
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

#ifndef ZUECI_EMBED_NO_TO_ECI
/* Test zueci_utf8_to_eci() & zueci_dest_len_eci() */
static void test_utf8_to_eci(const testCtx *const p_ctx) {

    struct item {
        unsigned char *data;
        int length;
        int ret;
        int expected_length;
    };
    /* s/\/\*[ 0-9]*\*\//\=printf("\/\*%3d*\/", line(".") - line("'<")): */
    static const struct item data[] = {
        /*  0*/ { TU("\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"), 32, 0, 32 },
        /*  1*/ { TU(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\x7F"), 96, 0, 96 },
        /*  2*/ { TU("\xC2\x80"), -1, 0, 4 }, /* U+0080 -> 0x81308130 */
        /*  3*/ { TU("\xE3\x80\x80"), -1, 0, 2 }, /* U+3000 IDEOGRAPHIC SPACE */
        /*  4*/ { TU("\xED\xA0\x80"), -1, ZUECI_ERROR_INVALID_UTF8, -1 }, /* U+D800 high surrogate invalid UTF-8 */
        /*  5*/ { TU("\xED\xBF\xBF"), -1, ZUECI_ERROR_INVALID_UTF8, -1 }, /* U+DFFF low surrogate invalid UTF-8 */
        /*  6*/ { TU("\xEF\xBF\xBE"), -1, 0, 4 }, /* U+FFFE (reversed BOM) -> 0x8431A438 */
        /*  7*/ { TU("\xEF\xBF\xBF"), -1, 0, 4 }, /* U+FFFF -> 0x8431A439 */
        /*  8*/ { TU("\xC2\xA4"), -1, 0, 2 }, /* U+00A4 CURRENCY SIGN */
        /*  9*/ { TU("\xE9\xBD\x84"), -1, 0, 2 }, /* U+9F44 Han */
        /* 10*/ { TU("\xE9\xBD\x85"), -1, 0, 2 }, /* U+9F45 Han */
        /* 11*/ { TU("\xEF\xA8\xA9"), -1, 0, 2 }, /* U+FA29 CJK */
        /* 12*/ { TU("\xEF\xA8\xAA"), -1, 0, 4 }, /* U+FA2A CJK -> 0x84309C38 */
        /* 13*/ { TU("A\xC2\xA4\xE9\xBD\x84\xE9\xBD\x85~\xE3\x80\x80\x7F\xC2\xA7\xEF\xA8\xA9\xEF\xA8\xAA"), -1, 0, 19 }, /* Multiple (good) mappings */
        /* 14*/ { TU("\xE2\x82\xAC"), -1, 0, 2 }, /* U+20AC EURO SIGN */
    };
    int data_size = ZUECI_ASIZE(data);
    int i, src_len, ret;
    const int eci = 32;

    unsigned char dest[1024];
    unsigned char dest2[1024];
#ifdef ZUECI_TEST_ICONV
    iconv_t cd_from_u, cd_to_u;
#endif

    testStart("test_utf8_to_eci");

#ifdef ZUECI_TEST_ICONV
    cd_from_u = iconv_open("GB18030" /*to*/, "UTF-8" /*from*/);
    assert_zero(cd_from_u == (iconv_t) -1, "iconv_open cd_from_u == -1\n");
    cd_to_u = iconv_open("UTF-8" /*to*/, "GB18030" /*from*/);
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
            assert_zero(memcmp(dest2, data[i].data, src_len),
                "i:%d memcmp(dest2, data[i].data, %d) != 0\n", i, src_len);

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

#ifndef ZUECI_EMBED_NO_TO_UTF8
/* Test zueci_eci_to_utf8() & zueci_dest_len_utf8() */
static void test_eci_to_utf8(const testCtx *const p_ctx) {

    struct item {
        unsigned char *data;
        int length;
        unsigned int replacement_char;
        unsigned int flags;
        int ret;
        int expected_length;
    };
    /* s/\/\*[ 0-9]*\*\//\=printf("\/\*%3d*\/", line(".") - line("'<")): */
    static const struct item data[] = {
        /*  0*/ { TU("\xA1\xA7"), -1, 0, 0, 0, 2 }, /* 0xA1A7 -> U+00A8 */
        /*  1*/ { TU("\xE1\xC0"), -1, 0, 0, 0, 3 }, /* 0xE1C0 -> U+5D02 */
        /*  2*/ { TU("\xA4\xF3"), -1, 0, 0, 0, 3 }, /* 0xA4F3 -> U+3093 */
        /*  3*/ { TU("\xA4\xF4"), -1, 0, 0, 0, 3 }, /* 0xA4F4 -> U+E772 */
        /*  4*/ { TU("\xA4\xFE"), -1, 0, 0, 0, 3 }, /* 0xA4FE -> U+3013 */
        /*  5*/ { TU("\xD2\xBB"), -1, 0, 0, 0, 3 }, /* 0xD2BB -> U+4E00 */
        /*  6*/ { TU("\xFD\x9B"), -1, 0, 0, 0, 3 }, /* 0xFD9B -> U+9FA5 */
        /*  7*/ { TU("\x82\x35\x8F\x33"), -1, 0, 0, 0, 3 }, /* 0x82358F33 -> U+9FA6 */
        /*  8*/ { TU("\x82\x35\x98\x32"), -1, 0, 0, 0, 3 }, /* 0x82359832 -> U+9FFF */
        /*  9*/ { TU("\xA1\x7F"), -1, 0, 0, ZUECI_ERROR_INVALID_DATA, -1 }, /* 0xA17F invalid trailing byte */
        /* 10*/ { TU("\xA1\xFF"), -1, 0, 0, ZUECI_ERROR_INVALID_DATA, -1 }, /* 0xA1FF invalid trailing byte */
        /* 11*/ { TU("\xA1"), -1, 0, 0, ZUECI_ERROR_INVALID_DATA, -1 }, /* 0xA1 missing trailing byte */
        /* 12*/ { TU("\xA1"), -1, 0x00BF, 0, ZUECI_WARN_INVALID_DATA, 2 }, /* -> U+00BF */
        /* 13*/ { TU("\xA1\x3F"), -1, 0x00BF, 0, ZUECI_WARN_INVALID_DATA, 2 }, /* -> U+00BF (invalid trailing byte) */
        /* 14*/ { TU("\xA1\x3FG"), -1, 0x00BF, 0, ZUECI_WARN_INVALID_DATA, 3 }, /* -> U+00BF G */
        /* 15*/ { TU("\x81\x30\x81"), -1, 0, 0, ZUECI_ERROR_INVALID_DATA, -1 }, /* 4-byter missing trailing byte */
        /* 16*/ { TU("\x81\x30\x81"), -1, 0xFFFD, 0, ZUECI_WARN_INVALID_DATA, 6 }, /* -> U+FFFD U+FFFD (treated as 2-byter + 2-byter missing trailing byte) */
        /* 17*/ { TU("\x81\x30\x81G"), -1, 0xFFFD, 0, ZUECI_WARN_INVALID_DATA, 3 }, /* -> U+FFFD (treated as 4-byter) */
        /* 18*/ { TU("\x81\x30\x81GH"), -1, 0xFFFD, 0, ZUECI_WARN_INVALID_DATA, 4 }, /* -> U+FFFD H */
        /* 19*/ { TU("\x81\x30"), -1, 0xFFFD, 0, ZUECI_WARN_INVALID_DATA, 3 }, /* -> U+FFFD */
        /* 20*/ { TU("\x81\x30G"), -1, 0xFFFD, 0, ZUECI_WARN_INVALID_DATA, 4 }, /* -> U+FFFD G (treated as 2-byter) */
        /* 21*/ { TU("\x81\x30GH"), -1, 0xFFFD, 0, ZUECI_WARN_INVALID_DATA, 3 }, /* -> U+FFFD (treated as 4-byter) */
        /* 22*/ { TU("\x81\x30GHI"), -1, 0xFFFD, 0, ZUECI_WARN_INVALID_DATA, 4 }, /* -> U+FFFD I (treated as 4-byter) */
        /* 23*/ { TU("\x81"), -1, 0xFFFD, 0, ZUECI_WARN_INVALID_DATA, 3 }, /* -> U+FFFD */
        /* 24*/ { TU("\x81\x2F"), -1, 0xFFFD, 0, ZUECI_WARN_INVALID_DATA, 3 }, /* -> U+FFFD */
        /* 25*/ { TU("\x81\x2FG"), -1, 0xFFFD, 0, ZUECI_WARN_INVALID_DATA, 4 }, /* -> U+FFFD G (treated as 2-byter) */
        /* 26*/ { TU("\x80\x40"), -1, 0, 0, ZUECI_ERROR_INVALID_DATA, -1 }, /* 0x80 invalid leading byte */
        /* 27*/ { TU("\x80\x40"), -1, 0x00BF, 0, ZUECI_WARN_INVALID_DATA, 2 }, /* -> U+00BF */
        /* 28*/ { TU("\x80\x40G"), -1, 0x00BF, 0, ZUECI_WARN_INVALID_DATA, 3 }, /* -> U+00BF G */
        /* 29*/ { TU("\x00"), 1, 0, 0, 0, 1 }, /* NUL */
        /* 30*/ { TU("\x81\x30\x81\x30"), -1, 0, 0, 0, 2 }, /* U+0080 */
        /* 31*/ { TU("\xE3\x32\x9A\x35"), -1, 0, 0, 0, 4 }, /* U+10FFFF */
        /* 32*/ { TU("\xE3\x32\x9A\x36"), -1, 0, 0, ZUECI_ERROR_INVALID_DATA, -1 }, /* Unmapped */
        /* 33*/ { TU("\xE3\x32\x9A\x36"), -1, '?', 0, ZUECI_WARN_INVALID_DATA, 1 }, /* -> ? */
        /* 34*/ { TU("A\xA1\xA7\xE1\xC0;\xA4\xF3\x7F"), -1, 0, 0, 0, 11 }, /* -> A U+00AE U+5D02 ; U+3093 <DEL> */
        /* 35*/ { TU("\xA4\xFF~\xA1\xA7\xE1\xC0;\xA4\xF3\xA4\x7F\x7F"), -1, 0xFFFD, 0, ZUECI_WARN_INVALID_DATA, 17 }, /* -> U+FFFD ~ U+00AE U+5D02 ; U+3093 U+FFFD <DEL> */
        /* 36*/ { TU("A\xE3\x32\x9A\x36\xA1\xA7G"), -1, 0x00BF, 0, ZUECI_WARN_INVALID_DATA, 6 }, /* -> A U+00BF U+00A8 G */
        /* 37*/ { TU("\xA2\xE3"), -1, 0, 0, 0, 3 }, /* U+20AC (EURO SIGN) */
    };
    int data_size = ZUECI_ASIZE(data);
    int i, src_len, ret;
    const int eci = 32;

    unsigned char dest[1024];
    unsigned char dest2[1024];
#ifdef ZUECI_TEST_ICONV
    iconv_t cd_to_u;
#endif

    testStart("test_eci_to_utf8");

#ifdef ZUECI_TEST_ICONV
    cd_to_u = iconv_open("UTF-8" /*to*/, "GB18030" /*from*/);
    assert_zero(cd_to_u == (iconv_t) -1, "iconv_open cd_to_u == -1\n");
#endif

    for (i = 0; i < data_size; i++) {
        int dest_len = 0, utf8_dest_len = 0;
        int dest2_len = 0, eci_dest_len = 0;

        if (testContinue(p_ctx, i)) continue;

        /* zueci_dest_len_utf8() */

        src_len = data[i].length != -1 ? data[i].length : (int) ustrlen(data[i].data);
        ret = zueci_dest_len_utf8(eci, data[i].data, src_len, data[i].replacement_char, data[i].flags, &utf8_dest_len);
        assert_equal(ret, data[i].ret, "i:%d zueci_dest_len_utf8 ret %d != %d\n", i, ret, data[i].ret);
        if (ret < ZUECI_ERROR) {
            assert_nonzero(utf8_dest_len + 1 <= ZUECI_ASIZE(dest),
                "i:%d utf8_dest_len %d + 1 > %d\n", i, utf8_dest_len, ZUECI_ASIZE(dest));
        }

        /* zueci_eci_to_utf8() */

        ret = zueci_eci_to_utf8(eci, data[i].data, src_len, data[i].replacement_char, data[i].flags, dest, &dest_len);
        assert_equal(ret, data[i].ret, "i:%d zueci_eci_to_utf8 ret %d != %d\n", i, ret, data[i].ret);
        if (ret < ZUECI_ERROR) {
            assert_equal(dest_len, data[i].expected_length, "i:%d dest_len %d != %d (%.*s)\n",
                i, dest_len, data[i].expected_length, dest_len, dest);
            assert_nonzero(dest_len == utf8_dest_len, "i:%d dest_len %d != utf8_dest_len %d\n",
                i, dest_len, utf8_dest_len);

            if (ret == 0) {
#ifdef ZUECI_TEST_ICONV
                {
                    char out_to_u_buf[1024] = {0};
                    char *in_to_u = (char *) data[i].data, *out_to_u = out_to_u_buf;
                    size_t in_to_u_bytes_left = (size_t) src_len;
                    size_t out_to_u_bytes_left = ZUECI_ASIZE(out_to_u_buf);

                    while (in_to_u_bytes_left) {
                        size_t to_u_ret = iconv(cd_to_u, &in_to_u, &in_to_u_bytes_left,
                            &out_to_u, &out_to_u_bytes_left);
                        assert_zero(to_u_ret, "i:%d to_u_ret %d != 0 (%d: %s)\n",
                            i, (int) to_u_ret, errno, strerror(errno));
                    }
                    assert_equal(data[i].expected_length + (int) out_to_u_bytes_left, ZUECI_ASIZE(out_to_u_buf),
                        "i:%d data[i].expected_length %d + out_to_u_bytes_left %d != %d\n",
                        i, data[i].expected_length, (int) out_to_u_bytes_left, ZUECI_ASIZE(out_to_u_buf));
                    assert_zero(in_to_u_bytes_left, "i:%d in_to_u_bytes_left %d != 0\n",
                        i, (int) in_to_u_bytes_left);
                    assert_zero(memcmp(out_to_u_buf, dest, dest_len),
                        "i:%d memcmp(out_to_u_buf, dest, %d) != 0\n", i, dest_len);
                }
#endif

#ifndef ZUECI_EMBED_NO_TO_ECI
                /* Check round-trip */

                ret = zueci_dest_len_eci(eci, dest, dest_len, &eci_dest_len);
                assert_zero(ret, "i:%d zueci_dest_len_eci ret %d != 0\n", i, ret);
                assert_nonzero(eci_dest_len + 1 <= ZUECI_ASIZE(dest2), "i:%d eci_dest_len %d + 1 > %d\n",
                    i, eci_dest_len, ZUECI_ASIZE(dest2));

                ret = zueci_utf8_to_eci(eci, dest, dest_len, dest2, &dest2_len);
                assert_equal(ret, data[i].ret, "i:%d zueci_utf8_to_eci ret %d != %d\n", i, ret, data[i].ret);
                assert_equal(dest2_len, src_len, "i:%d dest2_len %d != src_len %d\n", i, dest2_len, src_len);
                assert_zero(memcmp(dest2, data[i].data, src_len),
                    "i:%d memcmp(dest2, data[i].data, %d) != 0\n", i, src_len);

                assert_nonzero(dest2_len <= eci_dest_len, "i:%d dest2_len %d > eci_dest_len %d\n",
                    i, dest2_len, eci_dest_len);
#else
                (void)dest2; (void)dest2_len; (void)eci_dest_len;
#endif
            }
        }
    }

#ifdef ZUECI_TEST_ICONV
    assert_zero(iconv_close(cd_to_u), "iconv_close cd_to_u fail\n");
#endif

    testFinish();
}
#endif /* ZUECI_EMBED_NO_TO_UTF8 */

#ifndef ZUECI_EMBED_NO_TO_ECI
#include "test_gb18030_tab.h"

/* As control convert to GB 18030 using table generated from GB18030.TXT plus simple processing.
   The version of GB18030.TXT is jdk-1.4.2/GB18030.TXT taken from
   https://haible.de/bruno/charsets/conversion-tables/GB18030.html
   The generated file tests/test_gb18030_tab.h does not include U+10000..10FFFF codepoints to save space.
   See also tools/data/GB18030.TXT.README and tests/tools/gen_test_tab_h.php. */
static int u_gb18030_2(zueci_u32 u, unsigned char *dest) {
    int tab_length, start_i, end_i;
    int i;
    zueci_u32 mb;

    /* GB18030 two-byte extension */
    if (u == 0x1E3F) { /* GB 18030-2005 change, was PUA U+E7C7 below, see Table 3-39, p.111, Lunde 2nd ed. */
        dest[0] = 0xA8;
        dest[1] = 0xBC;
        return 2;
    }
    /* GB18030 four-byte extension */
    if (u == 0xE7C7) { /* PUA */
        dest[0] = 0x81;
        dest[1] = 0x35;
        dest[2] = 0xF4;
        dest[3] = 0x37;
        return 4;
    }
    /* GB18030 two-byte extension */
    if (u >= 0x9FB4 && u <= 0x9FBB) { /* GB 18030-2005 change, were PUA, see Table 3-37, p.108, Lunde 2nd ed. */
        if (u == 0x9FB4) {
            mb = 0xFE59;
        } else if (u == 0x9FB5) {
            mb = 0xFE61;
        } else if (u == 0x9FB6 || u == 0x9FB7) {
            mb = 0xFE66 + (u - 0x9FB6);
        } else if (u == 0x9FB8) {
            mb = 0xFE6D;
        } else if (u == 0x9FB9) {
            mb = 0xFE7E;
        } else if (u == 0x9FBA) {
            mb = 0xFE90;
        } else {
            mb = 0xFEA0;
        }
        dest[0] = (unsigned char) (mb >> 8);
        dest[1] = (unsigned char) mb;
        return 2;
    }
    /* GB18030 two-byte extension */
    if (u >= 0xFE10 && u <= 0xFE19) { /* GB 18030-2005 change, were PUA, see Table 3-37, p.108, Lunde 2nd ed. */
        if (u == 0xFE10) {
            mb = 0xA6D9;
        } else if (u == 0xFE11) {
            mb = 0xA6DB;
        } else if (u == 0xFE12) {
            mb = 0xA6DA;
        } else if (u >= 0xFE13 && u <= 0xFE16) {
            mb = 0xA6DC + (u - 0xFE13);
        } else if (u == 0xFE17 || u == 0xFE18) {
            mb = 0xA6EC + (u - 0xFE17);
        } else {
            mb = 0xA6F3;
        }
        dest[0] = (unsigned char) (mb >> 8);
        dest[1] = (unsigned char) mb;
        return 2;
    }
    /* GB18030 four-byte extension */
    /* These are Vertical Forms (U+FE1A..FE1F unassigned) and Combining Half Marks (U+FE20..FE2F) */
    if (u >= 0xFE1A && u <= 0xFE2F) {
        if (u >= 0xFE1A && u <= 0xFE1D) {
            mb = 0x84318336 + (u - 0xFE1A);
        } else if (u >= 0xFE1E && u <= 0xFE27) {
            mb = 0x84318430 + (u - 0xFE1E);
        } else {
            mb = 0x84318530 + (u - 0xFE28);
        }
        dest[0] = (unsigned char) (mb >> 24);
        dest[1] = (unsigned char) (mb >> 16);
        dest[2] = (unsigned char) (mb >> 8);
        dest[3] = (unsigned char) mb;
        return 4;
    }
    /* GB18030 */
    /* Code set 3 (Unicode U+10000..U+10FFFF) */
    if (u >= 0x10000 /*&& u < 0x10400*/) { /* Not being called for U+10400..U+10FFFF */
        mb = u - 0x10000;
        dest[0] = 0x90;
        dest[1] = 0x30;
        mb = 0x8130 + (mb % 10) + 0x100 * (mb / 10);
        dest[2] = (unsigned char) (mb >> 8);
        dest[3] = (unsigned char) mb;
        return 4;
    }

    tab_length = ZUECI_ASIZE(test_gb18030_tab);
    start_i = test_gb18030_tab_ind[u >> 10];
    end_i = start_i + 0x800 > tab_length ? tab_length : start_i + 0x800;
    for (i = start_i; i < end_i; i += 2) {
        if (test_gb18030_tab[i + 1] == u) {
            mb = test_gb18030_tab[i];
            if (mb <= 0xFFFF) {
                if (mb & 0xFF00) {
                    dest[0] = (unsigned char) (mb >> 8);
                    dest[1] = (unsigned char) mb;
                    return 2;
                }
                *dest = (unsigned char) mb;
                return 1;
            }
            dest[0] = (unsigned char) (mb >> 24);
            dest[1] = (unsigned char) (mb >> 16);
            dest[2] = (unsigned char) (mb >> 8);
            dest[3] = (unsigned char) mb;
            return 4;
        }
    }
    return 0;
}

ZUECI_INTERN int zueci_u_gb18030_test(const zueci_u32 u, unsigned char *dest);
#ifndef ZUECI_EMBED_NO_TO_UTF8
ZUECI_INTERN int zueci_gb18030_u_test(const unsigned char *src, const zueci_u32 len, const unsigned int flags,
                    zueci_u32 *p_u);
#endif

/* Test all Unicode codepoints (and all GB18030) */
static void test_u_gamut(const testCtx *const p_ctx) {

    int ret, ret2, ret3;
    unsigned char val[4];
    unsigned char val2[4];
    zueci_u32 u3;
    zueci_u32 i;
#ifdef ZUECI_TEST_ICONV
    iconv_t cd_from_u;
#endif

    testStart("test_u_gamut");

#ifdef ZUECI_TEST_ICONV
    cd_from_u = iconv_open("GB18030" /*to*/, "UTF-8" /*from*/);
    assert_zero(cd_from_u == (iconv_t) -1, "iconv_open cd_from_u == -1\n");
#endif

    for (i = 0; i < 0x10400; i++) { /* Deal with U+10400..U+10FFFF below, programmatically filled */
        if (i == 0xD800) { /* Skip UTF-16 surrogates */
            i = 0xE000;
        }
        if (testContinue(p_ctx, i)) continue;

        /* zueci_u_gb18030() */

        memset(val, 0, ZUECI_ASIZE(val));
        memset(val2, 0, ZUECI_ASIZE(val2));
        ret = zueci_u_gb18030_test(i, val);
        assert_nonzero(ret, "i:%d 0x%04X ret %d == 0\n", (int) i, i, ret);
        ret2 = u_gb18030_2(i, val2);
        assert_equal(ret, ret2, "i:%d 0x%04X ret %d != ret2 %d, val 0x%02X%02X, val2 0x%02X%02X\n",
            (int) i, i, ret, ret2, val[0], val[1], val2[0], val2[1]);

        assert_equal(val[0], val2[0],
            "i:%d 0x%04X val[0] 0x%02X != val2[0] 0x%02X, val[1] 0x%02X, val2[1] 0x%02X\n",
            (int) i, i, val[0], val2[0], val[1], val2[1]);
        assert_equal(val[1], val2[1], "i:%d 0x%04X val[1] 0x%02X != val2[1] 0x%02X\n",
            (int) i, i, val[1], val2[1]);
        assert_equal(val[2], val2[2],
            "i:%d 0x%04X val[2] 0x%02X != val2[2] 0x%02X, val[3] 0x%2X, val2[3] 0x%02X\n",
            (int) i, i, val[2], val2[2], val[3], val2[3]);
        assert_equal(val[3], val2[3], "i:%d 0x%04X val[3] 0x%02X != val2[3] 0x%02X\n",
            (int) i, i, val[3], val2[3]);

#ifndef ZUECI_EMBED_NO_TO_UTF8
        /* Check round-trip */

        u3 = 0;
        ret3 = zueci_gb18030_u_test(val, ret, 0, &u3);
        assert_equal(ret3, ret2, "i:%d 0x%04X val 0x%02X%02X, ret3 %d != ret2 %d\n",
            (int) i, i, val[0], val[1], ret3, ret2);
        /* 24 PUA that are now non-PUA (Table 3-37, p.109, Lunde 2nd ed.) so don't round-trip */
        if ((i >= 0xE78D && i <= 0xE796) || (i >= 0xE816 && i <= 0xE818) || i == 0xE81E || i == 0xE826
                || i == 0xE82B || i == 0xE82C || i == 0xE831 || i == 0xE832 || i == 0xE83B || i == 0xE843
                || i == 0xE854 || i == 0xE855 || i == 0xE864) {
        } else {
            assert_equal(i, u3, "i:%d 0x%04X != u3 0x%04X, val 0x%02X%02X%02X%02X\n",
                (int) i, i, u3, val[0], val[1], val[2], val[3]);
        }
#else
            (void)ret3; (void)u3;
#endif

#ifdef ZUECI_TEST_ICONV
        {
            char in_from_u_buf[5];
            char out_from_u_buf[5] = {0};
            char *in_from_u = in_from_u_buf, *out_from_u = out_from_u_buf;
            size_t out_from_u_bytes_left = ZUECI_ASIZE(out_from_u_buf);
            size_t in_from_u_bytes_left = testConvertUnicodeToUtf8(i, (unsigned char *) in_from_u_buf);

            while (in_from_u_bytes_left) {
                size_t from_u_ret = iconv(cd_from_u, &in_from_u, &in_from_u_bytes_left,
                    &out_from_u, &out_from_u_bytes_left);
                assert_zero(from_u_ret, "i:%d 0x%04X from_u_ret %d != 0 (%d: %s)\n",
                    i, i, (int) from_u_ret, errno, strerror(errno));
            }
            assert_equal(ret + (int) out_from_u_bytes_left, ZUECI_ASIZE(out_from_u_buf),
                "i:%d ret %d + out_from_u_bytes_left %d != %d\n",
                i, ret, (int) out_from_u_bytes_left, ZUECI_ASIZE(out_from_u_buf));
            assert_zero(in_from_u_bytes_left, "i:%d in_from_u_bytes_left %d != 0\n",
                i, (int) in_from_u_bytes_left);
            assert_zero(memcmp(out_from_u_buf, val, ret),
                "i:%d memcmp(out_from_u_buf, val, %d) != 0\n", i, ret);
        }
#endif
    }

    for (i = 0x10400; i < 0x10FFFF; i++) { /* U+10400..U+10FFFF, check round-trip */
        if (testContinue(p_ctx, i)) continue;

        memset(val, 0, ZUECI_ASIZE(val));
        ret = zueci_u_gb18030_test(i, val);
        assert_nonzero(ret == 2 || ret == 4, "i:%d 0x%04X ret %d != 2 && != 4, val 0x%02X%02X%02X%02X\n",
            (int) i, i, ret, val[0], val[1], val[2], val[3]);

#ifndef ZUECI_EMBED_NO_TO_UTF8
        /* Check round-trip */

        u3 = 0;
        ret3 = zueci_gb18030_u_test(val, ret, 0, &u3);
        assert_nonzero(ret3 == 2 || ret3 == 4, "i:%d 0x%04X ret3 %d != 2 && != 4, u3 0x%04X, val 0x%02X%02X%02X%02X\n",
            (int) i, i, ret3, u3, val[0], val[1], val[2], val[3]);
        assert_equal(i, u3, "i:%d 0x%04X != u3 0x%04X, val 0x%02X%02X%02X%02X\n",
            (int) i, i, u3, val[0], val[1], val[2], val[3]);
#endif

#ifdef ZUECI_TEST_ICONV
        {
            char in_from_u_buf[5];
            char out_from_u_buf[5] = {0};
            char *in_from_u = in_from_u_buf, *out_from_u = out_from_u_buf;
            size_t out_from_u_bytes_left = ZUECI_ASIZE(out_from_u_buf);
            size_t in_from_u_bytes_left = testConvertUnicodeToUtf8(i, (unsigned char *) in_from_u_buf);

            while (in_from_u_bytes_left) {
                size_t from_u_ret = iconv(cd_from_u, &in_from_u, &in_from_u_bytes_left,
                    &out_from_u, &out_from_u_bytes_left);
                assert_zero(from_u_ret, "i:%d 0x%04X from_u_ret %d != 0 (%d: %s)\n",
                    i, i, (int) from_u_ret, errno, strerror(errno));
            }
            assert_equal(ret + (int) out_from_u_bytes_left, ZUECI_ASIZE(out_from_u_buf),
                "i:%d ret %d + out_from_u_bytes_left %d != %d\n",
                i, ret, (int) out_from_u_bytes_left, ZUECI_ASIZE(out_from_u_buf));
            assert_zero(in_from_u_bytes_left, "i:%d in_from_u_bytes_left %d != 0\n",
                i, (int) in_from_u_bytes_left);
            assert_zero(memcmp(out_from_u_buf, val, ret),
                "i:%d memcmp(out_from_u_buf, val, %d) != 0\n", i, ret);
        }
#endif
    }

#ifdef ZUECI_TEST_ICONV
    assert_zero(iconv_close(cd_from_u), "iconv_close cd_from_u fail\n");
#endif

    testFinish();
}
#endif /* ZUECI_EMBED_NO_TO_ECI */

#ifndef ZUECI_EMBED_NO_TO_UTF8
ZUECI_INTERN int zueci_gb18030_u_test(const unsigned char *src, const zueci_u32 len, const unsigned int flags,
                    zueci_u32 *p_u);

/* Test all GB 18030 codepoints */
static void test_gb18030_gamut(const testCtx *const p_ctx) {

    int ret;
    unsigned char src[4];
    zueci_u32 u;
    int i, j, k, l;
#ifdef ZUECI_TEST_ICONV
    iconv_t cd_to_u;
#endif

    testStart("test_gb18030_gamut");

#ifdef ZUECI_TEST_ICONV
    cd_to_u = iconv_open("UTF-8" /*to*/, "GB18030" /*from*/);
    assert_zero(cd_to_u == (iconv_t) -1, "iconv_open cd_to_u == -1\n");
#endif

    /* 2-byters */
    for (i = 0x81; i < 0xFF; i++) {
        zueci_u16 mb2;
        src[0] = (unsigned char) i;

        for (j = 0x40; j < 0xFF; j++) {
            src[1] = (unsigned char) j;
            mb2 = (src[0] << 8) | src[1];

            if (testContinue(p_ctx, mb2)) continue;

            ret = zueci_gb18030_u_test(src, 2, 0, &u);
            if (ret) {
#ifndef ZUECI_EMBED_NO_TO_ECI
                unsigned char dest[4] = {0};
                int ret2 = zueci_u_gb18030_test(u, dest);
                assert_equal(ret2, ret, "i:%d j:%d mb2 0x%04X ret2 %d != ret %d\n", i, j, mb2, ret2, ret);
                assert_nonzero(ret <= 2, "i:%d j:%d mb2 0x%04X ret %d > 2\n", i, j, mb2, ret);
                assert_zero(memcmp(dest, src, ret), "i:%d j:%d mb2 0x%04X memcmp(dest, src, %d) != 0\n",
                    i, j, mb2, ret);
#endif
            } else if (j != 0x7F) {
                assert_zero(ret, "i:%d j:%d mb2 0x%04X ret %d != 0\n", i, j, mb2, ret);
            }

#ifdef ZUECI_TEST_ICONV
            {
                char out_to_u_buf[5] = {0};
                char *in_to_u = (char *) src, *out_to_u = out_to_u_buf;
                size_t out_to_u_bytes_left = ZUECI_ASIZE(out_to_u_buf);

                if (ret) {
                    size_t in_to_u_bytes_left = ret;
                    unsigned char utf8_buf[5] = {0};
                    int utf8_len = testConvertUnicodeToUtf8(u, utf8_buf);

                    while (in_to_u_bytes_left) {
                        size_t to_u_ret = iconv(cd_to_u, &in_to_u, &in_to_u_bytes_left,
                            &out_to_u, &out_to_u_bytes_left);
                        assert_zero(to_u_ret, "i:%d j:%d mb2 0x%04X to_u_ret %d != 0 (%d: %s)\n",
                            i, j, mb2, (int) to_u_ret, errno, strerror(errno));
                    }
                    assert_equal(utf8_len + (int) out_to_u_bytes_left, ZUECI_ASIZE(out_to_u_buf),
                        "i:%d j:%d mb2 0x%04X u 0x%04X utf8_len %d + out_to_u_bytes_left %d != %d\n",
                        i, j, mb2, u, utf8_len, (int) out_to_u_bytes_left, ZUECI_ASIZE(out_to_u_buf));
                    assert_zero(in_to_u_bytes_left, "i:%d j:%d mb2 0x%04X u 0x%04X in_to_u_bytes_left %d != 0\n",
                        i, j, mb2, u, (int) in_to_u_bytes_left);
                    assert_zero(memcmp(out_to_u_buf, utf8_buf, utf8_len),
                        "i:%d j:%d mb2 0x%04X u 0x%04X memcmp(out_to_u_buf, src, %d) != 0\n", i, j, mb2, u, utf8_len);

                } else {
                    size_t in_to_u_bytes_left = 2;
                    size_t to_u_ret = 0;
                    while (in_to_u_bytes_left) {
                        if ((to_u_ret = iconv(cd_to_u, &in_to_u, &in_to_u_bytes_left,
                                &out_to_u, &out_to_u_bytes_left))) {
                            break;
                        }
                    }
                    assert_nonzero(to_u_ret, "i:%d j:%d mb 0x%04X to_u_ret == 0\n", i, j, mb2);
                }
            }
#endif
        }
    }

    /* 4-byters */
    for (i = 0x81; i < 0xFF; i++) {
        zueci_u32 mb4;
        src[0] = (unsigned char) i;

        for (j = 0x30; j < 0x3A; j++) {
            src[1] = (unsigned char) j;

            for (k = 0x81; k < 0xFF; k++) {
                src[2] = (unsigned char) k;

                for (l = 0x30; l < 0x3A; l++) {
                    src[3] = (unsigned char) l;
                    mb4 = ((zueci_u32) src[0] << 24) | ((zueci_u32) src[1] << 16) | ((zueci_u32) src[2] << 8) | src[3];

                    if (testContinue(p_ctx, mb4)) continue;

                    ret = zueci_gb18030_u_test(src, 4, 0, &u);
                    if (ret) {
#ifndef ZUECI_EMBED_NO_TO_ECI
                        unsigned char dest[4] = {0};
                        int ret2 = zueci_u_gb18030_test(u, dest);
                        zueci_u32 u2;

                        /* 2-byte extension GB 18030-2005 change, 18 BMP were PUA, see Table 3-37, p.109, Lunde
                           2nd ed. */
                        if ((mb4 >= 0x82359037 && mb4 <= 0x82359134) || (mb4 >= 0x84318236 && mb4 <= 0x84318335)) {
                            if (mb4 <= 0x82359134) { /* 10 */
                                u2 = 0x9FB4 + (mb4 - (mb4 < 0x82359130 ? 0x82359037 : 0x82359130 - 3));
                            } else { /* 8 */
                                u2 = 0xFE10 + (mb4 - (mb4 < 0x84318330 ? 0x84318236 : 0x84318330 - 4));
                            }
                            assert_equal(u, u2, "i:%d j:%d mb4 0x%X u 0x%04X != 0x%04X\n", i, j, mb4, u, u2);

                        /* and 6 non-BMP were PUA, see Table 3-37, Lunde, 2nd ed. */
                        } else if (mb4 == 0x95329031 || mb4 == 0x95329033 || mb4 == 0x95329730 || mb4 == 0x9536B937
                                || mb4 == 0x9630BA35 || mb4 == 0x9635B630) {

                            if (mb4 == 0x95329031) {
                                u2 = 0x20087;
                            } else if (mb4 == 0x95329033) {
                                u2 = 0x20089;
                            } else if (mb4 == 0x95329730) {
                                u2 = 0x200CC;
                            } else if (mb4 == 0x9536B937) {
                                u2 = 0x215D7;
                            } else if (mb4 == 0x9630BA35) {
                                u2 = 0x2298F;
                            } else {
                                u2 = 0x241FE;
                            }
                            assert_equal(u, u2, "i:%d j:%d mb4 0x%X u 0x%04X != 0x%04X\n", i, j, mb4, u, u2);

                        } else {
                            assert_equal(ret2, ret, "i:%d j:%d mb4 0x%X ret2 %d != ret %d\n", i, j, mb4, ret2, ret);
                            assert_zero(memcmp(dest, src, ret),
                                "i:%d j:%d mb4 0x%X u 0x%04X memcmp(dest, src, %d) != 0\n", i, j, mb4, u, ret);
                        }
#endif
                    } else if (mb4 <= 0xE3329A35 && (mb4 >= 0x90308130 || mb4 <= 0x8431A439)) { /* If not out-of-range */
                        assert_zero(ret, "i:%d j:%d mb4 0x%X u 0x%04X mb4 in range ret %d != 0\n", i, j, mb4, u, ret);
                    }

#ifdef ZUECI_TEST_ICONV
                    {
                        char out_to_u_buf[5] = {0};
                        char *in_to_u = (char *) src, *out_to_u = out_to_u_buf;
                        size_t out_to_u_bytes_left = ZUECI_ASIZE(out_to_u_buf);

                        if (ret) {
                            size_t in_to_u_bytes_left = ret;
                            unsigned char utf8_buf[5] = {0};
                            int utf8_len = testConvertUnicodeToUtf8(u, utf8_buf);

                            while (in_to_u_bytes_left) {
                                size_t to_u_ret = iconv(cd_to_u, &in_to_u, &in_to_u_bytes_left,
                                    &out_to_u, &out_to_u_bytes_left);
                                assert_zero(to_u_ret, "i:%d j:%d mb4 0x%X to_u_ret %d != 0 (%d: %s)\n",
                                    i, j, mb4, (int) to_u_ret, errno, strerror(errno));
                            }
                            assert_equal(utf8_len + (int) out_to_u_bytes_left, ZUECI_ASIZE(out_to_u_buf),
                                "i:%d j:%d mb4 0x%X u 0x%04X utf8_len %d + out_to_u_bytes_left %d != %d\n",
                                i, j, mb4, u, utf8_len, (int) out_to_u_bytes_left, ZUECI_ASIZE(out_to_u_buf));
                            assert_zero(in_to_u_bytes_left,
                                "i:%d j:%d mb4 0x%X u 0x%04X in_to_u_bytes_left %d != 0\n",
                                i, j, mb4, u, (int) in_to_u_bytes_left);
                            assert_zero(memcmp(out_to_u_buf, utf8_buf, utf8_len),
                                "i:%d j:%d mb4 0x%X u 0x%04X memcmp(out_to_u_buf, src, %d) != 0\n",
                                i, j, mb4, u, utf8_len);

                        } else {
                            size_t in_to_u_bytes_left = 2;
                            size_t to_u_ret = 0;
                            while (in_to_u_bytes_left) {
                                if ((to_u_ret = iconv(cd_to_u, &in_to_u, &in_to_u_bytes_left,
                                        &out_to_u, &out_to_u_bytes_left))) {
                                    break;
                                }
                            }
                            assert_nonzero(to_u_ret, "i:%d j:%d mb4 0x%X to_u_ret == 0\n", i, j, mb4);
                        }
                    }
#endif
                }
            }
        }
    }

#ifdef ZUECI_TEST_ICONV
    assert_zero(iconv_close(cd_to_u), "iconv_close cd_to_u fail\n");
#endif

    testFinish();
}
#endif /* ZUECI_EMBED_NO_TO_UTF8 */

int main(int argc, char *argv[]) {

    testFunction funcs[] = { /* name, func */
#ifndef ZUECI_EMBED_NO_TO_ECI
        { "test_utf8_to_eci", test_utf8_to_eci },
#endif
#ifndef ZUECI_EMBED_NO_TO_UTF8
        { "test_eci_to_utf8", test_eci_to_utf8 },
#endif
#ifndef ZUECI_EMBED_NO_TO_ECI
        { "test_u_gamut", test_u_gamut },
#endif
#ifndef ZUECI_EMBED_NO_TO_UTF8
        { "test_gb18030_gamut", test_gb18030_gamut },
#endif
    };

    testRun(argc, argv, funcs, ZUECI_ASIZE(funcs));

    testReport();

    return 0;
}

/* vim: set ts=4 sw=4 et : */
