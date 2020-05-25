#include "explorer.h"

Explorer::Explorer(QWidget *parent) : QTreeView(parent), _fsModel(new DragAndDropModel(this))
{
    this->setModel(_fsModel);
    _fsModel->setReadOnly(false);
    this->setRootIndex(_fsModel->setRootPath(QDir::currentPath()));

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));


    for(int i=1; i<8; i++)
        this->hideColumn(i);
    this->setHeaderHidden(true);

      connect(this->selectionModel(),SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),this,SLOT(select_change(const QItemSelection&,const QItemSelection&)));


      //drag&drop
      setDragDropMode(QAbstractItemView::InternalMove);
      setSelectionMode(QAbstractItemView::ExtendedSelection);
      setDragEnabled(true);
      setAcceptDrops(true);
      setDropIndicatorShown(true);
}

void Explorer::set_source(QString path)
{
    this->setRootIndex(_fsModel->setRootPath(path));
}

void Explorer::point_to_editor(Editor* ed)
{
    _ed = ed;
}

void Explorer::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::RightButton) {
        _rightClicked = true;
    } else {
        _rightClicked = false;
        QTreeView::mousePressEvent(e);
    }
}

void Explorer::select_change(const QItemSelection& /*a*/,const QItemSelection& /*b*/)
{
    if(_rightClicked) return;
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

void Explorer::onCustomContextMenu(const QPoint &point)
{
    _rightClicked=true;
    QMenu *contextMenu = new QMenu;
    contextMenu->addAction("ASD");
    QModelIndex index = indexAt(point);
    if (index.isValid()) {
        contextMenu->exec(viewport()->mapToGlobal(point));
    }
}

