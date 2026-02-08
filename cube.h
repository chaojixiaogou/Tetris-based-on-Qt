#ifndef CUBE_H
#define CUBE_H

#include <QMainWindow>
#include "helper.h"
#include<QSoundEffect>

namespace Ui {
class cube;
}

class cube : public QMainWindow
{
    Q_OBJECT

public:
    explicit cube(QWidget *parent = 0);
    ~cube();
    void paintEvent(QPaintEvent *event);
    void initcube();//定义移动方块
    int number();//随机数
    QVector<QRect> icube();//长条
    QVector<QRect> lcube();//L形
    QVector<QRect> jcube();//J形
    QVector<QRect> ocube();//田字
    QVector<QRect> scube();//S形
    QVector<QRect> zcube();//Z形
    QVector<QRect> tcube();//T形
    void keyPressEvent(QKeyEvent *event);
    void initbottom();//定义底部和两边
    void blsfix();//判断方块是否要固定
    void cinfixedcube();//将移动方块赋值给固定方块
    void deletefullline();//消行
    void blsgg();//判断是否游戏结束
    void cinnextcube();//输入下一个方块
    void playbgm();//播放音乐
    void playmovingmusic();
    void playfixedmusic();
    void playggmusic();

private slots:
    void cube_update();


    void on_pushButton_clicked();





    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();



    void on_checkBox_clicked();

    void on_checkBox_2_clicked();

private:
    Ui::cube *ui;
    QTimer *timer;//定时器
    QVector<QRect> movingcube;//正在控制的方块
    QVector<QRect> nextcube;//下一个方块
    int colornum;//配对颜色的随机数
    int nextnum;//下一个方块
    bool blsrun;//是否开始
    bool blsover;//是否结束
    bool blsrecreat;//是否重新生成方块
    int inum;//长条状态
    int lnum;//L形状态
    int jnum;//J形状态
    int snum;//S形状态
    int znum;//Z形状态
    int tnum;//T形状态
    QVector<QRect> fixedcube;//已固定的方块
    int fixedcolor[10000];
    int deletefixedrect[10000];//被消除的格子序号
    int deleterectnum=0;//被消除的格子数
    bool blsmovingcubefixed=false;//判断当前移动方块是否固定（用于不画移动方块）
    bool blsfixedmovingcube=false;//判断当前移动方块是否固定（用于创造延迟）
    int score;//得分
    int deletelinenum;//消除行数
    bool blsclickstart=false;//是否点击开始游戏
    bool firstup=true;//使第一个方块上移一格
    bool blstimego;//判断时间是否进行
    QSoundEffect *bgm;//bgm
    QSoundEffect *movingmusic;//移动音效
    QSoundEffect *fixedmusic;//固定音效
    QSoundEffect *ggmusic;//结束音效
    bool blsplaymusic=false;//是否播放音效

};

#endif // CUBE_H
