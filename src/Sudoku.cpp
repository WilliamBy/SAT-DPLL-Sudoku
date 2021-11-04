/* Sudoku.cpp 二进制数独类实现 */
#include "Sudoku.h"
#include <cstdlib>
#include <ctime>

/**
 * @brief Construct a new Sudoku:: Sudoku object
 * @post 生成默认初始盘局(阶数、棋盘、中间输出路径初始化), 设定默认中间文件输出路径
 */
Sudoku::Sudoku() : solver("mid/input.cnf", "mid/output.res"), cnfFilePath("mid/input.cnf")
{
    degree = 8;
    for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 8; j++)
        {
            board[i][j] = -1;
        }
    board[1][2] = 0;
    board[2][4] = 1;
    board[2][6] = 1;
    board[2][8] = 0;
    board[3][3] = 0;
    board[4][2] = 1;
    board[5][5] = 1;
    board[6][2] = 0;
    board[6][6] = 1;
    board[7][2] = 0;
    board[7][5] = 0;
    board[8][5] = 0;
    board[8][7] = 0;
}

/**
 * @brief Construct a new Sudoku:: Sudoku object
 * @param d 阶数
 * @param l 难度系数
 */
Sudoku::Sudoku(int d, int l) : solver("mid/input.cnf", "mid/output.res"), cnfFilePath("mid/input.cnf")
{
    degree = d;
    initialize(d, l);
}

/**
 * @brief 随机生成可解盘局
 * @param d 阶数
 * @param l 难度系数
 */
void Sudoku::initialize(int d, int l)
{
    d = d >= 4 ? d + d % 2 : 4;
    degree = d;
    /* 产生可满足终局 */
    srand(time(NULL));
    while (1)
    {
        int n = rand() % (d * d) + 1; //填入n个数
        for (int i = 1; i <= d; i++)
            for (int j = 1; j <= d; j++)
            {
                board[i][j] = -1;
            }
        for (int i = 0; i < n; i++)
        {
            board[rand() % d + 1][rand() % d + 1] = rand() % 2;
        }
        cnfGen();
        solver.initialize();
        if (solver.execDPLL())
            break;
        else
            continue;
    }
    for (int i = 1; i <= degree; i++)
    {
        for (int j = 1; j <= degree; j++)
        {
            board[i][j] = (solver.res->vars[mapper(i, j)] == SAT) ? 1 : 0;
        }
    }
    /* 挖洞 */
    srand(time(NULL));
    if (l != 1 || l != 3) l = 2;
    for (int i = 1; i <= degree; i++)
    {
        for (int j = 1; j <= degree; j++)
        {
            if (rand() % (l + 1) == 0) 
                board[i][j] = -1;
        }
    }
    return;
}

/**
 * @brief 行列-变量映射
 * @pre map数组已填充
 * @param i 
 * @param j 
 * @return int 
 */
int Sudoku::mapper(int i, int j)
{
    return map[i][j];
}

/**
 * @brief 输出（到流os）自第s + 1行/列开始往后的所有k组合正文字子句, 其中输出每一组合前要输出一次pre
 * @pre degree、board已经初始化 
 * @post 输出该行/列根据约束2产生的所有纯正子句到os，同时更新clsNum    
 * @param flag 1表示行，0表示列
 * @param rc 行/列号
 * @param s 开始点
 * @param k k组合
 * @param pre 前缀输出
 * @param os 输出流对象
 */
void Sudoku::combIterPos(int flag, int rc, int s, int k, string pre, ostream &os)
{
    if (s + k > degree)
    {
        cerr << "overflow!" << endl; // ANCHOR: 递归枚举组合函数异常
        exit(1);
    }
    if (k == 0)
    {
        os << pre << "0" << endl; //结束一行
        clsNum++;
        return;
    }
    if (flag == 1) //行
    {
        for (int i = s + 1; i + k - 1 <= degree; i++)
        {
            ostringstream oss;
            oss << pre << mapper(rc, i) << " ";
            combIterPos(flag, rc, i, k - 1, oss.str(), os); //递归
        }
    }
    else //列
    {
        for (int i = s + 1; i + k - 1 <= degree; i++)
        {
            ostringstream oss;
            oss << pre << mapper(i, rc) << " ";
            combIterPos(flag, rc, i, k - 1, oss.str(), os); //递归
        }
    }
}

