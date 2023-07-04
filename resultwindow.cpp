#include "resultwindow.h"
#include<note.h>
#include<selectionwindow.h>
#include<gamewindow.h>
#include<fstream>

extern int songID;
extern QWidget* currentWindow;
extern std::string PATH;
extern std::string songName;

ResultWindow::ResultWindow(QWidget *parent)
    : QWidget{parent}
{
    this->resize(windowWidth,windowHeight);
    this->setAttribute(Qt::WA_DeleteOnClose);

    //back按键
    int id=QFontDatabase::addApplicationFont(":/images/images/font/Exo-Medium 22.05.14.ttf");
    QString family=QFontDatabase::applicationFontFamilies(id).at(0);
    QFont title(family);

    back=new QPushButton(this);
    back->setStyleSheet("border-image:url(:/images/images/buttons/back.png);");
    back->resize(324,97);
    connect(back,&QPushButton::pressed,[=]{
        back->setStyleSheet("border-image:url(:/images/images/buttons/back_pressed.png);");
    });
    back->move(0,800-97);
    back->show();

    backContent=new QLabel(this);
    title.setPixelSize(30);
    backContent->setFont(title);
    backContent->resize(70,30);
    backContent->move(100,725);
    backContent->setText("Back");
    backContent->setStyleSheet("color:rgb(51,0,51);");
    backContent->show();

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
        qDebug()<<"RESULT METADATA READ ERROR";
    }
    std::string artiststr;
    std::getline(metadata,artiststr);
    metadata.close();

    QPalette palette=songname->palette();
    palette.setColor(songname->foregroundRole(),Qt::white);
    songname->setPalette(palette);
    palette=artist->palette();
    palette.setColor(artist->foregroundRole(),Qt::white);
    artist->setPalette(palette);

    songname->setAlignment(Qt::AlignCenter);
    artist->setAlignment(Qt::AlignCenter);
    int songnameSize=std::min(coverLength/3,(leftPadding-coverLength)/((int)songName.length()));
    title.setPixelSize(songnameSize);
    songname->setFont(title);
    int artistSize=std::min(coverLength/3,(leftPadding-coverLength)/((int)artiststr.length()));
    title.setPixelSize(artistSize);
    artist->setFont(title);
    songname->move(coverLength,0);
    artist->move(coverLength,coverLength/2);
    songname->setText(QString::fromStdString(songName));
    artist->setText(QString::fromStdString(artiststr));
    songname->show();
    artist->show();

    //背景图
    QPalette pa(this->palette());
    QImage img = QImage(":/images/images/background/6_b.jpg");
    img = img.scaled(this->size());
    QBrush *pic = new QBrush(img);
    pa.setBrush(QPalette::Window,*pic);
    this->setPalette(pa);

    //数据显示
    int yshift=-40;
    //读取
    std::ifstream result;
    int sc_res,p_res,gr_res,gd_res,m_res,maxc_res;
    double acc_res;
    result.open(PATH+"result.txt");
    if(!result){
        qDebug()<<"RESULT READ ERROR";
    }
    result>>p_res>>gr_res>>gd_res>>m_res>>maxc_res>>acc_res>>sc_res;
    result.close();

    //衬底
    base=new QLabel(this);
    base->resize(732,694);
    base->move(523,93+yshift);
    base->setStyleSheet("background-color:rgba(0,0,0,150);");
    base->show();

    //评价
    rating=new QLabel(this);
    rating->resize(420,510);
    rating->move(60,75);
    if(acc_res>=0.96) rating->setPixmap(QPixmap(":/images/images/ratings/S.png"));
    else if(acc_res>=0.9) rating->setPixmap(QPixmap(":/images/images/ratings/A.png"));
    else if(acc_res>=0.8) rating->setPixmap(QPixmap(":/images/images/ratings/B.png"));
    else if(acc_res>=0.7) rating->setPixmap(QPixmap(":/images/images/ratings/C.png"));
    else rating->setPixmap(QPixmap(":/images/images/ratings/D.png"));
    rating->show();

    //分数
    score=new QLabel(this);
    scoreTitle=new QLabel(this);
    score->setStyleSheet("color:white;");
    scoreTitle->setStyleSheet("color:white;");
    title.setPixelSize(50);
    scoreTitle->setFont(title);
    title.setPixelSize(40);
    score->setFont(title);
    score->resize(682,40);
    scoreTitle->resize(200,50);
    scoreTitle->move(574,153+yshift);
    score->move(774,158+yshift);
    scoreTitle->setText("SCORE");
    score->setText(QString::fromStdString(std::to_string(sc_res)));
    score->show();
    scoreTitle->show();

    //判定
    title.setPixelSize(40);
    for(int i=0;i<4;i++){
        judgeNum[i]=new QLabel(this);
        judgeNum[i]->setStyleSheet("color:white;");
        judgeNum[i]->setFont(title);
        judgeNum[i]->resize(100,72);
        judgeNum[i]->setAlignment(Qt::AlignVCenter);
        judgeTitle[i]=new QLabel(this);
        judgeTitle[i]->resize(256,72);
        judgeNum[i]->show();
        judgeTitle[i]->show();
    }
    judgeNum[0]->move(794,273+yshift);
    judgeNum[1]->move(1170,273+yshift);
    judgeNum[2]->move(794,415+yshift);
    judgeNum[3]->move(1170,415+yshift);
    judgeTitle[0]->move(528,273+yshift);
    judgeTitle[1]->move(904,273+yshift);
    judgeTitle[2]->move(528,415+yshift);
    judgeTitle[3]->move(904,415+yshift);
    judgeTitle[0]->setPixmap(QPixmap(":/images/images/judge/mania-hit300.png"));
    judgeTitle[1]->setPixmap(QPixmap(":/images/images/judge/mania-hit200.png"));
    judgeTitle[2]->setPixmap(QPixmap(":/images/images/judge/mania-hit100.png"));
    judgeTitle[3]->setPixmap(QPixmap(":/images/images/judge/mania-hit0.png"));
    judgeNum[0]->setText(QString::fromStdString(std::to_string(p_res)));
    judgeNum[1]->setText(QString::fromStdString(std::to_string(gr_res)));
    judgeNum[2]->setText(QString::fromStdString(std::to_string(gd_res)));
    judgeNum[3]->setText(QString::fromStdString(std::to_string(m_res)));

    //最大连击数
    maxCombo=new QLabel(this);
    maxComboTitle=new QLabel(this);
    maxComboTitle->move(574,677+yshift);
    maxCombo->move(894,682+yshift);
    title.setPixelSize(50);
    maxComboTitle->setFont(title);
    title.setPixelSize(40);
    maxCombo->setFont(title);
    maxCombo->setStyleSheet("color:white;");
    maxComboTitle->setStyleSheet("color:white;");
    maxComboTitle->setText("MAX COMBO");
    maxCombo->setText(QString::fromStdString(std::to_string(maxc_res)));
    maxCombo->resize(682,40);
    maxComboTitle->resize(300,50);
    maxCombo->show();
    maxComboTitle->show();

    //准确度
    accuracy=new QLabel(this);
    accuracyTitle=new QLabel(this);
    accuracyTitle->move(574,557+yshift);
    accuracy->move(844,562+yshift);
    title.setPixelSize(50);
    accuracyTitle->setFont(title);
    title.setPixelSize(40);
    accuracy->setFont(title);
    accuracy->setStyleSheet("color:white;");
    accuracyTitle->setStyleSheet("color:white;");
    int acc=(int)(acc_res*10000);
    std::string afterPoint;
    if(acc%100>=10) afterPoint=std::to_string(acc%100);
    else if(acc%100>0) afterPoint="0"+std::to_string(acc%100);
    else afterPoint="00";
    accuracy->setText(QString::fromStdString(std::to_string(acc/100)+"."+afterPoint+"%"));
    accuracyTitle->setText("ACCURACY");
    accuracy->resize(682,40);
    accuracyTitle->resize(250,50);
    accuracy->show();
    accuracyTitle->show();

    //游戏界面到结算界面的切换动画
    openAnimation=new OpenAnimation(this);
    openAnimation->play();

    //结算界面到选曲界面的切换动画
    shutterAnimation=new ShutterAnimation(this);
    connect(shutterAnimation,SIGNAL(finished()),this,SLOT(goToSelection()));
    connect(back,&QPushButton::released,[=]{
        back->setStyleSheet("border-image:url(:/images/images/buttons/back.png);");
        shutterAnimation->play();
    });
}

ResultWindow::~ResultWindow(){
    delete back;
    delete backContent;
    delete openAnimation;
    delete shutterAnimation;
    delete cover;
    delete songname;
    delete artist;
    delete rating;
    delete score;
    delete scoreTitle;
    for(int i=0;i<4;i++){
        delete judgeNum[i];
        delete judgeTitle[i];
    }
    delete maxComboTitle;
    delete maxCombo;
    delete accuracyTitle;
    delete accuracy;
    delete base;
}

void ResultWindow::goToSelection(){
    currentWindow=new SelectionWindow(nullptr,songID);
    currentWindow->show();
    this->close();
}
