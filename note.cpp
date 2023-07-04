#include"note.h"

Note::Note(QWidget* gameWindow,int duration,int _moveTime,int _lane){
   note=new QLabel(gameWindow);
   moveTime=_moveTime;
   lane=_lane;
   judged=0;
   judgement=-1; //0-miss 1-great 2-good 3-perfect
   note->move(leftPadding+laneWidth*(lane-1),-noteHeight);

   //animation相关
   note_drop.setPropertyName("pos");
   note_drop.setTargetObject(note);
   note_drop.setStartValue(note->pos());
   note_drop.setEndValue(QPoint(leftPadding+laneWidth*(lane-1),windowHeight));
   note_drop.setDuration(duration);

   //音符背景相关
   QPixmap noteBackground;
   if(lane==1||lane==4) noteBackground.load(":/images/images/mania-note1.png");
   if(lane==2||lane==3) noteBackground.load(":/images/images/mania-note2.png");
   note->setPixmap(noteBackground);
   note->resize(noteWidth,noteHeight);
}

void Note::startMove(){
    note->show();
    note_drop.start();
}

void Note::reset(int _lane,int _moveTime){ //有可能有bug，或许需要重新设animation相关
    lane=_lane;
    moveTime=_moveTime;
    judged=0;
    note->move(leftPadding+laneWidth*(lane-1),-noteHeight);
}

Note::~Note(){
   delete note;
}

void Note::passData(){
   if(!judged){
       qDebug()<<"MISS";
       emit dataUpdate(judged,judgement,lane);
   }
}
