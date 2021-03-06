
#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include "highlighter.h"
#include "mainstatus.h"
#include "clang_compiler.h"
#include "clang_completion.h"
#include "debuggeredit.h"
#include "saver.h"
#include <QStringListModel>
#include <QCompleter>

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
    ~Editor();

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void set_source(const QString& path);
    void set_ws(const QString& path);
    void point_to_status(mainStatus *status = nullptr);
    void point_to_debugger(debuggerEdit* d);
    void save();
protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *e) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);
    void on_text_change();
    void insert_debug(const QString &msg);
    void on_complete_info(const QStringList &msg);
    void insertCompletion(const QString &completion);


private:
    QWidget *lineNumberArea;
    Highlighter *highlighter;
    debuggerEdit *_deb = nullptr;
    Saver *_saver = nullptr;
    QCompleter *_completionPOP;
    Clang_Compiler *_clangC = nullptr;
    Clang_Completion *_clangComplete = nullptr;

    mainStatus *_status;
    QString _currentFile;
    QSet<Qt::Key> _keysPressed;

    QString _src;
    QString _ws;
    bool _erasing = false;

    bool _changedFile = false;
    bool _savePending = false;

    void exchange_bracket(int pos);
    bool get_bracket_erased();
    void compile();
    void complete();
};

///////////////////////////////////////////////////////////////////////////////////////////////////

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
