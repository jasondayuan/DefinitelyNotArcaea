#include<note.h>
#include<notehandler.h>
#include<fstream>
#include<iostream>
#include<string>
#include<QDir>
#include<QDebug>
#include<resultwindow.h>

extern QWidget* currentWindow;

int DURATION=3000;
int OFFSET=0;
int BASETIME=3000;
int TAILTIME=3000;

int UPPER_GOOD;
int UPPER_GREAT;
int UPPER_PERFECT;
int LOWER_PERFECT;
int LOWER_GREAT;
int LOWER_GOOD;

const int PERFECT=50;
const int GREAT=70;
const int GOOD=90;

std::string PATH="/Users/jasonzhao/Desktop/Qt/merge/GameWindow/"; //改成当前这个项目中的第一层文件所在的路径
std::string songName;
int songID=0;

NoteHandler::NoteHandler(QWidget* _gameWindow){
   gameWindow=_gameWindow;
   total_miss=0;
   total_good=0;
   total_great=0;
   total_perfect=0;
   combo=0;
   max_combo=0;
   accuracy=0;
   score=0;
   inProgressJudgementShow=0;
   curComboMul=0;
   comboMul[0]=-32;
   comboMul[1]=-8;
   comboMul[2]=-2;
   comboMul[3]=1;

   //读取谱面文件
   std::ifstream chart;
   chart.open(PATH+"charts/"+songName+".txt");
   if(!chart.is_open()) qDebug()<<"CHART READ ERROR\n";
   chart>>totalNotes;
   for(int i=0;i<totalNotes;i++){
       chart>>moveTimeArr[i]>>laneArr[i];
   }
   chart.close();

   //读取偏移和速度
   std::ifstream settings;
   settings.open(PATH+"settings.txt");
   if(!settings.is_open()) qDebug()<<"GAMEPLAY SETTINGS READ ERROR\n";
   settings>>DURATION>>OFFSET;
   BASETIME+=OFFSET;
   settings.close();

   //音乐设置
   player=new QMediaPlayer(this);
   audioOutput=new QAudioOutput(this);
   player->setAudioOutput(audioOutput);
   player->setSource(QUrl(QString::fromStdString(PATH+"songs/"+songName+".ogg")));

   //设置handledNotes
   int lastNoteTime=0;
   for(int i=0;i<totalNotes;i++){
       if(moveTimeArr[i]>lastNoteTime) lastNoteTime=moveTimeArr[i];
       Note* newNote=new Note(gameWindow,DURATION,moveTimeArr[i],laneArr[i]);
       connect(&newNote->note_drop,SIGNAL(finished()),newNote,SLOT(passData())); //数据传输——late miss
       connect(newNote,SIGNAL(dataUpdate(bool,int,int)),this,SLOT(dataUpdate(bool,int,int)));
       QTimer::singleShot(BASETIME+moveTimeArr[i],Qt::PreciseTimer,newNote,SLOT(startMove()));
       handledNotes[laneArr[i]].push(newNote);
   }

   //音乐播放
   QTimer::singleShot(BASETIME,Qt::PreciseTimer,this,SLOT(playMusic()));

   //计算判定范围
   double PIXEL_PER_SEC=((double)windowHeight)/((double)DURATION);
   UPPER_GOOD=laneLength-((int)(((double)GOOD)*PIXEL_PER_SEC));
   LOWER_GOOD=laneLength+((int)(((double)GOOD)*PIXEL_PER_SEC));
   UPPER_GREAT=laneLength-((int)(((double)GREAT)*PIXEL_PER_SEC));
   LOWER_GREAT=laneLength+((int)(((double)GREAT)*PIXEL_PER_SEC));
   UPPER_PERFECT=laneLength-((int)(((double)PERFECT)*PIXEL_PER_SEC));
   LOWER_PERFECT=laneLength+((int)(((double)PERFECT)*PIXEL_PER_SEC));

   //数据显示相关
       //加载Pixmap
       //background数组设置 0-9：数字0-9 10：点 11:百分号 12:MISS 13:GOOD 14:GREAT 15:PERFECT 16:空白
   background[0].load(":/images/images/font/combo-0.png");
   background[1].load(":/images/images/font/combo-1.png");
   background[2].load(":/images/images/font/combo-2.png");
   background[3].load(":/images/images/font/combo-3.png");
   background[4].load(":/images/images/font/combo-4.png");
   background[5].load(":/images/images/font/combo-5.png");
   background[6].load(":/images/images/font/combo-6.png");
   background[7].load(":/images/images/font/combo-7.png");
   background[8].load(":/images/images/font/combo-8.png");
   background[9].load(":/images/images/font/combo-9.png");
   background[10].load(":/images/images/font/default-dot.png");
   background[11].load(":/images/images/font/default-percent.png");
   background[12].load(":/images/images/judge/mania-hit0.png");
   background[13].load(":/images/images/judge/mania-hit100.png");
   background[14].load(":/images/images/judge/mania-hit200.png");
   background[15].load(":/images/images/judge/mania-hit300.png");
   background[16].load(":/images/images/judge/mania-hit300g.png");
       //entity设置
   for(int i=0;i<9;i++){
       scoreEntity[i]=new QLabel(gameWindow);
       scoreEntity[i]->move(windowWidth-(i+1)*fontWidth-fontPadding,0);
       scoreEntity[i]->setPixmap(background[0]);
       scoreEntity[i]->resize(fontWidth,fontHeight);
       scoreEntity[i]->show();
   }
   for(int i=0;i<7;i++){
       accuracyEntity[i]=new QLabel(gameWindow);
       accuracyEntity[i]->move(windowWidth-(i+1)*fontWidth-fontPadding,fontHeight);
       if(i!=0&&i!=3&&i!=5&&i!=6) accuracyEntity[i]->setPixmap(background[0]);
       else if(i==0){
           accuracyEntity[i]->setPixmap(background[11]);
       }else if(i==3){
           accuracyEntity[i]->setPixmap(background[10]);
       }else{
           accuracyEntity[i]->setPixmap(background[16]);
       }
       accuracyEntity[i]->resize(fontWidth,fontHeight);
       accuracyEntity[i]->show();
   }
   for(int i=1;i<=6;i++){
       comboDigitToPos[i][0]=windowWidth/2-i*fontWidth/2+(i-1)*fontWidth;
       comboDigitToPos[i][1]=windowHeight/10;
   }
   for(int i=0;i<6;i++){
       comboEntity[i]=new QLabel(gameWindow);
       if(i==0){
           comboEntity[i]->setPixmap(background[0]);
           comboEntity[i]->move(comboDigitToPos[1][0],comboDigitToPos[1][1]);
       }else{
           comboEntity[i]->setPixmap(background[16]);
       }
       comboEntity[i]->resize(fontWidth,fontHeight);
       comboEntity[i]->show();
       comboEntity[i]->raise();
   }
   judgementEntity=new QLabel(gameWindow);
   judgementEntity->setPixmap(background[16]);
   judgementEntity->resize(judgementWidth,judgementHeight);
   judgementEntity->move(windowWidth/2-judgementWidth/2,windowHeight/10+50);
   judgementEntity->show();
   judgementEntity->raise();

   //切换动画
   openAnimation=new OpenAnimation(gameWindow);
   openAnimation->shutter[0]->raise();
   openAnimation->shutter[1]->raise();
   openAnimation->play();

   //结束后切换至结算界面
   resultShutterAnimation=new ShutterAnimation(gameWindow);
   int shutterStartTime=BASETIME+lastNoteTime+TAILTIME;
   QTimer::singleShot(shutterStartTime,Qt::PreciseTimer,resultShutterAnimation,SLOT(play()));

   //切换之前停止音乐播放，输出结果
   QTimer::singleShot(shutterStartTime-10,Qt::PreciseTimer,player,SLOT(stop()));
   QTimer::singleShot(shutterStartTime-10,Qt::PreciseTimer,this,SLOT(outputResult()));

   //关闭窗口和开启结算界面
   QTimer::singleShot(shutterStartTime+2000,Qt::PreciseTimer,nullptr,[=]{
       currentWindow=new ResultWindow();
       currentWindow->show();
   });
   QTimer::singleShot(shutterStartTime+2000,Qt::PreciseTimer,gameWindow,SLOT(close()));
}

