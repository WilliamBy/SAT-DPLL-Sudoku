/* LinkList.hpp 单链表模板 */
#include <cstddef>
#include <iostream>
#include "Node.hpp"
using namespace std;

/* 单链表申明 */
template <typename T>
class LinkList
{
public:
    Node<T> *l, *tail;
    int length; //链表长度

    LinkList();
    LinkList(const LinkList<T> &L);
    ~LinkList();
    LinkList<T> &operator=(const LinkList<T> &L); //深拷贝赋值函数

    T &operator[](int index);                     //从0索引
    bool deleteByIndex(int index);                //删除节点（按索引）
    bool deleteByItem(T target);                  //删除节点（按值）
    bool addNode(const T &val);                   //追加节点
    int indexOf(const T &val);                    //查找值对应的索引
    bool isEmpty();                               //判空
    void traverse(void (*func)(T &val));          //遍历

    /* TEST */
    void testLinkList(const T &v1, const T &v2);
};

/* 单链表实现 */
template <typename T>
LinkList<T>::LinkList()
{
    tail = l = new Node<T>();
    length = 0;
}

template <typename T>
LinkList<T>::LinkList(const LinkList<T> &L)
{
    length = L.length;
    l = new Node<T>();
    Node<T> *Lp = L.l->next, *p = l, *tmp;
    while (Lp != NULL)
    {
        p->next = new Node<T>(*Lp);
        p = p->next;
        Lp = Lp->next;
    }
    tail = p;
    p->next = NULL;
}

template <typename T>
LinkList<T>::~LinkList()
{
    length = 0;
    Node<T> *ptr = l, *tmp;
    while (ptr != NULL)
    {
        tmp = ptr;
        ptr = ptr->next;
        delete tmp;
    }
    l = NULL;
}

template <typename T>
bool LinkList<T>::addNode(const T &val)
{
    tail->next = new Node<T>(val);
    tail = tail->next;
    length++;
    return true;
}

template <typename T>
bool LinkList<T>::deleteByIndex(int index)
{
    Node<T> *ptr = l->next, *pre = l;
    for (int i = 0; i < index && ptr != NULL; i++)
    {
        pre = ptr;
        ptr = ptr->next;
    }
    if (ptr == NULL)
        return false;
    else if (ptr == tail)
        tail = pre;
    pre->next = ptr->next;
    delete ptr;
    length--;
    return true;
}

template <typename T>
T &LinkList<T>::operator[](int index)
{
    Node<T> *ptr = l->next;
    for (int i = 0; i < index && ptr != NULL; i++)
    {
        ptr = ptr->next;
    }
    if (ptr == NULL) return l->item;
    return ptr->item;
}

template <typename T>
int LinkList<T>::indexOf(const T &val)
{
    Node<T> *ptr = l->next;
    int index = 0;
    while (ptr != NULL && ptr->item != val)
    {
        ptr = ptr->next;
        index++;
    }
    if (ptr == NULL)
        return -1;
    else
        return index;
}
template <typename T>
LinkList<T> &LinkList<T>::operator=(const LinkList<T> &L)
{
    if (this == &L) return *this;

    length = 0;
    Node<T> *ptr = l, *tmp;
    while (ptr != NULL)
    {
        tmp = ptr;
        ptr = ptr->next;
        delete tmp;
    }
    l = NULL;

    length = L.length;
    l = new Node<T>();
    Node<T> *Lp = L.l->next, *p = l;
    while (Lp != NULL)
    {
        p->next = new Node<T>(*Lp);
        p = p->next;
        Lp = Lp->next;
    }
    tail = p;
    p->next = NULL;
    return *this;
}

template <typename T>
bool LinkList<T>::isEmpty()
{
    return (length == 0) ? true : false;
}

template <typename T>
void LinkList<T>::testLinkList(const T &v1, const T &v2)
{
    addNode(v1);
    addNode(v2);
    addNode(v1);
    addNode(v1);
    addNode(v1);
    cout << operator[](1) << ' ' << operator[](2) << endl;
    cout << "debug0" << endl;
    LinkList<T> tmp(*this);
    cout << "debug1" << endl;
    deleteByIndex(1);
    cout << "debug2" << endl;
    cout << operator[](1) << ' ' << operator[](2) << endl;
    cout << tmp[1] << ' ' << tmp[2];
}

template <typename T>
void LinkList<T>::traverse(void (*func)(T &val))
{
    Node<T> ptr = l->next;
    while (ptr != NULL)
    {
        func(ptr->item);
        ptr = ptr->next;
    }
}

template <typename T>
bool LinkList<T>::deleteByItem(T target)
{
    Node<T> *pre = l, *ptr = l->next;
    while (ptr != NULL && ptr->item != target)
    {
        ptr = ptr->next;
        pre = pre->next;
    }
    if (ptr == NULL) return false;
    pre->next = ptr->next;
    delete ptr;
    return true;
}