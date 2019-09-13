#include "snake.h"
#include <QDebug>

Snake::Snake(size_t X, size_t Y, size_t x_start, size_t y_start):size(10),limit_X(X),limit_Y(Y){
    for(size_t x = 0; x < size; x++){
        std::vector<int> node {static_cast<int>(x*width+x_start),static_cast<int>(y_start)};
        nodes.insert(nodes.begin(),node);
    }
}

void Snake::rePaint(QGraphicsScene &scene) const{
    for(std::vector<int> node : nodes){
        scene.addRect(node[0],node[1],width,width,QPen(Qt::blue),QBrush(Qt::darkBlue));
    }
}

void Snake::move(){
    nodes.pop_back();
    grow();
}

void Snake::setDir(Snake::Direction dir){
    lastdir = this->dir;
    this->dir = dir;
    qDebug() << lastdir << "  " << this->dir;
}

int Snake::get_X(){
    return nodes[0][0];
}

int Snake::get_Y(){
    return nodes[0][1];
}

void Snake::grow(){
    if(dir == Snake::Direction::right && lastdir != Snake::Direction::left){
        std::vector<int> node  = nodes[0];
        node[0] += width;
        if(node[0] > 500)node[0] = 0;
        else if(node[0] < 0) node[0] = 500;
        nodes.insert(nodes.begin(),node);
    }else if(dir == Snake::Direction::down && lastdir != Snake::Direction::up){
        std::vector<int> node  = nodes[0];
        node[1] += width;
        if(node[1] > 500)node[1] = 0;
        else if(node[1] < 0) node[1] = 500;
        nodes.insert(nodes.begin(),node);
    }else if(dir == Snake::Direction::up && lastdir != Snake::Direction::down){
        if(lastdir == Snake::Direction::down)qDebug()<<"ERROR";
        std::vector<int> node  = nodes[0];
        node[1] -= width;
        if(node[1] > 500)node[1] = 0;
        else if(node[1] < 0) node[01] = 500;
        nodes.insert(nodes.begin(),node);
    }else if(dir == Snake::Direction::left && lastdir != Snake::Direction::right){
        std::vector<int> node  = nodes[0];
        node[0] -= width;
        if(node[0] > 500)node[0] = 0;
        else if(node[0] < 0) node[0] = 500;
        nodes.insert(nodes.begin(),node);
    }
}

bool Snake::check_Col(){
    int flag = 0;

    for(std::vector<int> node : nodes){
        if(flag == 0){
            flag++;
            continue;
        }
        if(node[0] == get_X() && node[1] == get_Y()){
            return true;
        }
    }
    return false;
}

