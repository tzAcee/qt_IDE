#include "mainstatus.h"

mainStatus::mainStatus(QMainWindow *parent) : QStatusBar(parent)
{
    set_msg("TEST");
}

void mainStatus::set_msg(const QString msg)
{
    showMessage(msg);
}

QString mainStatus::msg()
{
    return this->currentMessage();
}
