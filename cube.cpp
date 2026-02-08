#include "cube.h"
#include "ui_cube.h"
#include<QPainter>
#include<QTimer>
#include<QKeyEvent>
#include<QSoundEffect>


cube::cube(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::cube),blsrun(true),blsrecreat(false),blsover(false)
{
    ui->setupUi(this);
    this->setGeometry(QRect(600,300,400,370));
    ui->pushButton->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_3->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_4->setFocusPolicy(Qt::NoFocus);
    ui->checkBox->setFocusPolicy(Qt::NoFocus);
    ui->checkBox_2->setFocusPolicy(Qt::NoFocus);
    ui->pushButton->setShortcut(Qt::Key_H);
    ui->pushButton_2->setShortcut(Qt::Key_B);
    ui->pushButton_3->setShortcut(Qt::Key_P);
    ui->pushButton_4->setShortcut(Qt::Key_O);
}

cube::~cube()
{
    delete ui;
}
void cube::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    if(!blsrun){
        initbottom();
        initcube();
        cinnextcube();


    }

    if(blsfixedmovingcube){
        cinfixedcube();

    }
    if(blsrecreat){
        initcube();
        cinnextcube();
    }

    if(blsfixedmovingcube)
        blsfixedmovingcube=false;
    blsfix();
    deletefullline();
    blsgg();

    //显示分数
    QFont font("Courier",15);
    painter.setFont(font);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawText(180,50,"分数：");
    if(blsclickstart){
    painter.drawText(280,70,QString::number(score));
    }else{
        painter.drawText(280,70,QString::number(0));
    }
    //显示消除行数
    painter.drawText(180,110,"消除行：");
    if(blsclickstart){
    painter.drawText(280,130,QString::number(deletelinenum));
    }else{
        painter.drawText(280,130,QString::number(0));
    }
    //显示下一个
    painter.drawText(180,170,"下一个：");
    painter.setPen(Qt::black);
    switch(nextnum){
    case 0:
        painter.setBrush(Qt::cyan);
        break;
    case 1:
        painter.setBrush(Qt::blue);
        break;
    case 2:
        painter.setBrush(QColor(255,160,90));
        break;
    case 3:
        painter.setBrush(Qt::yellow);
        break;
    case 4:
        painter.setBrush(Qt::green);
        break;
    case 5:
        painter.setBrush(Qt::red);
        break;
    case 6:
        painter.setBrush(QColor(126,22,113));
        break;
    }
    if(blsclickstart)
    painter.drawRects(nextcube);

    //画游戏背景
    painter.setPen(Qt::black);//设置画笔颜色
    painter.setBrush((Qt::gray));//设置填充颜色
    painter.drawRect(20,20,150,300);
    //画格子
    /*painter.setPen(Qt::blue);
    for(int i=1;i<10;i++){
        painter.drawLine(20+i*15,20,20+i*15,320);//画竖线
    }
    for(int i=1;i<20;i++){
         painter.drawLine(20,20+i*15,170,20+i*15);//画横线
    }*/

    //画移动方块
    if(blsclickstart){
        if(firstup){
            for(int i=0;i<4;i++){
                movingcube[i].setTop(movingcube[i].top()-15);
                movingcube[i].setBottom(movingcube[i].bottom()-15);
            }
            firstup=false;
        }
    painter.setPen(Qt::black);
    switch(colornum){
    case 0:
        painter.setBrush(Qt::cyan);
        break;
    case 1:
        painter.setBrush(Qt::blue);
        break;
    case 2:
        painter.setBrush(QColor(255,160,90));
        break;
    case 3:
        painter.setBrush(Qt::yellow);
        break;
    case 4:
        painter.setBrush(Qt::green);
        break;
    case 5:
        painter.setBrush(Qt::red);
        break;
    case 6:
        painter.setBrush(QColor(126,22,113));
        break;
    }
    if(!blsmovingcubefixed)
    painter.drawRects(&movingcube[0],movingcube.size());
}
    //画固定方块
    for(int i=50;i<fixedcube.size();i++){
        bool sfcz=true;
        for(int k=0;k<deleterectnum;k++){
            if(i==deletefixedrect[k]){
                sfcz=false;
            }
        }
        if(sfcz){
        switch(fixedcolor[i]){
        case 0:
            painter.setBrush(Qt::cyan);
            break;
        case 1:
            painter.setBrush(Qt::blue);
            break;
        case 2:
            painter.setBrush(QColor(255,160,90));
            break;
        case 3:
            painter.setBrush(Qt::yellow);
            break;
        case 4:
            painter.setBrush(Qt::green);
            break;
        case 5:
            painter.setBrush(Qt::red);
            break;
        case 6:
            painter.setBrush(QColor(126,22,113));
            break;
        }
        painter.drawRect(fixedcube[i]);
        }
    }
    if(blsmovingcubefixed){
        blsmovingcubefixed=false;
    }




    //游戏结束
    if(blsover){
        painter.drawPixmap(30,110,130,100,QPixmap(":/picture/picture/game.png"));
        painter.drawPixmap(30,110,130,100,QPixmap(":/picture/picture/over.png"));
        if(blsplaymusic)
        playggmusic();
        timer->stop();
    }
}
int cube::number(){
    int n;
    n=qrand()%7;
    return n;
}

void cube::playbgm(){


    bgm=new QSoundEffect;
    bgm->setSource(QUrl::fromLocalFile(":/picture/music/bgm.WAV~1.wav"));
    bgm->setLoopCount(100);  //循环次数
    bgm->setVolume(0.13f); //音量  0~1之间
    bgm->play();
}

void cube::playmovingmusic(){
    movingmusic=new QSoundEffect;
    movingmusic->setSource(QUrl::fromLocalFile(":/picture/music/move.wav"));
    movingmusic->setLoopCount(0);  //循环次数
    movingmusic->setVolume(0.25f); //音量  0~1之间
    movingmusic->play();
}

void cube::playfixedmusic(){
    fixedmusic=new QSoundEffect;
    fixedmusic->setSource(QUrl::fromLocalFile(":/picture/music/fix.wav"));
    fixedmusic->setLoopCount(0);  //循环次数
    fixedmusic->setVolume(0.35f); //音量  0~1之间
    fixedmusic->play();
}

