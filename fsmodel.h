#ifndef FSMODEL_H
#define FSMODEL_H

#include <QFileSystemModel>
#include <QProgressDialog>

class FSModel : public QFileSystemModel
{
public:
    explicit FSModel(QObject *parent = nullptr);

    void toggleIndex(QModelIndex index);
    void clearSelect();
    void removeFiles(QModelIndex file);
    void copyFiles(QModelIndex source, QString destinationDir);
    virtual QVariant data(const QModelIndex &index, int role) const override;

private:
    QModelIndexList selectedIndex;

    bool copyDir(const QDir srcDir, const QDir dstDir);
    QFileInfoList getList(QStringList files);
};

#endif // FSMODEL_H
