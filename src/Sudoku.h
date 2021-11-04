/* Sudoku.h 二进制数独求解类 */
#include "SATSolverDPLL.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class Sudoku
{
    SATSolverDPLL solver;
    string cnfFilePath;  //生成的.cnf文件路径
    int degree; //阶数
    int board[50][50]; //棋盘
    int cnfGen();   //根据棋盘生成.cnf文件
    int map[50][50]; //存储映射
    int mapper(int i, int j);   //行列-变量映射
    string discoder(int var);   //解码
    void combIterPos(int flag, int rc, int s, int k, string pre, ostream &os);  // 组合输出(正)
    void combIterNeg(int flag, int rc, int s, int k, string pre, ostream &os);  // 组合输出(负)
    int clsNum;

public:
    Sudoku();
    Sudoku(int d, int l);
    void initialize(int d, int l); //随机生成可解盘局
    int solve();    //求解盘局
    void showIniBoard(); //显示初始棋盘
    void showResBoard(); //显示结果棋盘
    void demo();    //演示模块
};