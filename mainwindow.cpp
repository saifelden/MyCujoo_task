#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <string>
#include <QPixmap>
#include <QDebug>
#include<iostream>
#include <utility>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include<QFile>
using namespace std;

vector< pair<int,int> > blue;
vector< pair<int,int> > red;
vector< pair <int,int> > white;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_upload_btn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
       QPixmap the_Image;
       the_Image.load(fileName);
       ui->frame->setPixmap(the_Image.scaled(250, 250, Qt::IgnoreAspectRatio, Qt::FastTransformation));
       img = the_Image.toImage();
}


void MainWindow::on_write_btn_clicked()
{

    get_blue_shape();
    get_red_shape();
    get_white_shape();
    vector< pair<int,int> > sol = traverse_white();
    pair<int,int> min_red,min_blue,max_red,max_blue,min_white,max_white;
    int min_sum = 1000,max_sum=0;

    for(int i=0;i<blue.size();i++)
    {
        if(blue[i].first+blue[i].second<min_sum)
        {
            min_blue = make_pair( blue[i].first,blue[i].second);
            min_sum = blue[i].first+blue[i].second;
        }
        if(blue[i].first+blue[i].second>max_sum)
        {
            max_blue = make_pair( blue[i].first,blue[i].second);
            max_sum = blue[i].first+blue[i].second;
        }
    }
    min_sum=1000;
    max_sum = 0;
    for(int i=0;i<red.size();i++)
    {
        if(red[i].first+red[i].second<min_sum)
        {
            min_red = make_pair( red[i].first,red[i].second);
            min_sum = red[i].first+red[i].second;
        }
        if(red[i].first+red[i].second>max_sum)
        {
            max_red = make_pair( red[i].first,red[i].second);
            max_sum = red[i].first+red[i].second;
        }
    }
    min_sum=1000;
    max_sum = 0;
    for(int i=0;i<sol.size();i++)
    {
        if(sol[i].first+sol[i].second<min_sum)
        {
            min_white = make_pair( sol[i].first,sol[i].second);
            min_sum = sol[i].first+sol[i].second;
        }
        if(sol[i].first+sol[i].second>max_sum)
        {
            max_white = make_pair( sol[i].first,sol[i].second);
            max_sum = sol[i].first+sol[i].second;
        }
    }

    cout<<min_blue.first<<" "<<min_blue.second<<" "<<max_blue.first<<" "<<max_blue.second<<endl;
    cout<<min_red.first<<" "<<min_red.second<<" "<<max_red.first<<" "<<max_red.second<<endl;
    cout<<min_white.first<<" "<<min_white.second<<" "<<max_white.first<<" "<<max_white.second<<endl;

    cout<<white.size()<<endl;
    QJsonObject coord1;
    QJsonObject coord2;
    coord1["x"]=min_blue.second;
    coord1["y"]=min_blue.first;
    coord2["x"]=max_blue.second;
    coord2["y"]=max_blue.first;
    QJsonArray vert1;
    vert1.push_back(coord1);
    vert1.push_back(coord2);
    QJsonObject first_shape;
    first_shape["vertices"]= vert1;
    QJsonObject first;
    first["boundingPoly"] = first_shape;
    first["description"]= "Circle";

       QJsonObject coord3;
       QJsonObject coord4;
       coord3["x"]=min_red.second;
       coord3["y"]=min_red.first;
       coord4["x"]=max_red.second;
       coord4["y"]=max_red.first;
       QJsonArray vert2;
       vert2.push_back(coord3);
       vert2.push_back(coord4);
       QJsonObject second_shape;
       second_shape["vertices"]= vert2;
       QJsonObject second;
       second["boundingPoly"] = second_shape;
       second["description"]= "Rectangle";

       QJsonObject coord5;
       QJsonObject coord6;
       coord5["x"]=min_white.second;
       coord5["y"]=min_white.first;
       coord6["x"]=max_white.second;
       coord6["y"]=max_white.first;
       QJsonArray vert3;
       vert3.push_back(coord5);
       vert3.push_back(coord6);
       QJsonObject third_shape;
       third_shape["vertices"]= vert3;
       QJsonObject third;
       third["boundingPoly"] = third_shape;
       third["description"]= "Rectangle";

       QJsonArray jsonarr;
       jsonarr.push_back(first);
       jsonarr.push_back(second);
       jsonarr.push_back(third);
       QJsonDocument jsondoc = QJsonDocument(jsonarr);
       QFile jsonFile("/home/saifeldein/git_repositories/MyCujoo_task/output.json");
       jsonFile.open(QFile::WriteOnly);
       jsonFile.write(jsondoc.toJson());



}
void MainWindow::get_blue_shape()
{
    for(int i=0;i<img.height();i++)
    {
        for(int j=0;j<img.width();j++)
        {

                QColor clrCurrent(img.pixel(j,i));
                if(clrCurrent.blue()>170 && clrCurrent.green()<30 && clrCurrent.red()<30)
                {
                    blue.push_back(make_pair(i,j));

                }

        }
    }
}

void MainWindow::get_white_shape()
{
    for(int i=0;i<img.height();i++)
    {
        for(int j=0;j<img.width();j++)
        {

                QColor clrCurrent(img.pixel(j,i));
                if(clrCurrent.blue()>200 && clrCurrent.green()>200 && clrCurrent.red()>200)
                {
                    white.push_back(make_pair(i,j));

                }

        }
    }
}

void MainWindow::get_red_shape()
{
    for(int i=0;i<img.height();i++)
    {
        for(int j=0;j<img.width();j++)
        {

                QColor pixel(img.pixel(j,i));
                if(pixel.red()>170 && pixel.green()<30 && pixel.blue()<30)
                {
                    red.push_back(make_pair(i,j));
                }

        }
    }
}

vector< pair<int,int> > MainWindow::traverse_white()
{
    for(int i=0;i<img.height();i++)
    {
        vector<int> tmp(img.width(),0);
        ht.push_back(tmp);
    }
    vector< pair<int,int> > sol,max_sol;
    for (int i=0;i<white.size();i++)
    {
        int curr_i = white[i].first;
        int curr_j = white[i].second;
        if(ht[curr_i][curr_j]==0)
        {
            sol = dfs(curr_i,curr_j);
            if(sol.size()>max_sol.size())
            {
                max_sol = sol;
            }
        }
    }

    return max_sol;

}
vector< pair<int,int> > MainWindow::dfs(int i,int j)
{
    vector< pair<int,int> > sol1,sol2;

    if(i+1 <img.height())
    {
        QColor pixel1(img.pixel(j,i+1));
        if(pixel1.blue()>200 &&pixel1.red()>200 && pixel1.green()>200 && ht[i+1][j]==0)
        {
            ht[i+1][j]=1;
            sol1 = dfs(i+1,j);
            sol1.push_back(make_pair(i+1,j));
        }

    }
    if(j+1 <img.width())
    {
        QColor pixel2(img.pixel(j+1,i));
        if(pixel2.blue()>200 &&pixel2.red()>200 && pixel2.green()>200 && ht[i][j+1]==0)
        {
            ht[i][j+1]=1;
            sol2 = dfs(i,j+1);
            sol2.push_back(make_pair(i,j+1));
        }

    }

   sol1.insert(sol1.end(), sol2.begin(), sol2.end());
   return sol1;
}

