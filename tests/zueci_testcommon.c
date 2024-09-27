/* Shared test macros/routines */
/*
    libzueci - an open source UTF-8 ECI library adapted from libzint
    Copyright (C) 2022 gitlost
 */
/*
 * Adapted from qrencode/tests/common.c
 * Copyright (C) 2006-2017 Kentaro Fukuchi <kentaro@fukuchi.org>
 */
/* Due to above: */
/* SPDX-License-Identifier: LGPL-2.1+ */

#include "zueci_testcommon.h"
#include "../zueci.h"
#include "../zueci_common.h"

#include <assert.h>

static int tests = 0;
static int failed = 0;
static int skipped = 0;
int assertionFailed = 0;
int assertionNum = 0;
const char *assertionFilename = "";
static const char *testName = NULL;
static const char *testFunc = NULL;

/* Visual C++ 6 doesn't support variadic args to macros, so make do with functions, which have inferior behaviour,
   e.g. don't exit on failure, `assert_equal()` type-specific */
#if _MSC_VER == 1200 || (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199000L) /* VC6 or C89 */
#include <stdarg.h>
void assert_zero(int exp, const char *fmt, ...) {
    assertionNum++;
    if (exp != 0) {
        va_list args; assertionFailed++; va_start(args, fmt); vprintf(fmt, args); va_end(args); testFinish();
    }
}
void assert_nonzero(int exp, const char *fmt, ...) {
    assertionNum++;
    if (exp == 0) {
        va_list args; assertionFailed++; va_start(args, fmt); vprintf(fmt, args); va_end(args); testFinish();
    }
}
void assert_null(const void *exp, const char *fmt, ...) {
    assertionNum++;
    if (exp != NULL) {
        va_list args; assertionFailed++; va_start(args, fmt); vprintf(fmt, args); va_end(args); testFinish();
    }
}
void assert_nonnull(const void *exp, const char *fmt, ...) {
    assertionNum++;
    if (exp == NULL) {
        va_list args; assertionFailed++; va_start(args, fmt); vprintf(fmt, args); va_end(args); testFinish();
    }
}
void assert_equal(int e1, int e2, const char *fmt, ...) {
    assertionNum++;
    if (e1 != e2) {
        va_list args; assertionFailed++; va_start(args, fmt); vprintf(fmt, args); va_end(args); testFinish();
    }
}
void assert_notequal(int e1, int e2, const char *fmt, ...) {
    assertionNum++;
    if (e1 == e2) {
        va_list args; assertionFailed++; va_start(args, fmt); vprintf(fmt, args); va_end(args); testFinish();
    }
}
#endif

/* Begin individual test function */
void testStartReal(const char *func, const char *name) {
    tests++;
    if (func && *func && name && *name && strcmp(func, name) == 0) {
        testName = "";
    } else {
        testName = name;
    }
    testFunc = func ? func : "";
    assertionFailed = 0;
    assertionNum = 0;
    printf("_____%d: %s: %s...\n", tests, testFunc, testName ? testName : "");
}

/* End individual test function */
void testFinish(void) {
    if (testName && *testName) {
        printf(".....%d: %s: %s ", tests, testFunc, testName);
    } else {
        printf(".....%d: %s: ", tests, testFunc);
    }
    if (assertionFailed) {
        printf("FAILED. (%d assertions failed.)\n", assertionFailed);
        failed++;
    } else {
        printf("PASSED. (%d assertions passed.)\n", assertionNum);
    }
}

/* Skip (and end) individual test function */
void testSkip(const char *msg) {
    skipped++;
    if (testName && *testName) {
        printf(".....%d: %s: %s ", tests, testFunc, testName);
    } else {
        printf(".....%d: %s: ", tests, testFunc);
    }
    if (assertionFailed) {
        printf("FAILED. (%d assertions failed.)\n", assertionFailed);
        failed++;
    } else {
        printf("SKIPPED. %s. (%d assertions passed.)\n", msg, assertionNum);
    }
}

