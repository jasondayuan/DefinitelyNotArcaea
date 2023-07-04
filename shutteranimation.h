#ifndef SHUTTERANIMATION_H
#define SHUTTERANIMATION_H

#include <QObject>
#include<QLabel>
#include<QPropertyAnimation>
#include<QtMultimedia>
#include<QPixmap>

class ShutterAnimation : public QObject
{
    Q_OBJECT
public:
    explicit ShutterAnimation(QWidget *parent = nullptr,int duration=1000,int windowWidth=1280,int windowHeight=800);
    QWidget* parentWindow;
    QPropertyAnimation* animation[2]; //0-左侧 1-右侧
    QLabel* shutter[2]; //0-左侧 1-右侧
    QAudioOutput* audioOutput;
    QMediaPlayer* player;
    ~ShutterAnimation();

public slots:
    void play();
    void emitFinished();

signals:
    void finished();
};

#endif // SHUTTERANIMATION_H
