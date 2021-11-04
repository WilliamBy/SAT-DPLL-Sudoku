/* 队列模板 */
#include "../LinkList/LinkList.hpp"

template <typename T>
class Queue
{
    LinkList<T> l;  //队列的链表实现

public:
    int length;

    Queue();
    Queue(const Queue &q);
    ~Queue();
    Queue<T> &operator= (const Queue &q);  //深拷贝赋值

    T &enter(const T &val);    //入队，返回队尾元素引用
    T leave(); //出队，返回出队元素副本
};

template <typename T>
Queue<T>::Queue(): l()
{
    length = 0;
}

template <typename T>
Queue<T>::Queue(const Queue<T> &q): l(q)
{
    length = q.length;
}

template <typename T>
Queue<T>::~Queue()
{
    length = 0;
}

template <typename T>
Queue<T> &Queue<T>::operator= (const Queue &q)
{
    l = q.l;
    length = q.length;
    return *this;
}

template <typename T>
T &Queue<T>::enter(const T &val)
{
    l.addNode(val);
    return *this;
}

template <typename T>
T Queue<T>::leave()
{
    if (l.isEmpty()) return T();
    l.deleteByIndex(0);
}