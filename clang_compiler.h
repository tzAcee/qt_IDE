#ifndef CLANGER_H
#define CLANGER_H

#include <QThread>
#include <clang-c/Index.h>
#include <QDirIterator>
#include <QMutex>

class Clang_Compiler : public QThread
{
    Q_OBJECT
public:
    Clang_Compiler(const QString &ws);
    void set(const QString& ws);
private:
    std::vector<QString> _files;
    QString _ws;
  //  debuggerEdit &_deb;
    void collect_paths(const QString& ws);
    void compile(const QString& ws);
    QMutex mutex;

protected:
    void run();

signals:
    void debug_add(const QString &msg);
};

#endif // CLANGER_H
