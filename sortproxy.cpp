#include "sortproxy.h"
#include <QDebug>
#include <QFileSystemModel>

SortProxy::SortProxy(QObject *parent)
    : QSortFilterProxyModel{parent}
{}

bool SortProxy::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QFileSystemModel *fsm = qobject_cast<QFileSystemModel *>(sourceModel());

    QFileInfo leftFileInfo = fsm->fileInfo(source_left);
    QFileInfo rightFileInfo = fsm->fileInfo(source_right);

    QString leftName = source_left.data(QFileSystemModel::FileNameRole).toString();
    QString rightName = source_right.data(QFileSystemModel::FileNameRole).toString();

    if (leftName == "..")
        return true;
    if (rightName == "..")
        return false;

    if (!leftFileInfo.isDir() && rightFileInfo.isDir()) {
        return false;
    }
    if (leftFileInfo.isDir() && !rightFileInfo.isDir()) {
        return true;
    }

    return leftName < rightName;
    //return QSortFilterProxyModel::lessThan(source_left, source_right);
}
