/* Node.hpp 节点模板 */

/* 节点申明 */
template <typename T>
class Node
{
public:
    Node<T> *next; //后继元素
    T item;        //节点内容
    Node();
    Node(const Node<T> &n);
    Node(const T &val); //按值构造
    Node<T> &operator= (const Node<T> &n);  //深拷贝赋值
};

/* 节点实现 */
template <typename T>
Node<T>::Node() : item()
{
    next = NULL;
}

template <typename T>
Node<T>::Node(const Node<T> &n) : item(n.item)
{
    next = n.next;
}

template <typename T>
Node<T>& Node<T>::operator= (const Node<T> &n)
{
    item = n.item;
    next = n.next;
    return *this;
}

template <typename T>
Node<T>::Node(const T &val) : item(val)
{
    next = NULL;
} //按值构造