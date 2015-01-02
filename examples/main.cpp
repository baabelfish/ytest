#include "../ytest.hpp"

Ytest([]{
    test("A", []{
        test("AA", []{
            test("AAA", []{
                AreEqual(5, 5);
                IsTrue(true);
            });
        });
    });
    test("B", []{
        test("BA", []{
            test("BAA", []{
                IsFalse(true);
            });
        });
    });
});

EXEC()
