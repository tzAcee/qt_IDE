#ifndef EXPLORER_H
#define EXPLORER_H

#include <QWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QDebug>
#include "editor.h"

class Explorer : public QTreeView
{
    Q_OBJECT
public:
    Explorer(QWidget *parent = nullptr);

    void point_to_editor(Editor* ed);
    void set_source(QString path);
private:
    QFileSystemModel *_fsModel;

private:
  Editor* _ed = nullptr;

private slots:
  void select_change(const QItemSelection& a,const QItemSelection& b);
};

#endif // EXPLORER_H
