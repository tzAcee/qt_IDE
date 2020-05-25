#include "explorer.h"
#include <QVBoxLayout>

Explorer::Explorer(QWidget *parent) : QTreeView(parent), _fsModel(new QFileSystemModel(this))
{
    this->setModel(_fsModel);

    this->setRootIndex(_fsModel->setRootPath(QDir::currentPath()));


    for(int i=1; i<8; i++)
        this->hideColumn(i);

      connect(this->selectionModel(),SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),this,SLOT(select_change(const QItemSelection&,const QItemSelection&)));
}

void Explorer::set_source(QString path)
{
    this->setRootIndex(_fsModel->setRootPath(path));
}

void Explorer::point_to_editor(Editor* ed)
{
    _ed = ed;
}

void Explorer::select_change(const QItemSelection& a,const QItemSelection& b)
{

  QModelIndexList list = selectionModel()->selectedIndexes();
  QFileSystemModel* model = (QFileSystemModel*)this->model();
  int row = -1;
  foreach (QModelIndex index, list)
  {
      if (index.row()!=row && index.column()==0)
      {
          QFileInfo fileInfo = model->fileInfo(index);
          _ed->set_source(fileInfo.filePath());
          qDebug() << fileInfo.filePath() << '\n';
          row = index.row();
      }
  }
}

