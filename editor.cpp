
#include "editor.h"

#include <QPainter>
#include <QTextBlock>
#include <QMessageBox>
#include <QTextStream>

#include <QAbstractItemView>
#include <QScrollBar>


//![constructor]

Editor::Editor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);
    _completionPOP = new QCompleter(this);
    highlighter = new Highlighter(document());

    connect(this, SIGNAL (blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL (updateRequest(QRect, int)), this, SLOT(updateLineNumberArea(QRect, int)));
    connect(this, SIGNAL (cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL (textChanged()), this, SLOT(on_text_change()));


    _completionPOP->setCaseSensitivity(Qt::CaseInsensitive);
    _completionPOP->setWidget(this);
    _completionPOP->setCompletionMode(QCompleter::PopupCompletion);


    connect(_completionPOP, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));


    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    set_ws(QDir::currentPath());

}
void Editor::insertCompletion(const QString &completion)
{
    if(_completionPOP->widget() != this)
        return;
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    tc.removeSelectedText(); //capitalization may have been wrong, since we're doing case insensitive completing
    tc.insertText(completion);
    setTextCursor(tc);
}

void Editor::focusInEvent(QFocusEvent *e)
{
    _completionPOP->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}

void Editor::insert_debug(const QString &msg)
{
    if(msg.contains("fatal error:"))
             _deb->fatal(msg);
         else if(msg.contains("warning:"))
             _deb->warn(msg);
         else if(msg.contains("error:"))
             _deb->error(msg);
         else
             _deb->info(msg);
}

//![constructor]

//![extraAreaWidth]

int Editor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

//![extraAreaWidth]

//![slotUpdateExtraAreaWidth]

void Editor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

//![slotUpdateExtraAreaWidth]

//![slotUpdateRequest]

void Editor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void Editor::save()
{
    if(_currentFile.size() <= 0)
    {
        this->_deb->error("Select a file before you save something.");
        return;
    }

    if(_saver != nullptr)
    {
        if(_saver->isRunning())
        {
        _deb->warn("A Save process is running!");
        return;
        }
        delete _saver;
    }
    _saver = new Saver(_currentFile, _src);
    _saver->start();
    //_saver->wait();
    _status->set_msg(_currentFile);
    _changedFile = true;
    _savePending = false;
}

void Editor::point_to_debugger(debuggerEdit *d)
{
    _deb = d;
}

void Editor::compile()
{
    if(_ws.size() <= 0)
    {
        _deb->error("No Workspace set!");
        return;
    }
    if(_clangC != nullptr)
    {
        if(_clangC->isRunning())
        {
        _deb->warn("A Build process is running!");
        return;
        }
        delete _clangC;
    }

    _clangC = new Clang_Compiler(_ws);
        connect(_clangC, SIGNAL(debug_add(const QString&)), this, SLOT(insert_debug(const QString&)));
    _clangC->start();
}

Editor::~Editor()
{
    delete _clangC;
    delete _saver;
    delete _clangComplete;
}


void Editor::keyPressEvent(QKeyEvent *event)
{
    if(_completionPOP->popup()->isVisible())
       {
           switch (event->key())
           {
           case Qt::Key_Enter:
           case Qt::Key_Return:
           case Qt::Key_Escape:
           case Qt::Key_Tab:
           case Qt::Key_Backtab:
               event->ignore();
               return;
           default:
               break;
           }
       }

    if(event->key()==Qt::Key_F5)
    {
        compile();
    }

    _keysPressed.insert((Qt::Key)event->key());

    if(_keysPressed.size() == 2)
    {
        if(_keysPressed.contains(Qt::Key_S) && _keysPressed.contains(Qt::Key_Control))
        {
            if(_savePending)
                save();
        }
        _keysPressed.clear();
    }


 // popup it up!

    return QPlainTextEdit::keyPressEvent(event);
}

void Editor::point_to_status(mainStatus *status)
{
    _status = status;
}

//![resizeEvent]

void Editor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void Editor::set_ws(const QString &path)
{
    _ws =path;
}

void Editor::set_source(const QString &path)
{
    _changedFile = false;
    _src = "";
    QFile _fl(path);
    if(!_fl.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(0, "error", _fl.errorString());
    }
    _currentFile = _fl.fileName();
    QTextStream in(&_fl);

    while(!in.atEnd()) {
        QString line = in.readLine();
        _src.append(line+"\n");
    }

    _fl.close();
    this->setPlainText(_src);

    _status->set_msg(_currentFile);
    _changedFile = true;
}

bool Editor::get_bracket_erased()
{
    std::string def = toPlainText().toStdString();
    if(def.size() < _src.toStdString().size())
    {
            return true;
    }

    return false;

}
void Editor::exchange_bracket(int pos)
{
    std::string def = toPlainText().toStdString();
    if(pos < 0) return;
    switch(def.at(pos))
    {
        case '{':
        {
            if(_erasing == false)
            {
                insertPlainText(QString(" }"));
                QTextCursor curCur = textCursor();
                curCur.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
                setTextCursor(curCur);
            }
            break;
        }
        case '[':
        {
            if(_erasing == false)
            {
                insertPlainText(QString(" ]"));
                QTextCursor curCur = this->textCursor();
                curCur.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
                setTextCursor(curCur);
            }
            break;
        }
    }
}

void Editor::complete()
{
    QTextCursor cursor = textCursor();
    int y = cursor.blockNumber() + 1;
    int x = cursor.columnNumber() + 1;

    _clangComplete = new Clang_Completion(_src, y, x);
    connect(_clangComplete, SIGNAL(new_info(const QStringList&)), this, SLOT(on_complete_info(const QStringList&)));
    _clangComplete->start();
}

void Editor::on_text_change()
{


    if(_changedFile || _savePending == false)
    {
        _status->set_msg(_status->msg()+" [*]");
        _changedFile = false;
        _savePending = true;
    }
    _erasing = get_bracket_erased();
     QTextCursor curCur = textCursor();
     exchange_bracket(curCur.position()-1);

    _src = toPlainText();
        complete();
}

//![resizeEvent]

//![cursorPositionChanged]

void Editor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::green).darker(320);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

//![cursorPositionChanged]

//![extraAreaPaintEvent_0]

void Editor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

//![extraAreaPaintEvent_0]

//![extraAreaPaintEvent_1]
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
//![extraAreaPaintEvent_1]

//![extraAreaPaintEvent_2]
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;

    }
}

void Editor::on_complete_info(const QStringList &msg)
{
    _completionPOP->setModel(new QStringListModel(msg, _completionPOP));

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word

    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    QString completionPrefix = tc.selectedText();
    if (toPlainText().isEmpty() || eow.contains(completionPrefix.right(1)))
    {
        _completionPOP->popup()->hide();
        return;
    }

    if (completionPrefix != _completionPOP->completionPrefix()) {
        _completionPOP->setCompletionPrefix(completionPrefix);
        _completionPOP->popup()->setCurrentIndex(_completionPOP->completionModel()->index(0, 0));
    }

    QRect cr = cursorRect();
    cr.setWidth(_completionPOP->popup()->sizeHintForColumn(0)
                + _completionPOP->popup()->verticalScrollBar()->sizeHint().width());

    _completionPOP->complete(cr);
}

