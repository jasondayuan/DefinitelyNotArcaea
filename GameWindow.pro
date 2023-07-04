QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    gamewindow.cpp \
    note.cpp \
    notehandler.cpp \
    openanimation.cpp \
    resultwindow.cpp \
    selectionwindow.cpp \
    settingswindow.cpp \
    shutteranimation.cpp \
    startwindow.cpp

HEADERS += \
    gamewindow.h \
    note.h \
    notehandler.h \
    openanimation.h \
    resultwindow.h \
    selectionwindow.h \
    settingswindow.h \
    shutteranimation.h \
    startwindow.h

FORMS += \
    gamewindow.ui \
    selectionwindow.ui \
    settingswindow.ui \
    startwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT += multimedia

RESOURCES += \
    images.qrc \
    se.qrc

DISTFILES += \
    charts/Lachryma.txt \
    images/background/bg_dark.png \
    images/background/lane_back.png \
    images/font/combo-0.png \
    images/font/combo-0@2x.png \
    images/font/combo-1.png \
    images/font/combo-1@2x.png \
    images/font/combo-2.png \
    images/font/combo-2@2x.png \
    images/font/combo-3.png \
    images/font/combo-3@2x.png \
    images/font/combo-4.png \
    images/font/combo-4@2x.png \
    images/font/combo-5.png \
    images/font/combo-5@2x.png \
    images/font/combo-6.png \
    images/font/combo-6@2x.png \
    images/font/combo-7.png \
    images/font/combo-7@2x.png \
    images/font/combo-8.png \
    images/font/combo-8@2x.png \
    images/font/combo-9.png \
    images/font/combo-9@2x.png \
    images/font/default-comma.png \
    images/font/default-comma@2x.png \
    images/font/default-dot.png \
    images/font/default-percent.png \
    images/font/default-x.png \
    images/judge/mania-hit0.png \
    images/judge/mania-hit0@2x.png \
    images/judge/mania-hit100.png \
    images/judge/mania-hit100@2x.png \
    images/judge/mania-hit200.png \
    images/judge/mania-hit200@2x.png \
    images/judge/mania-hit300.png \
    images/judge/mania-hit300@2x.png \
    images/judge/mania-hit300g.png \
    images/judge/mania-hit50.png \
    images/judge/mania-hit50@2x.png \
    images/judge_line.png \
    images/mania-note1.png \
    images/mania-note2.png \
    images/sep_line.png \
    images/song_score_tab.png \
    metadata/Lachryma.txt \
    settings.txt
