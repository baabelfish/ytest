#pragma once

#include <memory>
#include <cstring>
#include <stack>
#include <string>
#include <functional>
#include <vector>
#include <iostream>

#include "print.hpp"
#include "assertion.hpp"
#include "test.hpp"
#include "current.hpp"
#include "ytestcontainer.hpp"

inline void xtest(std::string name, FTest) {
    s_current_test->subtests.emplace_back([]{}, name, s_current_test->filename, s_current_test->depth + 1, true);
}
inline void xtest(FTest t) {
    xtest("", t);
}

inline void test(std::string name, FTest t) {
    s_current_test->subtests.emplace_back(t, name, s_current_test->filename, s_current_test->depth + 1, false);
}
inline void test(FTest t) {
    test("", t);
}

#define COMBINE_HELPER(x, y) x ## y
#define COMBINE(x, y) COMBINE_HELPER(x, y)

// #define Ytest(...)\
// auto COMBINE(YtestRegisterHelper, __LINE__) = attach(__FILE__)(__VA_ARGS__);
#define Ytest(...)\
static YtestContainer COMBINE(YtestRegisterHelper, __LINE__)(__FILE__, __VA_ARGS__);

#define EXEC()\
int main(int argc, char* argv[]) {\
    YtestContainer::init(argc, argv);\
    return YtestContainer::execute();\
}
