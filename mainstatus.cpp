#include "mainstatus.h"

mainStatus::mainStatus(QMainWindow *parent) : QStatusBar(parent)
{
    set_msg("Welcome!");
}

void mainStatus::set_msg(const QString msg)
{
    showMessage(msg);
}

QString mainStatus::msg()
{
    return this->currentMessage();
}
