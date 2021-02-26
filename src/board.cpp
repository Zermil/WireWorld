#include <conio.h>
#include <fstream>
#include <string>
#include "./board.h"

Board::Board()
	: initial_board(10, std::vector<State>(20, State::EMPTY)),
	buffer_board(10, std::vector<State>(20, State::EMPTY)),
	initializer({ 10, 5 })
{
	// Just for my OCD
}

Board::Board(size_t row, size_t col)
	: initial_board(row, std::vector<State>(col, State::EMPTY)),
	buffer_board(row, std::vector<State>(col, State::EMPTY)),
	initializer({ static_cast<int>(col / 2), static_cast<int>(row / 2) })
{
	// Just for my OCD
}

// Read initial_board from file
Board::Board(std::string filePathName)
{
	size_t row = 0;
	size_t col = 0;

	std::string inputLine;
	std::ifstream fileToRead(filePathName);

	assert(fileToRead.is_open() && "OnO unu, Unable to open specified file!");

	// First loop just to get dimensions and error check
	while (getline(fileToRead, inputLine))
	{
		if (row == 0)
			col = inputLine.length();
		else if (inputLine.length() > col || inputLine.length() < col)
			assert(false && "o3o Inconsistent number of columns in file!");

		row++;
	}

	fileToRead.close();

	if (row <= 4 || col <= 4)
		assert(false && "Incorrect board size, expected minimum 5x5 board");

	initializer.posX = static_cast<int>(col / 2);
	initializer.posY = static_cast<int>(row / 2);

	initial_board = states_board(row, std::vector<State>(col, State::EMPTY));

	row = 0; // Reuse for counting iterations

	// Open file again, no need to create another extra read buffer
	fileToRead.open(filePathName);

	while (getline(fileToRead, inputLine))
	{
		for (size_t i = 0; i < col; ++i)
		{
			char currentCharacter = inputLine[i];

			switch (currentCharacter)
			{
			case '.':
				initial_board[row][i] = State::EMPTY;
				break;
			case static_cast<char>(State::E_HEAD):
				initial_board[row][i] = State::E_HEAD;
				break;
			case static_cast<char>(State::E_TAIL):
				initial_board[row][i] = State::E_TAIL;
				break;
			case static_cast<char>(State::CONDUCTOR):
				initial_board[row][i] = State::CONDUCTOR;
				break;
			default:
				assert(false && "OvO, Unreachable: Illegal character detected!");
			}
		}

		row++;
	}

	fileToRead.close();
	buffer_board = initial_board;
}

// the '+1' is important when specifying a column
// it SHIFTS everything by ONE to the right, BECAUSE
// we are drawing additional ONE character at the beginning '|'
void Board::update_print_board()
{
	for (size_t i = 0; i < initial_board.size(); ++i)
	{
		for (size_t j = 0; j < initial_board[0].size(); ++j)
		{
			if (initial_board[i][j] == buffer_board[i][j])
				continue;

			setCursorPosition(j + 1, i);
			std::cout << static_cast<char>(initial_board[i][j]);
		}
	}

	std::cout.flush();
	buffer_board = initial_board;
}

void Board::draw_initial_state()
{
	cls(); // Clear screen for the board

	for (size_t i = 0; i < initial_board.size(); ++i)
	{
		std::cout << '|';

		for (size_t j = 0; j < initial_board[0].size(); ++j)
		{
			std::cout << static_cast<char>(initial_board[i][j]);
		}

		std::cout << "|\n";
	}

	setCursorPosition(initializer.posX + 1, initializer.posY);
	std::cout << static_cast<char>(State::INITIALIZER);
	std::cout.flush();
}

void Board::move_initializer(Direction direction)
{
	// Clear the previous initializer from console
	// set it to whatever is in initial_board at that location
	setCursorPosition(initializer.posX + 1, initializer.posY);
	std::cout << static_cast<char>(initial_board[initializer.posY][initializer.posX]);
	std::cout.flush();

	// calculate new position, wrap around
	switch (direction)
	{
	case Direction::UP:
		initializer.posY = initializer.posY - 1 < 0 ? initial_board.size() - 1 : initializer.posY - 1;
		break;
	case Direction::DOWN:
		initializer.posY = initializer.posY + 1 >= (int)initial_board.size() ? 0 : initializer.posY + 1;
		break;
	case Direction::LEFT:
		initializer.posX = initializer.posX - 1 < 0 ? initial_board[0].size() - 1 : initializer.posX - 1;
		break;
	case Direction::RIGHT:
		initializer.posX = initializer.posX + 1 >= (int)initial_board[0].size() ? 0 : initializer.posX + 1;
		break;
	default:
		assert(false && "uwu, unreachable: unrecognized direction!");
	}

	// Draw initializer at new position
	setCursorPosition(initializer.posX + 1, initializer.posY);
	std::cout << static_cast<char>(State::INITIALIZER);
	std::cout.flush();
}