/* End test program */
void testReport(void) {
    if (failed && skipped) {
        printf("Total %d tests, %d skipped, %d fails.\n", tests, skipped, failed);
        exit(-1);
    }
    if (failed) {
        printf("Total %d tests, %d fails.\n", tests, failed);
        exit(-1);
    }
    if (skipped) {
        printf("Total %d tests, %d skipped.\n", tests, skipped);
    } else if (tests) {
        printf("Total %d tests, all passed.\n", tests);
    } else {
        printf("Total %d tests.\n", tests);
    }
}

/* Verifies that a string `src` (length <= 9) only uses digits. On success returns value in `p_val` */
static int validate_int(const char src[], int *p_val) {
    int val = 0;
    int i;
    const int length = (int) strlen(src);

    if (length > 9) { /* Prevent overflow */
        return 0;
    }
    for (i = 0; i < length; i++) {
        if (src[i] < '0' || src[i] > '9') {
            return 0;
        }
        val *= 10;
        val += src[i] - '0';
    }
    *p_val = val;

    return 1;
}

/* Verifies that a string `src` only uses digits or a comma-separated range of digits.
   On success returns value in `p_val` and if present a range end value in `p_val_end` */
static int validate_int_range(const char src[], int *p_val, int *p_val_end) {
    int val = 0;
    int val_end = -1;
    const int length = (int) strlen(src);
    int i, j;

    for (i = 0; i < length; i++) {
        if (src[i] < '0' || src[i] > '9') {
            if (src[i] != ',') {
                return 0;
            }
            val_end = 0;
            for (j = i + 1; j < length; j++) {
                if (src[j] < '0' || src[j] > '9') {
                    return 0;
                }
                if (j - (i + 1) >= 9) { /* Prevent overflow */
                    return 0;
                }
                val_end *= 10;
                val_end += src[j] - '0';
            }
            break;
        }
        if (i >= 9) { /* Prevent overflow */
            return 0;
        }
        val *= 10;
        val += src[i] - '0';
    }
    *p_val = val;
    *p_val_end = val_end;

    return 1;
}

/* Begin test program, parse args */
void testRun(int argc, char *argv[], testFunction funcs[], int funcs_size) {
    int i, ran;
    char *optarg;
    char *func = NULL;
    char func_buf[256 + 5];
    testCtx ctx;

    ctx.index = ctx.index_end = ctx.exclude = ctx.exclude_end = -1;
    ctx.generate = ctx.debug = 0;

    if (argc) {
        const char *filename;
#ifdef _WIN32
        if ((filename = strrchr(argv[0], '\\')) == NULL) {
            filename = strrchr(argv[0], '/');
        }
#else
        filename = strrchr(argv[0], '/');
#endif
        if (filename) {
            assertionFilename = filename + 1;
        } else {
            assertionFilename = argv[0];
        }
    }

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            if (i + 1 == argc) {
                fprintf(stderr, "testRun: -d debug value missing, ignored\n");
            } else {
                optarg = argv[++i];
                if (!validate_int(optarg, &ctx.debug)) {
                    fprintf(stderr, "testRun: -d debug value invalid, ignored\n");
                    ctx.debug = 0;
                }
            }
        } else if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 == argc) {
                fprintf(stderr, "testRun: -f func value missing, ignored\n");
            } else {
                optarg = argv[++i];
                if (strlen(optarg) < 256) {
                    if (strncmp(optarg, "test_", 5) == 0) {
                        strcpy(func_buf, optarg);
                    } else {
                        strcpy(func_buf, "test_");
                        strcat(func_buf, optarg);
                    }
                    func = func_buf;
                } else {
                    fprintf(stderr, "testRun: -f func value too long, ignored\n");
                    func = NULL;
                }
            }
        } else if (strcmp(argv[i], "-g") == 0) {
            ctx.generate = 1;
        } else if (strcmp(argv[i], "-i") == 0) {
            if (i + 1 == argc) {
                fprintf(stderr, "testRun: -i index value missing, ignored\n");
            } else {
                optarg = argv[++i];
                if (!validate_int_range(optarg, &ctx.index, &ctx.index_end)) {
                    fprintf(stderr, "testRun: -i index value invalid, ignored\n");
                    ctx.index = ctx.index_end = -1;
                }
            }
        } else if (strcmp(argv[i], "-x") == 0) {
            if (i + 1 == argc) {
                fprintf(stderr, "testRun: -x exclude value missing, ignored\n");
            } else {
                optarg = argv[++i];
                if (!validate_int_range(optarg, &ctx.exclude, &ctx.exclude_end)) {
                    fprintf(stderr, "testRun: -x exclude value invalid, ignored\n");
                    ctx.exclude = ctx.exclude_end = -1;
                }
            }
        } else {
            fprintf(stderr, "testRun: unknown arg '%s', ignored\n", argv[i]);
        }
    }

    ran = 0;
    for (i = 0; i < funcs_size; i++) {
        if (func && strcmp(func, funcs[i].name) != 0) {
            continue;
        }
        (*funcs[i].func)(&ctx);
        ran++;
    }

    if (func && !ran) {
        fprintf(stderr, "testRun: unknown -f func arg '%s'\n", func);
    }
}

