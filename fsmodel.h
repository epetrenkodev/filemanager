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
    void renameFiles(QModelIndex source, QString destinationDir);
    void mkDir(QModelIndex parent, QString dirName);
    virtual QVariant data(const QModelIndex &index, int role) const override;

private:
    QModelIndexList selectedIndex;

    bool copyDir(const QDir srcDir, const QDir dstDir);
};

#endif // FSMODEL_H
