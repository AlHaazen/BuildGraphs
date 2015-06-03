#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <logic.cpp>
#include <stdlib.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{

    ui->setupUi(this);
    scene  =new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    ui->centralWidget->setLayout(ui->verticalLayout_2);
    scene->addLine(-100,0,100,0);
    scene->addLine(0,-100,0,100);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;

}

void MainWindow::on_actionAbout_Qt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_pushButton_clicked()
{

    scene->clear();
    scene->addLine(-100,0,100,0,QPen(Qt::blue));
    scene->addLine(0,-100,0,100, QPen(Qt::blue));



    for(double i=left; i<=right; i+=precision)
    {
        char  *cs= new char[ui->lineEdit->text().length()+1];
        strcpy(cs,  ui->lineEdit->text().toStdString().c_str());
        ss=cs;
        double y1=xAt(i-precision);
        delete cs;
        cs= new char[ui->lineEdit->text().length()+1];

        strcpy(cs,  ui->lineEdit->text().toStdString().c_str());
        ss=cs;
        double y2=xAt(i);
        strcpy(cs,  ui->lineEdit->text().toStdString().c_str());
        ss=cs;
        double y3=xAt(i-2*precision);
        delete cs;

        scene->addLine(scale*(i-precision),-y1*scale,scale*i,-y2*scale);
        if(ui->checkBox->isChecked())
            scene->addLine
                    (
                        scale*(i-precision),
                        -(y1-y3)*scale/precision,
                        scale*i,
                        -(y2-y1)*scale/precision,
                        QPen(Qt::red)

                    );
      //  qDebug()<<y2-y1;
    }

}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    left=arg1;
    on_pushButton_clicked();
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    right = arg1;
    on_pushButton_clicked();
}

void MainWindow::on_spinBox_3_valueChanged(int arg1)
{
    scale=arg1;
    on_pushButton_clicked();
}
