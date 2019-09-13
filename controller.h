#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QLabel>
#include <QTcpSocket>
#include <QPushButton>
#include <QHostAddress>
#include <QTextCodec>
#include "snake.h"

#define up_Arrow        0x1000013
#define right_Arrow     0x1000014
#define left_Arrow      0x1000012
#define down_Arrow      0x1000015

class Controller : public QMainWindow{
    Q_OBJECT

public:

    explicit Controller(QWidget *parent = nullptr);
    ~Controller()override;

private:
    int score;
    QTimer *timer;
    QLabel *scorelbl;
    Snake *s;
    Snake *s_multi;
    QGraphicsView *view;
    QGraphicsScene *sc;
    QPushButton *singlePlayerBtn;
    QPushButton *multiPlayerBtn;
    QTcpSocket *socket;
    bool initSnakes;
    int appleX;
    int appleY;
    bool multiPlayer;
    int multi_player1_x, multi_player1_y, multi_player2_x, multi_player2_y;
    void generateApple();
    void sendData(int);
    void singlePlayerCore();
    void multiPlayerCore();


private slots:
    void timerHandeler();
    void onDataRecived();
    void onServerConnected();
    void onServerDisconnected();
    void singlePlayerBtnHandeler();
    void multiPlayerBtnHandeler();



protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAINWINDOW_H
