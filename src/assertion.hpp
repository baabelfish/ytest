#pragma once

#include <iostream>
#include <functional>
#include "exception.hpp"
#include "current.hpp"

namespace internal {

template<typename Comparator>
inline void fail(Comparator c, bool a, size_t line, std::string msg) {
    if (!c(a)) {
        s_current_test->fails.emplace_back(exc::AssertException::Type::Fail, line, msg);
        throw std::exception();
    }
    else {
        s_current_test->successes.emplace_back(exc::AssertException::Type::Success, line, msg);
    }
}

template<typename Comparator, typename T, typename G>
inline void fail(Comparator&& c, const T& a, const G& b, size_t line, std::string msg) {
    if (!c(a, b)) {
        s_current_test->fails.emplace_back(exc::AssertException::Type::Fail, line, msg);
        throw std::exception();
    }
    else {
        s_current_test->successes.emplace_back(exc::AssertException::Type::Success, line, msg);
    }
}

struct isFalse {
    inline bool operator()(bool t) { return !t; }
};

struct isTrue {
    inline bool operator()(bool t) { return t; }
};

struct areNotEqual {
    template<typename T, typename G>
    inline bool operator()(const T& t, const G& g) { return t != g; }
};

struct areEqual {
    template<typename T, typename G>
    inline bool operator()(const T& t, const G& g) { return t == g; }
};

} // namespace internal

#define AreEqual(...) internal::fail(internal::areEqual(), __VA_ARGS__, __LINE__, "Should be equal: " #__VA_ARGS__);
#define AreNotEqual(...) internal::fail(internal::areNotEqual(), __VA_ARGS__, __LINE__, "Should not be equal: " #__VA_ARGS__);
#define IsTrue(...) internal::fail(internal::isTrue(), __VA_ARGS__, __LINE__, "Should be true: " #__VA_ARGS__);
#define IsFalse(...) internal::fail(internal::isFalse(), __VA_ARGS__, __LINE__, "Should be false: " #__VA_ARGS__);

namespace assertion {

} // namespace assertion
