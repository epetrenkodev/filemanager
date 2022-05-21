#include "fsmodel.h"

#include <QBrush>

FSModel::FSModel(QObject *parent)
    : QFileSystemModel{parent}
{}

QVariant FSModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::ForegroundRole) {
        if (selectedIndex.contains(index)) {
            return QBrush(QColor(255, 0, 0));
        } else {
            return QBrush(QColor(0, 0, 0));
        }
    }
    return QFileSystemModel::data(index, role);
}

void FSModel::toggleIndex(QModelIndex index)
{
    qDebug() << "toggle" << filePath(index);
    if (selectedIndex.contains(index)) {
        selectedIndex.removeAll(index);
    } else {
        selectedIndex.append(index);
    }
    emit dataChanged(index, index);
}