NoteHandler::~NoteHandler(){
   while(!notePool.empty()){
       Note* curNote=notePool.top();
       delete curNote;
       notePool.pop();
   }
   delete player;
   delete audioOutput;
   delete resultShutterAnimation;
   for(int i=0;i<9;i++){
       delete scoreEntity[i];
   }
   for(int i=0;i<7;i++){
       delete accuracyEntity[i];
   }
   for(int i=0;i<6;i++){
       delete comboEntity[i];
   }
   delete judgementEntity;
}

void NoteHandler::judge(int lane){
   if(handledNotes[lane].empty()){
       return;
   }
   Note* judgedNote=handledNotes[lane].front();
   QPoint pos=judgedNote->note->pos();
   int y=pos.y();
   if(y<UPPER_GOOD){
       return;
   }
   if(y>LOWER_GOOD){
       if(!handledNotes[lane].empty()){
           qDebug()<<"PREJUDGED";
           judgedNote->judged=1;
           judgedNote->judgement=0;
           dataUpdate(1,0,lane);
           judgedNote=handledNotes[lane].front();
           y=judgedNote->note->pos().y();
       }else{
           return;
       }
   }
   if(y<UPPER_GREAT||y>LOWER_GREAT){
       qDebug()<<"GOOD";
       judgedNote->judged=1;
       judgedNote->judgement=1;
       dataUpdate(1,1,lane);
       return;
   }
   if(y<UPPER_PERFECT||y>LOWER_PERFECT){
       qDebug()<<"GREAT";
       judgedNote->judged=1;
       judgedNote->judgement=2;
       dataUpdate(1,2,lane);
       return;
   }
   if(y<=LOWER_PERFECT&&y>=UPPER_PERFECT){
       qDebug()<<"PERFECT";
       judgedNote->judged=1;
       judgedNote->judgement=3;
       dataUpdate(1,3,lane);
       return;
   }
}

