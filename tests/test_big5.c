/* Test ECI 28 Big5 Chinese (Taiwan) */
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
        /*  2*/ { TU("\xC2\x80"), -1, ZUECI_ERROR_INVALID_DATA, -1 }, /* No mapping for U+0080 */
        /*  3*/ { TU("\xE3\x80\x80"), -1, 0, 2 }, /* U+3000 IDEOGRAPHIC SPACE */
        /*  4*/ { TU("\xED\xA0\x80"), -1, ZUECI_ERROR_INVALID_UTF8, -1 }, /* U+D800 high surrogate invalid UTF-8 */
        /*  5*/ { TU("\xED\xBF\xBF"), -1, ZUECI_ERROR_INVALID_UTF8, -1 }, /* U+DFFF low surrogate invalid UTF-8 */
        /*  6*/ { TU("\xEF\xBF\xBE"), -1, ZUECI_ERROR_INVALID_DATA, -1 }, /* U+FFFE (reversed BOM) not allowed */
        /*  7*/ { TU("\xEF\xBF\xBF"), -1, ZUECI_ERROR_INVALID_DATA, -1 }, /* No mapping for U+FFFF */
        /*  8*/ { TU("\xC2\xA5"), -1, 0, 2 }, /* U+00A5 YEN SIGN */
        /*  9*/ { TU("\xE9\xBE\x98"), -1, 0, 2 }, /* U+9F98 last mapping */
        /* 10*/ { TU("\xE9\xBE\x99"), -1, ZUECI_ERROR_INVALID_DATA, -1 }, /* U+9F99 unmapped */
        /* 11*/ { TU("A\xE9\xBE\x98\xC2\xA5\xE3\x80\x80" "B"), -1, 0, 8 }, /* Multiple (good) mappings */
    };
    int data_size = ZUECI_ASIZE(data);
    int i, src_len, ret;
    const int eci = 28;

    unsigned char dest[1024];
    unsigned char dest2[1024];
#ifdef ZUECI_TEST_ICONV
    iconv_t cd_from_u, cd_to_u;
#endif

    testStart("test_utf8_to_eci");

#ifdef ZUECI_TEST_ICONV
    cd_from_u = iconv_open("BIG5" /*to*/, "UTF-8" /*from*/);
    assert_zero(cd_from_u == (iconv_t) -1, "iconv_open cd_from_u == -1\n");
    cd_to_u = iconv_open("UTF-8" /*to*/, "BIG5" /*from*/);
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
        /*  0*/ { TU("\xA1\xC4"), -1, 0, 0, 0, 3 }, /* 0xA1C4 -> U+FF3F FULLWIDTH LOW LINE */
        /*  1*/ { TU("\xA1\x5A"), -1, 0, 0, ZUECI_ERROR_INVALID_DATA, -1 }, /* U+2574 not in Big5 but in Big5-2003 as 0xA15A */
        /*  2*/ { TU("\xA1\x5A"), -1, '?', 0, ZUECI_WARN_INVALID_DATA, 1 }, /* -> ? */
        /*  3*/ { TU("\xA1\x5A"), -1, 0xFFFD, 0, ZUECI_WARN_INVALID_DATA, 3 }, /* -> U+FFFD */
        /*  4*/ { TU("\xA1\x7F"), -1, 0, 0, ZUECI_ERROR_INVALID_DATA, -1 }, /* 0xA17F invalid trailing byte */
        /*  5*/ { TU("\xA1\x7F"), -1, 0x00BF, 0, ZUECI_WARN_INVALID_DATA, 2 }, /* -> U+00BF */
        /*  6*/ { TU("\xA1\xFF"), -1, 0, 0, ZUECI_ERROR_INVALID_DATA, -1 }, /* 0xA1FF invalid trailing byte */
        /*  7*/ { TU("\xA1"), -1, 0, 0, ZUECI_ERROR_INVALID_DATA, -1 }, /* 0xA1 missing trailing byte */
        /*  8*/ { TU("\xA1"), -1, 0xFFFD, 0, ZUECI_WARN_INVALID_DATA, 3 }, /* -> U+FFFD */
        /*  9*/ { TU("\xA0\x40"), -1, 0, 0, ZUECI_ERROR_INVALID_DATA, -1 }, /* 0xA040 bad lead byte */
        /* 10*/ { TU("\xA0\x40"), -1, 0xFFFD, 0, ZUECI_WARN_INVALID_DATA, 3 }, /* -> U+FFFD */
        /* 11*/ { TU("\xA0\x40G"), -1, 0xFFFD, 0, ZUECI_WARN_INVALID_DATA, 4 }, /* -> U+FFFD G */
        /* 12*/ { TU("\x00"), 1, 0, 0, 0, 1 }, /* NUL */
        /* 13*/ { TU("A\xA1\xC4\x7F\xA4\x40\xF9\xD4" "B"), -1, 0, 0, 0, 12 }, /* -> A U+FF3F <DEL> U+4E00 U+9F49 B */
        /* 14*/ { TU("A\xA1\xC4\x7F\xA4\x40\xA1\x5A\xF9\xD4\xFF\xFF" "B"), -1, '?', 0, ZUECI_WARN_INVALID_DATA, 14 }, /* -> A U+FF3F <DEL> U+4E00 ? U+9F49 ? B */
    };
    int data_size = ZUECI_ASIZE(data);
    int i, src_len, ret;
    const int eci = 28;

    unsigned char dest[1024];
    unsigned char dest2[1024];
