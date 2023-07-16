#include <iostream>
#include "demo.h"
#include "CPPDemo.h"
// #include "array.h"

using namespace std;

// g++ -pthread -std=c++17   -o main main.cpp demo.cpp CPPDemo.cpp
// g++ -pthread -std=c++20   -o main main.cpp demo.cpp CPPDemo.cpp
// g++ -pthread -std=c++2b   -o main main.cpp demo.cpp CPPDemo.cpp
// g++ -pthread -std=gnu++2b -o main main.cpp demo.cpp CPPDemo.cpp
int main()
{
    // DemoParameters();
    //DemoArray();
    //DemoIterators();
    //DemoReverseIterators();
    DemoIteratorsWithoutInheritance();
    // DemoBinaryTree();
    // DemoHash();

    // DemoPreandPostIncrement();
    // DemoSmartPointers();
    return 0;
}