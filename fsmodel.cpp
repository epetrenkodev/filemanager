#include "fsmodel.h"
#include "fsutils.h"

#include <QApplication>
#include <QBrush>
#include <QFile>
#include <QMessageBox>

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
    if (selectedIndex.contains(index)) {
        selectedIndex.removeAll(index);
    } else {
        selectedIndex.append(index);
    }
    emit dataChanged(index, index);
}

void FSModel::clearSelect()
{
    selectedIndex.clear();
}

void FSModel::removeFiles(QModelIndex file)
{
    int count = 1;
    QProgressDialog progress;
    progress.setMinimumDuration(0);
    progress.setCancelButtonText("Отмена");
    progress.setFixedWidth(300);
    progress.setWindowTitle("Удаление");
    progress.setWindowIcon(QIcon(":/icons/delete.ico"));

    if (selectedIndex.isEmpty()) {
        progress.setMaximum(1);
        QFile::moveToTrash(filePath(file));
    } else {
        progress.setMaximum(selectedIndex.count());
        for (QModelIndex index : selectedIndex) {
            progress.setLabelText(filePath(index));
            progress.setValue(count++);
            qApp->processEvents();
            QFile::moveToTrash(filePath(index));
            if (progress.wasCanceled())
                break;
        }
        selectedIndex.clear();
    }
}

void FSModel::copyFiles(QModelIndex source, QString destinationDir)
{
    QStringList copyList;

    if (selectedIndex.isEmpty()) {
        copyList.append(filePath(source));
    } else {
        QStringList tmpList;
        for (QModelIndex &index : selectedIndex) {
            copyList.append(filePath(index));
        }
    }

    FSUtils::copy(copyList, destinationDir);

    selectedIndex.clear();
    emit dataChanged(QModelIndex(), QModelIndex());
}

void FSModel::renameFiles(QModelIndex source, QString destinationDir)
{
    QStringList copyList;

    if (selectedIndex.isEmpty()) {
        copyList.append(filePath(source));
    } else {
        QStringList tmpList;
        for (QModelIndex &index : selectedIndex) {
            copyList.append(filePath(index));
        }
    }

    FSUtils::move(copyList, destinationDir);

    selectedIndex.clear();
    emit dataChanged(QModelIndex(), QModelIndex());
}

void FSModel::mkDir(QModelIndex parent, QString dirName)
{
    if (!mkdir(parent, dirName).isValid()) {
        QMessageBox(QMessageBox::Warning,
                    "File Manager",
                    "Каталог с таким именем уже существует!",
                    QMessageBox::Ok)
            .exec();
    }
}
