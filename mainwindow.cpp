#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QPlainTextEdit>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    //_editor = new editor(ui->centralwidget->findChild("editor"));
    ui->setupUi(this);
    //QPlainTextEdit *a = ui->centralwidget->findChild<QPlainTextEdit*>("editor");
      //  _editor = new Editor(a);
    ui->treeView->point_to_editor(ui->editor);

}

MainWindow::~MainWindow()
{
    delete ui;
}

