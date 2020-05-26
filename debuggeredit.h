#ifndef DEBUGGEREDIT_H
#define DEBUGGEREDIT_H

#include <QObject>
#include <QPlainTextEdit>
#include <QWidget>
#include <QTextCharFormat>
#include <QDateTime>

class debuggerEdit : public QPlainTextEdit
{
    Q_OBJECT
private:
    void time();
public:
    debuggerEdit(QWidget *par = nullptr);
    void warn(const QString& msg);
    void info(const QString& msg);
    void success(const QString& msg);
    void error(const QString& msg);
    void fatal(const QString& msg);
    void clear();
};

#endif // DEBUGGEREDIT_H