void Board::update(Inst instruction)
{
	switch (instruction)
	{
	case Inst::INST_MU:
		move_initializer(Direction::UP);
		break;
	case Inst::INST_MD:
		move_initializer(Direction::DOWN);
		break;
	case Inst::INST_ML:
		move_initializer(Direction::LEFT);
		break;
	case Inst::INST_MR:
		move_initializer(Direction::RIGHT);
		break;
	case Inst::INST_CTE:
		initial_board[initializer.posY][initializer.posX] = State::EMPTY;
		buffer_board[initializer.posY][initializer.posX] = State::EMPTY;
		break;
	case Inst::INST_CTEH:
		initial_board[initializer.posY][initializer.posX] = State::E_HEAD;
		buffer_board[initializer.posY][initializer.posX] = State::E_HEAD;
		break;
	case Inst::INST_CTET:
		initial_board[initializer.posY][initializer.posX] = State::E_TAIL;
		buffer_board[initializer.posY][initializer.posX] = State::E_TAIL;
		break;
	case Inst::INST_CTC:
		initial_board[initializer.posY][initializer.posX] = State::CONDUCTOR;
		buffer_board[initializer.posY][initializer.posX] = State::CONDUCTOR;
		break;
	default:
		assert(false && "uwu OwO!, unreachable: instruction not recognized!");
	}
}

int Board::count_neighbours(int row, int col)
{
	int e_head_neighours = 0;

	for (int i = -1; i < 2; ++i)
	{
		for (int j = -1; j < 2; ++j)
		{
			// Skip yourself
			if (i == 0 && j == 0)
				continue;

			const int row_index = row + i;
			const int col_index = col + j;

			// neighbour outside of boundaries
			if (row_index < 0 || col_index < 0 || col_index >= (int)initial_board[0].size() || row_index >= (int)initial_board.size())
				continue;

			if (initial_board[row_index][col_index] == State::E_HEAD)
				e_head_neighours++;
		}
	}

	return e_head_neighours;
}

states_board Board::get_next_states()
{
	// static because we don't reallty care about 
	// always initializing it with EMPTY, either
	// way it's going to change every State inside it 
	// according to initial_board
	static states_board next_board(initial_board.size(), std::vector<State>(initial_board[0].size(), State::EMPTY));

	for (size_t i = 0; i < initial_board.size(); ++i)
	{
		for (size_t j = 0; j < initial_board[0].size(); ++j)
		{
			const State cell = initial_board[i][j];

			switch (cell)
			{
			case State::EMPTY:
				next_board[i][j] = State::EMPTY;
				break;
			case State::E_HEAD:
				next_board[i][j] = State::E_TAIL;
				break;
			case State::E_TAIL:
				next_board[i][j] = State::CONDUCTOR;
				break;
			case State::CONDUCTOR:
			{
				const int e_head_neighbours_count = count_neighbours(i, j);
				next_board[i][j] = (e_head_neighbours_count == 1 || e_head_neighbours_count == 2) ? State::E_HEAD : State::CONDUCTOR;
				break;
			}
			default:
				assert(false && "uwu owo, unreachable: unrecognized state");
			}
		}
	}

	return next_board;
}

void Board::start_simulation()
{
	// Don't draw initializer
	setCursorPosition(initializer.posX + 1, initializer.posY);
	std::cout << static_cast<char>(initial_board[initializer.posY][initializer.posX]);
	std::cout.flush();

	const states_board original = initial_board;

	while (true)
	{
		const int c = _getch();

		// 'e' = 101, 69
		// 'r' = 114, 67

		if (c == 101 || c == 69) // Nice
		{
			// Draw initializer again, we've quit the simulation
			setCursorPosition(initializer.posX + 1, initializer.posY);
			std::cout << static_cast<char>(State::INITIALIZER);
			std::cout.flush();
			break;
		}

		switch (c)
		{
		case 67:
		case 114:
			initial_board = original;
			break;
		default:
			initial_board = get_next_states();
		}

		update_print_board();
	}
}