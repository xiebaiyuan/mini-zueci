/* Shared test macros/routines */
/*
    libzueci - an open source UTF-8 ECI library adapted from libzint
    Copyright (C) 2022 gitlost
 */
/*
 * Adapted from qrencode/tests/common.h
 * Copyright (C) 2006-2017 Kentaro Fukuchi <kentaro@fukuchi.org>
 */
/* Due to above: */
/* SPDX-License-Identifier: LGPL-2.1+ */

#ifndef ZUECI_TESTCOMMON_H
#define ZUECI_TESTCOMMON_H

#define ZUECI_DEBUG_TEST_PRINT           16

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../zueci.h"
#include "../zueci_common.h"

#ifdef _MSC_VER
#  if _MSC_VER != 1200 /* VC6 */
#    pragma warning(disable: 4996) /* function or variable may be unsafe */
#  endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern int assertionFailed;
extern int assertionNum;
extern const char *assertionFilename;

#if _MSC_VER < 1900 /* MSVC 2015 */
#define testStart(__arg__) (testStartReal("", __arg__))
#else
#define testStart(__arg__) (testStartReal(__func__, __arg__))
#endif
void testStartReal(const char *func, const char *name);
void testFinish(void);
void testSkip(const char *msg);
void testReport(void);

typedef struct s_testCtx {
    int index;
    int index_end;
    int exclude;
    int exclude_end;
    int generate;
    int debug;
} testCtx;
typedef void (*testFunc_t)(const testCtx *const p_ctx);
typedef struct s_testFunction {
    const char *name; testFunc_t func;
} testFunction;
void testRun(int argc, char *argv[], testFunction funcs[], int funcs_size);
int testContinue(const testCtx *const p_ctx, const int i);

#if _MSC_VER == 1200 || (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199000L) /* VC6 or C89 */
void assert_zero(int exp, const char *fmt, ...);
void assert_nonzero(int exp, const char *fmt, ...);
void assert_null(const void *exp, const char *fmt, ...);
void assert_nonnull(const void *exp, const char *fmt, ...);
void assert_equal(int e1, int e2, const char *fmt, ...);
void assert_notequal(int e1, int e2, const char *fmt, ...);
#else
#define assert_exp(__exp__, ...) \
    { assertionNum++; if (!(__exp__)) { assertionFailed++; printf("%s:%d ", assertionFilename, __LINE__); \
      printf(__VA_ARGS__); testFinish(); return; } }

#define assert_zero(__exp__, ...) assert_exp((__exp__) == 0, __VA_ARGS__)
#define assert_nonzero(__exp__, ...) assert_exp((__exp__) != 0, __VA_ARGS__)
#define assert_null(__ptr__, ...) assert_exp((__ptr__) == NULL, __VA_ARGS__)
#define assert_nonnull(__ptr__, ...) assert_exp((__ptr__) != NULL, __VA_ARGS__)
#define assert_equal(__e1__, __e2__, ...) assert_exp((__e1__) == (__e2__), __VA_ARGS__)
#define assert_notequal(__e1__, __e2__, ...) assert_exp((__e1__) != (__e2__), __VA_ARGS__)
#endif

/* Helpers to cast away char pointer signedness */
#define ustrlen(source) strlen((const char *) (source))
#define ustrcpy(target, source) strcpy((char *) (target), (const char *) (source))
#define ustrcat(target, source) strcat((char *) (target), (const char *) (source))
#define ustrncat(target, source, count) strncat((char *) (target), (const char *) (source), (count))

#define TU(p) ((unsigned char *) (p))

/* Helper to convert Unicode codepoint `u` to UTF-8 */
int testConvertUnicodeToUtf8(const zueci_u32 u, unsigned char *buf);

/* Helper to convert a single UTF-8 sequence to a Unicode codepoint */
zueci_u32 testConvertUtf8ToUnicode(const unsigned char *src, const int src_len);

#ifdef __cplusplus
}
#endif

/* vim: set ts=4 sw=4 et : */
#endif /* ZUECI_TESTCOMMON_H */