void NoteHandler::dataUpdate(bool judged,int judgement,int lane){ //对handledNotes的pop由dataUpdate全权管理
   qDebug()<<judged<<judgement<<lane;
   int total_notes;
   if(!judged){ //LATE MISS
       combo=0;
       ++total_miss;
       curComboMul+=comboMul[0];
       if(curComboMul>100) curComboMul=100;
       if(curComboMul<0) curComboMul=0;
       total_notes=total_miss+total_good+total_great+total_perfect;
       accuracy=(0.25*total_good+0.75*total_great+1.00*total_perfect)/(double)total_notes;
       notePool.push(handledNotes[lane].front());
       handledNotes[lane].front()->note->hide();
       handledNotes[lane].pop();
       emit dataShowSig(0);
   }else{
       switch(judgement){
       case 0:  //PREJUDGED MISS
           combo=0;
           ++total_miss;
           curComboMul+=comboMul[judgement];
           if(curComboMul>100) curComboMul=100;
           if(curComboMul<0) curComboMul=0;
           total_notes=total_miss+total_good+total_great+total_perfect;
           accuracy=(0.25*total_good+0.75*total_great+1.00*total_perfect)/(double)total_notes;
           notePool.push(handledNotes[lane].front());
           handledNotes[lane].front()->note->hide();
           handledNotes[lane].pop();
           emit dataShowSig(0);
           break;
       case 1: //GOOD
           ++combo;
           if(combo>max_combo) max_combo=combo;
           ++total_good;
           curComboMul+=comboMul[judgement];
           if(curComboMul>100) curComboMul=100;
           if(curComboMul<0) curComboMul=0;
           score+=4*(curComboMul+100);
           total_notes=total_miss+total_good+total_great+total_perfect;
           accuracy=(0.25*total_good+0.75*total_great+1.00*total_perfect)/(double)total_notes;
           notePool.push(handledNotes[lane].front());
           handledNotes[lane].front()->note->hide();
           handledNotes[lane].pop();
           emit dataShowSig(1);
           break;
       case 2: //GREAT
           ++combo;
           if(combo>max_combo) max_combo=combo;
           ++total_great;
           curComboMul+=comboMul[judgement];
           if(curComboMul>100) curComboMul=100;
           if(curComboMul<0) curComboMul=0;
           score+=7*(curComboMul+100);
           total_notes=total_miss+total_good+total_great+total_perfect;
           accuracy=(0.25*total_good+0.75*total_great+1.00*total_perfect)/(double)total_notes;
           notePool.push(handledNotes[lane].front());
           handledNotes[lane].front()->note->hide();
           handledNotes[lane].pop();
           emit dataShowSig(2);
           break;
       case 3: //PERFECT
           ++combo;
           if(combo>max_combo) max_combo=combo;
           ++total_perfect;
           curComboMul+=comboMul[judgement];
           if(curComboMul>100) curComboMul=100;
           if(curComboMul<0) curComboMul=0;
           score+=10*(curComboMul+100);
           total_notes=total_miss+total_good+total_great+total_perfect;
           accuracy=(0.25*total_good+0.75*total_great+1.00*total_perfect)/(double)total_notes;
           notePool.push(handledNotes[lane].front());
           handledNotes[lane].front()->note->hide();
           handledNotes[lane].pop();
           emit dataShowSig(3);
           break;
       default:
           break;
       }
   }
   qDebug()<<"lane:"<<lane<<handledNotes[lane].empty()<<handledNotes[lane].size();
   qDebug()<<" ";
}

