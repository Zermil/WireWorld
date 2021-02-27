// ======================================= //
// https://en.wikipedia.org/wiki/Wireworld //
// ======================================= //
#include <conio.h>
#include "./../src/board.h"

// Regular map can't be accessed by index(???), which is weird and dumb
struct My_map {
	std::string key;
	std::string value;
};

void load_board(const std::string& boardPath);
void draw_indicator(int x, int y);
void clear_indicator(int x, int y);
void draw_menu(const My_map available_boards[], int board_size);

int main(int argc, char* argv[])
{
	const int BOARDS_SIZE = 4;
	const My_map available_boards[BOARDS_SIZE] = {
		{ "OR GATE", "./board_files/or_gate.board" },
		{ "XOR GATE", "./board_files/xor_gate.board" },
		{ "AND GATE", "./board_files/and_gate.board" },
		{ "CREATE OWN", "./board_files/big_board.board" },
	};

	int indicator_index = 0;

	draw_menu(available_boards, BOARDS_SIZE);

	// ESC = 27
	// UP = 72
	// DOWN = 80

	draw_indicator(available_boards[indicator_index].key.length(), indicator_index);

	while (true) 
	{
		const int c = _getch();

		if (c == 27)
			break;

		switch (c)
		{
		case 72:
			clear_indicator(available_boards[indicator_index].key.length(), indicator_index);
			indicator_index = indicator_index - 1 < 0 ? BOARDS_SIZE - 1 : indicator_index - 1;
			break;
		case 80:
			clear_indicator(available_boards[indicator_index].key.length(), indicator_index);
			indicator_index = indicator_index + 1 >= BOARDS_SIZE ? 0 : indicator_index + 1;
			break;
		case 13:
			load_board(available_boards[indicator_index].value);
			draw_menu(available_boards, BOARDS_SIZE);
			break;
		}

		draw_indicator(available_boards[indicator_index].key.length(), indicator_index);
	}

	setCursorPosition(0, BOARDS_SIZE + 2);
	system("pause");
	return 0;
}

void load_board(const std::string& boardPath)
{
	Board board(boardPath);
	board.draw_initial_state();

	while (true)
	{
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
}

// UTILITIES
void draw_indicator(int x, int y)
{
	setCursorPosition(x + 1, y + 2);
	std::cout << '*';
	std::cout.flush();
}

void clear_indicator(int x, int y)
{
	setCursorPosition(x + 1, y + 2);
	std::cout << '\0';
	std::cout.flush();
}

void draw_menu(const My_map available_boards[], int board_size)
{
	cls();

	std::cout << "CHOOSE CIRCUIT\n";
	std::cout << "=====================\n";

	for (int i = 0; i < board_size; ++i)
		std::cout << available_boards[i].key << '\n';
}