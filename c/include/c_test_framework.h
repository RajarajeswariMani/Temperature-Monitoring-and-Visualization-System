#ifndef C_TEST_FRAMEWORK_H
#define C_TEST_FRAMEWORK_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Simple C test framework */

typedef struct {
  int total_tests;
  int passed_tests;
  int failed_tests;
  const char *current_test_name;
  int current_test_line;
} TestContext;

extern TestContext g_test_context;

/* Initialize test framework */
#define TEST_INIT()                                                            \
  do {                                                                         \
    g_test_context.total_tests = 0;                                            \
    g_test_context.passed_tests = 0;                                           \
    g_test_context.failed_tests = 0;                                           \
  } while (0)

/* Begin a test */
#define TEST_BEGIN(test_name)                                                  \
  do {                                                                         \
    g_test_context.total_tests++;                                              \
    g_test_context.current_test_name = (test_name);                            \
    g_test_context.current_test_line = __LINE__;                               \
    printf("  [TEST %d] %s ... ", g_test_context.total_tests, test_name);      \
    fflush(stdout);                                                            \
  } while (0)

/* End a test (success) */
#define TEST_PASS()                                                            \
  do {                                                                         \
    g_test_context.passed_tests++;                                             \
    printf("PASS\n");                                                          \
  } while (0)

/* Assertion: condition must be true */
#define ASSERT_TRUE(condition, message)                                        \
  do {                                                                         \
    if (!(condition)) {                                                        \
      printf("FAIL\n");                                                        \
      printf("    ERROR at line %d: %s\n", __LINE__, message);                 \
      g_test_context.failed_tests++;                                           \
      return;                                                                  \
    }                                                                          \
  } while (0)

/* Assertion: condition must be false */
#define ASSERT_FALSE(condition, message) ASSERT_TRUE(!(condition), message)

/* Assertion: integers equal */
#define ASSERT_INT_EQ(actual, expected, message)                               \
  do {                                                                         \
    if ((actual) != (expected)) {                                              \
      printf("FAIL\n");                                                        \
      printf("    ERROR at line %d: %s\n", (int)__LINE__, message);            \
      printf("    Expected: %d, Got: %d\n", (int)(expected), (int)(actual));   \
      g_test_context.failed_tests++;                                           \
      return;                                                                  \
    }                                                                          \
  } while (0)

/* Assertion: integers not equal */
#define ASSERT_INT_NE(actual, not_expected, message)                           \
  do {                                                                         \
    if ((actual) == (not_expected)) {                                          \
      printf("FAIL\n");                                                        \
      printf("    ERROR at line %d: %s\n", __LINE__, message);                 \
      printf("    Value should not be: %d\n", (int)(not_expected));            \
      g_test_context.failed_tests++;                                           \
      return;                                                                  \
    }                                                                          \
  } while (0)

/* Assertion: strings equal */
#define ASSERT_STR_EQ(actual, expected, message)                               \
  do {                                                                         \
    if (strcmp((actual), (expected)) != 0) {                                   \
      printf("FAIL\n");                                                        \
      printf("    ERROR at line %d: %s\n", __LINE__, message);                 \
      printf("    Expected: \"%s\", Got: \"%s\"\n", (expected), (actual));     \
      g_test_context.failed_tests++;                                           \
      return;                                                                  \
    }                                                                          \
  } while (0)

/* Assertion: pointers equal */
#define ASSERT_PTR_EQ(actual, expected, message)                               \
  do {                                                                         \
    if ((actual) != (expected)) {                                              \
      printf("FAIL\n");                                                        \
      printf("    ERROR at line %d: %s\n", __LINE__, message);                 \
      printf("    Expected pointer: %p, Got: %p\n", (void *)(expected),        \
             (void *)(actual));                                                \
      g_test_context.failed_tests++;                                           \
      return;                                                                  \
    }                                                                          \
  } while (0)

/* Assertion: pointers not equal */
#define ASSERT_PTR_NE(actual, not_expected, message)                           \
  do {                                                                         \
    if ((actual) == (not_expected)) {                                          \
      printf("FAIL\n");                                                        \
      printf("    ERROR at line %d: %s\n", __LINE__, message);                 \
      printf("    Pointer should not be: %p\n", (void *)(not_expected));       \
      g_test_context.failed_tests++;                                           \
      return;                                                                  \
    }                                                                          \
  } while (0)

/* Print test results summary */
#define TEST_SUMMARY()                                                         \
  do {                                                                         \
    printf("\n========================================\n");                    \
    printf("TEST RESULTS SUMMARY\n");                                          \
    printf("========================================\n");                      \
    printf("Total Tests:  %d\n", g_test_context.total_tests);                  \
    printf("Passed:       %d\n", g_test_context.passed_tests);                 \
    printf("Failed:       %d\n", g_test_context.failed_tests);                 \
    printf("========================================\n");                      \
    if (g_test_context.failed_tests == 0) {                                    \
      printf("✓ ALL TESTS PASSED!\n");                                         \
    } else {                                                                   \
      printf("✗ SOME TESTS FAILED!\n");                                        \
    }                                                                          \
    printf("========================================\n");                      \
  } while (0)

/* Return exit code (0 if all passed, 1 if any failed) */
#define TEST_EXIT_CODE() (g_test_context.failed_tests == 0 ? 0 : 1)

#ifdef __cplusplus
}
#endif

#endif /* C_TEST_FRAMEWORK_H */
