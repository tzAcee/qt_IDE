#ifndef CLANG_COMPLETION_H
#define CLANG_COMPLETION_H

#include <QThread>
#include <clang-c/Index.h>
#include <QStringList>

class Clang_Completion : public QThread
{
    Q_OBJECT
public:
    Clang_Completion(QString &file, int ln, int col);

private:
    QString _file;
    int _line = 0;
    int _col = 0;

    void complete();

protected:
    void run();
signals:
    void new_info(const QStringList &ls);
};

#endif // CLANG_COMPLETION_H
