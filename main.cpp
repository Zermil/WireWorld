// =======================================
// https://en.wikipedia.org/wiki/Wireworld
// =======================================
#include <conio.h>
#include "./src/board.h"

int main(int charc, char* argv[])
{
	size_t row;
	size_t col;

	std::cin >> col >> row;

	// Min-size of the board is 5x5 
	if ((int)row <= 4 || (int)col <= 4)
	{
		std::cout << "Please provide big enough board!\n  minimal size: 5 5\n";

		system("pause");
		return 1;
	}

	Board board(row, col);
	cls(); // Clear screen for the board

	// Initial board draw, since I don't want to draw '|' character every time
	// I update the board in some way (call print_board()), this works better for larger sizes
	// and also it's the first thing that came to my mind, we have to 
	// draw it anyway soooo...

	// '+1' needed, because we're drawing '|' explained -> Board::print_board()
	for (size_t i = 0; i < row; ++i)
	{
		setCursorPosition(0, i);
		std::cout << '|';

		for (size_t j = 0; j < col; ++j)
		{
			setCursorPosition((int)j + 1, (int)i);
			std::cout << static_cast<char>(State::EMPTY);
		}

		setCursorPosition((int)col + 1, i);
		std::cout << "|\n";
	}

	std::cout.flush();
	  
	while (true)
	{	
		board.print_board(); // Drawing "again" because of initializer's position (NOT really again, just look for updates)

		const int c = _getch();

		// UP = 72
		// DOWN = 80
		// LEFT = 75
		// RIGHT = 77
		// SPACE = 32
		// ESC = 27

		// 1 = 49
		// 2 = 50
		// 3 = 51
		// 4 = 52

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
			board.start_simulation();
			break;
		}
	}

	setCursorPosition(0, row);
	system("pause");
	return 0;
}