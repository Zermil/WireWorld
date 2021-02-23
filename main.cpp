// =======================================
// https://en.wikipedia.org/wiki/Wireworld
// =======================================
#include <iostream>
#include <conio.h>
#include "./src/board.h"

int main(int charc, char* argv[])
{
    int rows;
    int cols;

    std::cin >> cols >> rows;

    if (rows <= 4 || cols <= 4)
    {
        std::cout << "Please provide big enough board\n  min_size: 5 5\n";
        system("pause");
        return 1;
    }

    Board board(rows, cols);

    while (true)
    {
        board.print();
        const int c = _getch();

        // UP = 72
        // DOWN = 80
        // LEFT = 75
        // RIGHT = 77
        // SPACE = 32
        // ESC = 27

        if (c == 27)
            break;

        switch (c)
        {
        case 72:
            board.update(Inst::INST_MU);
            break;
        case 80:
            board.update(Inst::INST_MD);
            break;
        case 75:
            board.update(Inst::INST_ML);
            break;
        case 77:
            board.update(Inst::INST_MR);
            break;
        case 49:
            board.update(Inst::INST_CTE);
            break;
        case 50:
            board.update(Inst::INST_CTEH);
            break;
        case 51:
            board.update(Inst::INST_CTET);
            break;
        case 52:
            board.update(Inst::INST_CTC);
            break;
        case 32:
            board.start_loop();
            break;
        }
    }

    system("pause");
    return 0;
}