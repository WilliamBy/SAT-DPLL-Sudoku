/* 测试程序 */
# include "SATSolverDPLL.h"
# include <cstring>

int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        SATSolverDPLL solver(argv[1], argv[2]);
        solver.initialize();
        solver.execDPLL();
        solver.showResult();
        solver.saveResult();
    }
    return 0;
}