#include <algorithm>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <filesystem>
#include <memory>
#include <map>
#include <set>
#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp> // Include for boost::split
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptors.hpp>
#include <chrono>
#include <thread>
#include <compare>
#include <initializer_list>
#include <ctime>
#include <concepts>

using namespace std;

struct Data
{
    string txt = "";
    long n = 0;
};

struct StackElement
{
    Data data;
    StackElement *under = nullptr;
};

struct Stack
{
    unsigned long cnt = 0;
    StackElement *peak = nullptr;
};

void show(StackElement *stackElement)
{
    if(nullptr != stackElement)
    {
        cout << "txt: " << stackElement->data.txt << ", n: " << stackElement->data.n << endl;
    }
}

void showPeak(Stack &stack)
{
    if(stack.cnt)
    {
        show(stack.peak);
    }
    else
    {
        cout << "\nNo elements on stack\n";
    }
}

void showAll(Stack &stack)
{
    StackElement *stackElement = stack.peak;
    while(nullptr != stackElement)
    {
        show(stackElement);
        stackElement = stackElement->under;
    }
}

void addToStack(Stack &stack, const Data &data)
{
    StackElement *newElement = new StackElement();
    newElement->data = data;
    newElement->under = stack.peak;
    stack.peak = newElement;
    stack.cnt++;
    // TODO: Add incrementing in constructor
}

void deleteFromStack(Stack &stack)
{
    if(stack.cnt)
    {
        StackElement *newElement = stack.peak->under;
        delete stack.peak;
        stack.cnt--;
        stack.peak = newElement;
    }
}

void copyStack(Stack &stack)
{
    if(stack.cnt)
    {
        StackElement *newElement = new StackElement();
        newElement->data.n = stack.peak->data.n;
        newElement->data.txt = stack.peak->data.txt;
        newElement->under = stack.peak;
        stack.peak = newElement;
        stack.cnt++;
    }
}

int main()
{
    setlocale(LC_ALL, "");
    srand(time(0));
    
    Stack stack{};
    addToStack(stack, {"Robert", 100});
    addToStack(stack, {"Kate", 120});
    addToStack(stack, {"Ben", 90});
    showPeak(stack);
    cout << stack.cnt << endl;
    showAll(stack);
    deleteFromStack(stack);
    cout << stack.cnt << endl;
    showAll(stack);
    copyStack(stack);
    cout << stack.cnt << endl;
    showAll(stack);


    return EXIT_SUCCESS;
}
