#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    int cur;
    int cur1;
    
    int the_maximum_bit;
private slots:
    void    processing();
    void    slot_cursor(int old, int g);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
