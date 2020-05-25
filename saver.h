#ifndef SAVER_H
#define SAVER_H

#include <QThread>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

class Saver : public QThread
{
  Q_OBJECT
public:
    Saver(QString path, QString content);
protected:
    void run();
private:
    QString &_path;
    QString &_content;
};

#endif // SAVER_H
