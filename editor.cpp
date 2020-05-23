
#include "editor.h"

#include <QPainter>
#include <QTextBlock>

//![constructor]

Editor::Editor(QPlainTextEdit *parent) : QPlainTextEdit(parent)
{
    //_ui = parent;
    lineNumberArea = new LineNumberArea(this);
    setFixedSize(500, 500);

        highlighter = new Highlighter(document());

    connect(this, SIGNAL (blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL (updateRequest(QRect, int)), this, SLOT(updateLineNumberArea(QRect, int)));
    connect(this, SIGNAL (cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL (textChanged()), this, SLOT(on_text_change()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
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

//![slotUpdateRequest]

//![resizeEvent]

void Editor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
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

void Editor::on_text_change()
{
    _erasing = get_bracket_erased();
     QTextCursor curCur = textCursor();
     exchange_bracket(curCur.position()-1);

    _src = toPlainText();
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
//![extraAreaPaintEvent_2]
