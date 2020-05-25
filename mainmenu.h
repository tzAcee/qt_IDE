#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QMenuBar>
#include <QFileDialog>
#include <QLabel>
#include "explorer.h"

class mainMenu : public QMenuBar
{
    Q_OBJECT
public:
    mainMenu(QMainWindow *parent = nullptr);
    void link_to_exp(Explorer* e);
    void link_ws_label(QLabel* lb);
private slots:
    void open(QAction *a);

private:
    Explorer *_exp;
    QLabel *_wsLabel;
};

#endif // MAINMENU_H
