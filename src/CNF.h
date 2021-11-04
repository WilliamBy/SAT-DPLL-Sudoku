/* CNF.h 合取范式类-基于邻接表和计数器 */
#include "LinkList/LinkList.hpp"
#include "Stack/Stack.hpp"
#include <cstdlib>
#include <cstring>

/* 状态枚举量，用于描述子句和文字、变量状态 */
enum Status
{
    UN, //未配值，初始状态
    SAT, //满足
    DIS, //不满足
    UNIT, //单元子句
};

class CNF
{
public:
    const int extra = 5;
    int varNum; //变量数
    Status stat;    //CNF是否满足
    Status *vars;   //变量值列表

    int clsNum; //子句数
    int *clsLitNum;  //子句总文字数列表（0索引，余同）
    int *clsDisNum; //子句不满足文字数列表
    int *clsSatNum; //子句已满足文字数列表
    int satClsNum;  //已满足子句数

    LinkList<int> *adjPos;  //正文字邻接表
    LinkList<int> *adjNeg;  //负文字邻接表
    CNF();
    CNF(const CNF &cnf);    //深拷贝构造函数
    ~CNF();

    CNF &operator= (const CNF &cnf);    //深拷贝赋值
};