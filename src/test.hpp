#pragma once

#include <memory>
#include <cstring>
#include <stack>
#include <string>
#include <functional>
#include <vector>
#include <iostream>
#include "exception.hpp"

using FTest = std::function<void()>;

struct Test {
    FTest test;
    std::vector<Test> subtests;
    std::string name;
    std::string filename;
    size_t line;
    size_t depth;
    bool is_disabled;

    std::vector<exc::AssertException> fails;
    std::vector<exc::AssertException> successes;
    std::vector<exc::AssertException> warnings;

    Test(FTest f, std::string name, std::string filename, size_t depth, bool is_disabled = false):
        test(f),
        subtests(),
        name(name),
        filename(filename),
        depth(depth),
        is_disabled(is_disabled) {}

    inline bool hasChildren() const {
        return !subtests.empty() || !fails.empty() || !warnings.empty() || !successes.empty();
    }
};

