#ifndef SELECTIONWINDOW_H
#define SELECTIONWINDOW_H

#include <QWidget>
#include<QPushButton>
#include<QLabel>
#include<openanimation.h>
#include<shutteranimation.h>

namespace Ui {
class SelectionWindow;
}

class SelectionWindow : public QWidget
{
    Q_OBJECT

public:
    int current_first_song_index;
    int total_songs;
    QLabel* covers[5];
    QLabel* metadata[3]; //0-曲名 1-by 2-曲师
    QLabel* backContent;
    QPushButton* start_game;
    QPushButton* settings;
    QPushButton* up;
    QPushButton* down;
    QPushButton* back;
    std::string songnames[100];
    OpenAnimation* openAnimation;
    ShutterAnimation* gameShutterAnimation;
    ShutterAnimation* settingsShutterAnimation;
    ShutterAnimation* backShutterAnimation;
    explicit SelectionWindow(QWidget *parent = nullptr,int currentSong=0);
    ~SelectionWindow();

public slots:
    void go_up();
    void go_down();
    void setMetadata(int ind);
    void startGame();
    void goToSettings();
    void goToStart();

private:
    Ui::SelectionWindow *ui;
};

#endif // SELECTIONWINDOW_H
