#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "converter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    the_maximum_bit =   0;
    ui->lineEdit->setText("18");
    
    ui->textEdit_2->setReadOnly(true);
    
    cur =   ui->lineEdit_2->cursorPosition();
    
    QRegExp exp("^[0-9]{0,2}$");
    ui->lineEdit->setValidator(new QRegExpValidator(exp, this));
    
    QRegExp exp_2("^[0-9]{0,500}[,.]{0,1}[0-9]{0,2}$");
    ui->lineEdit_2->setValidator(new QRegExpValidator(exp_2, this));
    
    connect(ui->lineEdit,   SIGNAL(textChanged(QString)),   SLOT(processing()));
    connect(ui->lineEdit_2, SIGNAL(textChanged(QString)),   SLOT(processing()));
    connect(ui->checkBox,   SIGNAL(stateChanged(int)),      SLOT(processing()));
    
    connect(ui->lineEdit_2, SIGNAL(cursorPositionChanged(int,int)),   SLOT(slot_cursor(int, int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void    MainWindow::processing()
{
    QStringList list    =   ui->lineEdit_2->text().split(QRegExp("[,.]"));
    
    static QString old  =   ui->lineEdit_2->text();
    
    if (list[0].size() > (the_maximum_bit + 3)) {
        if (ui->lineEdit_2->text()[0] == '0') {
            ui->lineEdit_2->setText(ui->lineEdit_2->text().mid(1));
            ui->lineEdit_2->setCursorPosition(cur);
        } else {
            ui->lineEdit_2->setText(old);
            
            if ((cur ==  (the_maximum_bit + 2)) && (cur1 == (the_maximum_bit + 3)))
                ui->lineEdit_2->setCursorPosition(cur1);
            else
                ui->lineEdit_2->setCursorPosition(cur);
        }
    }
    
    old =   ui->lineEdit_2->text();
    
    Converter   converter(ui->lineEdit_2->text(), ui->lineEdit->text(), ui->checkBox->isChecked(), the_maximum_bit);
    ui->textEdit_2->setText(converter.result());
}

void    MainWindow::slot_cursor(int old, int g)
{
    cur     =   old;
    cur1    =   g;
}