void cube::playggmusic(){
    ggmusic=new QSoundEffect;
    ggmusic->setSource(QUrl::fromLocalFile(":/picture/music/over.wav"));
    ggmusic->setLoopCount(0);  //循环次数
    ggmusic->setVolume(0.30f); //音量  0~1之间
    ggmusic->play();
}

QVector<QRect> cube::icube(){
    QVector<QRect> cubi;
    cubi.resize(4);
    inum=1;//默认开始为横着
    for(int i=0;i<4;i++){
        QRect rect(65+15*i,20,15,15);
        cubi[i]=rect;
    }
    return cubi;
}
QVector<QRect> cube::lcube(){
    QVector<QRect> cubi;
    cubi.resize(4);
    lnum=1;//默认开始为下三上右一
    for(int i=0;i<3;i++){
        QRect rect(80+15*i,35,15,15);
        cubi[i]=rect;
    }
    QRect rect(110,20,15,15);
    cubi[3]=rect;
    return cubi;
}
QVector<QRect> cube::jcube(){
    QVector<QRect> cubi;
    cubi.resize(4);
    jnum=1;//默认开始为下三上左一
    for(int i=0;i<3;i++){
        QRect rect(80+15*i,35,15,15);
        cubi[i]=rect;
    }
    QRect rect(80,20,15,15);
    cubi[3]=rect;
    return cubi;
}
QVector<QRect> cube::ocube(){
    QVector<QRect> cubi;
    cubi.resize(4);
    for(int i=0;i<2;i++){
        QRect rect(80,20+15*i,15,15);
        cubi[i]=rect;
    }
    for(int i=2;i<4;i++){
        QRect rect(95,20+15*(i-2),15,15);
        cubi[i]=rect;
    }
    return cubi;
}
QVector<QRect> cube::scube(){
    QVector<QRect> cubi;
    cubi.resize(4);
    snum=1;
    for(int i=0;i<2;i++){
        QRect rect(95+15*i,20,15,15);
        cubi[i]=rect;
    }
    for(int i=2;i<4;i++){
        QRect rect(80+15*(i-2),35,15,15);
        cubi[i]=rect;
    }
    return cubi;
}
QVector<QRect> cube::zcube(){
    QVector<QRect> cubi;
    cubi.resize(4);
    znum=1;
    for(int i=0;i<2;i++){
        QRect rect(80+15*i,20,15,15);
        cubi[i]=rect;
    }
    for(int i=2;i<4;i++){
        QRect rect(95+15*(i-2),35,15,15);
        cubi[i]=rect;
    }
    return cubi;
}
QVector<QRect> cube::tcube(){
    QVector<QRect> cubi;
    cubi.resize(4);
    tnum=1;//默认开始为山形
    for(int i=0;i<3;i++){
        QRect rect(80+15*i,35,15,15);
        cubi[i]=rect;
    }
    QRect rect(95,20,15,15);
    cubi[3]=rect;
    return cubi;
}

void cube::initcube(){
    blsrecreat=false;
    colornum=nextnum;
    movingcube.resize(4);//方块格数
    switch(colornum){
    case 0:
        movingcube=icube();
        break;
    case 1:
        movingcube=lcube();
        break;
    case 2:
        movingcube=jcube();
        break;
    case 3:
        movingcube=ocube();
        break;
    case 4:
        movingcube=scube();
        break;
    case 5:
        movingcube=zcube();
        break;
    case 6:
        movingcube=tcube();
        break;
    }
    if(blsfixedmovingcube){
        for(int i=0;i<4;i++){
                    movingcube[i].setTop(movingcube[i].top()-15);
                    movingcube[i].setBottom(movingcube[i].bottom()-15);
                }
    }
    nextnum=number();

}

void cube::cinnextcube(){
    nextcube.resize(4);
    switch(nextnum){
    case 0:
        nextcube=icube();
        break;
    case 1:
        nextcube=lcube();
        break;
    case 2:
        nextcube=jcube();
        break;
    case 3:
        nextcube=ocube();
        break;
    case 4:
        nextcube=scube();
        break;
    case 5:
        nextcube=zcube();
        break;
    case 6:
        nextcube=tcube();
        break;
    }
    for(int i=0;i<4;i++){
        nextcube[i].setBottom(nextcube[i].bottom()+165);
        nextcube[i].setTop(nextcube[i].top()+165);
        nextcube[i].setLeft(nextcube[i].left()+195);
        nextcube[i].setRight(nextcube[i].right()+195);
    }
}

void cube::initbottom(){
    blsrun=true;
    score=0;
    deletelinenum=0;
    nextnum=number();
    fixedcube.resize(50);
    for(int i=0;i<10;i++){
        QRect rect(20+15*i,320,15,15);
        fixedcube[i]=rect;
    }
    for(int i=10;i<30;i++){
        QRect rect(5,20+15*(i-10),15,15);
        fixedcube[i]=rect;
    }
    for(int i=30;i<50;i++){
        QRect rect(170,20+15*(i-30),15,15);
        fixedcube[i]=rect;
    }
    timer=new QTimer(this);//设计时器
    timer->start(500);//设定时间500ms
    connect(timer,SIGNAL(timeout()),SLOT(cube_update()));
}

void cube::blsfix(){
    for(int i=0;i<4;i++){
        for(int j=0;j<fixedcube.size();j++){
            bool sfcz=true;
            for(int k=0;k<deleterectnum;k++){
                if(j==deletefixedrect[k]){
                    sfcz=false;
                }
            }
            if(sfcz){
                if(movingcube[i].bottom()==fixedcube[j].bottom()-15&&movingcube[i].left()==fixedcube[j].left()){
                    blsfixedmovingcube=true;
                    return;
            }

            }
        }
    }
}

void cube::cinfixedcube(){
    blsrecreat=true;
    blsmovingcubefixed=true;
    if(blsplaymusic)
    playfixedmusic();
    fixedcube.resize(fixedcube.size()+4);
    for(int k=fixedcube.size()-1,l=0;k>fixedcube.size()-5;k--,l++){
        fixedcube[k]=movingcube[l];
        fixedcolor[k]=colornum;
    }
    score+=10;
}

