#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>
#include <openanimation.h>
#include <shutteranimation.h>
#include <QLabel>
#include <QPushButton>

class ResultWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ResultWindow(QWidget *parent = nullptr);
    OpenAnimation* openAnimation;
    ShutterAnimation* shutterAnimation;
    QLabel* backContent;
    QPushButton* back;
    QLabel* rating;
    QLabel* cover;
    QLabel* songname;
    QLabel* artist;
    QLabel* score;
    QLabel* scoreTitle;
    QLabel* judgeNum[4]; //0-Perfect 1-Great 2-Good 3-Miss
    QLabel* judgeTitle[4];
    QLabel* maxCombo;
    QLabel* maxComboTitle;
    QLabel* accuracy;
    QLabel* accuracyTitle;
    QLabel* base;
    ~ResultWindow();

public slots:
    void goToSelection();

signals:

};

#endif // RESULTWINDOW_H
