/* STtack.hpp 线性栈模板 */
#include <cstring>
#include <cstddef>

/* 线性栈申明 */
template <typename T>
class Stack
{
    const int defaultCapacity = 1000;   //栈的缺省最大容量
    const int extra = 5;    //保险空间大小
    T* container;   //容器，指向一片连续的存储空间的起始地址
    int top;    //栈顶 (zero indexed)
    int cap;    //容量大小
    
public:
    int length; //栈长度 

    Stack();
    Stack(const Stack<T> &s);
    Stack(int cap); //按最大容量创建栈
    ~Stack();
    Stack<T>& operator= (const Stack<T> &s);    //深拷贝赋值

    bool push(const T &val); //压栈
    T pop();   //出栈
    T& getTop(); //获得栈顶元素地址
    bool isEmpty(); //判空
    bool isFull();  //判满
};

/* Stack 类的实现 */
template <typename T>
Stack<T>::Stack()
{
    container = new T[defaultCapacity + extra];
    cap = defaultCapacity;
    top = length = 0;
}

template <typename T>
Stack<T>::Stack(const Stack<T> &s)
{
    cap = s.cap;
    length = s.length;
    container = new T[cap + extra];
    for (int i = 0; i < length; i++)
    {
        container[i] = s.container[i];
    }
    top = s.top;
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack<T> &s)
{
    if (this == &s) return *this;

    delete[] container;
    container = NULL;
    length = top = cap = 0;

    cap = s.cap;
    length = s.length;
    container = new T[cap + extra];
    for (int i = 0; i < length; i++)
    {
        container[i] = s.container[i];
    }
    top = s.top;
}

template <typename T>
Stack<T>::Stack(int c)
{
    container = new T[c + extra];
    cap = c;
    top = length = 0;
}

template <typename T>
Stack<T>::~Stack()
{
    delete[] container;
    container = NULL;
    length = top = cap = 0;
}

template <typename T>
bool Stack<T>::push(const T &val)
{
    if (isFull()) return false;
    container[++top] = val;
    length++;
    return true;
}

template <typename T>
T Stack<T>::pop()
{
    T res;
    if(isEmpty()) return T();
    length--;
    return container[top--];
}

template <typename T>
T& Stack<T>::getTop()
{
    return container[top];
}

template <typename T>
bool Stack<T>::isEmpty()
{
    if (top == 0) return true;
    else return false;
}

template <typename T>
bool Stack<T>::isFull()
{
    if (length >= cap) return true;
    else return false;
}