void cube::deletefullline(){
    int q[20][10]={0};//所有的格子
    int p[20]={0};//已满行所在顺序
    int n=0;//已满行数
    for(int i=50;i<fixedcube.size();i++){
        bool sfcz=true;
        for(int k=0;k<deleterectnum;k++){
            if(i==deletefixedrect[k]){
                sfcz=false;
            }
        }
        if(sfcz){
            q[(fixedcube[i].bottom()-20)/15][(fixedcube[i].right()-20)/15]=1;
        }

    }
    for(int i=0;i<20;i++){
        bool x=true;
        for(int j=0;j<10;j++){
            if(q[i][j]==0){
                x=false;
            }
        }
        if(x){
            p[n]=i;
            n++;
        }
    }
    for(int i=0;i<n;i++){
        for(int j=50;j<fixedcube.size();j++){
            bool sfcz=true;
            for(int k=0;k<deleterectnum;k++){
                if(j==deletefixedrect[k]){
                    sfcz=false;
                }
            }
            if(sfcz){
                if(fixedcube[j].top()==20+15*p[i]){
                    deletefixedrect[deleterectnum]=j;
                    deleterectnum++;
                }else{
                if(fixedcube[j].top()<20+15*p[i]){
                    fixedcube[j].setTop(fixedcube[j].top()+15);
                    fixedcube[j].setBottom(fixedcube[j].bottom()+15);
                }
                }
            }
        }
    }
    switch(n){
    case 1:
        score+=100;
        break;
    case 2:
        score+=200;
        break;
    case 3:
        score+=400;
        break;
    case 4:
        score+=800;
        break;
    default:;
    }
    deletelinenum+=n;
}

void cube::blsgg(){
    for(int i=50;i<fixedcube.size();i++){
        bool sfcz=true;
        for(int k=0;k<deleterectnum;k++){
            if(i==deletefixedrect[k]){
                sfcz=false;
            }
        }
        if(sfcz){
            if(fixedcube[i].top()<=20){
                blsover=true;
                return;
            }
        }
    }
}

void cube::cube_update(){
    if(!blsfixedmovingcube){
    for(int i=0;i<4;i++){
        movingcube[i].setTop(movingcube[i].top()+15);
        movingcube[i].setBottom(movingcube[i].bottom()+15);
    }
    }
    update();//更新
}

