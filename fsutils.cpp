#include "fsutils.h"

#include <QApplication>
#include <QDirIterator>
#include <QIcon>
#include <QMessageBox>
#include <QProgressDialog>

FSUtils::FSUtils(QObject *parent)
    : QObject{parent}
{}

QFileInfoList FSUtils::getList(QStringList paths)
{
    QFileInfoList list;
    for (QString &path : paths) {
        list << QFileInfo(path);
        QDirIterator di(path, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
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

bool FSUtils::copy(QStringList paths, QString dest)
{
    if (dest.isEmpty() || paths.isEmpty())
        return false;
    if (paths.contains(dest)) {
        warning("Нельзя скопровать каталог сам в себя");
        return false;
    }
    int pos = QFileInfo(paths[0]).path().size();
    QFileInfoList copyList = getList(paths);
    std::sort(copyList.begin(), copyList.end(), [](auto d1, auto d2) {
        return d1.path() < d2.path();
    });
    int count = 1;
    QProgressDialog progress("", "Отмена", 0, copyList.count());
    progress.setFixedWidth(300);
    progress.setWindowTitle("Копирование");
    progress.setWindowIcon(QIcon(":/icons/copy.ico"));
    for (QFileInfo &entry : copyList) {
        QString sName = entry.filePath();
        QString dName = dest + QDir::separator() + sName.mid(pos);

        progress.setLabelText(sName + " -> " + dName);
        progress.setValue(count++);
        qApp->processEvents();
        if (progress.wasCanceled())
            break;

        if (entry.isDir()) {
            QDir().mkdir(dName);
        } else {
            QFile::copy(sName, dName);
        }
    }
    return true;
}

bool FSUtils::move(QStringList paths, QString dest)
{
    if (dest.isEmpty() || paths.isEmpty())
        return false;
    if (paths.contains(dest)) {
        warning("Нельзя переместить каталог сам в себя");
        return false;
    }
    int pos = QFileInfo(paths[0]).path().size();
    QFileInfoList copyList = getList(paths);
    std::sort(copyList.begin(), copyList.end(), [](auto d1, auto d2) {
        return d1.path() > d2.path();
    });
    int count = 1;
    QProgressDialog progress("", "Отмена", 0, copyList.count());
    progress.setFixedWidth(300);
    progress.setWindowTitle("Перемещение");
    progress.setWindowIcon(QIcon(":/icons/move.ico"));
    for (QFileInfo &entry : copyList) {
        QString sName = entry.filePath();
        QString dName = dest + QDir::separator() + sName.mid(pos);

        progress.setLabelText(sName + " -> " + dName);
        progress.setValue(count++);
        qApp->processEvents();
        if (progress.wasCanceled())
            break;

        if (entry.isDir()) {
            QDir().mkdir(dName);
            QDir().rmdir(sName);
        } else {
            QFile::rename(sName, dName);
        }
    }
    return true;
}

void FSUtils::warning(QString message)
{
    QMessageBox(QMessageBox::Warning, "File Manager", message, QMessageBox::Ok).exec();
}
