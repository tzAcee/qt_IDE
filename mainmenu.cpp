#include "mainmenu.h"

mainMenu::mainMenu(QMainWindow *parent):QMenuBar(parent)
{
    QMenu* fileMenu = new QMenu("File");
    fileMenu->addAction("Open");

    addAction(fileMenu->menuAction());

   // connect(fileMenu, &QAction::triggered, this, &mainMenu::open());
    connect(fileMenu, SIGNAL(triggered(QAction*)), this, SLOT(open(QAction*)));
}

void mainMenu::open(QAction *a)
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open your Workspace"),
                                                    QDir::currentPath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    _exp->set_source(dir);
    _wsLabel->setText(dir);
}

void mainMenu::link_to_exp(Explorer *e)
{
    _exp = e;
}

void mainMenu::link_ws_label(QLabel *lb)
{
    _wsLabel = lb;
    _wsLabel->setText(QDir::currentPath());
}
