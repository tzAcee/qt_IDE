
#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include "highlighter.hpp"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;

//![codeeditordefinition]

class Editor : public QPlainTextEdit
{
    Q_OBJECT

public:
    Editor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void set_source(QString path);
        //QPlainTextEdit *_ui;

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);
    void on_text_change();

private:
    QWidget *lineNumberArea;
    Highlighter *highlighter;


    QString _src;
    bool _erasing = false;

    void exchange_bracket(int pos);
    bool get_bracket_erased();
};

//![codeeditordefinition]
//![extraarea]

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(Editor *editor) : QWidget(editor), codeEditor(editor)
    {}

    QSize sizeHint() const override
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    Editor *codeEditor;
};

//![extraarea]

#endif