#ifdef ZUECI_TEST_ICONV
    iconv_t cd_to_u;
#endif

    testStart("test_eci_to_utf8");

#ifdef ZUECI_TEST_ICONV
    cd_to_u = iconv_open("UTF-8" /*to*/, "BIG5" /*from*/);
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
            assert_equal(dest_len, data[i].expected_length, "i:%d dest_len %d != %d\n",
                i, dest_len, data[i].expected_length);
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
#include "test_big5_tab.h"

/* As control convert to Big5 using simple table generated from
   https://www.unicode.org/Public/MAPPINGS/OBSOLETE/EASTASIA/OTHER/BIG5.TXT plus simple processing */
static int u_big5_2(zueci_u32 u, unsigned char *dest) {
    int tab_length = ZUECI_ASIZE(test_big5_tab);
    int start_i = test_big5_tab_ind[u >> 10];
    int end_i = start_i + 0x800 > tab_length ? tab_length : start_i + 0x800;
    zueci_u32 mb;
    int i;

    if (u < 0x80) {
        *dest = (unsigned char) u;
        return 1;
    }
    for (i = start_i; i < end_i; i += 2) {
        if (test_big5_tab[i + 1] == u) {
            mb = test_big5_tab[i];
            if (mb & 0xFF00) {
                dest[0] = (unsigned char) (mb >> 8);
                dest[1] = (unsigned char) mb;
                return 2;
            }
            *dest = (unsigned char) mb;
            return 1;
        }
    }
    return 0;
}

ZUECI_INTERN int zueci_u_big5_test(const zueci_u32 u, unsigned char *dest);
#ifndef ZUECI_EMBED_NO_TO_UTF8
ZUECI_INTERN int zueci_big5_u_test(const unsigned char *src, const zueci_u32 len, const unsigned int flags,
                    zueci_u32 *p_u);
#endif

