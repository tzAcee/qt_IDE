#ifndef EXPLORER_H
#define EXPLORER_H

#include <QWidget>
#include <QTreeView>
//#include <QFileSystemModel>
#include "DragAndDropModel.h"
#include <QDebug>
#include "editor.h"
#include "debuggeredit.h"
#include <QSignalMapper>
#include <QMenu>

class Explorer : public QTreeView
{
    Q_OBJECT
public:
    Explorer(QWidget *parent = nullptr);

    void point_to_editor(Editor* ed);
    void set_source(QString path);
    void link_deb(debuggerEdit *deb);
    void set_editorWS(const QString& ws);
private:
  DragAndDropModel *_fsModel;
  Editor* _ed = nullptr;
  bool _rightClicked = false;
  debuggerEdit* _deb;

protected:
  void mousePressEvent(QMouseEvent *e);

private slots:
  void select_change(const QItemSelection& a,const QItemSelection& b);
  void onCustomContextMenu(const QPoint &point);
  void del_file(const QString& fil);
  void new_file(const QString& fil);
  void new_dir(const QString& fil);
};

#endif // EXPLORER_H
