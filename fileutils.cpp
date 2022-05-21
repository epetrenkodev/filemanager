#include "fileutils.h"

#include <QApplication>
#include <QDirIterator>
#include <QIcon>
#include <QProgressDialog>
#include <Qt>

FileUtils::FileUtils(QObject *parent)
    : QObject(parent)
{
    progress = new QProgressDialog;
    progress->setMinimumDuration(0);
    progress->setCancelButtonText("Отмена");
    progress->setFixedWidth(300);
}

void FileUtils::remove(QStringList files)
{
    int count = 1;
    progress->setMaximum(files.size());
    qDebug() << progress->maximum();
    progress->setWindowTitle("Удаление");
    progress->setWindowIcon(QIcon(":/icons/delete.ico"));

    for (QString &name : files) {
        progress->setLabelText(name);
        progress->setValue(count++);
        qApp->processEvents();
        qDebug() << "remove " << name << " === " << QFile::moveToTrash(name);
        if (progress->wasCanceled())
            break;
    }

    //    qint64 comp = 0;

    //    progress->setWindowTitle("Удаление");
    //    progress->setWindowIcon(QIcon(":/icons/delete.ico"));
    //    progress->setLabelText("Подготовка...");
    //    progress->show();

    //    getList(files);
    //    std::sort(entryList.begin(), entryList.end(), [](auto d1, auto d2) {
    //        return d1.path() > d2.path();
    //    });

    //    for (auto &d : entryList) {
    //        if (d.isFile())
    //            comp += d.size();
    //        progress->setLabelText(d.fileName());
    //        progress->setValue(comp * 100 / total);
    //        qApp->processEvents();
    //        if (progress->wasCanceled())
    //            break;
    //        if (d.isDir()) {
    //            qDebug() << "rmdir " << d.filePath() << " === " << QFile::moveToTrash(d.filePath());

    //        } else {
    //            qDebug() << "remove " << d.filePath() << " === " << QFile::moveToTrash(d.filePath());
    //        }
    //    }
}

void FileUtils::getList(QStringList files)
{
    for (QString &entry : files) {
        entryList << QFileInfo(entry);
        QDirIterator di(entry, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
        while (di.hasNext()) {
            di.next();
            if (di.fileName() != "." && di.fileName() != "..") {
                entryList << di.fileInfo();
                if (di.fileInfo().isFile())
                    total += di.fileInfo().size();
            }
            qApp->processEvents();
        }
    }
}
