#include "snake.hpp"

#include <utility>
#include <random>

Game::Game(Boundary boundary) : mBoundary(boundary) {
}

void Game::addSnake(SnakeStrategy snakeStrategy) {
    SnakeID const nextSnakeID{ static_cast<uint32_t>(mSnakes.size() + 1) };
    mSnakes.push_back({ nextSnakeID, std::move(snakeStrategy), {}});
}

bool Game::isGameOver() const {
    return !mSnakes.empty();
}

void Game::advanceTurn() {
    auto const buildBoard = [this]() -> Board {
        Board b;
        for (SnakeWithABrain const& smartSnake : mSnakes) {
            for (Cell const& bodyPart : smartSnake.flesh.body)
            {
                Board::CellContentsDetailed bodyCell;
                bodyCell.snakeBody = smartSnake.id;
                b.contents.insert({ bodyPart, bodyCell });
            }

            Board::CellContentsDetailed headCell;
            headCell.snakeHead = smartSnake.id;
            b.contents.insert({ smartSnake.flesh.head, headCell });
        }
        for (FoodAtCell const& foodAtCell : mFood)
        {
            Board::CellContentsDetailed foodCell;
            foodCell.food = foodAtCell.food;
            b.contents.insert({ foodAtCell.cell, foodCell });
        }
        return b;
    };

    std::map<SnakeID, Move> plannedMoves;
    Board const board = buildBoard();
    for (SnakeWithABrain& smartSnake : mSnakes) {
        plannedMoves.insert({ smartSnake.id, smartSnake.brain(board) });
    }

    // TODO: Execute the planned moves, kill dead snakes, add more food, grow snakes, etc.
}

void Game::startBattle() {
    if (mSnakes.empty())
        return;

    // Shuffle the snakes so that we don't get some determinism in starting location.
    std::random_device randomDevice;
    std::mt19937 rng(randomDevice());
    std::shuffle(mSnakes.begin(), mSnakes.end(), rng);

    // TODO: Disperse the snakes randomly, give them some body, and some food according to how the tournament will run approximately.
}

bool Snake::livesInCell(Cell cell) const {
    return head == cell || std::find(body.begin(), body.end(), cell) != body.end();
}

Board::CellContents Board::whatsInTheCell(Cell cell, SnakeID whosAsking) const {
    auto const contentsOfThisCell = contents.find(cell);
    if (contentsOfThisCell == contents.end()) {
        return CellContents::EMPTY;
    }
    else if (contentsOfThisCell->second.food) {
        return CellContents::FOOD;
    }
    else if (contentsOfThisCell->second.snakeBody) {
        if (contentsOfThisCell->second.snakeBody.value() == whosAsking)
            return CellContents::THIS_SNAKES_BODY;
        else
            return CellContents::ENEMY_SNAKE;
    }
    else if (contentsOfThisCell->second.snakeHead) {
        if (contentsOfThisCell->second.snakeHead.value() == whosAsking)
            return CellContents::THIS_SNAKES_HEAD;
        else
            return CellContents::ENEMY_SNAKE;
    }

    return CellContents::EMPTY;
}
