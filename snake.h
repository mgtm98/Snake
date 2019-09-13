#ifndef SNAKE_H
#define SNAKE_H

#include <QPainter>
#include <QGraphicsScene>
#include <vector>

class Snake{

public:
    Snake(size_t limitX, size_t limitY, size_t x_start, size_t y_start);
    enum Direction{
        up,down,left,right
    };
    void rePaint(QGraphicsScene &scene) const;
    void move();
    void setDir(Snake::Direction);
    void grow();
    int get_X();
    int get_Y();
    bool check_Col();

private:
    const size_t width = 10;                            // width of node
    size_t size;                                        // size of the snake (nodes number)
    std::vector<std::vector<int>> nodes;
    Snake::Direction dir = Snake::Direction::right;
    Snake::Direction lastdir = Snake::Direction::right;
    size_t limit_X;
    size_t limit_Y;

};

#endif // SNAKE_H
