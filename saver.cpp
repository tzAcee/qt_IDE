#include "saver.h"

Saver::Saver(QString path, QString content) : _path(path), _content(content)
{

}

void Saver::run()
{
    QFile _fl(_path);
    if(!_fl.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(0, "error", _fl.errorString());
        return;
    }

   QTextStream stream(&_fl);
   stream << _content;
   _fl.close();

}