/**
 * @brief 输出（到流os）自第s + 1行/列开始往后的所有k组合负文字子句, 其中输出每一组合前要输出一次pre
 * @pre degree、board已经初始化
 * @post 输出该行/列根据约束2产生的所有纯负子句到os，同时更新clsNum    
 * @param flag 1表示行，0表示列
 * @param rc 行/列号
 * @param s 开始点
 * @param k k组合
 * @param pre 前缀输出
 * @param os 输出流对象
 */
void Sudoku::combIterNeg(int flag, int rc, int s, int k, string pre, ostream &os)
{
    if (s + k > degree)
    {
        cerr << "overflow!" << endl; // ANCHOR: 递归枚举组合函数异常
        exit(1);
    }
    if (k == 0)
    {
        os << pre << "0" << endl; //结束一行
        clsNum++;
        return;
    }
    if (flag == 1)
    {
        for (int i = s + 1; i + k - 1 <= degree; i++)
        {
            ostringstream oss;
            oss << pre << -mapper(rc, i) << " ";
            combIterNeg(flag, rc, i, k - 1, oss.str(), os); //递归
        }
    }
    else
    {
        for (int i = s + 1; i + k - 1 <= degree; i++)
        {
            ostringstream oss;
            oss << pre << -mapper(i, rc) << " ";
            combIterNeg(flag, rc, i, k - 1, oss.str(), os); //递归
        }
    }
}

/**
 * @brief 根据盘局生成.cnf
 * @pre board已经初始化
 * @return int 
 * @retval -1 失败
 * @retval 0 成功
 */
