#include "settingswindow.h"
#include "ui_settingswindow.h"
#include<QFontDatabase>
#include<fstream>

extern QWidget* currentWindow;
extern std::string PATH;
extern int songID;

SettingsWindow::SettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    mask=new QLabel(this);
    mask->resize(1280,800);
    mask->move(0,0);
    mask->setStyleSheet("background-image:url(:/images/images/background/4_b.jpg);");
    mask->show();

    QPixmap window;
    window.load(":/images/images/background/dialogbase.png");
    settingsWindow=new QLabel(this);
    settingsWindow->setPixmap(window);
    settingsWindow->resize(795,691);
    settingsWindow->move(242,70);
    settingsWindow->show();

    QPixmap head;
    head.load(":/images/images/background/dialogtop.png");
    settingsHead=new QLabel(this);
    settingsHead->resize(796,159);
    settingsHead->move(241,39);
    settingsHead->setPixmap(head);
    settingsHead->show();

    headTitle=new QLabel(this);
    headTitle->resize(796,159);
    headTitle->move(241,0);
    headTitle->setAlignment(Qt::AlignCenter);
    int id=QFontDatabase::addApplicationFont(":/images/images/font/Exo-Medium 22.05.14.ttf");
    QString family=QFontDatabase::applicationFontFamilies(id).at(0);
    QFont title(family);
    QPalette palette=headTitle->palette();
    palette.setColor(headTitle->foregroundRole(),Qt::white);
    headTitle->setPalette(palette);
    title.setWeight(QFont::Normal);
    title.setPixelSize(30);
    headTitle->setFont(title);
    headTitle->setText("Settings");
    headTitle->show();

    back=new QPushButton(this);
    back->resize(440,80);
    back->move(640-220,625);
    back->setStyleSheet("border-image:url(:/images/images/buttons/button.png);");
    connect(back,&QPushButton::pressed,[=]{
        back->setStyleSheet("border-image:url(:/images/images/buttons/button-pressed.png);");
    });
    back->show();

    backContent=new QLabel(this);
    backContent->resize(100,80);
    backContent->move(590,625);
    backContent->setAlignment(Qt::AlignCenter);
    title.setPixelSize(40);
    backContent->setFont(title);
    backContent->setText("Back");
    backContent->setStyleSheet("color:rgb(51,0,51);");
    backContent->show();

    speedTitle=new QLabel(this);
    offsetTitle=new QLabel(this);
    speedTitle->setAlignment(Qt::AlignCenter);
    offsetTitle->setAlignment(Qt::AlignCenter);
    speedTitle->setStyleSheet("color:rgb(51,0,51);");
    offsetTitle->setStyleSheet("color:rgb(51,0,51);");
    title.setPixelSize(40);
    speedTitle->setFont(title);
    offsetTitle->setFont(title);
    speedTitle->setText("Speed");
    offsetTitle->setText("Offset");
    speedTitle->resize(400,50);
    offsetTitle->resize(400,50);
    speedTitle->move(240,325);
    offsetTitle->move(640,325);
    speedTitle->show();
    offsetTitle->show();

    speed=new QSpinBox(this);
    offset=new QSpinBox(this);
    speed->setRange(200,2000);
    offset->setRange(-100,100);
    speed->resize(300,50);
    offset->resize(300,50);
    speed->move(300,405);
    offset->move(700,405);
    speed->show();
    offset->show();

    speed->setFocusPolicy(Qt::ClickFocus);
    offset->setFocusPolicy(Qt::ClickFocus);

    int initSpeed=0,initOffset=0;
    std::ifstream settingsRead;
    settingsRead.open(PATH+"settings.txt");
    if(!settingsRead){
        qDebug()<<"SETTINGS READ ERROR";
    }
    settingsRead>>initSpeed>>initOffset;
    speed->setValue(initSpeed);
    offset->setValue(initOffset);
    settingsRead.close();

    openAnimation=new OpenAnimation(this);
    openAnimation->play();

    shutterAnimation=new ShutterAnimation(this);
    connect(back,&QPushButton::released,[=]{
        back->setStyleSheet("border-image:url(:/images/images/buttons/button.png);");
        shutterAnimation->play();
    });
    connect(shutterAnimation,SIGNAL(finished()),this,SLOT(goToSelection()));
}

SettingsWindow::~SettingsWindow()
{
    delete mask;
    delete settingsHead;
    delete settingsWindow;
    delete headTitle;
    delete speedTitle;
    delete offsetTitle;
    delete speed;
    delete offset;
    delete back;
    delete backContent;
    delete openAnimation;
    delete shutterAnimation;
    delete ui;
}

void SettingsWindow::goToSelection(){
    int curSpeed=speed->value(),curOffset=offset->value();
    std::ofstream settingsWrite;
    settingsWrite.open(PATH+"settings.txt");
    if(!settingsWrite){
        qDebug()<<"SETTINGS WRITE ERROR";
    }
    settingsWrite<<curSpeed<<" "<<curOffset;
    settingsWrite.close();
    currentWindow=new SelectionWindow(nullptr,songID);
    currentWindow->show();
    this->close();
}
