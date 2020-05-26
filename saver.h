#ifndef SAVER_H
#define SAVER_H

#include <QThread>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QMutex>

class Saver : public QThread
{
  Q_OBJECT
public:
    Saver(const QString& path, const QString& content);
protected:
    void run();
private:
    QString _path;
    QMutex mutex;
    QString _content;
};

#endif // SAVER_H
