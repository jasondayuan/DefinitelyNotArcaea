#include "selectionwindow.h"
#include "ui_selectionwindow.h"
#include "gamewindow.h"
#include "settingswindow.h"
#include "startwindow.h"
#include<fstream>
#include<cstring>
#include<QFontDatabase>

extern std::string PATH;
extern std::string songName;
extern QWidget* currentWindow;
extern int songID;

SelectionWindow::SelectionWindow(QWidget *parent,int currentSong) :
    QWidget(parent),
    ui(new Ui::SelectionWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    //读取曲名
    current_first_song_index=currentSong;
    std::ifstream songs;
    songs.open(PATH+"songs/songs.txt"); //从songs.txt获取曲名，第一行是总曲目数
    songs>>total_songs;
    songs.get();
    for(int i=0;i<total_songs;i++){
        std::getline(songs,songnames[i]);
    }

    //左右按键
    up=new QPushButton(this);
    down=new QPushButton(this);
    up->setStyleSheet("border-image:url(:/images/images/buttons/arrow_left.png);");
    down->setStyleSheet("border-image:url(:/images/images/buttons/arrow_right.png);");
    up->resize(182,230);
    down->resize(182,230);
    up->move(50,250);
    down->move(1230-182,250);
    connect(up,SIGNAL(clicked()),this,SLOT(go_down()));
    connect(down,SIGNAL(clicked()),this,SLOT(go_up()));
    connect(up,&QPushButton::pressed,[=]{
        up->setStyleSheet("border-image:url(:/images/images/buttons/arrow_left_pressed.png);");
    });
    connect(down,&QPushButton::pressed,[=]{
        down->setStyleSheet("border-image:url(:/images/images/buttons/arrow_right_pressed.png);");
    });
    connect(up,&QPushButton::released,[=]{
        up->setStyleSheet("border-image:url(:/images/images/buttons/arrow_left.png);");
    });
    connect(down,&QPushButton::released,[=]{
        down->setStyleSheet("border-image:url(:/images/images/buttons/arrow_right.png);");
    });

    //曲目信息
    for(int i=0;i<3;i++){
        metadata[i]=new QLabel(this);
        metadata[i]->show();
    }
    int id=QFontDatabase::addApplicationFont(":/images/images/font/Exo-Medium 22.05.14.ttf");
    QString family=QFontDatabase::applicationFontFamilies(id).at(0);
    QFont for_metadata(family);
    metadata[1]->resize(1000,50);
    metadata[1]->move(465,70);
    for_metadata.setWeight(QFont::Light);
    for_metadata.setPixelSize(20);
    metadata[1]->setFont(for_metadata);

    metadata[0]->resize(1000,50);
    metadata[0]->move(415,20);
    for_metadata.setWeight(QFont::Bold);
    for_metadata.setPixelSize(30);
    metadata[0]->setFont(for_metadata);

    metadata[2]->resize(300,50);
    metadata[2]->move(505,70);
    metadata[2]->setFont(for_metadata);

    //封面图
    for(int i=0;i<5;i++){
        covers[i]=new QLabel(this);
        covers[i]->setScaledContents(true);
        covers[i]->setFrameShape(QFrame::Box);
    }
    covers[1]->raise();
    covers[3]->raise();
    covers[2]->raise();

    int centerX=640,centerY=375;
    int dist1=250,dist2=350;
    covers[0]->move(centerX-dist2-50,centerY-50);
    covers[0]->resize(100,100);
    covers[1]->move(centerX-dist1-125,centerY-125);
    covers[1]->resize(250,250);
    covers[2]->move(centerX-225,centerY-225);
    covers[2]->resize(450,450);
    covers[3]->move(centerX+dist1-125,centerY-125);
    covers[3]->resize(250,250);
    covers[4]->move(centerX+dist2-50,centerY-50);
    covers[4]->resize(100,100);

    //设置初始曲目的信息，包含曲目，曲师和封面图
    setMetadata(currentSong);

    //start按键
    start_game = new QPushButton(this);
    start_game->move(372,755-123);
    start_game->setStyleSheet("border-image:url(:/images/images/buttons/world_play_button.png);");
    start_game->resize(537,123);
    connect(start_game,&QPushButton::pressed,[=]{
        start_game->setStyleSheet("border-image:url(:/images/images/buttons/world_play_button_pressed.png);");
    });
    connect(start_game,&QPushButton::released,[=]{
        start_game->setStyleSheet("border-image:url(:/images/images/buttons/world_play_button.png);");
        gameShutterAnimation->play();
    });
    start_game->show();

    //setting按键
    settings = new QPushButton(this);
    settings->setStyleSheet("border-image:url(:/images/images/buttons/settings-v2.png);");
    settings->resize(137,49);
    connect(settings,&QPushButton::pressed,[=]{
        settings->setStyleSheet("border-image:url(:/images/images/buttons/settings-v2-pressed.png);");
    });
    settings->move(1280-137,0);
    settings->show();

    //返回按键
    back=new QPushButton(this);
    back->setStyleSheet("border-image:url(:/images/images/buttons/back.png);");
    back->resize(324,97);
    connect(back,&QPushButton::pressed,[=]{
        back->setStyleSheet("border-image:url(:/images/images/buttons/back_pressed.png);");
    });
    back->move(0,800-97);
    back->show();

    backContent=new QLabel(this);
    for_metadata.setPixelSize(30);
    backContent->setFont(for_metadata);
    backContent->resize(70,30);
    backContent->move(100,725);
    backContent->setText("Back");
    backContent->setStyleSheet("color:rgb(51,0,51);");
    backContent->show();

    //背景图
    QPalette pa(this->palette());
    QImage img = QImage(":/images/images/background/0_b.jpg");
    img = img.scaled(this->size());
    QBrush *pic = new QBrush(img);
    pa.setBrush(QPalette::Window,*pic);
    this->setPalette(pa);

    //从开始界面进入选曲界面的动画
    openAnimation=new OpenAnimation(this);
    openAnimation->play();

    //从选曲界面进入游戏界面的动画
    gameShutterAnimation=new ShutterAnimation(this);
    connect(gameShutterAnimation,SIGNAL(finished()),this,SLOT(startGame()));

    //从选曲界面进入设定界面的动画
    settingsShutterAnimation=new ShutterAnimation(this);
    connect(settingsShutterAnimation,SIGNAL(finished()),this,SLOT(goToSettings()));
    connect(settings,&QPushButton::released,[=]{
        settings->setStyleSheet("border-image:url(:/images/images/buttons/settings-v2.png);");
        settingsShutterAnimation->play();
    });

    //从选曲界面返回开始界面的动画
    backShutterAnimation=new ShutterAnimation(this);
    connect(backShutterAnimation,SIGNAL(finished()),this,SLOT(goToStart()));
    connect(back,&QPushButton::released,[=]{
        back->setStyleSheet("border-image:url(:/images/images/buttons/back.png);");
        backShutterAnimation->play();
    });
}

