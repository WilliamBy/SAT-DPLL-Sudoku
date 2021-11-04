/* SATSovlerDPLL.h 基于DPLL的SAT问题求解器实现 */
#include "SATSolverDPLL.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstring>
#include <cmath>
#include <algorithm>

#pragma warning(disable : 4996)

/**
 * @brief Construct a new SATSolverDPLL::SATSolverDPLL object
 * 
 */
SATSolverDPLL::SATSolverDPLL()
{
    org = NULL;
    res = NULL;
    clses = NULL;
}

/**
 * @brief Construct a new SATSolverDPLL::SATSolverDPLL object   
 * @post 预设输入输出路径
 * @param infile 
 * @param outfile 
 */
SATSolverDPLL::SATSolverDPLL(const char *infile, const char *outfile)
{
    org = NULL;
    res = NULL;
    clses = NULL;
    strcpy(input, infile);
    strcpy(output, outfile);
    for (int i = 0; i < 100; i++)
    {
        negExp[i] = 0;
    }
}

/**
 * @brief Destroy the SATSolverDPLL::SATSolverDPLL object
 */
SATSolverDPLL::~SATSolverDPLL()
{
    if (clses)
        delete[] clses;
    if (org)
        delete org;
    if (res)
        delete res;
    org = res = NULL;
    clses = NULL;
}

/**
 * @brief 将.cnf文件内容解析成 CNF 对象, 更新 org 和 clses
 * @return  bool - 表征解析是否顺利
 * @retval  true - 成功
 * @retval  false - 失败
 */
bool SATSolverDPLL::parseCNF()
{
    ifstream ifs(input);
    if (!ifs)
    {
        cerr << "the file: " << input << " can't be opened!" << endl;
        ifs.close();
        return false;
    }

    char c;
    char str[1000];

    while (true)
    {
        ifs >> c;
        if (c == 'c') // 注释行忽略
        {
            ifs.getline(str, 1000);
        }
        else // 总体信息行
        {
            ifs >> str;
            break;
        }
    }
    org = new CNF();
    org->stat = UN;
    ifs >> org->varNum;
    ifs >> org->clsNum;
    org->vars = new Status[org->varNum + org->extra];
    for (int i = 0; i < org->varNum + org->extra; i++)
    {
        org->vars[i] = UN;
    }

    org->satClsNum = 0;
    org->clsLitNum = new int[org->clsNum + org->extra];
    org->clsDisNum = new int[org->clsNum + org->extra];
    org->clsSatNum = new int[org->clsNum + org->extra];
    for (int i = 0; i < org->clsNum + org->extra; i++)
    {
        org->clsLitNum[i] = org->clsDisNum[i] = org->clsSatNum[i] = 0;
    }

    clses = new LinkList<int>[org->clsNum + org->extra];
    org->adjPos = new LinkList<int>[org->varNum + org->extra];
    org->adjNeg = new LinkList<int>[org->varNum + org->extra];
    int num = 0;
    for (int i = 0; i < org->clsNum; i++) //按行读取每个子句的信息
    {
        while (true)
        {
            ifs >> num;
            if (num < 0) //负文字
            {
                clses[i].addNode(num);        //文字加入对应子句的文字链表
                org->adjNeg[-num].addNode(i); //子句加入对应负文字的邻接表
                org->clsLitNum[i]++;          //子句总文字数递增
            }
            else if (num > 0) //正文字
            {
                clses[i].addNode(num);       //文字加入对应子句的文字链表
                org->adjPos[num].addNode(i); //子句加入对应正文字的邻接表
                org->clsLitNum[i]++;         //子句总文字数递增
            }
            else
            {
                ifs.getline(str, 1000);
                break;
            } //换行符丢弃，开始读取下一行
        }
    }
    ifs.close();
    return true;
}

/**
 * @brief 进行 DPLL 过程并让 SATSolverDPLL::res 指向SAT-CNF对象
 * @param cnf - 进行 DPLL 的 CNF 对象
 * @return DPLL 过程结果
 * @retval SAT - 可满足
 * @retval DIS - 不可满足
 */
