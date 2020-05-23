#include "explorer.h"
#include <QVBoxLayout>

Explorer::Explorer(QWidget *parent) : QTreeView(parent), _fsModel(new QFileSystemModel())
{

    _fsModel->setRootPath(QDir::currentPath());
    this->setModel(_fsModel);


    for(int i=1; i<8; i++)
        this->hideColumn(i);
}
