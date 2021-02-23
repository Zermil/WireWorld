#ifndef BOARD_H_
#define BOARD_H_

#include <vector>

enum class Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

enum class State {
	EMPTY,
	E_HEAD,
	E_TAIL,
	CONDUCTOR,
	STATES_SIZE,
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

typedef std::vector<std::vector<State>> initialized_board;

struct Board {
	const char LOOK_UP[static_cast<int>(State::STATES_SIZE)] = { ' ', 'O', 'o', '#' };
	initialized_board states;

	struct Initializer {
		int posX;
		int posY;
	} initializer;

	Board(int rows, int cols);

	void move_initializer(Direction direction);
	void print(bool drawInitializer = true);
	void update(Inst instruction);
	int count_neighbours(int row, int col);
	initialized_board next_generation();
	void start_loop();
};

#endif // BOARD_H_