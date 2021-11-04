#include "Stack.hpp"
#include <iostream>
using namespace std;

int main()
{
    Stack<int> s1, s2(100);
    int tmp = -1;
    int *q;
    cout << s1.isEmpty() << endl;
    cout << s1.push(1) << " " << s1.push(2) << " " << s1.push(3) << endl;
    cout << s1.isEmpty() << " " << s1.isFull() << endl;
    s1.pop(tmp);
    cout << tmp << " " << s1.getTop() << endl;
    s1.getTop() = 10; s1.pop(tmp);
    cout << tmp << " " << s1.getTop()<< " " << s1.length;
    return 0;
}