SelectionWindow::~SelectionWindow()
{
    for(int i=0;i<5;i++){
        delete covers[i];
    }
    for(int i=0;i<3;i++){
        delete metadata[i];
    }
    delete start_game;
    delete settings;
    delete up;
    delete down;
    delete openAnimation;
    delete back;
    delete backContent;
    delete gameShutterAnimation;
    delete settingsShutterAnimation;
    delete backShutterAnimation;
    delete ui;
}

void SelectionWindow::go_up(){
    ++current_first_song_index;
    current_first_song_index=(current_first_song_index+total_songs)%total_songs;
    setMetadata(current_first_song_index);
}
void SelectionWindow::go_down(){
    --current_first_song_index;
    current_first_song_index=(current_first_song_index+total_songs)%total_songs;
    setMetadata(current_first_song_index);
}

void SelectionWindow::setMetadata(int ind){
    metadata[0]->setText(QString::fromStdString(songnames[ind]));
    metadata[1]->setText("by");
    std::string artist;
    std::ifstream artist_read;
    artist_read.open(PATH+"metadata/"+songnames[ind]+".txt");
    std::getline(artist_read,artist);
    metadata[2]->setText(QString::fromStdString(artist));

    for(int i=(((ind-1+total_songs)%total_songs)-1+total_songs)%total_songs,p=0;p<5;++p){
        covers[p]->setPixmap(QPixmap(QString::fromStdString(PATH+"covers/"+songnames[i]+".jpg")));
        ++i;
        i=(i+total_songs)%total_songs;
    }
}

void SelectionWindow::startGame(){
    songName=songnames[current_first_song_index];
    songID=current_first_song_index;
    currentWindow=new GameWindow();
    currentWindow->show();
    this->close();
}

void SelectionWindow::goToSettings(){
    songID=current_first_song_index;
    currentWindow=new SettingsWindow();
    currentWindow->show();
    this->close();
}

void SelectionWindow::goToStart(){
    currentWindow=new StartWindow();
    currentWindow->show();
    this->close();
}