int Sudoku::cnfGen()
{
    for (int i = 1; i <= degree; i++)
        for (int j = 1; j <= degree; j++)
            map[i][j] = (i - 1) * degree + j; //初始化映射数组
    ofstream ofs;
    ofs.open(cnfFilePath);
    ostringstream ossBuffer;
    if (!ofs)
    {
        cerr << "cnfFilePath hasn't be set!" << endl;
        return -1;
    }
    /* 说明行 */
    ofs << "c 棋盘对应的.cnf文件" << endl;
    clsNum = 0;
    /* 约束0：起始盘局约束 */
    for (int i = 1; i <= degree; i++)
    {
        for (int j = 1; j <= degree; j++)
        {
            if (board[i][j] == 1)
            {
                ossBuffer << mapper(i, j) << " 0" << endl;
                clsNum++;
            }
            else if (board[i][j] == 0)
            {
                ossBuffer << -mapper(i, j) << " 0" << endl;
                clsNum++;
            }
        }
    }
    /* 约束1：行or列里不出现连续的3个相同值 */
    for (int i = 1; i <= degree; i++)
    {
        for (int j = 1; j <= degree - 2; j++)
        {
            /* 行 */
            ossBuffer << mapper(i, j) << " "
                      << mapper(i, j + 1) << " "
                      << mapper(i, j + 2) << " 0" << endl;

            ossBuffer << -mapper(i, j) << " "
                      << -mapper(i, j + 1) << " "
                      << -mapper(i, j + 2) << " 0" << endl;
            /* 列 */
            ossBuffer << mapper(j, i) << " "
                      << mapper(j + 1, i) << " "
                      << mapper(j + 2, i) << " 0" << endl;

            ossBuffer << -mapper(j, i) << " "
                      << -mapper(j + 1, i) << " "
                      << -mapper(j + 2, i) << " 0" << endl;

            clsNum += 4;
        }
    }
    /* 约束2：同一行or列 0 1 个数一致 */
    for (int i = 1; i <= degree; i++)
    {
        /* 行 */
        combIterPos(1, i, 0, degree / 2 + 1, "", ossBuffer);
        combIterNeg(1, i, 0, degree / 2 + 1, "", ossBuffer);
        /* 列 */
        combIterPos(0, i, 0, degree / 2 + 1, "", ossBuffer);
        combIterNeg(0, i, 0, degree / 2 + 1, "", ossBuffer);
    }
    /* 约束3：不能有完全相同的两行/列 */
    int maxVar = degree * degree;
    int basic = maxVar;
    /* 两行约束 */
    for (int i = 1; i < degree; i++)
        for (int j = i + 1; j <= degree; j++)
        {
            basic = maxVar;
            /* 1ijk1 1ijk0 1ijk */
            for (int k = 1; k <= degree; k++)
            {
                int a = mapper(i, k); //ik
                int b = mapper(j, k); //jk
                maxVar++;             //1ijk1
                ossBuffer << a << " " << -maxVar << " 0" << endl;
                ossBuffer << b << " " << -maxVar << " 0" << endl;
                ossBuffer << -a << " " << -b << " " << maxVar << " 0" << endl;

                maxVar++; //1ijk0
                ossBuffer << -a << " " << -maxVar << " 0" << endl;
                ossBuffer << -b << " " << -maxVar << " 0" << endl;
                ossBuffer << a << " " << b << " " << maxVar << " 0" << endl;

                maxVar++;       //1ijk
                a = maxVar - 2; //1ijk1
                b = maxVar - 1; //1ijk0
                ossBuffer << -a << " " << maxVar << " 0" << endl;
                ossBuffer << -b << " " << maxVar << " 0" << endl;
                ossBuffer << a << " " << b << " " << -maxVar << " 0" << endl;
                clsNum += 9;
            }
            /* V-1ijk */
            for (int k = 1; k <= degree; k++)
                ossBuffer << -(basic + 3 * k) << " ";
            ossBuffer << " 0" << endl;
            clsNum++;
        }
    /* 两列约束 */
    for (int i = 1; i < degree; i++)
        for (int j = i + 1; j <= degree; j++)
        {
            basic = maxVar;
            /* 0ijk1 0ijk0 0ijk */
            for (int k = 1; k <= degree; k++)
            {
                int a = mapper(k, i); //ik
                int b = mapper(k, j); //jk
                maxVar++;             //0ijk1
                ossBuffer << a << " " << -maxVar << " 0" << endl;
                ossBuffer << b << " " << -maxVar << " 0" << endl;
                ossBuffer << -a << " " << -b << " " << maxVar << " 0" << endl;

                maxVar++; //0ijk0
                ossBuffer << -a << " " << -maxVar << " 0" << endl;
                ossBuffer << -b << " " << -maxVar << " 0" << endl;
                ossBuffer << a << " " << b << " " << maxVar << " 0" << endl;

                maxVar++;       //0ijk
                a = maxVar - 2; //0ijk1
                b = maxVar - 1; //0ijk0
                ossBuffer << -a << " " << maxVar << " 0" << endl;
                ossBuffer << -b << " " << maxVar << " 0" << endl;
                ossBuffer << a << " " << b << " " << -maxVar << " 0" << endl;
                clsNum += 9;
            }
            /* V-0ijk */
            for (int k = 1; k <= degree; k++)
                ossBuffer << -(basic + 3 * k) << " ";
            ossBuffer << " 0" << endl;
            clsNum++;
        }
    /* 总体信息 */
    ofs << "p cnf "
        << maxVar << " "
        << clsNum << endl;
    ofs << ossBuffer.str();
    ofs.close();
    return 0;
}

/**
 * @brief 可视化初始棋盘
 * 
 */
void Sudoku::showIniBoard()
{
    cout << "[Initial Puzzle]" << endl;
    string hole = " ";
    for (int i = 1; i <= degree; i++)
    {
        for (int j = 1; j <= degree; j++)
        {
            hole = (board[i][j] == -1) ? " " : to_string(board[i][j]);
            if (j != degree)
                cout << "|" << hole;
            else
                cout << "|" << hole << "|";
        }
        cout << endl;
    }
    cout << "\n"
         << endl;
    return;
}

/**
 * @brief 解码
 * @param var 
 * @return string
 * @retval "i j" - i 为行号， j为列号
 */
string Sudoku::discoder(int var) //
{
    int i = (var - 1) / degree + 1;
    int j = var - (i - 1) * degree;
    ostringstream oss;
    oss << i << " " << j;
    return oss.str();
}