void NoteHandler::dataShow(int judgement){
   //score
   int tmp_score=score;
   for(int i=0;i<9;i++){
       if(tmp_score){
           scoreEntity[i]->setPixmap(background[tmp_score%10]);
           tmp_score/=10;
       }else{
           scoreEntity[i]->setPixmap(background[0]);
       }
   }
   //accuracy
   int tmp_accuracy=(int)(accuracy*10000);
   if(tmp_accuracy<100){ //0.xx
       accuracyEntity[1]->setPixmap(background[tmp_accuracy%10]);
       accuracyEntity[2]->setPixmap(background[tmp_accuracy/10]);
       accuracyEntity[4]->setPixmap(background[0]);
       accuracyEntity[5]->setPixmap(background[16]);
       accuracyEntity[6]->setPixmap(background[16]);
   }else if(tmp_accuracy<1000){ //<10.00
       accuracyEntity[1]->setPixmap(background[tmp_accuracy%10]);
       accuracyEntity[2]->setPixmap(background[(tmp_accuracy/10)%10]);
       accuracyEntity[4]->setPixmap(background[tmp_accuracy/100]);
       accuracyEntity[5]->setPixmap(background[16]);
       accuracyEntity[6]->setPixmap(background[16]);
   }else if(tmp_accuracy<10000){ //<100.00
       accuracyEntity[1]->setPixmap(background[tmp_accuracy%10]);
       accuracyEntity[2]->setPixmap(background[(tmp_accuracy/10)%10]);
       accuracyEntity[4]->setPixmap(background[(tmp_accuracy/100)%10]);
       accuracyEntity[5]->setPixmap(background[tmp_accuracy/1000]);
       accuracyEntity[6]->setPixmap(background[16]);
   }else{ //100.00
       accuracyEntity[1]->setPixmap(background[0]);
       accuracyEntity[2]->setPixmap(background[0]);
       accuracyEntity[4]->setPixmap(background[0]);
       accuracyEntity[5]->setPixmap(background[0]);
       accuracyEntity[6]->setPixmap(background[1]);
   }
   //combo
   int tmp_combo=combo;
   int digits=0;
   while(tmp_combo){
       ++digits;
       tmp_combo/=10;
   }
   if(combo==0) digits=1;
   tmp_combo=combo;
   int startX=comboDigitToPos[digits][0];
   int startY=comboDigitToPos[digits][1];
   for(int i=0;i<6;i++){
       if(i>=digits){
           comboEntity[i]->setPixmap(background[16]);
           continue;
       }
       comboEntity[i]->move(startX-i*fontWidth,startY);
       comboEntity[i]->setPixmap(background[tmp_combo%10]);
       tmp_combo/=10;
   }
   //judgement
   judgementEntity->setPixmap(background[12+judgement]);
   ++inProgressJudgementShow;
   QTimer::singleShot(judgementShowTime,Qt::PreciseTimer,this,SLOT(judgementShow()));
}

void NoteHandler::judgementShow(){
   --inProgressJudgementShow;
   if(inProgressJudgementShow==0){
       judgementEntity->setPixmap(background[16]);
   }
}

void NoteHandler::playMusic(){
   player->play();
}

void NoteHandler::outputResult(){
   std::ofstream result;
   result.open(PATH+"result.txt");
   if(!result){
       qDebug()<<"RESULT WRITE ERROR";
   }
   result<<total_perfect<<"\n"<<total_great<<"\n"<<total_good<<"\n"<<total_miss<<"\n";
   result<<max_combo<<"\n"<<accuracy<<"\n"<<score;
   result.close();
}
