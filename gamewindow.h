#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QKeyEvent>
#include <notehandler.h>
#include<string>
#include<shutteranimation.h>

extern std::string PATH;
extern std::string songName;

const int coverLength=60;

QT_BEGIN_NAMESPACE
namespace Ui { class GameWindow; }
QT_END_NAMESPACE

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    void keyPressEvent(QKeyEvent* event);
    NoteHandler* notehandler;
    QLabel* background;
    QLabel* cover;
    QLabel* songname;
    QLabel* artist;

private:
    Ui::GameWindow *ui;

signals:
    void keyPressed(int id);
};
#endif // GAMEWINDOW_H