Status SATSolverDPLL::DPLL(CNF cnf)
{
    int litToSat = 0;
    Status stat = UN;
    while (true)
    {
        int litToSat = unitSearch(cnf); //查找单子句须被满足的文字
        if (litToSat == 0)              //没有发现单子句则分裂
        {
            litToSat = decide(cnf);
            if (litToSat == 0) //所有子句均已满足（全消隐）
            {
                cnf.stat = SAT;
                res = new CNF(cnf); //更新赋值后的CNF
                return SAT;
            }
            CNF cpy(cnf);     //cnf副本
            if (litToSat > 0) //先满足正文字
            {
                /* 进入分支一 */
                applyAssign(cnf, litToSat, SAT); //先令该正文字满足
                if (cnf.stat == SAT)             //该分支找到SAT解
                    return SAT;
                else if (cnf.stat == UN) //尚未得出结论则递归dpll过程
                {
                    if (DPLL(cnf) == SAT) //该分支找到SAT解
                        return SAT;
                }
                /* 分支一未找到满足解则进入分支二 */
                applyAssign(cpy, litToSat, DIS); //令该文字不满足
                if (cpy.stat == SAT)             //该分支找到SAT解
                    return SAT;
                else if (cpy.stat == DIS) //该分支不满足
                    return DIS;
                else //尚未得出结论则递归dpll过程
                    return DPLL(cpy);
            }
            else //先满足负文字
            {
                /* 进入分支一 */
                applyAssign(cnf, -litToSat, DIS); //先令该负文字满足
                if (cnf.stat == SAT)              //该分支找到SAT解
                    return SAT;
                else if (cnf.stat == UN) //尚未得出结论则递归dpll过程
                {
                    if (DPLL(cnf) == SAT) //该分支找到SAT解
                        return SAT;
                }
                /* 分支一未找到满足解则进入分支二 */
                applyAssign(cpy, -litToSat, SAT); //令该文字不满足
                if (cpy.stat == SAT)              //该分支找到SAT解
                    return SAT;
                else if (cpy.stat == DIS) //该分支不满足
                    return DIS;
                else //尚未得出结论则递归dpll过程
                    return DPLL(cpy);
            }
        }
        else //发现单子句则令该文字满足
        {
            if (litToSat < 0)
                applyAssign(cnf, -litToSat, DIS);
            else
                applyAssign(cnf, litToSat, SAT);
            if (cnf.stat == SAT)
                return SAT;
            else if (cnf.stat == DIS)
                return DIS;
            else
                continue; //继续执行BCP
        }
    }
}

/**
 * @brief 隐藏子句
 * @param cnf - 进行 子句隐藏 的 CNF 对象
 * @param cls - 要隐藏的子句的序列号
 */
void SATSolverDPLL::hideCls(CNF &cnf, int cls)
{
    if (cls >= cnf.clsNum)
        exit(1);
    Node<int> *ptr = clses[cls].l->next;
    int lit;
    while (ptr != NULL)
    {
        lit = ptr->item;
        if (lit > 0)
            cnf.adjPos[lit].deleteByItem(cls);
        else if (lit < 0)
            cnf.adjNeg[-lit].deleteByItem(cls);
        else
            exit(1);
        ptr = ptr->next;
    }
}

/**
 * @brief 对变量应用赋值，更新数据结构（cnf，clses）
 * @param cnf - CNF 对象
 * @param var - 要赋值的变量名
 * @param val - 要赋的值
 * @return Status - 返回赋值后CNF的状态
 * @retval SAT - 满足
 * @retval DIS - 不满足
 * @retval UN - 未得出结论
 */
Status SATSolverDPLL::applyAssign(CNF &cnf, int var, Status val)
{
    cnf.vars[var] = val;
    int cls = -1; //存储子句序号
    Node<int> *ptr;
    if (val == SAT)
    {
        /* 正文字 */
        ptr = cnf.adjPos[var].l->next;
        while (ptr != NULL)
        {
            cls = ptr->item;
            ptr = ptr->next;      //指向下一条子句
            hideCls(cnf, cls);    //已满足子句隐藏
            cnf.clsSatNum[cls]++; //已满足文字计数器递增
            /* 已满足子句数递增并检查CNF是否已经满足 */
            if (cnf.clsNum == ++cnf.satClsNum)
            {
                cnf.stat = SAT;
                res = new CNF(cnf); //更新赋值后的CNF
                return SAT;
            }
        }
        /* 负文字 */
        ptr = cnf.adjNeg[var].l->next;
        while (ptr != NULL)
        {
            /* 不满足文字计数器递增并检查子句是否不再满足 */
            if (cnf.clsLitNum[ptr->item] == ++(cnf.clsDisNum[ptr->item]))
            {
                cnf.stat = DIS;
                return DIS;
            }
            ptr = ptr->next;
        }
    }
    else
    {
        /* 正文字 */
        ptr = cnf.adjPos[var].l->next;
        while (ptr != NULL)
        {
            /* 不满足文字计数器递增并检查子句是否不再满足 */
            if (cnf.clsLitNum[ptr->item] == ++(cnf.clsDisNum[ptr->item]))
            {
                cnf.stat = DIS;
                return DIS;
            }
            ptr = ptr->next;
        }
        /* 负文字 */
        ptr = cnf.adjNeg[var].l->next;
        while (ptr != NULL)
        {
            cls = ptr->item;
            ptr = ptr->next;      //指向下一条子句
            hideCls(cnf, cls);    //已满足子句隐藏
            cnf.clsSatNum[cls]++; //已满足文字计数器递增
            /* 已满足子句数递增并检查CNF是否已经满足 */
            if (cnf.clsNum == ++cnf.satClsNum)
            {
                cnf.stat = SAT;
                res = new CNF(cnf); //更新赋值后的CNF
                return SAT;
            }
        }
    }
    return UN;
}

