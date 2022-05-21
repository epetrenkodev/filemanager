#include "mainwindow.h"
#include "fileutils.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    source = ui->leftPanel;
    target = ui->rightPanel;

    keyF3 = new QShortcut(Qt::Key_F3, this, SLOT(view()));
    keyF4 = new QShortcut(Qt::Key_F4, this, SLOT(edit()));
    keyF5 = new QShortcut(Qt::Key_F5, this, SLOT(copy()));
    keyF6 = new QShortcut(Qt::Key_F6, this, SLOT(rename()));
    keyF7 = new QShortcut(Qt::Key_F7, this, SLOT(mkdir()));
    keyF8 = new QShortcut(Qt::Key_F8, this, SLOT(remove()));
    keyDel = new QShortcut(Qt::Key_Delete, this, SLOT(remove()));

    connect(ui->leftPanel, SIGNAL(setSource()), SLOT(setSource()));
    connect(ui->rightPanel, SIGNAL(setSource()), SLOT(setSource()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::copyDir(const QDir srcDir, const QDir dstDir)
{
    QDir formDir_ = srcDir;
    QDir toDir_ = dstDir;

    if (!toDir_.exists()) {
        if (!toDir_.mkdir(dstDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = formDir_.entryInfoList();
    foreach (QFileInfo fileInfo, fileInfoList) {
        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        // Копировать подкаталоги
        if (fileInfo.isDir()) {
            // Рекурсивно вызывать копию
            if (!copyDir(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName())))
                return false;
        }
        // Копировать подфайл
        else {
            if (toDir_.exists(fileInfo.fileName())) {
                toDir_.remove(fileInfo.fileName());
            }
            if (!QFile::copy(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName()))) {
                return false;
            }
        }
    }
    return true;
}

void MainWindow::view() {}

void MainWindow::edit() {}

void MainWindow::copy()
{
    QFileInfo sourceFile = source->selectedFileInfo();
    QFileInfo targetFile = target->currentDirInfo();
    if (sourceFile.fileName() != ".." && targetFile.path() != "") {
        if (sourceFile.isDir()) {
            copyDir(QDir(sourceFile.absoluteFilePath()),
                    QDir(targetFile.absoluteFilePath() + '/' + sourceFile.fileName()));
        } else {
            QFile::copy(sourceFile.absoluteFilePath(),
                        targetFile.absoluteFilePath() + '/' + sourceFile.fileName());
        }
    }
}

void MainWindow::rename()
{
    QFileInfo sourceFile = source->selectedFileInfo();
    QFileInfo targetFile = target->currentDirInfo();
    if (sourceFile.fileName() != ".." && targetFile.path() != "") {
        if (sourceFile.isDir()) {
            copyDir(QDir(sourceFile.absoluteFilePath()),
                    QDir(targetFile.absoluteFilePath() + '/' + sourceFile.fileName()));
            removeDir(QDir(sourceFile.absoluteFilePath()));
        } else {
            QFile::copy(sourceFile.absoluteFilePath(),
                        targetFile.absoluteFilePath() + '/' + sourceFile.fileName());
            QFile::moveToTrash(sourceFile.absoluteFilePath());
        }
    }
}

void MainWindow::mkdir() {}

void MainWindow::remove()
{
    QFileInfo fileInfo = source->selectedFileInfo();
    QString name = fileInfo.fileName();
    QString fullName = fileInfo.absoluteFilePath();
    FileUtils f;
    f.remove(QStringList() << fullName);

    //    QFileInfo fileInfo = source->selectedFileInfo();
    //    QString name = fileInfo.fileName();
    //    QString fullName = fileInfo.absoluteFilePath();
    //    if (name != ".." && name != "") {
    //        if (fileInfo.isDir()) {
    //            QMessageBox msgBox(QMessageBox::Information,
    //                               "File Manager",
    //                               "Удалить папку " + fullName + " со всем содержимым?",
    //                               QMessageBox::Ok | QMessageBox::Cancel);
    //            if (msgBox.exec() == QMessageBox::Ok) {
    //                QDir dir(fullName);
    //                removeDir(dir);
    //            }
    //        } else {
    //            QMessageBox msgBox(QMessageBox::Information,
    //                               "File Manager",
    //                               "Удалить " + fullName + "?",
    //                               QMessageBox::Ok | QMessageBox::Cancel);
    //            if (msgBox.exec() == QMessageBox::Ok) {
    //                QFile::moveToTrash(fullName);
    //            }
    //        }
    //    }
}

void MainWindow::setSource()
{
    if (source != sender()) {
        target = source;
        source = static_cast<PanelView *>(sender());
    }
}

void MainWindow::selectFile(QModelIndex) {}

bool MainWindow::removeDir(QDir dir)
{
    QStringList dirs = dir.entryList(QDir::Dirs | QDir::AllDirs | QDir::NoDotAndDotDot);
    QStringList files = dir.entryList(QDir::Files);

    foreach (QString entry, files) {
        QString entryAbsPath = dir.absolutePath() + "/" + entry;
        QFile::remove(entryAbsPath);
    }

    foreach (QString entry, dirs) {
        QString entryAbsPath = dir.absolutePath() + "/" + entry;
        QDir dr(entryAbsPath);
        removeDir(dr);
    }

    return QDir().rmdir(dir.absolutePath());
}
