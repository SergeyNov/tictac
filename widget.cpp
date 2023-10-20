#include "widget.h"
#include "ui_widget.h"
#include <QTabBar>
#include "stylehelper.h"
#include <QStyleOption>
#include <QFontDatabase>
#include <QDebug>
#include <time.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    configurationTabWidget();
     //ui->pushButton_12->setStyleSheet("color:#f00;");
    //ui->tabWidget->setStyleSheet("QPushButton{color:#f00;}");
    setInterfaceStyle();
    addFonts();
    //changeButtonStyle(1,2,StyleHelper::getCrossVictoryStyle());
    configurationGameAreaButtons();
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&Widget::onComputerSlot);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
    QWidget::paintEvent(event);

}

void Widget::setInterfaceStyle()
{
  setStyleSheet(StyleHelper::getMainWidgetStyle());
  ui->startButton->setStyleSheet(StyleHelper::getStartButtonsStyle());
  ui->aboutButton->setStyleSheet(StyleHelper::getStartButtonsStyle());
  ui->leftButton->setStyleSheet(StyleHelper::getLeftButtonStyle());
  ui->rightButton->setStyleSheet(StyleHelper::getRightButtonActiveStyle());
  ui->tabWidget->setStyleSheet(StyleHelper::getTabWidgetStyle());
  ui->tab->setStyleSheet(StyleHelper::getTabStyle());

//  ui->pushButton_0_0->setStyleSheet(StyleHelper::getBlankButtonStyle());
//  ui->pushButton_0_1->setStyleSheet(StyleHelper::getCrossNormalStyle());
//  ui->pushButton_0_2->setStyleSheet(StyleHelper::getCrossVictoryStyle());
//  ui->pushButton_1_0->setStyleSheet(StyleHelper::getCrossLostStyle());

//  ui->pushButton_2_0->setStyleSheet(StyleHelper::getZeroNormalStyle());
//  ui->pushButton_2_1->setStyleSheet(StyleHelper::getZeroVictoryStyle());
//  ui->pushButton_2_2->setStyleSheet(StyleHelper::getZeroLostStyle());

  ui->messageLabel->setStyleSheet(StyleHelper::getVictoryMessageStyle());
  ui->messageLabel->setText(QString::fromLocal8Bit("Ходят крестики"));


  setGameAreaButtonStyle();

  ui->messageLabel->setText(QString::fromLocal8Bit(""));
  ui->messageLabel->setStyleSheet(StyleHelper::getNormalMessageStyle());
  ui->aboutTextLabel->setStyleSheet(StyleHelper::getAboutTextStyle());


}

void Widget::changeButtonStatus(int num)
{
    if(num == 1){
        ui->leftButton->setStyleSheet(StyleHelper::getLeftButtonStyle());
        ui->rightButton->setStyleSheet(StyleHelper::getRightButtonActiveStyle());
    }
    else
    {
        ui->leftButton->setStyleSheet(StyleHelper::getLeftButtonActiveStyle());
        ui->rightButton->setStyleSheet(StyleHelper::getRightButtonStyle());

    }

}

void Widget::configurationGameAreaButtons()
{
    QGridLayout *grid = qobject_cast <QGridLayout*>(ui->tab->layout());
    for(int row = 0;row<3;row++){
        for(int column=0;column<3;column++)
        {
            QPushButton *btn = qobject_cast <QPushButton*>(grid->itemAtPosition(row,column)->widget());
            btn->setProperty("row",row);
            btn->setProperty("column",column);
            connect(btn,&QPushButton::clicked,this,&Widget::onGameAreaButtonClicked);
        }
    }

}

void Widget::configurationTabWidget()
{
    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setMaximumHeight(320);
    ui->tabWidget->setCurrentWidget(0);
}

