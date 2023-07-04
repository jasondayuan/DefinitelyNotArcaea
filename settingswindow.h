#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include<QLabel>
#include<QSpinBox>
#include<QPushButton>
#include<selectionwindow.h>
#include<shutteranimation.h>
#include<openanimation.h>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();
    QLabel* mask;
    QLabel* settingsHead;
    QLabel* settingsWindow;
    QLabel* headTitle;
    QLabel* speedTitle;
    QLabel* offsetTitle;
    QSpinBox* speed;
    QSpinBox* offset;
    QPushButton* back;
    QLabel* backContent;
    OpenAnimation* openAnimation;
    ShutterAnimation* shutterAnimation;

public slots:
    void goToSelection();

private:
    Ui::SettingsWindow *ui;
};

#endif // SETTINGSWINDOW_H