/* Call in a dataset loop to determine if a datum should be tested according to -i & -x args */
int testContinue(const testCtx *const p_ctx, const int i) {
    if (p_ctx->index != -1) {
        if (p_ctx->index_end != -1) {
            if (i < p_ctx->index || (p_ctx->index_end && i > p_ctx->index_end)) {
                return 1;
            }
        } else if (i != p_ctx->index) {
            return 1;
        }
    }
    if (p_ctx->exclude != -1) {
        if (p_ctx->exclude_end != -1) {
            if (i >= p_ctx->exclude && (p_ctx->exclude_end == 0 || i <= p_ctx->exclude_end)) {
                return 1;
            }
        } else if (i == p_ctx->exclude) {
            return 1;
        }
    }
    if (p_ctx->debug & ZUECI_DEBUG_TEST_PRINT) {
        printf("i:%d\n", i);
    }
    return 0;
}

/* Helper to convert Unicode codepoint `u` to UTF-8 */
int testConvertUnicodeToUtf8(const zueci_u32 u, unsigned char *buf) {
    int len = 0;

    if (u < 0x80) {
        buf[0] = (unsigned char) u;
        len = 1;
    } else if (u < 0x800) {
        buf[0] = (unsigned char) (0xC0 | (u >> 6));
        buf[1] = (unsigned char) (0x80 | (u & 0x3F));
        len = 2;
    } else if (u < 0x10000) {
        buf[0] = (unsigned char) (0xE0 | (u >> 12));
        buf[1] = (unsigned char) (0x80 | ((u >> 6) & 0x3F));
        buf[2] = (unsigned char) (0x80 | (u & 0x3F));
        len = 3;
    } else {
        buf[0] = (unsigned char) (0xF0 | (u >> 18));
        buf[1] = (unsigned char) (0x80 | ((u >> 12) & 0x3F));
        buf[2] = (unsigned char) (0x80 | ((u >> 6) & 0x3F));
        buf[3] = (unsigned char) (0x80 | (u & 0x3F));
        len = 4;
    }
    buf[len] = '\0';

    return len;
}

ZUECI_INTERN unsigned int zueci_decode_utf8_test(unsigned int *p_state, zueci_u32 *p_u, const unsigned char byte);

/* Helper to convert a single UTF-8 sequence to a Unicode codepoint */
zueci_u32 testConvertUtf8ToUnicode(const unsigned char *src, const int src_len) {
    int src_posn = 0;
    zueci_u32 u = 0, state = 0;

    while (src_posn < src_len) {
        zueci_decode_utf8_test(&state, &u, src[src_posn++]);
        if (state == 12) {
            return 0;
        }
        if (state == 0) {
            break;
        }
    }

    return state == 0 ? u : 0;
}

/* vim: set ts=4 sw=4 et : */
