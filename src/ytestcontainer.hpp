#pragma once

#include "current.hpp"
#include "config.hpp"

class YtestContainer {
    struct Config {
        PrintStrategy print_strat = PrintStrategy::FailuresOnly;
    };

    static inline auto& config() {
        static Config config;
        return config;
    }

    static inline std::vector<Test>& testContainer() {
        static std::vector<Test> tests;
        return tests;
    }

public:
    YtestContainer(std::string file, FTest test) {
        Test root(test, "", file, 0);
        root.test = test;
        testContainer().push_back(root);
    }

    virtual ~YtestContainer() {}

    inline static void parseParameters(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            std::string param(argv[i]);
            if (param == "--verbose") {
                config().print_strat = PrintStrategy::All;
            }
        }
    }

    inline static void init(int argc, char* argv[]) {
        parseParameters(argc, argv);
    }

    inline static void executeLower(Test& root) {
        try { root.test(); }
        catch(...) {}
        for (auto& x : root.subtests) {
            s_current_test = &x;
            x.depth = root.depth + 1;
            executeLower(x);
        }
    }

    inline static int execute() {
        using namespace Color;
        int err = 0;
        for (auto& x : testContainer()) {
            if (config().print_strat == PrintStrategy::All) {
                if (s_current_test != nullptr && x.filename != s_current_test->filename) {
                    std::cout << std::endl;
                }
                std::cout << BRIGHT << FG_MAGENTA << UNDERLINE
                    << x.filename << ":"
                    << RESET;
                std::cout << std::endl;
            }

            s_current_test = &x;

            try {
                x.depth = 0;
                executeLower(x);
            }
            catch(...) {
                std::cout << "Non recognized exception" << std::endl;
            }
            std::cout << Color::RESET;
            auto terr = traverse(x, config().print_strat);
            if (terr > err) err = terr;
        }
        s_current_test = nullptr;
        if (err == 0)
            std::cout << Color::BRIGHT << Color::FG_GREEN << "All tests successful." << std::endl;
        return err;
    }
};
