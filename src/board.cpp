#include <iostream>
#include <cassert>
#include <conio.h>
#include "./board.h"

Board::Board(int rows, int cols) :
    states(rows, std::vector<State>(cols, State::EMPTY))
{
    initializer.posY = rows / 2;
    initializer.posX = cols / 2;
}

void Board::move_initializer(Direction direction)
{
    // Wrap around
    switch (direction)
    {
    case Direction::UP:
        initializer.posY = initializer.posY - 1 < 0 ? (int)states.size() - 1 : initializer.posY - 1;
        break;
    case Direction::DOWN:
        initializer.posY = initializer.posY + 1 >= (int)states.size() ? 0 : initializer.posY + 1;
        break;
    case Direction::LEFT:
        initializer.posX = initializer.posX - 1 < 0 ? (int)states[0].size() - 1 : initializer.posX - 1;
        break;
    case Direction::RIGHT:
        initializer.posX = initializer.posX + 1 >= (int)states[0].size() ? 0 : initializer.posX + 1;
        break;
    default:
        assert(false && "uwu, unreachable: unrecognized direction!");
    }
}

void Board::print(bool drawInitializer)
{
    system("cls");

    for (size_t i = 0; i < states.size(); ++i)
    {
        std::cout << '|';
        
        for (size_t j = 0; j < states[0].size(); ++j)
        {
            if (drawInitializer && ((size_t)initializer.posX == j && (size_t)initializer.posY == i)) 
            {
                std::cout << 'X';
                continue;
            }

            std::cout << LOOK_UP[static_cast<int>(states[i][j])];
        }

        std::cout << "|\n";
    }
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
        states[initializer.posY][initializer.posX] = State::EMPTY;
        break;
    case Inst::INST_CTEH:
        states[initializer.posY][initializer.posX] = State::E_HEAD;
        break;
    case Inst::INST_CTET:
        states[initializer.posY][initializer.posX] = State::E_TAIL;
        break;
    case Inst::INST_CTC:
        states[initializer.posY][initializer.posX] = State::CONDUCTOR;
        break;
    default:
        assert(false && "uwu OwO!, unreachable: instruction not recognized!");
    }
}

int Board::count_neighbours(int row, int col)
{
    int neighbours_e_heads = 0;

    for (int i = -1; i < 2; ++i)
    {
        for (int j = -1; j < 2; ++j)
        {
            const int indexY = row + i;
            const int indexX = col + j;

            // Skip yourself
            if (indexX == col && indexY == row)
                continue;

            // Don't count neighbours outside of boundaries
            // no wrap, skips out of bounds neighbours
            if (indexY < 0 || indexX < 0 || indexY >= (int)states.size() || indexX >= (int)states[0].size())
                continue;

            // Only increment if the neighbour is E_HEAD (electron head)
            if (states[indexY][indexX] == State::E_HEAD)
                neighbours_e_heads++;
        }
    }

    return neighbours_e_heads;
}

initialized_board Board::next_generation()
{
    initialized_board new_states
        (states.size(), std::vector<State>(states[0].size(), State::EMPTY));

    for (size_t i = 0; i < states.size(); ++i)
    {
        for (size_t j = 0; j < states[0].size(); ++j)
        {
            State cell = states[i][j];

            switch (cell)
            {
            case State::EMPTY:
                new_states[i][j] = State::EMPTY;
                break;
            case State::E_HEAD:
                new_states[i][j] = State::E_TAIL;
                break;
            case State::E_TAIL:
                new_states[i][j] = State::CONDUCTOR;
                break;
            case State::CONDUCTOR: {
                const int conductor_neighbours = count_neighbours(i, j);

                (conductor_neighbours == 1 || conductor_neighbours == 2) ? 
                    new_states[i][j] = State::E_HEAD : new_states[i][j] = State::CONDUCTOR;

                break;
            }
            default:
                assert(false && "uwu owo, unreachable: unrecognized state");
            }
        }
    }

    return new_states;
}

void Board::start_loop()
{
    initialized_board original = states;
    print(false);
    
    while (true)
    {
        const int c = _getch();

        // 'e' = 101
        // 'r' = 114

        if (c == 101)
            break;

        switch (c)
        {
        case 114:
            states = original;
            break;
        default:
            states = next_generation();
            break;
        }

        print(false);
    }
}