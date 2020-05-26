#include "debuggeredit.h"


debuggerEdit::debuggerEdit(QWidget *par) : QPlainTextEdit(par)
{
    setReadOnly(true);
    this->setPlainText("");
}

void debuggerEdit::warn(const QString& msg)
{
        time();
    insertPlainText("WARN: "+msg);

    int bufferSz = 6;
    int cur = textCursor().position()-msg.size()-bufferSz;
    int begin = cur;
    int end = cur+bufferSz-2;


    QTextCharFormat fmt;
    fmt.setBackground(QColor(255,127,80));
    fmt.setFontWeight(70);

    QTextCursor cursor(document());
    cursor.setPosition(begin, QTextCursor::MoveAnchor);
    cursor.setPosition(end, QTextCursor::KeepAnchor);
    cursor.setCharFormat(fmt);
    insertPlainText("\n");
}

void debuggerEdit::fatal(const QString &msg)
{
    time();
    insertPlainText("FATAL: "+msg);

    int bufferSz = 7;
    int cur = textCursor().position()-msg.size()-bufferSz;
    int begin = cur;
    int end = cur+bufferSz-2;


    QTextCharFormat fmt;
    fmt.setBackground(QColor(0,0,0));
    fmt.setFontWeight(70);

    QTextCursor cursor(document());
    cursor.setPosition(begin, QTextCursor::MoveAnchor);
    cursor.setPosition(end, QTextCursor::KeepAnchor);
    cursor.setCharFormat(fmt);
    insertPlainText("\n");
}

void debuggerEdit::error(const QString& msg)
{
        time();
    insertPlainText("ERROR: "+msg);

    int bufferSz = 7;
    int cur = textCursor().position()-msg.size()-bufferSz;
    int begin = cur;
    int end = cur+bufferSz-2;


    QTextCharFormat fmt;
    fmt.setBackground(QColor(255, 0, 0, 127));
    fmt.setFontWeight(70);

    QTextCursor cursor(document());
    cursor.setPosition(begin, QTextCursor::MoveAnchor);
    cursor.setPosition(end, QTextCursor::KeepAnchor);
    cursor.setCharFormat(fmt);

    insertPlainText("\n");
}

void debuggerEdit::info(const QString& msg)
{
    //
    if(msg.size() == 0) return;
    time();

    insertPlainText("INFO: "+msg);

    int bufferSz = 6;
    int cur = textCursor().position()-msg.size()-bufferSz;
    int begin = cur;
    int end = cur+bufferSz-2;


    QTextCharFormat fmt;
    fmt.setBackground(QColor(0, 0, 255, 127));
    fmt.setFontWeight(70);

    QTextCursor cursor(document());
    cursor.setPosition(begin, QTextCursor::MoveAnchor);
    cursor.setPosition(end, QTextCursor::KeepAnchor);
    cursor.setCharFormat(fmt);
    insertPlainText("\n");

}

void debuggerEdit::success(const QString& msg)
{
    time();
    insertPlainText("NICE: "+msg);

    int bufferSz = 6;
    int cur = textCursor().position()-msg.size()-bufferSz;
    int begin = cur;
    int end = cur+bufferSz-2;


    QTextCharFormat fmt;
    fmt.setBackground(QColor(46,139,87));
    fmt.setFontWeight(70);

    QTextCursor cursor(document());
    cursor.setPosition(begin, QTextCursor::MoveAnchor);
    cursor.setPosition(end, QTextCursor::KeepAnchor);
    cursor.setCharFormat(fmt);

    insertPlainText("\n");
}

void debuggerEdit::clear()
{
    this->setPlainText("");
}

void debuggerEdit::time()
{
    QString tString = QDateTime::currentDateTime().toString("hh:mm:ss");
    this->insertPlainText(tString+" | ");
}
