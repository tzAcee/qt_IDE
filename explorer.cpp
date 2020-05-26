#include "explorer.h"
#include <QStandardItem>

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

void Explorer::set_editorWS(const QString &ws)
{
    _ed->set_ws(ws);
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

    QModelIndex index = indexAt(point);
    QMenu *contextMenu = new QMenu;


    QFileSystemModel* model = (QFileSystemModel*)this->model();
    int row = -1;
    if (index.row()!=-1 && index.column()==0)
    {
         QFileInfo fileInfo = model->fileInfo(index);

         QSignalMapper* delMapper = new QSignalMapper (this) ;
         QAction * delAct = new QAction("Delete " +fileInfo.fileName());
         connect(delAct, SIGNAL(triggered()), delMapper, SLOT(map()));
         delMapper->setMapping(delAct, fileInfo.filePath());
         connect (delMapper, SIGNAL(mapped(const QString&)), this, SLOT(del_file(const QString&))) ;

         QAction *fileAct = new QAction("New File");
         QSignalMapper* fileMapper = new QSignalMapper (this) ;
         connect(fileAct, SIGNAL(triggered()), fileMapper, SLOT(map()));
         fileMapper->setMapping(fileAct, fileInfo.filePath());
         connect (fileMapper, SIGNAL(mapped(const QString&)), this, SLOT(new_file(const QString&))) ;

         QAction *dirAct = new QAction("New Directory");
         QSignalMapper* dirMapper = new QSignalMapper (this) ;
         connect(dirAct, SIGNAL(triggered()), dirMapper, SLOT(map()));
         dirMapper->setMapping(dirAct, fileInfo.filePath());
         connect (dirMapper, SIGNAL(mapped(const QString&)), this, SLOT(new_dir(const QString&))) ;

         row = index.row();

         contextMenu->addAction(fileAct);
         contextMenu->addAction(dirAct);
         contextMenu->addAction(delAct);
     }

    if (index.isValid()) {
        contextMenu->exec(viewport()->mapToGlobal(point));
    }
}

void Explorer::del_file(const QString& fil)
{
    try
    {

           QFileInfo tmpI(fil);
           if(tmpI.isDir())
           {

               QDir dir(fil);
               dir.removeRecursively();
                          _deb->success("Deleted directory and all it's content- "+fil);
           }
           else
           {
               QFile tmp(fil);
               tmp.remove();
                          _deb->success("Deleted file- "+fil);
           }


    }
    catch (std::exception& e)
    {
        _deb->error(e.what());
    }

}

void Explorer::new_file(const QString& fil)
{
    try
    {
                QFileInfo fInf(fil);
                if(fInf.isDir())
                {
                   QFile tmp(fil+"/new_file");
                   tmp.open(QIODevice::ReadWrite);
                   _deb->success("Created file in "+fil);
                }
                else
                {
                    QFile tmp(fInf.absolutePath()+"/new_file");
                    tmp.open(QIODevice::ReadWrite);
                    _deb->success("Created file in "+fInf.absolutePath());
                }
    }
    catch (std::exception& e)
    {
        _deb->error(e.what());
    }
}

void Explorer::new_dir(const QString& fil)
{
    try
    {

                QFileInfo fInf(fil);
                if(fInf.isDir())
                {
                    QDir dir;
                    dir.mkpath(fil+"/new_folder");
                    _deb->success("Created directory in "+fil);
                }
                else
                {
                    QDir dir;
                    dir.mkpath(fInf.absolutePath()+"/new_file");
                    _deb->success("Created directory in "+fInf.absolutePath());
                }
    }
    catch (std::exception& e)
    {
        _deb->error(e.what());
    }
}

void Explorer::link_deb(debuggerEdit *deb)
{
    _deb = deb;
}

