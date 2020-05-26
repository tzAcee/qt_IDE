#ifndef DRAGANDDROPMODEL_H
#define DRAGANDDROPMODEL_H

#include <QFileSystemModel>
#include <QWidget>

class DragAndDropModel : public QFileSystemModel
{
public:
    DragAndDropModel(QWidget *parent) : QFileSystemModel(parent) {}

    Qt::DropActions supportedDropActions() const override
    {
        return Qt::CopyAction | Qt::MoveAction;
    }

    Qt::ItemFlags flags(const QModelIndex &index) const override
    {
        if (!index.isValid())
            return 0;

        return Qt::ItemIsDragEnabled |Qt::ItemIsEditable| Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
    }
};

#endif // DRAGANDDROPMODEL_H
