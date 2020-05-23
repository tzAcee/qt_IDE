#ifndef EXPLORER_H
#define EXPLORER_H

#include <QWidget>
#include <QTreeView>
#include <QFileSystemModel>

class Explorer : public QTreeView
{
    Q_OBJECT
public:
    Explorer(QWidget *parent = nullptr);
private:
    QFileSystemModel *_fsModel;
};

#endif // EXPLORER_H