void Widget::addFonts()
{
    int id = QFontDatabase::addApplicationFont(":/resourses/fonts/Roboto-Medium.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    qDebug() << family;
    int id2 = QFontDatabase::addApplicationFont(":/resourses/fonts/Roboto-MediumItalic.ttf");
    QString family2 = QFontDatabase::applicationFontFamilies(id2).at(0);
    qDebug() << family2;
}

void Widget::changeButtonStyle(int row, int column, QString style)
{
    QGridLayout *grid = qobject_cast <QGridLayout*>(ui->tab->layout());
    QPushButton *btn = qobject_cast <QPushButton*>(grid->itemAtPosition(row,column)->widget());
    btn->setStyleSheet(style);
}

void Widget::setGameAreaButtonStyle()
{
    QString style = StyleHelper::getBlankButtonStyle();
    for(int row = 0;row<3;row++){
        for(int column=0;column<3;column++)
        {
            changeButtonStyle(row,column,style);
        }
    }

}

void Widget::start()
{
    setGameAreaButtonStyle();
    for (int r=0;r<3;r++) {
        for(int c=0;c<3;c++){
            gameArea[r][c] = '-';
        }
    }
    progress = 0;
    gameStart = true;
    stop = false;
    if(player!='X')
        computerInGame();

}

void Widget::lockPlayer()
{
    if(player == 'X')
    {
        playerLocked = false;
    }
    else
    {
        playerLocked = true;
    }
}

void Widget::computerInGame()
{
   if(stop)
        return;
   srand(time(0));
   int index = rand()%4;
   QStringList list = {QString::fromLocal8Bit("Мой ход"),QString::fromLocal8Bit("Так так так..."),QString::fromLocal8Bit("Х... сложно"),QString::fromLocal8Bit("Дайте подумать...")};
   ui->messageLabel->setText(list.at(index));
   timer->start(2000);
}

void Widget::checkGameStop()
{
    winner = '-';
    QString style;
    char symbols[2] = {'X','0'};
    for(int i=0;i<2;i++){
        for (int row=0;row<3;row++) {
            if((gameArea[row][0] == symbols[i]) && (gameArea[row][1] == symbols[i]) && (gameArea[row][2] == symbols[i])) {
                stop = true;
                winner = symbols[i];
                style = getWinnerStyle();
                changeButtonStyle(row,0,style);
                changeButtonStyle(row,1,style);
                changeButtonStyle(row,2,style);
                return;
            }
        }
        for (int col=0;col<3;col++) {
            if((gameArea[0][col] == symbols[i]) && (gameArea[1][col] == symbols[i]) && (gameArea[2][col] == symbols[i])) {
                stop = true;
                winner = symbols[i];
                style = getWinnerStyle();
                changeButtonStyle(0,col,style);
                changeButtonStyle(1,col,style);
                changeButtonStyle(2,col,style);

                return;
            }
        }
        if((gameArea[0][0] == symbols[i]) && (gameArea[1][1] == symbols[i]) && (gameArea[2][2] == symbols[i])) {
            stop = true;
            winner = symbols[i];
            style = getWinnerStyle();
            changeButtonStyle(0,0,style);
            changeButtonStyle(1,1,style);
            changeButtonStyle(2,2,style);
            return;
        }
        if((gameArea[0][2] == symbols[i]) && (gameArea[1][1] == symbols[i]) && (gameArea[2][0] == symbols[i])) {
            stop = true;
            winner = symbols[i];
            style = getWinnerStyle();
            changeButtonStyle(0,2,style);
            changeButtonStyle(1,1,style);
            changeButtonStyle(2,0,style);
            return;
        }



    }
    if(progress == 9){
        stop = true;
    }

}

void Widget::endGame()
{
    if(stop){
        if(winner == player){
            ui->messageLabel->setText(QString::fromLocal8Bit("Победа!"));
            ui->messageLabel->setStyleSheet(StyleHelper::getVictoryMessageStyle());
        }else if(winner == '-') {
            ui->messageLabel->setText(QString::fromLocal8Bit("Ничья!"));
        } else {
            ui->messageLabel->setText(QString::fromLocal8Bit("Проиграл"));
            ui->messageLabel->setStyleSheet(StyleHelper::getLostMessageStyle());
        }
        playerLocked = true;
        ui->startButton->setText(QString::fromLocal8Bit("Играть"));
        ui->startButton->setStyleSheet(StyleHelper::getStartButtonsStyle());
        ui->leftButton->setDisabled(false);
        ui->rightButton->setDisabled(false);
        gameStart = false;
    }
}

QString Widget::getWinnerStyle()
{
    QString style;
    if(winner==player)
    {
        if(player == 'X'){
            style = StyleHelper::getCrossVictoryStyle();
        }else{
            style = StyleHelper::getZeroVictoryStyle();
        }
    }else{
        if(player == 'X') {
            style = StyleHelper::getZeroLostStyle();
        }else{
            style = StyleHelper::getCrossLostStyle();
        }
    }
    return style;
}


void Widget::on_leftButton_clicked()
{
   changeButtonStatus(1);
   player = 'X';
}

void Widget::on_rightButton_clicked()
{
    changeButtonStatus(0);
    player = '0';

}

void Widget::on_startButton_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
    if(gameStart){
        playerLocked = true;
        ui->startButton->setText(QString::fromLocal8Bit("Играть"));
        ui->startButton->setStyleSheet(StyleHelper::getStartButtonsStyle());
        ui->leftButton->setDisabled(false);
        ui->rightButton->setDisabled(false);
        gameStart = false;

        ui->messageLabel->setText(QString::fromLocal8Bit("Проиграл"));
        ui->messageLabel->setStyleSheet(StyleHelper::getLostMessageStyle());

    } else {
        ui->messageLabel->setText(QString::fromLocal8Bit("Поехали!"));
        ui->messageLabel->setStyleSheet(StyleHelper::getNormalMessageStyle());

        start();
        lockPlayer();
        ui->startButton->setText(QString::fromLocal8Bit("Сдаюсь..."));
        ui->startButton->setStyleSheet(StyleHelper::getStartButtonsGameStyle());
        ui->leftButton->setDisabled(true);
        ui->rightButton->setDisabled(true);

    }
}

void Widget::onGameAreaButtonClicked()
{
    if(!playerLocked){
        QPushButton *btn = qobject_cast<QPushButton*>(sender());
        int row = btn->property("row").toInt();
        int column = btn->property("column").toInt();
        qDebug() << row << ":" << column;
        QString style;
        if(player == 'X'){
            style = StyleHelper::getCrossNormalStyle();
            gameArea[row][column] = 'X';
        }
        else{
            style = StyleHelper::getZeroNormalStyle();
            gameArea[row][column] = '0';
        }
        changeButtonStyle(row,column,style);
        playerLocked = true;
        progress++;
        checkGameStop();
        endGame();
        computerInGame();

    }
}

void Widget::onComputerSlot()
{
    char comp;
    QString style;
    if(player == 'X'){
        comp = '0';
        style = StyleHelper::getZeroNormalStyle();
    }
    else{
        comp = 'X';
        style = StyleHelper::getCrossNormalStyle();
     }
    timer->stop();

        while(true){
            int row = rand()%3;
            int column = rand()%3;
            if(gameArea[row][column] == '-'){
                gameArea[row][column] = comp;

                changeButtonStyle(row,column,style);
                ui->messageLabel->setText(QString::fromLocal8Bit("Твой ход!"));
                progress++;
                checkGameStop();
                endGame();

                playerLocked = false;
                break;
            }
        }

}

void Widget::on_aboutButton_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}