/* Test all Unicode BMP codepoints */
static void test_u_gamut(const testCtx *const p_ctx) {

    int ret, ret2, ret3;
    unsigned char val[2];
    unsigned char val2[2];
    zueci_u32 u3;
    zueci_u32 i;
#ifdef ZUECI_TEST_ICONV
    iconv_t cd_from_u;
#endif

    testStart("test_u_gamut");

#ifdef ZUECI_TEST_ICONV
    cd_from_u = iconv_open("BIG5" /*to*/, "UTF-8" /*from*/);
    assert_zero(cd_from_u == (iconv_t) -1, "iconv_open cd_from_u == -1\n");
#endif

    for (i = 0; i < 0xFFFE; i++) { /* Exclude not-a-characters reverse BOM 0xFFFE and 0xFFFF */
        if (i == 0xD800) { /* Skip UTF-16 surrogates */
            i = 0xE000;
        }
        if (testContinue(p_ctx, i)) continue;

        /* zueci_u_big5() */

        memset(val, 0, ZUECI_ASIZE(val));
        memset(val2, 0, ZUECI_ASIZE(val2));
        ret = zueci_u_big5_test(i, val);
        ret2 = u_big5_2(i, val2);
        assert_equal(ret, ret2, "i:%d 0x%04X ret %d != ret2 %d, val 0x%02X%02X, val2 0x%02X%02X\n",
            (int) i, i, ret, ret2, val[0], val[1], val2[0], val2[1]);
        if (ret) {
            assert_equal(val[0], val2[0],
                "i:%d 0x%04X val[0] 0x%02X != val2[0] 0x%02X, val[1] 0x%02X, val2[1] 0x%02X\n",
                (int) i, i, val[0], val2[0], val[1], val2[1]);
            if (ret == 2) {
                assert_equal(val[1], val2[1], "i:%d 0x%04X val[1] 0x%02X != val2[1] 0x%02X\n",
                    (int) i, i, val[1], val2[1]);
            }

#ifndef ZUECI_EMBED_NO_TO_UTF8
            /* Check round-trip */

            u3 = 0;
            ret3 = zueci_big5_u_test(val, ret, 0, &u3);
            assert_equal(ret3, ret2, "i:%d 0x%04X val 0x%02X%02X, ret3 %d != ret2 %d\n", (int) i, i, val[0], val[1], ret3, ret2);
            assert_equal(i, u3, "i:%d 0x%04X != u3 0x%04X\n", (int) i, i, u3);
#else
            (void)ret3; (void)u3;
#endif
        }

#ifdef ZUECI_TEST_ICONV
        {
            char in_from_u_buf[5];
            char out_from_u_buf[5] = {0};
            char *in_from_u = in_from_u_buf, *out_from_u = out_from_u_buf;
            size_t out_from_u_bytes_left = ZUECI_ASIZE(out_from_u_buf);
            size_t in_from_u_bytes_left = testConvertUnicodeToUtf8(i, (unsigned char *) in_from_u_buf);

            if (ret) {
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

            } else {
                size_t from_u_ret = 0;
                while (in_from_u_bytes_left) {
                    if ((from_u_ret = iconv(cd_from_u, &in_from_u, &in_from_u_bytes_left,
                        &out_from_u, &out_from_u_bytes_left))) {
                        break;
                    }
                }
                assert_nonzero(from_u_ret, "i:%d 0x%04X from_u_ret == 0\n", i, i);
            }
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
ZUECI_INTERN int zueci_big5_u_test(const unsigned char *src, const zueci_u32 len, const unsigned int flags,
                    zueci_u32 *p_u);

/* Test all BIG5 codepoints */
static void test_big5_gamut(const testCtx *const p_ctx) {

    int ret;
    unsigned char src[2];
    zueci_u32 u;
    int i, j;
#ifdef ZUECI_TEST_ICONV
    iconv_t cd_to_u;
#endif

    testStart("test_big5_gamut");

#ifdef ZUECI_TEST_ICONV
    cd_to_u = iconv_open("UTF-8" /*to*/, "BIG5" /*from*/);
    assert_zero(cd_to_u == (iconv_t) -1, "iconv_open cd_to_u == -1\n");
#endif

    for (i = 0xA1; i < 0xFA; i++) {

        src[0] = (unsigned char) i;
        for (j = 0x40; j < 0xFF; j++) {

            if (testContinue(p_ctx, i + j)) continue;

            src[1] = (unsigned char) j;
            ret = zueci_big5_u_test(src, 2, 0, &u);
#ifndef ZUECI_EMBED_NO_TO_ECI
            if (ret) {
                unsigned char dest[4] = {0};
                int ret2 = zueci_u_big5_test(u, dest);
                assert_equal(ret2, ret, "i:%d j:%d 0x%02X%02X ret2 %d != ret %d\n", i, j, i, j, ret2, ret);
                assert_zero(memcmp(dest, src, ret), "i:%d j:%d 0x%02X%02X memcmp(dest, src, %d) != 0\n",
                    i, j, i, j, ret);
            }
#else
            (void)ret;
#endif

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
                        assert_zero(to_u_ret, "i:%d j:%d 0x%02X%02X to_u_ret %d != 0 (%d: %s)\n",
                            i, j, i, j, (int) to_u_ret, errno, strerror(errno));
                    }
                    assert_equal(utf8_len + (int) out_to_u_bytes_left, ZUECI_ASIZE(out_to_u_buf),
                        "i:%d j:%d 0x%02X%02X, u 0x%04X, utf8_len %d + out_to_u_bytes_left %d != %d\n",
                        i, j, i, j, u, utf8_len, (int) out_to_u_bytes_left, ZUECI_ASIZE(out_to_u_buf));
                    assert_zero(in_to_u_bytes_left,
                        "i:%d j:%d 0x%02X%02X, u 0x%04X, in_to_u_bytes_left %d != 0\n",
                        i, j, i, j, u, (int) in_to_u_bytes_left);
                    assert_zero(memcmp(out_to_u_buf, utf8_buf, utf8_len),
                        "i:%d j:%d 0x%02X%02X, u 0x%04X, memcmp(out_to_u_buf, src, %d) != 0\n",
                        i, j, i, j, u, utf8_len);

                } else {
                    size_t in_to_u_bytes_left = 2;
                    size_t to_u_ret = 0;
                    while (in_to_u_bytes_left) {
                        if ((to_u_ret = iconv(cd_to_u, &in_to_u, &in_to_u_bytes_left,
                                &out_to_u, &out_to_u_bytes_left))) {
                            break;
                        }
                    }
                    assert_nonzero(to_u_ret, "i:%d j:%d 0x%02X%02X to_u_ret == 0\n", i, j, i, j);
                }
            }
#endif
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
        { "test_big5_gamut", test_big5_gamut },
#endif
    };

    testRun(argc, argv, funcs, ZUECI_ASIZE(funcs));

    testReport();

    return 0;
}

/* vim: set ts=4 sw=4 et : */
