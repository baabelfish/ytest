#include "../ytest.hpp"

Ytest([]{
    test("This is", []{
        test("a nestd", []{
            test("test", []{
                IsFalse(0);
            });
        });
        test("second", []{
            test("test", []{
                IsFalse(15 == 15);
            });
            test("test", []{
                IsFalse(15 == 15);
            });
        });
    });
});