/**
 * @brief 显示结果棋盘
 */
void Sudoku::showResBoard()
{
    if (solver.res)
    {
        if (solver.res->stat == SAT)
        {
            int val = 0; //填入棋盘的值
            cout << "[Solution]" << endl;
            for (int i = 1; i <= degree; i++)
            {
                for (int j = 1; j <= degree; j++)
                {
                    if (solver.res->vars[mapper(i, j)] == SAT)
                        val = 1;
                    else
                        val = 0;
                    if (j != degree)
                        cout << "|" << val;
                    else
                        cout << "|" << val << "|";
                }
                cout << endl;
            }
        }
        else
            cout << "The puzzle can't be solved:" << endl;
        showIniBoard();
    }
    else
        cout << "The puzzle hasn't be solved!" << endl;
    cout << endl;
    return;
}

/**
 * @brief 求解棋盘
 * @pre 棋局已经初始化
 * @post 回显结果
 * @return int 
 */
int Sudoku::solve()
{
    cnfGen();
    solver.initialize();
    if (solver.execDPLL())
    {
        int val = 0; //填入棋盘的值
        cout << "[Solution]" << endl;
        for (int i = 1; i <= degree; i++)
        {
            for (int j = 1; j <= degree; j++)
            {
                if (solver.res->vars[mapper(i, j)] == SAT)
                    val = 1;
                else
                    val = 0;
                if (j != degree)
                    cout << "|" << val;
                else
                    cout << "|" << val << "|";
            }
            cout << endl;
        }
    }
    else
    {
        cout << "The puzzle can't be solved:" << endl;
        showIniBoard();
    }
    solver.saveResult();
    return 0;
}

/**
 * @brief 演示模块
 * 
 */
void Sudoku::demo()
{
    cout << "------------------ Binary Sudoku --------------------" << endl;
    int opt = 0;
    while (opt != -1)
    {
        cout << "Choose one following functionality to continue\n"
             << "enter a number (from 1 ~ 5):" << endl
             << "1. Demo (default board)" << endl
             << "2. Solve a puzzle generated randomly" << endl
             << "(hint: type in -1 to exit)\n"
             << endl;
        cin >> opt;
        if (cin.fail()) //如果输入非数字，输入失败为真
        {
            cout << "\nPlease enter an integer!\n"
                 << endl;
            cin.clear(); //清空失败标志位
            cin.sync();  //清空输入缓冲区
            continue;
        }
        if (opt == -1)
        {
            return;
        }
        else if (opt == 1)
        {
            degree = 8;
            for (int i = 1; i <= 8; i++)
                for (int j = 1; j <= 8; j++)
                {
                    board[i][j] = -1;
                }
            board[1][2] = 0;
            board[2][4] = 1;
            board[2][6] = 1;
            board[2][8] = 0;
            board[3][3] = 0;
            board[4][2] = 1;
            board[5][5] = 1;
            board[6][2] = 0;
            board[6][6] = 1;
            board[7][2] = 0;
            board[7][5] = 0;
            board[8][5] = 0;
            board[8][7] = 0;
            cout << endl;
            showIniBoard();
            solve();
        }
        else if (opt == 2)
        {
            cout << "\nDegree of the puzzle: " << endl;
            cout << "(2, 4, 6, 8, 10, 12, 14, 16)" << endl;
            int d = 4;
            cin >> d;
            if (cin.fail())
            {
                cout << "\nPlease enter an integer!\n"
                     << endl;
                cin.clear(); //清空失败标志位
                cin.sync();  //清空输入缓冲区
                continue;
            }
            cout << "choose difficulty level: " << endl;
            cout << "1. simple\n"
                 << "2. normal\n"
                 << "3. hard\n";   
            int l = 2;
            cin >> l;
            if (cin.fail())
            {
                cout << "\nPlease enter an integer!\n"
                     << endl;
                cin.clear(); //清空失败标志位
                cin.sync();  //清空输入缓冲区
                continue;
            }
            initialize(d, l);
            showIniBoard();
            solve();
        }
        else
        {
            cout << "\nPlease enter a number from 1 ~ 2 or -1\n"
                 << endl;
            continue;
        }
    }
}