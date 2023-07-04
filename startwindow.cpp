#include "startwindow.h"
#include "ui_startwindow.h"
#include "selectionwindow.h"

extern QWidget* currentWindow;

StartWindow::StartWindow(QWidget *parent,bool firstOpen) :
    QWidget(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    //背景
    QPalette pa(this->palette());
    QImage img = QImage(":/images/images/background/5_b.jpg");
    img = img.scaled(this->size());
    QBrush *pic = new QBrush(img);
    pa.setBrush(QPalette::Window,*pic);
    this->setPalette(pa);

    //按键
    play=new QPushButton(this);
    play->setStyleSheet("border-image:url(:/images/images/buttons/world_play_button_split.png);");
    play->resize(438,185);
    play->move(1280-70-438,800-185-50);
    play->show();
    connect(play,&QPushButton::pressed,[=]{
        play->setStyleSheet("border-image:url(:/images/images/buttons/world_play_button_split_pressed.png);");
    });

    //标题
    arcaea=new QLabel(this);
    QPixmap arc;
    arc.load(":/images/images/title.png");
    arcaea->setPixmap(arc);
    arcaea->resize(968,260);
    arcaea->move(156,100);
    arcaea->show();

    int id=QFontDatabase::addApplicationFont(":/images/images/font/Exo-Medium 22.05.14.ttf");
    QString family=QFontDatabase::applicationFontFamilies(id).at(0);
    QFont title(family);
    title.setWeight(QFont::Black);
    title.setPixelSize(30);
    definitelynot=new QLabel(this);
    definitelynot->move(130,200);
    definitelynot->setFont(title);
    definitelynot->setText("(Definitely Not)");
    definitelynot->setStyleSheet("color:rgb(51,0,51);");
    definitelynot->show();

    //退出按键
    quit=new QPushButton(this);
    quit->setStyleSheet("border-image:url(:/images/images/buttons/back.png);");
    quit->resize(324,97);
    connect(quit,&QPushButton::pressed,[=]{
        quit->setStyleSheet("border-image:url(:/images/images/buttons/back_pressed.png);");
    });
    connect(quit,&QPushButton::released,[=]{
        quit->setStyleSheet("border-image:url(:/images/images/buttons/back_pressed.png);");
        this->close();
    });
    quit->move(0,800-97);
    quit->show();

    quitContent=new QLabel(this);
    title.setPixelSize(30);
    quitContent->setFont(title);
    quitContent->resize(70,30);
    quitContent->move(100,725);
    quitContent->setText("Quit");
    quitContent->setStyleSheet("color:rgb(51,0,51);");
    quitContent->show();

    //切换动画
    openAnimation=new OpenAnimation(this);
    if(!firstOpen) openAnimation->play();
    else openAnimation->hide();

    //从开始界面到选曲界面
    shutterAnimation=new ShutterAnimation(this);
    connect(play,&QPushButton::released,[=]{
        play->setStyleSheet("border-image:url(:/images/images/buttons/world_play_button_split.png);");
        shutterAnimation->play();
    });
    connect(shutterAnimation,SIGNAL(finished()),this,SLOT(goToSelection()));
}

StartWindow::~StartWindow()
{
    delete play;
    delete openAnimation;
    delete shutterAnimation;
    delete definitelynot;
    delete arcaea;
    delete quit;
    delete quitContent;
    delete ui;
}

void StartWindow::goToSelection(){
    currentWindow=new SelectionWindow();
    currentWindow->show();
    this->close();
}
