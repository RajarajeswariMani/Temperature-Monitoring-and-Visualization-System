#pragma once
/* Minimal, dependency-free C++ test framework -- deliberately mirrors
 * the C project's include/c_test_framework.h so both language tracks
 * read the same way and neither needs an external library (no
 * GoogleTest / Catch2 fetch required to build on a clean machine). */
#include <iostream>
#include <string>

struct TestContext {
    int total = 0;
    int passed = 0;
    int failed = 0;
};

inline TestContext &testContext() {
    static TestContext ctx;
    return ctx;
}

#define TEST_BEGIN(name) \
    do { \
        testContext().total++; \
        std::cout << "  [TEST " << testContext().total << "] " << (name) << " ... "; \
        std::cout.flush(); \
    } while (0)

#define TEST_PASS() \
    do { testContext().passed++; std::cout << "PASS\n"; } while (0)

#define ASSERT_TRUE(cond, message) \
    do { \
        if (!(cond)) { \
            std::cout << "FAIL\n    ERROR at line " << __LINE__ << ": " << (message) << "\n"; \
            testContext().failed++; \
            return; \
        } \
    } while (0)

#define ASSERT_FALSE(cond, message) ASSERT_TRUE(!(cond), message)

#define ASSERT_EQ(actual, expected, message) \
    do { \
        if (!((actual) == (expected))) { \
            std::cout << "FAIL\n    ERROR at line " << __LINE__ << ": " << (message) \
                       << " (expected " << (expected) << ", got " << (actual) << ")\n"; \
            testContext().failed++; \
            return; \
        } \
    } while (0)

inline int testSummaryAndExitCode() {
    auto &ctx = testContext();
    std::cout << "\n========================================\n";
    std::cout << "TEST RESULTS SUMMARY\n";
    std::cout << "========================================\n";
    std::cout << "Total Tests:  " << ctx.total << "\n";
    std::cout << "Passed:       " << ctx.passed << "\n";
    std::cout << "Failed:       " << ctx.failed << "\n";
    std::cout << "========================================\n";
    std::cout << (ctx.failed == 0 ? "\xE2\x9C\x93 ALL TESTS PASSED!\n" : "\xE2\x9C\x97 SOME TESTS FAILED!\n");
    std::cout << "========================================\n";
    return ctx.failed == 0 ? 0 : 1;
}
