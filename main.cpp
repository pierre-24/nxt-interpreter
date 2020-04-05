#include <iostream>

#include "ExecutionContext.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    auto* executionContext = new ExecutionContext("/home/pierre/code/nxt-interpreter/tests/simplecircle.rxe");
    return 0;
}
