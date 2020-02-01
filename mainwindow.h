#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<vector>
#include<iostream>
#include <utility>
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_upload_btn_clicked();

    void on_write_btn_clicked();
    void get_red_shape();
    void get_blue_shape();
    void get_white_shape();
    vector< pair<int,int> >traverse_white();
    vector< pair<int,int> > dfs(int i,int j);
private:
    Ui::MainWindow *ui;
    QImage img;
    vector< vector<int> > ht;


};

#endif // MAINWINDOW_H
