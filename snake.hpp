#ifndef BATTLESNAKE_TESTHARNESS_SNAKE_HPP
#define BATTLESNAKE_TESTHARNESS_SNAKE_HPP

#include <cstdint>
#include <vector>
#include <compare>
#include <set>
#include <map>
#include <optional>
#include <functional>

struct Boundary { uint8_t width; uint8_t height; };
struct Cell {
    uint8_t x;
    uint8_t y;
    friend auto operator<=>(Cell const& lhs, Cell const& rhs) = default;
};

struct Food {};
struct FoodAtCell { Food food; Cell cell; };

struct Health { uint32_t value; };
static constexpr Health MaxHealth{ 100 };

struct SnakeID {
    uint32_t value;
    friend auto operator<=>(SnakeID const& lhs, SnakeID const& rhs) = default;
};

struct Snake {
    Health health = MaxHealth;
    Cell head;
    std::set<Cell> body;

    [[nodiscard]] bool livesInCell(Cell cell) const;
};

struct Board {
    enum class CellContents {
        EMPTY, THIS_SNAKES_BODY, THIS_SNAKES_HEAD, ENEMY_SNAKE, FOOD,
    };
    [[nodiscard]] CellContents whatsInTheCell(Cell cell, SnakeID whosAsking) const;

    struct CellContentsDetailed {
        std::optional<Food> food;
        std::optional<SnakeID> snakeHead;
        std::optional<SnakeID> snakeBody;
    };
    std::map<Cell, CellContentsDetailed> contents;
};

enum class Move {
    UP, DOWN, LEFT, RIGHT
};
using SnakeStrategy = std::function<Move(Board const&)>;

class Game {
public:
    explicit Game(Boundary boundary);

    void addSnake(SnakeStrategy snakeStrategy);
    void startBattle();
    void advanceTurn();
    [[nodiscard]] bool isGameOver() const;

private:
    struct SnakeWithABrain { SnakeID id; SnakeStrategy brain; Snake flesh; };

    Boundary mBoundary;
    std::vector<SnakeWithABrain> mSnakes;
    std::vector<FoodAtCell> mFood;
};

#endif //BATTLESNAKE_TESTHARNESS_SNAKE_HPP