void cube::keyPressEvent(QKeyEvent *event){
    QKeyEvent *key=(QKeyEvent*) event;
    bool q=false;
    switch(key->key()){
    case Qt::Key_Up:
        switch(colornum){
        case 0:
            if(inum==1){
                movingcube[0].setBottom(movingcube[0].bottom()-15);
                movingcube[0].setTop(movingcube[0].top()-15);
                movingcube[0].setLeft(movingcube[0].left()+15);
                movingcube[0].setRight(movingcube[0].right()+15);
                movingcube[2].setBottom(movingcube[2].bottom()+15);
                movingcube[2].setTop(movingcube[2].top()+15);
                movingcube[2].setLeft(movingcube[2].left()-15);
                movingcube[2].setRight(movingcube[2].right()-15);
                movingcube[3].setBottom(movingcube[3].bottom()+30);
                movingcube[3].setTop(movingcube[3].top()+30);
                movingcube[3].setLeft(movingcube[3].left()-30);
                movingcube[3].setRight(movingcube[3].right()-30);
                for(int i=0;i<4;i++){
                    for(int j=0;j<fixedcube.size();j++){
                        bool sfcz=true;
                        for(int k=0;k<deleterectnum;k++){
                            if(j==deletefixedrect[k]){
                                sfcz=false;
                            }
                        }
                        if(sfcz){
                            if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                               q=true;
                            }
                        }

                    }
                }
                if(q){
                    movingcube[0].setBottom(movingcube[0].bottom()+15);
                    movingcube[0].setTop(movingcube[0].top()+15);
                    movingcube[0].setLeft(movingcube[0].left()-15);
                    movingcube[0].setRight(movingcube[0].right()-15);
                    movingcube[2].setBottom(movingcube[2].bottom()-15);
                    movingcube[2].setTop(movingcube[2].top()-15);
                    movingcube[2].setLeft(movingcube[2].left()+15);
                    movingcube[2].setRight(movingcube[2].right()+15);
                    movingcube[3].setBottom(movingcube[3].bottom()-30);
                    movingcube[3].setTop(movingcube[3].top()-30);
                    movingcube[3].setLeft(movingcube[3].left()+30);
                    movingcube[3].setRight(movingcube[3].right()+30);
                }
                if(!q)
                inum=2;
            }else{
                movingcube[0].setBottom(movingcube[0].bottom()+15);
                movingcube[0].setTop(movingcube[0].top()+15);
                movingcube[0].setLeft(movingcube[0].left()-15);
                movingcube[0].setRight(movingcube[0].right()-15);
                movingcube[2].setBottom(movingcube[2].bottom()-15);
                movingcube[2].setTop(movingcube[2].top()-15);
                movingcube[2].setLeft(movingcube[2].left()+15);
                movingcube[2].setRight(movingcube[2].right()+15);
                movingcube[3].setBottom(movingcube[3].bottom()-30);
                movingcube[3].setTop(movingcube[3].top()-30);
                movingcube[3].setLeft(movingcube[3].left()+30);
                movingcube[3].setRight(movingcube[3].right()+30);
                for(int i=0;i<4;i++){
                    for(int j=0;j<fixedcube.size();j++){
                        bool sfcz=true;
                        for(int k=0;k<deleterectnum;k++){
                            if(j==deletefixedrect[k]){
                                sfcz=false;
                            }
                        }
                        if(sfcz){
                            if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                               q=true;
                            }
                        }

                    }
                }
                if(q){
                    movingcube[0].setBottom(movingcube[0].bottom()-15);
                    movingcube[0].setTop(movingcube[0].top()-15);
                    movingcube[0].setLeft(movingcube[0].left()+15);
                    movingcube[0].setRight(movingcube[0].right()+15);
                    movingcube[2].setBottom(movingcube[2].bottom()+15);
                    movingcube[2].setTop(movingcube[2].top()+15);
                    movingcube[2].setLeft(movingcube[2].left()-15);
                    movingcube[2].setRight(movingcube[2].right()-15);
                    movingcube[3].setBottom(movingcube[3].bottom()+30);
                    movingcube[3].setTop(movingcube[3].top()+30);
                    movingcube[3].setLeft(movingcube[3].left()-30);
                    movingcube[3].setRight(movingcube[3].right()-30);
                }
                if(!q)
                inum=1;
            }
            break;
        case 1:
            switch(lnum){
            case 1:
                movingcube[0].setBottom(movingcube[0].bottom()-15);
                movingcube[0].setTop(movingcube[0].top()-15);
                movingcube[0].setLeft(movingcube[0].left()+15);
                movingcube[0].setRight(movingcube[0].right()+15);
                movingcube[2].setBottom(movingcube[2].bottom()+15);
                movingcube[2].setTop(movingcube[2].top()+15);
                movingcube[2].setLeft(movingcube[2].left()-15);
                movingcube[2].setRight(movingcube[2].right()-15);
                movingcube[3].setBottom(movingcube[3].bottom()+30);
                movingcube[3].setTop(movingcube[3].top()+30);
                for(int i=0;i<4;i++){
                    for(int j=0;j<fixedcube.size();j++){
                        bool sfcz=true;
                        for(int k=0;k<deleterectnum;k++){
                            if(j==deletefixedrect[k]){
                                sfcz=false;
                            }
                        }
                        if(sfcz){
                            if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                               q=true;
                            }
                        }

                    }
                }
                if(q){
                    movingcube[0].setBottom(movingcube[0].bottom()+15);
                    movingcube[0].setTop(movingcube[0].top()+15);
                    movingcube[0].setLeft(movingcube[0].left()-15);
                    movingcube[0].setRight(movingcube[0].right()-15);
                    movingcube[2].setBottom(movingcube[2].bottom()-15);
                    movingcube[2].setTop(movingcube[2].top()-15);
                    movingcube[2].setLeft(movingcube[2].left()+15);
                    movingcube[2].setRight(movingcube[2].right()+15);
                    movingcube[3].setBottom(movingcube[3].bottom()-30);
                    movingcube[3].setTop(movingcube[3].top()-30);
                }
                if(!q)
                lnum=2;
                break;
            case 2:
                movingcube[0].setBottom(movingcube[0].bottom()+15);
                movingcube[0].setTop(movingcube[0].top()+15);
                movingcube[0].setLeft(movingcube[0].left()+15);
                movingcube[0].setRight(movingcube[0].right()+15);
                movingcube[2].setBottom(movingcube[2].bottom()-15);
                movingcube[2].setTop(movingcube[2].top()-15);
                movingcube[2].setLeft(movingcube[2].left()-15);
                movingcube[2].setRight(movingcube[2].right()-15);
                movingcube[3].setLeft(movingcube[3].left()-30);
                movingcube[3].setRight(movingcube[3].right()-30);
                for(int i=0;i<4;i++){
                    for(int j=0;j<fixedcube.size();j++){
                        bool sfcz=true;
                        for(int k=0;k<deleterectnum;k++){
                            if(j==deletefixedrect[k]){
                                sfcz=false;
                            }
                        }
                        if(sfcz){
                            if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                               q=true;
                            }
                        }

                    }
                }
                if(q){
                    movingcube[0].setBottom(movingcube[0].bottom()-15);
                    movingcube[0].setTop(movingcube[0].top()-15);
                    movingcube[0].setLeft(movingcube[0].left()-15);
                    movingcube[0].setRight(movingcube[0].right()-15);
                    movingcube[2].setBottom(movingcube[2].bottom()+15);
                    movingcube[2].setTop(movingcube[2].top()+15);
                    movingcube[2].setLeft(movingcube[2].left()+15);
                    movingcube[2].setRight(movingcube[2].right()+15);
                    movingcube[3].setLeft(movingcube[3].left()+30);
                    movingcube[3].setRight(movingcube[3].right()+30);
                }
                if(!q)
                lnum=3;
                break;
            case 3:
                movingcube[0].setBottom(movingcube[0].bottom()+15);
                movingcube[0].setTop(movingcube[0].top()+15);
                movingcube[0].setLeft(movingcube[0].left()-15);
                movingcube[0].setRight(movingcube[0].right()-15);
                movingcube[2].setBottom(movingcube[2].bottom()-15);
                movingcube[2].setTop(movingcube[2].top()-15);
                movingcube[2].setLeft(movingcube[2].left()+15);
                movingcube[2].setRight(movingcube[2].right()+15);
                movingcube[3].setBottom(movingcube[3].bottom()-30);
                movingcube[3].setTop(movingcube[3].top()-30);
                for(int i=0;i<4;i++){
                    for(int j=0;j<fixedcube.size();j++){
                        bool sfcz=true;
                        for(int k=0;k<deleterectnum;k++){
                            if(j==deletefixedrect[k]){
                                sfcz=false;
                            }
                        }
                        if(sfcz){
                            if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                               q=true;
                            }
                        }

                    }
                }
                if(q){
                    movingcube[0].setBottom(movingcube[0].bottom()-15);
                    movingcube[0].setTop(movingcube[0].top()-15);
                    movingcube[0].setLeft(movingcube[0].left()+15);
                    movingcube[0].setRight(movingcube[0].right()+15);
                    movingcube[2].setBottom(movingcube[2].bottom()+15);
                    movingcube[2].setTop(movingcube[2].top()+15);
                    movingcube[2].setLeft(movingcube[2].left()-15);
                    movingcube[2].setRight(movingcube[2].right()-15);
                    movingcube[3].setBottom(movingcube[3].bottom()+30);
                    movingcube[3].setTop(movingcube[3].top()+30);
                }
                if(!q)
                lnum=4;
                break;
            case 4:
                movingcube[0].setBottom(movingcube[0].bottom()-15);
                movingcube[0].setTop(movingcube[0].top()-15);
                movingcube[0].setLeft(movingcube[0].left()-15);
                movingcube[0].setRight(movingcube[0].right()-15);
                movingcube[2].setBottom(movingcube[2].bottom()+15);
                movingcube[2].setTop(movingcube[2].top()+15);
                movingcube[2].setLeft(movingcube[2].left()+15);
                movingcube[2].setRight(movingcube[2].right()+15);
                movingcube[3].setLeft(movingcube[3].left()+30);
                movingcube[3].setRight(movingcube[3].right()+30);
                for(int i=0;i<4;i++){
                    for(int j=0;j<fixedcube.size();j++){
                        bool sfcz=true;
                        for(int k=0;k<deleterectnum;k++){
                            if(j==deletefixedrect[k]){
                                sfcz=false;
                            }
                        }
                        if(sfcz){
                            if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                               q=true;
                            }
                        }

                    }
                }
                if(q){
                    movingcube[0].setBottom(movingcube[0].bottom()+15);
                    movingcube[0].setTop(movingcube[0].top()+15);
                    movingcube[0].setLeft(movingcube[0].left()+15);
                    movingcube[0].setRight(movingcube[0].right()+15);
                    movingcube[2].setBottom(movingcube[2].bottom()-15);
                    movingcube[2].setTop(movingcube[2].top()-15);
                    movingcube[2].setLeft(movingcube[2].left()-15);
                    movingcube[2].setRight(movingcube[2].right()-15);
                    movingcube[3].setLeft(movingcube[3].left()-30);
                    movingcube[3].setRight(movingcube[3].right()-30);
                }
                if(!q)
                lnum=1;
                break;
            }
            break;
        case 2:
            switch(jnum){
            case 1:
                movingcube[0].setBottom(movingcube[0].bottom()-15);
                movingcube[0].setTop(movingcube[0].top()-15);
                movingcube[0].setLeft(movingcube[0].left()+15);
                movingcube[0].setRight(movingcube[0].right()+15);
                movingcube[2].setBottom(movingcube[2].bottom()+15);
                movingcube[2].setTop(movingcube[2].top()+15);
                movingcube[2].setLeft(movingcube[2].left()-15);
                movingcube[2].setRight(movingcube[2].right()-15);
                movingcube[3].setLeft(movingcube[3].left()+30);
                movingcube[3].setRight(movingcube[3].right()+30);
                for(int i=0;i<4;i++){
                    for(int j=0;j<fixedcube.size();j++){
                        bool sfcz=true;
                        for(int k=0;k<deleterectnum;k++){
                            if(j==deletefixedrect[k]){
                                sfcz=false;
                            }
                        }
                        if(sfcz){
                            if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                               q=true;
                            }
                        }

                    }
                }
                if(q){
                    movingcube[0].setBottom(movingcube[0].bottom()+15);
                    movingcube[0].setTop(movingcube[0].top()+15);
                    movingcube[0].setLeft(movingcube[0].left()-15);
                    movingcube[0].setRight(movingcube[0].right()-15);
                    movingcube[2].setBottom(movingcube[2].bottom()-15);
                    movingcube[2].setTop(movingcube[2].top()-15);
                    movingcube[2].setLeft(movingcube[2].left()+15);
                    movingcube[2].setRight(movingcube[2].right()+15);
                    movingcube[3].setLeft(movingcube[3].left()-30);
                    movingcube[3].setRight(movingcube[3].right()-30);
                }
                if(!q)
                jnum=2;
                break;
            case 2:
                movingcube[0].setBottom(movingcube[0].bottom()+15);
                movingcube[0].setTop(movingcube[0].top()+15);
                movingcube[0].setLeft(movingcube[0].left()+15);
                movingcube[0].setRight(movingcube[0].right()+15);
                movingcube[2].setBottom(movingcube[2].bottom()-15);
                movingcube[2].setTop(movingcube[2].top()-15);
                movingcube[2].setLeft(movingcube[2].left()-15);
                movingcube[2].setRight(movingcube[2].right()-15);
                movingcube[3].setBottom(movingcube[3].bottom()+30);
                movingcube[3].setTop(movingcube[3].top()+30);
                for(int i=0;i<4;i++){
                    for(int j=0;j<fixedcube.size();j++){
                        bool sfcz=true;
                        for(int k=0;k<deleterectnum;k++){
                            if(j==deletefixedrect[k]){
                                sfcz=false;
                            }
                        }
                        if(sfcz){
                            if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                               q=true;
                            }
                        }

                    }
                }
                if(q){
                    movingcube[0].setBottom(movingcube[0].bottom()-15);
                    movingcube[0].setTop(movingcube[0].top()-15);
                    movingcube[0].setLeft(movingcube[0].left()-15);
                    movingcube[0].setRight(movingcube[0].right()-15);
                    movingcube[2].setBottom(movingcube[2].bottom()+15);
                    movingcube[2].setTop(movingcube[2].top()+15);
                    movingcube[2].setLeft(movingcube[2].left()+15);
                    movingcube[2].setRight(movingcube[2].right()+15);
                    movingcube[3].setBottom(movingcube[3].bottom()-30);
                    movingcube[3].setTop(movingcube[3].top()-30);
                }
                if(!q)
                jnum=3;
                break;
            case 3:
                movingcube[0].setBottom(movingcube[0].bottom()+15);
                movingcube[0].setTop(movingcube[0].top()+15);
                movingcube[0].setLeft(movingcube[0].left()-15);
                movingcube[0].setRight(movingcube[0].right()-15);
                movingcube[2].setBottom(movingcube[2].bottom()-15);
                movingcube[2].setTop(movingcube[2].top()-15);
                movingcube[2].setLeft(movingcube[2].left()+15);
                movingcube[2].setRight(movingcube[2].right()+15);
                movingcube[3].setLeft(movingcube[3].left()-30);
                movingcube[3].setRight(movingcube[3].right()-30);
                for(int i=0;i<4;i++){
                    for(int j=0;j<fixedcube.size();j++){
                        bool sfcz=true;
                        for(int k=0;k<deleterectnum;k++){
                            if(j==deletefixedrect[k]){
                                sfcz=false;
                            }
                        }
                        if(sfcz){
                            if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                               q=true;
                            }
                        }

                    }
                }
                if(q){
                    movingcube[0].setBottom(movingcube[0].bottom()-15);
                    movingcube[0].setTop(movingcube[0].top()-15);
                    movingcube[0].setLeft(movingcube[0].left()+15);
                    movingcube[0].setRight(movingcube[0].right()+15);
                    movingcube[2].setBottom(movingcube[2].bottom()+15);
                    movingcube[2].setTop(movingcube[2].top()+15);
                    movingcube[2].setLeft(movingcube[2].left()-15);
                    movingcube[2].setRight(movingcube[2].right()-15);
                    movingcube[3].setLeft(movingcube[3].left()+30);
                    movingcube[3].setRight(movingcube[3].right()+30);
                }
                if(!q)
                jnum=4;
                break;
            case 4:
                movingcube[0].setBottom(movingcube[0].bottom()-15);
                movingcube[0].setTop(movingcube[0].top()-15);
                movingcube[0].setLeft(movingcube[0].left()-15);
                movingcube[0].setRight(movingcube[0].right()-15);
                movingcube[2].setBottom(movingcube[2].bottom()+15);
                movingcube[2].setTop(movingcube[2].top()+15);
                movingcube[2].setLeft(movingcube[2].left()+15);
                movingcube[2].setRight(movingcube[2].right()+15);
                movingcube[3].setBottom(movingcube[3].bottom()-30);
                movingcube[3].setTop(movingcube[3].top()-30);
                for(int i=0;i<4;i++){
                    for(int j=0;j<fixedcube.size();j++){
                        bool sfcz=true;
                        for(int k=0;k<deleterectnum;k++){
                            if(j==deletefixedrect[k]){
                                sfcz=false;
                            }
                        }
                        if(sfcz){
                            if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                               q=true;
                            }
                        }

                    }
                }
                if(q){
                    movingcube[0].setBottom(movingcube[0].bottom()+15);
                    movingcube[0].setTop(movingcube[0].top()+15);
                    movingcube[0].setLeft(movingcube[0].left()+15);
                    movingcube[0].setRight(movingcube[0].right()+15);
                    movingcube[2].setBottom(movingcube[2].bottom()-15);
                    movingcube[2].setTop(movingcube[2].top()-15);
                    movingcube[2].setLeft(movingcube[2].left()-15);
                    movingcube[2].setRight(movingcube[2].right()-15);
                    movingcube[3].setBottom(movingcube[3].bottom()+30);
                    movingcube[3].setTop(movingcube[3].top()+30);
                }
                if(!q)
                jnum=1;
                break;
            }
            break;
        case 4:
            switch(snum){
            case 1:
                movingcube[0].setBottom(movingcube[0].bottom()+30);
                movingcube[0].setTop(movingcube[0].top()+30);
                movingcube[1].setLeft(movingcube[1].left()-30);
                movingcube[1].setRight(movingcube[1].right()-30);
                for(int i=0;i<4;i++){
                    for(int j=0;j<fixedcube.size();j++){
                        bool sfcz=true;
                        for(int k=0;k<deleterectnum;k++){
                            if(j==deletefixedrect[k]){
                                sfcz=false;
                            }
                        }
                        if(sfcz){
                            if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                               q=true;
                            }
                        }

                    }
                }
                if(q){
                    movingcube[0].setBottom(movingcube[0].bottom()-30);
                    movingcube[0].setTop(movingcube[0].top()-30);
                    movingcube[1].setLeft(movingcube[1].left()+30);
                    movingcube[1].setRight(movingcube[1].right()+30);
                }
                if(!q)
                snum=2;
                break;
            case 2:
                movingcube[0].setBottom(movingcube[0].bottom()-30);
                movingcube[0].setTop(movingcube[0].top()-30);
                movingcube[1].setLeft(movingcube[1].left()+30);
                movingcube[1].setRight(movingcube[1].right()+30);
                for(int i=0;i<4;i++){
                    for(int j=0;j<fixedcube.size();j++){
                        bool sfcz=true;
                        for(int k=0;k<deleterectnum;k++){
                            if(j==deletefixedrect[k]){
                                sfcz=false;
                            }
                        }
                        if(sfcz){
                            if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                               q=true;
                            }
                        }

                    }
                }
                if(q){
                    movingcube[0].setBottom(movingcube[0].bottom()+30);
                    movingcube[0].setTop(movingcube[0].top()+30);
                    movingcube[1].setLeft(movingcube[1].left()-30);
                    movingcube[1].setRight(movingcube[1].right()-30);
                }
                if(!q)
                snum=1;
                break;
            }
            break;
        case 5:
            switch(znum){
            case 1:
                movingcube[0].setBottom(movingcube[0].bottom()+30);
                movingcube[0].setTop(movingcube[0].top()+30);
                movingcube[3].setLeft(movingcube[3].left()-30);
                movingcube[3].setRight(movingcube[3].right()-30);
                for(int i=0;i<4;i++){
                    for(int j=0;j<fixedcube.size();j++){
                        bool sfcz=true;
                        for(int k=0;k<deleterectnum;k++){
                            if(j==deletefixedrect[k]){
                                sfcz=false;
                            }
                        }
                        if(sfcz){
                            if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                               q=true;
                            }
                        }

                    }
                }
                if(q){
                    movingcube[0].setBottom(movingcube[0].bottom()-30);
                    movingcube[0].setTop(movingcube[0].top()-30);
                    movingcube[3].setLeft(movingcube[3].left()+30);
                    movingcube[3].setRight(movingcube[3].right()+30);
                }
                if(!q)
                znum=2;
                break;
            case 2:
                movingcube[0].setBottom(movingcube[0].bottom()-30);
                movingcube[0].setTop(movingcube[0].top()-30);
                movingcube[3].setLeft(movingcube[3].left()+30);
                movingcube[3].setRight(movingcube[3].right()+30);
                for(int i=0;i<4;i++){
                    for(int j=0;j<fixedcube.size();j++){
                        bool sfcz=true;
                        for(int k=0;k<deleterectnum;k++){
                            if(j==deletefixedrect[k]){
                                sfcz=false;
                            }
                        }
                        if(sfcz){
                            if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                               q=true;
                            }
                        }

                    }
                }
                if(q){
                    movingcube[0].setBottom(movingcube[0].bottom()+30);
                    movingcube[0].setTop(movingcube[0].top()+30);
                    movingcube[3].setLeft(movingcube[3].left()-30);
                    movingcube[3].setRight(movingcube[3].right()-30);
                }
                if(!q)
                znum=1;
                break;
            }
            break;
        case 6:
            switch(tnum){
            case 1:
                movingcube[0].setBottom(movingcube[0].bottom()+15);
                movingcube[0].setTop(movingcube[0].top()+15);
                movingcube[0].setLeft(movingcube[0].left()+15);
                movingcube[0].setRight(movingcube[0].right()+15);
                for(int i=0;i<4;i++){
                    for(int j=0;j<fixedcube.size();j++){
                        bool sfcz=true;
                        for(int k=0;k<deleterectnum;k++){
                            if(j==deletefixedrect[k]){
                                sfcz=false;
                            }
                        }
                        if(sfcz){
                            if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                               q=true;
                            }
                        }

                    }
                }
                if(q){
                    movingcube[0].setBottom(movingcube[0].bottom()-15);
                    movingcube[0].setTop(movingcube[0].top()-15);
                    movingcube[0].setLeft(movingcube[0].left()-15);
                    movingcube[0].setRight(movingcube[0].right()-15);
                }
                if(!q)
                tnum=2;
                break;
            case 2:
                movingcube[3].setBottom(movingcube[3].bottom()+15);
                movingcube[3].setTop(movingcube[3].top()+15);
                movingcube[3].setLeft(movingcube[3].left()-15);
                movingcube[3].setRight(movingcube[3].right()-15);
                for(int i=0;i<4;i++){
                    for(int j=0;j<fixedcube.size();j++){
                        bool sfcz=true;
                        for(int k=0;k<deleterectnum;k++){
                            if(j==deletefixedrect[k]){
                                sfcz=false;
                            }
                        }
                        if(sfcz){
                            if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                               q=true;
                            }
                        }

                    }
                }
                if(q){
                    movingcube[3].setBottom(movingcube[3].bottom()-15);
                    movingcube[3].setTop(movingcube[3].top()-15);
                    movingcube[3].setLeft(movingcube[3].left()+15);
                    movingcube[3].setRight(movingcube[3].right()+15);
                }
                if(!q)
                tnum=3;
                break;
            case 3:
                movingcube[2].setBottom(movingcube[2].bottom()-15);
                movingcube[2].setTop(movingcube[2].top()-15);
                movingcube[2].setLeft(movingcube[2].left()-15);
                movingcube[2].setRight(movingcube[2].right()-15);
                for(int i=0;i<4;i++){
                    for(int j=0;j<fixedcube.size();j++){
                        bool sfcz=true;
                        for(int k=0;k<deleterectnum;k++){
                            if(j==deletefixedrect[k]){
                                sfcz=false;
                            }
                        }
                        if(sfcz){
                            if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                               q=true;
                            }
                        }

                    }
                }
                if(q){
                    movingcube[2].setBottom(movingcube[2].bottom()+15);
                    movingcube[2].setTop(movingcube[2].top()+15);
                    movingcube[2].setLeft(movingcube[2].left()+15);
                    movingcube[2].setRight(movingcube[2].right()+15);
                }
                if(!q)
                tnum=4;
                break;
            case 4:
                movingcube[0].setBottom(movingcube[0].bottom()-15);
                movingcube[0].setTop(movingcube[0].top()-15);
                movingcube[0].setLeft(movingcube[0].left()-15);
                movingcube[0].setRight(movingcube[0].right()-15);
                movingcube[2].setBottom(movingcube[2].bottom()+15);
                movingcube[2].setTop(movingcube[2].top()+15);
                movingcube[2].setLeft(movingcube[2].left()+15);
                movingcube[2].setRight(movingcube[2].right()+15);
                movingcube[3].setBottom(movingcube[3].bottom()-15);
                movingcube[3].setTop(movingcube[3].top()-15);
                movingcube[3].setLeft(movingcube[3].left()+15);
                movingcube[3].setRight(movingcube[3].right()+15);
                for(int i=0;i<4;i++){
                    for(int j=0;j<fixedcube.size();j++){
                        bool sfcz=true;
                        for(int k=0;k<deleterectnum;k++){
                            if(j==deletefixedrect[k]){
                                sfcz=false;
                            }
                        }
                        if(sfcz){
                            if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                               q=true;
                            }
                        }

                    }
                }
                if(q){
                    movingcube[0].setBottom(movingcube[0].bottom()+15);
                    movingcube[0].setTop(movingcube[0].top()+15);
                    movingcube[0].setLeft(movingcube[0].left()+15);
                    movingcube[0].setRight(movingcube[0].right()+15);
                    movingcube[2].setBottom(movingcube[2].bottom()-15);
                    movingcube[2].setTop(movingcube[2].top()-15);
                    movingcube[2].setLeft(movingcube[2].left()-15);
                    movingcube[2].setRight(movingcube[2].right()-15);
                    movingcube[3].setBottom(movingcube[3].bottom()+15);
                    movingcube[3].setTop(movingcube[3].top()+15);
                    movingcube[3].setLeft(movingcube[3].left()-15);
                    movingcube[3].setRight(movingcube[3].right()-15);
                }
                if(!q)
                tnum=1;
                break;
            }
            break;
        default:;
        }
        if(blsfixedmovingcube){
            bool u=false;
            for(int i=0;i<4;i++){
                for(int j=0;j<fixedcube.size();j++){
                    bool sfcz=true;
                    for(int k=0;k<deleterectnum;k++){
                        if(j==deletefixedrect[k]){
                            sfcz=false;
                        }
                    }
                    if(sfcz){
                        if(movingcube[i].bottom()==fixedcube[j].bottom()-15&&movingcube[i].left()==fixedcube[j].left()){
                           u=true;

                        }
                    }

                }
            }
            if(!u){
                blsrecreat=false;
                blsfixedmovingcube=false;
                blsmovingcubefixed=false;
            }
        }
        if(blsplaymusic)
        playmovingmusic();
        update();
        break;
    case Qt::Key_Down:
        for(int i=0;i<4;i++){
            movingcube[i].setTop(movingcube[i].top()+15);
            movingcube[i].setBottom(movingcube[i].bottom()+15);
        }
        for(int i=0;i<4;i++){
            for(int j=0;j<fixedcube.size();j++){
                bool sfcz=true;
                for(int k=0;k<deleterectnum;k++){
                    if(j==deletefixedrect[k]){
                        sfcz=false;
                    }
                }
                if(sfcz){
                    if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                       q=true;
                    }
                }

            }
        }
        if(q){
            for(int i=0;i<4;i++){
                movingcube[i].setTop(movingcube[i].top()-15);
                movingcube[i].setBottom(movingcube[i].bottom()-15);
            }

        }
        blsfix();
        if(blsplaymusic)
        playmovingmusic();
        update();
        break;
    case Qt::Key_Left:
        for(int i=0;i<4;i++){
            movingcube[i].setLeft(movingcube[i].left()-15);
            movingcube[i].setRight(movingcube[i].right()-15);
        }
        if(blsfixedmovingcube){
            bool u=false;
            for(int i=0;i<4;i++){
                for(int j=0;j<fixedcube.size();j++){
                    bool sfcz=true;
                    for(int k=0;k<deleterectnum;k++){
                        if(j==deletefixedrect[k]){
                            sfcz=false;
                        }
                    }
                    if(sfcz){
                        if(movingcube[i].bottom()==fixedcube[j].bottom()-15&&movingcube[i].left()==fixedcube[j].left()){
                           u=true;

                        }
                    }

                }
            }
            if(!u){
                blsrecreat=false;
                blsfixedmovingcube=false;
                blsmovingcubefixed=false;
            }
        }
        for(int i=0;i<4;i++){
            for(int j=0;j<fixedcube.size();j++){
                bool sfcz=true;
                for(int k=0;k<deleterectnum;k++){
                    if(j==deletefixedrect[k]){
                        sfcz=false;
                    }
                }
                if(sfcz){
                    if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                       q=true;
                    }
                }

            }
        }

        if(q){
            for(int i=0;i<4;i++){
                movingcube[i].setLeft(movingcube[i].left()+15);
                movingcube[i].setRight(movingcube[i].right()+15);
            }
        }
        if(blsplaymusic)
        playmovingmusic();
        update();
        break;
    case Qt::Key_Right:
        for(int i=0;i<4;i++){
            movingcube[i].setLeft(movingcube[i].left()+15);
            movingcube[i].setRight(movingcube[i].right()+15);
        }
        if(blsfixedmovingcube){
            bool u=false;
            for(int i=0;i<4;i++){
                for(int j=0;j<fixedcube.size();j++){
                    bool sfcz=true;
                    for(int k=0;k<deleterectnum;k++){
                        if(j==deletefixedrect[k]){
                            sfcz=false;
                        }
                    }
                    if(sfcz){
                        if(movingcube[i].bottom()==fixedcube[j].bottom()-15&&movingcube[i].left()==fixedcube[j].left()){

                            u=true;
                        }
                    }

                }
            }
            if(!u){
                blsrecreat=false;
                blsfixedmovingcube=false;
                blsmovingcubefixed=false;
            }
        }
        for(int i=0;i<4;i++){
            for(int j=0;j<fixedcube.size();j++){
                bool sfcz=true;
                for(int k=0;k<deleterectnum;k++){
                    if(j==deletefixedrect[k]){
                        sfcz=false;
                    }
                }
                if(sfcz){
                    if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                       q=true;
                    }
                }

            }
        }
        if(q){
            for(int i=0;i<4;i++){
                movingcube[i].setLeft(movingcube[i].left()-15);
                movingcube[i].setRight(movingcube[i].right()-15);
            }
        }
        if(blsplaymusic)
        playmovingmusic();
        update();
        break;
    case Qt::Key_Space:
        for(;;){
            for(int i=0;i<4;i++){
                movingcube[i].setTop(movingcube[i].top()+ 15);
                movingcube[i].setBottom(movingcube[i].bottom()+15);
            }
            for(int i=0;i<4;i++){
                for(int j=0;j<fixedcube.size();j++){
                    bool sfcz=true;
                    for(int k=0;k<deleterectnum;k++){
                        if(j==deletefixedrect[k]){
                            sfcz=false;
                        }
                    }
                    if(sfcz){
                        if(movingcube[i].bottom()==fixedcube[j].bottom()&&movingcube[i].left()==fixedcube[j].left()){
                           q=true;
                        }
                    }

                }
            }
            if(q){
                for(int i=0;i<4;i++){
                    movingcube[i].setTop(movingcube[i].top()-15);
                    movingcube[i].setBottom(movingcube[i].bottom()-15);
                }
                blsrecreat=true;
                score+=10;
                fixedcube.resize(fixedcube.size()+4);
                for(int k=fixedcube.size()-1,l=0;k>fixedcube.size()-5;k--,l++){
                    fixedcube[k]=movingcube[l];
                    fixedcolor[k]=colornum;
                }
                break;
            }
        }
        if(blsplaymusic)
        playfixedmusic();
        update();
        break;
    default:;
    }
}

//帮助按钮
void cube::on_pushButton_clicked()
{

    helper *startwindow=new helper;
    startwindow->show();

}




//开始游戏按钮
void cube::on_pushButton_2_clicked()
{
    if(!blsclickstart){
    blsrun=false;
    blsclickstart=true;
    blstimego=true;
    }
}
//暂停/继续按钮
void cube::on_pushButton_3_clicked()
{
    if(blstimego){
        timer->stop();
        blstimego=false;
    }else{
        timer->start(500);
        blstimego=true;
    }
}

void cube::on_pushButton_4_clicked()
{
    this->close();
}




void cube::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked()){
        playbgm();
    }else{
        bgm->stop();
    }
}

void cube::on_checkBox_2_clicked()
{
    if(ui->checkBox_2->isChecked()){
        blsplaymusic=true;
    }else{
        blsplaymusic=false;
    }
}
