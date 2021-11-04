/* SATSovlerDPLL.h 基于DPLL的SAT问题求解器 */
#include "CNF.h"
using namespace std;

class SATSolverDPLL 
{   
    friend class Sudoku;
    const int fileNameMax = 100;
    char input[100];   //输入文件名
    char output[100];  //输出文件名
    string result;  //输出结果字符串
    double dpllTime;    //DPLL 时间
    CNF *org;      //初始cnf
    CNF *res;      //结果cnf
    LinkList<int> *clses;    //文字链表（不参与递归）
    float negExp[100];   //2的-i次幂，用于jw方法

    bool parseCNF(); //.cnf文件解析为CNF对象，初始化clses和org数据成员
    Status applyAssign(CNF &cnf, int var, Status val); //前置条件：传入cnf，变量以及变量赋值，后置条件：更新cnf邻接表、计数器、变量值，返回cnf状态
    int unitSearch(CNF &cnf);   //寻找单元子句，返回要满足的文字，若返回0则代表未找到单元子句
    void hideCls(CNF &cnf, int cls);    //子句消隐
    int decide(CNF &cnf);   //决定下一个要满足的文字，返回0表示子句均消隐
    Status DPLL(CNF cnf); //DPLL过程，传入CNF对象，返回结果


public:
    SATSolverDPLL();    //构造函数
    SATSolverDPLL(const char *infile, const char *outfile);
    ~SATSolverDPLL();

    bool initialize();  //初始化
    bool initialize(const char *infile, const char *outfile);

    bool execDPLL();    //执行DPLL，返回结果
    void showResult();  //回显结果
    void saveResult();  //保存结果
};