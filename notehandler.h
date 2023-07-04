#ifndef NOTEHANDLER_H
#define NOTEHANDLER_H
#include<note.h>
#include<QTimer>
#include<stack>
#include<queue>
#include<QtMultimedia>
#include<shutteranimation.h>
#include<openanimation.h>

const int fontHeight=50;
const int fontWidth=32;
const int fontPadding=10;
const int judgementWidth=256;
const int judgementHeight=72;

const int judgementShowTime=200;

class NoteHandler:public QObject{ //完成startHandle
    Q_OBJECT
public:
    QWidget* gameWindow;
    int totalNotes;
    int moveTimeArr[10000];
    int laneArr[10000];
    std::queue<Note*> handledNotes[5];
    std::stack<Note*> notePool;
    QMediaPlayer* player;
    QAudioOutput* audioOutput;
    OpenAnimation* openAnimation;
    ShutterAnimation* resultShutterAnimation;
    NoteHandler(QWidget* _gameWindow);
    ~NoteHandler();
    //统计
    int total_perfect;
    int total_great;
    int total_good;
    int total_miss;
    int combo;
    int max_combo;
    double accuracy;
    int score;
    int curComboMul;
    int comboMul[4]; //*0.01
    //显示从低位到高位
    QLabel* scoreEntity[9];
    QLabel* accuracyEntity[7];
    QLabel* comboEntity[6];
    int comboDigitToPos[7][2];
    QLabel* judgementEntity;
    int inProgressJudgementShow;
    QPixmap background[17]; //0-9:数字0-9 10:点 11:百分号 12:MISS 13:GOOD 14:GREAT 15:PERFECT 16:空白

public slots:
    void judge(int lane);
    void dataUpdate(bool judged,int judgement,int lane);
    void dataShow(int judgement);
    void judgementShow();
    void playMusic();
    void outputResult();

signals:
    void dataShowSig(int judgement); //0-MISS 1-GOOD 2-GREAT 3-PERFECT
};


#endif // NOTEHANDLER_H
