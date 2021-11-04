/* Stack.cpp 泛型栈的实现TODO */
#include "Stack.h"
using namespace pre;

template <typename T>
Stack<T>::Stack()
{
    bottom = new Node<T>;
    top = bottom;
}

template <typename T>
Stack<T>::Stack(const Stack &s)
{
    Node<T> *sp = s.top;
    Node<T> *tmp;
    bottom = top = new Node<T>(*s.top);
    while (sp != s.bottom)
    {
        sp = sp->pre;
        tmp = new Node<T>(*sp);
        bottom->pre = tmp;
        bottom = bottom->pre;
    }
}

template <typename T>
Stack<T> &Stack<T>::operator=(const Stack<T> &s)
{
    if (&s == this)
        return;
    s.~Stack();
    /* TODO */
}

template <typename T>
Stack<T>::~Stack()
{
    Node<T> *ptr = top;
    while (ptr != bottom)
    {
        Node<T> *ptr = top->pre;
        delete top;
        top = ptr;
    }
    delete bottom;
    bottom = ptr = NULL;
}