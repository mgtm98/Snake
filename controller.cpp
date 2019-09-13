#include "controller.h"


Controller::Controller(QWidget *parent):QMainWindow(parent),score(0),multiPlayer(0),initSnakes(0){

    scorelbl = new QLabel(this);
    view = new QGraphicsView(this);
    sc = new QGraphicsScene();
    timer = new QTimer(this);
    singlePlayerBtn = new QPushButton(this);
    multiPlayerBtn = new QPushButton(this);

    timer->setInterval(100);
    //view->setEnabled(false)
    view->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    view->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    view->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

    this->setFocus();
    this->setGeometry(50,50,520,580);
    view->setGeometry(5,10,505,505);

    scorelbl->setGeometry(20,530,150,30);
    scorelbl->setText("Score is " + QString::number(score));

    singlePlayerBtn->setText("Single Player");
    multiPlayerBtn->setText("Multi Player");

    connect(timer, SIGNAL(timeout()), this, SLOT(timerHandeler()));
    connect(singlePlayerBtn, SIGNAL(clicked()), this, SLOT(singlePlayerBtnHandeler()));
    connect(multiPlayerBtn, SIGNAL(clicked()), this, SLOT(multiPlayerBtnHandeler()));

    singlePlayerBtn->setGeometry(150,530,150,30);
    multiPlayerBtn->setGeometry(350,530,150,30);

}

Controller::~Controller(){
    delete s;
    delete sc;
    delete timer;
    delete view;
    delete scorelbl;
}

void Controller::keyPressEvent(QKeyEvent *event){
    if(event->key() == up_Arrow){
        s->setDir(Snake::Direction::up);
        sendData(up_Arrow);
        qDebug() << "Up Arrow" ;
    }else if(event->key() == down_Arrow){
        s->setDir(Snake::Direction::down);
        sendData(down_Arrow);
        qDebug() << "Down Arrow" ;
    }else if(event->key() == right_Arrow){
        s->setDir(Snake::Direction::right);
        sendData(right_Arrow);
        qDebug() << "Right Arrow" ;
    }else if(event->key() == left_Arrow){
        s->setDir(Snake::Direction::left);
        sendData(left_Arrow);
        qDebug() << "Left Arrow" ;
    }
}

void Controller::timerHandeler(){
    sc->clear();
    if(multiPlayer){
        multiPlayerCore();
    }else{
        singlePlayerCore();
    }
    view->setScene(sc);
    view->show();
}

void Controller::generateApple(){
    appleX = ( std::rand() % 51 )*10;
    appleY = ( std::rand() % 51 )*10;
}

void Controller::singlePlayerBtnHandeler(){
    generateApple();
    s = new Snake(500,500,0,0);
    s->rePaint(*sc);
    view->setScene(sc);
    view->show();
    timer->start();
    this->setFocus();
}

void Controller::multiPlayerBtnHandeler(){
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress("127.0.0.1"), 8005);
    connect(socket,SIGNAL(readyRead()),this,SLOT(onDataRecived()));
    connect(socket,SIGNAL(connected()),this,SLOT(onServerConnected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(onServerDisconnected()));
    multiPlayer = true;
    s = new Snake(500,500,0, 0);
    s_multi = new Snake(500,500,0,500);
    generateApple();
    this->setFocus();
}

void Controller::singlePlayerCore(){
    sc->clear();
    s->move();
    s->rePaint(*sc);
    if(s->get_X() == appleX && s->get_Y() == appleY){
        generateApple();
        s->grow();
        score++;
        scorelbl->setText("Score is " + QString::number(score));
    }
    if(s->check_Col()){
        qDebug() << "LOSER";
        timer->stop();
        qDebug() << "GAME ENDED";
    }
    sc->addEllipse(appleX,appleY,10,10,QPen(Qt::red),QBrush(Qt::darkRed));
}

void Controller::multiPlayerCore(){
    sc->clear();
    s->move();
    s_multi->move();
    s->rePaint(*sc);
    s_multi->rePaint(*sc);
//    qDebug() << appleX << " " << appleY << s->get_X() << " " << s->get_Y();
    if(s->get_X() == appleX && s->get_Y() == appleY){
        s->grow();
        sendData(100);
    }
    if(s_multi->get_X() == appleX && s_multi->get_Y() == appleY){
        s_multi->grow();
    }
    if(s->check_Col()){
        qDebug() << "LOSER";
        timer->stop();
        qDebug() << "GAME ENDED";
    }
    if(s_multi->check_Col()){
        qDebug() << "LOSER";
        timer->stop();
        qDebug() << "GAME ENDED";
    }
    sc->addEllipse(appleX,appleY,10,10,QPen(Qt::red),QBrush(Qt::darkRed));
}

void Controller::onDataRecived(){
     QByteArray data = socket->readAll();
     qDebug() << data;
     bool move_data ;
     int temp = data.toInt(&move_data);
     if(move_data){
         if(temp == up_Arrow){
             s_multi->setDir(Snake::Direction::up);
         }else if(temp == down_Arrow){
            s_multi->setDir(Snake::Direction::down);
         }else if(temp == right_Arrow){
            s_multi->setDir(Snake::Direction::right);
         }else if(temp == left_Arrow){
            s_multi->setDir(Snake::Direction::left);
         }else if(temp == 10){
             timer->start();
         }
     }else{
         QString data_str = QTextCodec::codecForMib(106)->toUnicode(data);
         QStringList List = data_str.split(" ");
         appleX = List[0].toInt()*10;
         appleY = List[1].toInt()*10;
     }
}

void Controller::onServerConnected(){
    qDebug() << "Connected to Server";
}

void Controller::onServerDisconnected(){
    qDebug() << "Server Disconnected";
    timer->stop();
}



void Controller::sendData(int data){
    if(multiPlayer){
        socket->write(QByteArray::number(data));
    }
}





