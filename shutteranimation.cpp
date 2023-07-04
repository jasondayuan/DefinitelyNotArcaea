#include "shutteranimation.h"
extern std::string PATH;

ShutterAnimation::ShutterAnimation(QWidget *parent,int duration,int windowWidth,int windowHeight): QObject{parent}{
    parentWindow=parent;
    int shutterSize[2][2];
    shutterSize[0][0]=1216;
    shutterSize[0][1]=965;
    shutterSize[1][0]=425;
    shutterSize[1][1]=965;

    //图片设置
    for(int i=0;i<2;i++){
        shutter[i]=new QLabel(parent);
        QPixmap sht;
        if(i==0){
            sht.load(":/images/images/shutter/shutter_l.png");
        }else{
            sht.load(":/images/images/shutter/shutter_r.png");
        }
        shutter[i]->resize(shutterSize[i][0],shutterSize[i][1]);
        if(i==0) shutter[i]->move(-shutterSize[i][0],windowHeight/2-shutterSize[i][1]/2);
        else shutter[i]->move(windowWidth,windowHeight/2-shutterSize[i][1]/2);
        shutter[i]->setPixmap(sht);
    }

    //动画设置
    for(int i=0;i<2;i++){
        animation[i]=new QPropertyAnimation(this);
        animation[i]->setEasingCurve(QEasingCurve::OutQuad);
        animation[i]->setPropertyName("pos");
        animation[i]->setTargetObject(shutter[i]);
        animation[i]->setStartValue(shutter[i]->pos());
        if(i==0) animation[i]->setEndValue(QPoint(0,windowHeight/2-shutterSize[i][1]/2));
        else animation[i]->setEndValue(QPoint(windowWidth-shutterSize[i][0],windowHeight/2-shutterSize[i][1]/2));
        animation[i]->setDuration(duration);
    }
    //音效设置
    player=new QMediaPlayer(parent);
    audioOutput=new QAudioOutput(parent);
    player->setAudioOutput(audioOutput);
    player->setSource(QUrl(QString::fromStdString(PATH+"soundeffects/shutter_close.wav")));

    //connect结束信号
    connect(animation[0],SIGNAL(finished()),this,SLOT(emitFinished()));
}

void ShutterAnimation::play(){
    shutter[0]->show();
    shutter[1]->show();
    shutter[0]->raise();
    shutter[1]->raise();
    animation[0]->start();
    animation[1]->start();
    player->play();
}

void ShutterAnimation::emitFinished(){
    emit finished();
}

ShutterAnimation::~ShutterAnimation(){
    delete shutter[0];
    delete shutter[1];
    delete animation[0];
    delete animation[1];
    delete player;
    delete audioOutput;
}
