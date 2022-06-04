#include "fsmodel.h"

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
    if (destinationDir.isEmpty())
        return;

    QFileInfoList list;
    if (selectedIndex.isEmpty()) {
        list = getList(QStringList(filePath(source)));
    } else {
        QStringList tmpList;
        for (QModelIndex index : selectedIndex) {
            tmpList << fileInfo(index).filePath();
        }
        list = getList(tmpList);
    }

    std::sort(list.begin(), list.end(), [](auto d1, auto d2) { return d1.path() < d2.path(); });

    int pos = list[0].path().size();
    int count = 1;
    QProgressDialog progress;
    progress.setMinimumDuration(0);
    progress.setCancelButtonText("Отмена");
    progress.setFixedWidth(300);
    progress.setWindowTitle("Копирование");
    progress.setWindowIcon(QIcon(":/icons/copy.ico"));
    progress.setMaximum(list.count());

    for (auto &f : list) {
        progress.setLabelText(f.filePath() + " -> " + destinationDir + f.filePath().mid(pos));
        progress.setValue(count++);
        qApp->processEvents();
        if (progress.wasCanceled())
            break;
        if (f.fileName() != "..") {
            if (f.isDir()) {
                QDir().mkdir(destinationDir + '/' + f.filePath().mid(pos));
            } else {
                QFile::copy(f.filePath(), destinationDir + '/' + f.filePath().mid(pos));
            }
        }
    }
    selectedIndex.clear();
    emit dataChanged(QModelIndex(), QModelIndex());
}

void FSModel::renameFiles(QModelIndex source, QString destinationDir)
{
    if (destinationDir.isEmpty())
        return;

    QFileInfoList list;
    if (selectedIndex.isEmpty()) {
        list = getList(QStringList(filePath(source)));
    } else {
        QStringList tmpList;
        for (QModelIndex index : selectedIndex) {
            tmpList << fileInfo(index).filePath();
        }
        list = getList(tmpList);
    }

    std::sort(list.begin(), list.end(), [](auto d1, auto d2) { return d1.path() > d2.path(); });

    int pos = list[0].path().size();
    int count = 1;
    QProgressDialog progress;
    progress.setMinimumDuration(0);
    progress.setCancelButtonText("Отмена");
    progress.setFixedWidth(300);
    progress.setWindowTitle("Перемещение");
    progress.setWindowIcon(QIcon(":/icons/move.ico"));
    progress.setMaximum(list.count());

    for (auto &f : list) {
        progress.setLabelText(f.filePath() + " -> " + destinationDir + f.filePath().mid(pos));
        progress.setValue(count++);
        qApp->processEvents();
        if (progress.wasCanceled())
            break;
        if (f.fileName() != "..") {
            if (f.isDir()) {
                QDir().mkdir(destinationDir + f.filePath().mid(pos));
                QDir().rmdir(f.filePath());
            } else {
                QFile::rename(f.filePath(), destinationDir + f.filePath().mid(pos));
            }
        }
    }
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

QFileInfoList FSModel::getList(QStringList files)
{
    QFileInfoList list;
    for (QString &entry : files) {
        list << QFileInfo(entry);
        QDirIterator di(entry, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
        while (di.hasNext()) {
            di.next();
            if (di.fileName() != "." && di.fileName() != "..") {
                list << di.fileInfo();
            }
            qApp->processEvents();
        }
    }
    return list;
}
