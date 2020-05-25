#ifndef MAINSTATUS_H
#define MAINSTATUS_H

#include <QMainWindow>
#include <QStatusBar>

class mainStatus : public QStatusBar
{
    Q_OBJECT
public:
    mainStatus(QMainWindow *parent);
    void set_msg(const QString msg);
    QString msg();
};

#endif // MAINSTATUS_H
