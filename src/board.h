#ifndef BOARD_H_
#define BOARD_H_
#define WIN32_LEAN_AND_MEAN // Reduce global namespace pollution

#include <iostream>
#include <vector>
#include <cassert>
#include <Windows.h>

enum class Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

enum class Inst {
	INST_MU,
	INST_MD,
	INST_ML,
	INST_MR,
	INST_CTE,
	INST_CTEH,
	INST_CTET,
	INST_CTC,
};

enum class State : char {
	EMPTY = ' ',
	E_HEAD = 'O',
	E_TAIL = 'o',
	CONDUCTOR = '#',
	INITIALIZER = '@',
};

// Helper function, clear screen
static void cls()
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi{};
	COORD topLeft = { 0, 0 };

	// Needed because we don't want to write out "junk" characters
	std::cout.flush();

	assert(GetConsoleScreenBufferInfo(hOut, &csbi) && "uwu, we did a little fuckie wookie. Couldn't get console buffer info!");

	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;
	DWORD written;

	// Flood-fill the console with spaces to clear it
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

	// Reset attributes of every character, clear colours and such
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

	// Move the cursor back to (0, 0)
	SetConsoleCursorPosition(hOut, topLeft);
}

// Helper function, set cursor position in console
// x - column, y - row
static void setCursorPosition(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { (SHORT)x, (SHORT)y };
	std::cout.flush();

	SetConsoleCursorPosition(hOut, coord);
}

using states_board = std::vector<std::vector<State>>;

class Board
{
public:
	Board();
	Board(size_t, size_t);
	Board(const std::string&);

public:
	void update(Inst);
	void draw_initial_state();
	void start_simulation();
	int getBoardRow() { return (int)initial_board.size(); }; // Utility for my OCD, so I can use it when calling system("pause") with set cursor

private:
	states_board initial_board;
	states_board buffer_board;

	struct Initializer {
		int posX;
		int posY;
	} initializer;

private:
	void update_print_board();
	void move_initializer(Direction);
	int count_neighbours(int, int);
	states_board get_next_states();
};

#endif // BOARD_H_