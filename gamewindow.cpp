#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <note.h>

#include<iostream>
#include<QPixmap>
#include<QFontDatabase>
#include<fstream>
#include<QPalette>

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    //背景图
    QPixmap bg;
    bg.load(":/images/images/background/bg_dark.png");
    background=new QLabel(this);
    background->setPixmap(bg);
    background->resize(windowWidth,windowHeight);
    background->show();
    background->lower();
    //轨道背景
    ui->judgeLine->raise();
    //封面图和曲目信息
    QPixmap cvr;
    cvr.load(QString::fromStdString(PATH+"covers/"+songName+".jpg"));
    cover=new QLabel(this);
    cover->setPixmap(cvr.scaled(coverLength,coverLength,Qt::KeepAspectRatio));
    cover->move(0,0);
    cover->show();

    songname=new QLabel(this);
    artist=new QLabel(this);
    songname->resize(leftPadding-coverLength,coverLength/2);
    artist->resize(leftPadding-coverLength,coverLength/2);
    std::ifstream metadata;
    metadata.open(PATH+"metadata/"+songName+".txt");
    if(!metadata){
        qDebug()<<"METADATA READ ERROR";
    }
    std::string artiststr;
    std::getline(metadata,artiststr);
    metadata.close();

    int id=QFontDatabase::addApplicationFont(":/images/images/font/Exo-Medium 22.05.14.ttf");
    QString family=QFontDatabase::applicationFontFamilies(id).at(0);
    QFont Exo(family);
    QPalette palette=songname->palette();
    palette.setColor(songname->foregroundRole(),Qt::white);
    songname->setPalette(palette);
    palette=artist->palette();
    palette.setColor(artist->foregroundRole(),Qt::white);
    artist->setPalette(palette);

    songname->setAlignment(Qt::AlignCenter);
    artist->setAlignment(Qt::AlignCenter);
    int songnameSize=std::min(coverLength/3,(leftPadding-coverLength)/((int)songName.length()));
    Exo.setPixelSize(songnameSize);
    songname->setFont(Exo);
    int artistSize=std::min(coverLength/3,(leftPadding-coverLength)/((int)artiststr.length()));
    Exo.setPixelSize(artistSize);
    artist->setFont(Exo);

    songname->move(coverLength,0);
    artist->move(coverLength,coverLength/2);
    songname->setText(QString::fromStdString(songName));
    artist->setText(QString::fromStdString(artiststr));
    songname->show();
    artist->show();

    //初始化
    notehandler=new NoteHandler(this);
    //connect
    connect(this,SIGNAL(keyPressed(int)),notehandler,SLOT(judge(int)));
    connect(notehandler,SIGNAL(dataShowSig(int)),notehandler,SLOT(dataShow(int)));
}

GameWindow::~GameWindow()
{
    delete background;
    delete notehandler;
    delete ui;
    delete cover;
    delete songname;
    delete artist;
}

void GameWindow::keyPressEvent(QKeyEvent* event){
    switch(event->key()){
    case Qt::Key_Q:{
        qDebug()<<"Q";
        emit keyPressed(1);
        break;
    }
    case Qt::Key_W:{
        qDebug()<<"W";
        emit keyPressed(2);
        break;
    }
    case Qt::Key_O:{
        qDebug()<<"O";
        emit keyPressed(3);
        break;
    }
    case Qt::Key_P:{
        qDebug()<<"P";
        emit keyPressed(4);
        break;
    }
    default:{
        break;
    }
    }
}
