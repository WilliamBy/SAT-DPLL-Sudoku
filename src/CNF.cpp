/* CNF.cpp 合取范式类实现-基于邻接表和计数器 */
#include "CNF.h"

CNF::CNF()
{
    clsLitNum = NULL; //子句总文字数列表（0索引，余同）
    clsDisNum = NULL; //子句不满足文字数列表
    clsSatNum = NULL; //子句已满足文字数列表
    varNum = 0;
    clsNum = 0;
    satClsNum = 0;
    stat = UN;
    adjNeg = NULL;
    adjPos = NULL;
    vars = NULL;
}

CNF::CNF(const CNF &cnf)
{
    stat = cnf.stat;
    varNum = cnf.varNum;

    clsNum = cnf.clsNum;
    satClsNum = cnf.satClsNum;
    clsLitNum = new int[clsNum + extra];
    clsDisNum = new int[clsNum + extra];
    clsSatNum = new int[clsNum + extra];
    for (int i = 0; i < clsNum + extra; i++)
    {
        clsLitNum[i] = cnf.clsLitNum[i];
        clsDisNum[i] = cnf.clsDisNum[i];
        clsSatNum[i] = cnf.clsSatNum[i];
    }

    vars = new Status[varNum + extra];
    adjPos = new LinkList<int>[varNum + extra];
    adjNeg = new LinkList<int>[varNum + extra];
    for (int i = 0; i < varNum + extra; i++)
    {
        vars[i] = cnf.vars[i];
        adjPos[i] = cnf.adjPos[i];
        adjNeg[i] = cnf.adjNeg[i];
    }
}

CNF::~CNF()
{
    if (vars)
        delete[] vars;
    if (clsLitNum)
        delete[] clsLitNum;
    if (clsDisNum)
        delete[] clsDisNum;
    if (clsSatNum)
        delete[] clsSatNum;
    if (adjNeg)
        delete[] adjNeg;
    if (adjPos)
        delete[] adjPos;
    clsLitNum = NULL; //子句总文字数列表（0索引，余同）
    clsDisNum = NULL; //子句不满足文字数列表
    clsSatNum = NULL; //子句已满足文字数列表
    varNum = 0;
    clsNum = 0;
    satClsNum = 0;
    stat = UN;
    adjNeg = NULL;
    adjPos = NULL;
    vars = NULL;
}

CNF &CNF::operator=(const CNF &cnf)
{
    if (this == &cnf)
        return *this;
    this->~CNF();
    stat = cnf.stat;
    varNum = cnf.varNum;

    clsNum = cnf.clsNum;
    satClsNum = cnf.satClsNum;
    clsLitNum = new int[clsNum + extra];
    clsDisNum = new int[clsNum + extra];
    clsSatNum = new int[clsNum + extra];
    for (int i = 0; i < clsNum + extra; i++)
    {
        clsLitNum[i] = cnf.clsLitNum[i];
        clsDisNum[i] = cnf.clsDisNum[i];
        clsSatNum[i] = cnf.clsSatNum[i];
    }

    vars = new Status[varNum + extra];
    adjPos = new LinkList<int>[varNum + extra];
    adjNeg = new LinkList<int>[varNum + extra];
    for (int i = 0; i < varNum + extra; i++)
    {
        vars[i] = cnf.vars[i];
        adjPos[i] = cnf.adjPos[i];
        adjNeg[i] = cnf.adjNeg[i];
    }
    return *this;
}