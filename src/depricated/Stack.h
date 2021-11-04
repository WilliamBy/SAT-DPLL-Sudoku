/* Stack.h 泛型栈的定义 */

/* 节点类 */
namespace pre
{
    template <typename T>
    class Node
    {
        Node<T> *pre; //前驱元素
        T item;       //节点内容
        friend class Stack;

    public:
        Node() : item()
        {
            pre = NULL;
        }
        Node(const Node<T> &n) : item(n.item)
        {
            pre = n.pre;
        }
    };
}

/* 栈类 */
using namespace pre;
template <typename T>
class Stack
{
    Node<T> *bottom, *top; //链表实现
    Stack();
    ~Stack();
    Stack(const Stack<T> &s);               //深拷贝构造
    Stack<T> &operator=(const Stack<T> &s); //深拷贝赋值
public:
    bool isEmpty(); //判空
    bool push(T v); //压栈
    bool pop(T &v); //出栈
    T getTop();     //返回栈顶元素
};