#ifndef OPENANIMATION_H
#define OPENANIMATION_H

#include <QObject>
#include<QLabel>
#include<QPropertyAnimation>
#include<QtMultimedia>
#include<QPixmap>

class OpenAnimation : public QObject
{
    Q_OBJECT
public:
    explicit OpenAnimation(QWidget *parent = nullptr,int duration=1000,int windowWidth=1280,int windowHeight=800);
    QWidget* parentWindow;
    QPropertyAnimation* animation[2]; //0-左侧 1-右侧
    QLabel* shutter[2]; //0-左侧 1-右侧
    QAudioOutput* audioOutput;
    QMediaPlayer* player;
    ~OpenAnimation();
signals:
    void finished();
public slots:
    void play();
    void emitFinished();
    void hide();
};

#endif // OPENANIMATION_H
