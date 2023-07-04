#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <shutteranimation.h>
#include <openanimation.h>

namespace Ui {
class StartWindow;
}

class StartWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr,bool firstOpen=false);
    ~StartWindow();
    QLabel* definitelynot;
    QLabel* arcaea;
    QPushButton* play;
    QPushButton* quit;
    QLabel* quitContent;
    OpenAnimation* openAnimation;
    ShutterAnimation* shutterAnimation;

public slots:
    void goToSelection();

private:
    Ui::StartWindow *ui;
};

#endif // STARTWINDOW_H