/**
 * @brief 查找单子句
 * @param cnf - 进行 DPLL 的 CNF 对象
 * @return int - 找到的单子句中未赋值的变量
 * @retval 0 - 未找到单元子句
 */
int SATSolverDPLL::unitSearch(CNF &cnf)
{
    for (int i = 0; i < cnf.clsNum; i++)
    {
        if (cnf.clsDisNum[i] == cnf.clsLitNum[i] - 1 && cnf.clsSatNum[i] == 0) //单元子句
        {
            for (int j = 0; j < cnf.clsLitNum[i]; j++)
            {
                Node<int> *ptr = clses[i].l->next;
                while (ptr != NULL && cnf.vars[abs(ptr->item)] != UN)
                {
                    ptr = ptr->next;
                }
                if (ptr == NULL)
                    continue; //忽略已满足子句
                else
                    return ptr->item;
            }
        }
    }
    return 0; //未找到单元子句
}

/**
 * @brief 变量决策 - JW 方法
 * @param cnf 要决定变量的 CNF 对象
 * @return int - 决策文字
 * @retval 0 - 未找到未赋值的变量
 * ! 默认句长最大不超过100
 */
int SATSolverDPLL::decide(CNF &cnf)
{
    /* JW决策法 */
    double maxWeight = 0, weight = 0;
    int litToSat = 0;
    Node<int> *ptr = NULL;
    int k; //句长
    for (int i = 1; i <= cnf.varNum; i++)
    {
        if (cnf.vars[i] == UN)
        {
            ptr = cnf.adjPos[i].l->next;
            weight = 0;
            while (ptr != NULL)
            {
                k = clses[ptr->item].length;
                if (negExp[k] > 0)
                    weight += negExp[k];
                else
                    negExp[k] = pow(2, -k);
                ptr = ptr->next;
            }
            if (weight > maxWeight)
            {
                maxWeight = weight;
                litToSat = i;
            }

            ptr = cnf.adjNeg[i].l->next;
            weight = 0;
            while (ptr != NULL)
            {
                k = clses[ptr->item].length;
                if (negExp[k] > 0)
                    weight += negExp[k];
                else
                    negExp[k] = pow(2, -k);
                ptr = ptr->next;
            }
            if (weight > maxWeight)
            {
                maxWeight = weight;
                litToSat = -i;
            }
        }
    }
    return litToSat;
}

/**
 * @brief 初始化求解器
 * @post 更新CNF::org对象
 * @return true 
 * @return false 
 */
bool SATSolverDPLL::initialize()
{
    if (input == NULL)
    {
        cerr << "input file path need to be accord" << endl;
        return false;
    }
    if (org) delete org;
    if (res) delete res;
    if (clses) delete[] clses;
    return parseCNF();
}

/**
 * @brief 初始化求解器
 * @post 设定输入输出，更新CNF::org对象
 * @param infile 输入文件路径
 * @param outfile 输出文件路径
 * @return true 
 * @return false 
 */
bool SATSolverDPLL::initialize(const char *infile, const char *outfile)
{
    strcpy(input, infile);
    strcpy(output, outfile);
    return initialize();
}

/**
 * @brief 执行DPLL过程
 * @pre cnf解析已完成
 * @post 更新result字符串
 * @return true 
 * @return false 
 */
bool SATSolverDPLL::execDPLL()
{
    clock_t start, end;
    start = clock(); //记录开始时间
    Status stat = DPLL(*org);
    end = clock(); //记录结束时间
    dpllTime = (double)(end - start) / CLOCKS_PER_SEC;
    ostringstream oss; //字符串流
    if (stat == DIS)
    {
        oss << "s 0\nv\nt " << dpllTime;
        result = oss.str();
        res = new CNF();
        res->stat = DIS;
        return false;
    }
    else if (stat == SAT)
    {
        oss << "s 1\nv";
        for (int i = 1; i <= res->varNum; i++)
        {
            if (res->vars[i] == SAT)
                oss << " " << i;
            else// if (res->vars[i] == DIS)
                oss << " " << -i;
            // else
            //     oss << " " << 0; // NOTE: 随意赋值
        }
        oss << "\nt " << dpllTime;
        result = oss.str();
        return true;
    }
    else
    {
        exit(1);
    }
}

/**
 * @brief 显示结果到标准输出
 */
void SATSolverDPLL::showResult()
{
    cout << result << endl;
}

/**
 * @brief 保存结果到预设的文件
 */
void SATSolverDPLL::saveResult()
{
    if (output == NULL)
    {
        cerr << "output file name hasn't be assigned!" << endl;
        return;
    }
    ofstream ofs(output);
    if (!ofs)
    {
        cerr << "can't open file: " << output << endl;
        return;
    }
    ofs << result;
    ofs.close();
}