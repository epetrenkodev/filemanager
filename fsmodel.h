#ifndef FSMODEL_H
#define FSMODEL_H

#include <QFileSystemModel>

class FSModel : public QFileSystemModel
{
public:
    QList<QModelIndex> selectedIndex;
    explicit FSModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    virtual QVariant data(const QModelIndex &index, int role) const override;
    void toggleIndex(QModelIndex index);
};

#endif // FSMODEL_H
