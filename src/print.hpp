#pragma once

#include <functional>
#include <iostream>
#include "config.hpp"

namespace Color {

const std::string RESET = "\033[0m";
const std::string BRIGHT = "\033[1m";
const std::string UNDERLINE = "\033[4m";

const std::string FG_BLACK = "\033[30m";
const std::string FG_RED = "\033[31m";
const std::string FG_GREEN = "\033[32m";
const std::string FG_YELLOW = "\033[33m";
const std::string FG_BLUE = "\033[34m";
const std::string FG_MAGENTA = "\033[35m";
const std::string FG_CYAN = "\033[36m";
const std::string FG_WHITE = "\033[37m";

const std::string BG_BLACK = "\033[40m";
const std::string BG_RED = "\033[41m";
const std::string BG_GREEN = "\033[42m";
const std::string BG_YELLOW = "\033[43m";
const std::string BG_BLUE = "\033[44m";
const std::string BG_MAGENTA = "\033[45m";
const std::string BG_CYAN = "\033[46m";
const std::string BG_WHITE = "\033[47m";

} // namespace

template<typename Node>
inline void indent(const Node& node, size_t weight, bool draw_lines) {
    auto amount = node.depth;
    for (std::size_t i = 0; i < (amount - 1) * weight; ++i)
        std::cout << " ";
    if (draw_lines) {
        if (node.subtests.empty())
            std::cout << "└──╼ ";
        else
            std::cout << "└─┬╼ ";
    }
    else {
        std::cout << "     ";
    }
}

template<typename Node>
void printName(const Node& node) {
    using namespace Color;
    bool has_successes = !node.successes.empty(),
         has_warnings = !node.warnings.empty(),
         has_failures = !node.fails.empty();

    std::cout << BRIGHT;
    if (node.is_disabled) std::cout << FG_BLACK << BRIGHT;
    else if (has_failures) std::cout << BRIGHT << FG_RED;
    else if (has_warnings) std::cout << BRIGHT << FG_YELLOW;
    else std::cout << RESET << FG_GREEN;
    if (!node.name.empty())
        std::cout << node.name;
    else
        std::cout << "Anonymous test";

    if (node.fails.empty() && has_successes) {
        std::cout << BRIGHT << FG_BLACK << " (";
        std::cout << FG_GREEN << node.successes.size();
        if (has_warnings) std::cout << " " << FG_YELLOW << node.warnings.size();
        std::cout << BRIGHT << FG_BLACK << ")";
    }
}

template<typename Node>
void printErrors(const Node& node, PrintStrategy strat) {
    using namespace Color;
    std::cout << RESET;
    if (!node.fails.empty()) {
        std::cout << std::endl;
        for (auto& x : node.fails) {
            if (strat != PrintStrategy::FailuresOnly) {
                indent(node, 2, false);
            }
            auto error = std::string(x.what()).empty() ? "Undefined error" : x.what();

            std::cout
                << FG_YELLOW << x.line << BRIGHT << FG_BLACK << ": " << RESET
                << error;
            if (strat == PrintStrategy::FailuresOnly) {
                std::cout
                    << BRIGHT << FG_BLACK << " ("
                    << RESET << FG_WHITE << node.filename
                    << BRIGHT << FG_BLACK << ")";
                std::cout << std::endl;
            }
        }
    }
}

template<typename Node>
inline int traverse(Node& node, PrintStrategy strat) {
    using namespace Color;
    int reval = !node.fails.empty()
        ? 2
        : !node.warnings.empty()
            ? 1 : 0;

    if (node.depth != 0) {
        std::cout << RESET;
        switch (strat) {
            case PrintStrategy::All:
                indent(node, 2, true);
                printName(node);
                printErrors(node, strat);
                std::cout << std::endl;
                break;
            case PrintStrategy::FailuresOnly:
                if (!node.warnings.empty() || !node.fails.empty()) {
                    printName(node);
                    printErrors(node, strat);
                }
                break;
            default:
                break;
        }
    }

    for (auto& x : node.subtests) {
        auto errcode = traverse(x, strat);
        if (errcode > reval)
            reval = errcode;
    }
    return reval;
}
