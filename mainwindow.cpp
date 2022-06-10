#include "mainwindow.h"
#include "fsutils.h"
#include "makedirdialog.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    source = ui->leftPanel;
    target = ui->rightPanel;

    intiShortcut();
    initConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::intiShortcut()
{
    //keyF3 = new QShortcut(Qt::Key_F3, this, SLOT(view()));
    //keyF4 = new QShortcut(Qt::Key_F4, this, SLOT(edit()));
    keyF5 = new QShortcut(Qt::Key_F5, this, SLOT(copy()));
    keyF6 = new QShortcut(Qt::Key_F6, this, SLOT(rename()));
    keyF7 = new QShortcut(Qt::Key_F7, this, SLOT(mkdir()));
    keyF8 = new QShortcut(Qt::Key_F8, this, SLOT(remove()));
    keyDel = new QShortcut(Qt::Key_Delete, this, SLOT(remove()));
}

void MainWindow::initConnect()
{
    connect(ui->leftPanel, SIGNAL(setSource()), SLOT(setSourcePanel()));
    connect(ui->rightPanel, SIGNAL(setSource()), SLOT(setSourcePanel()));

    connect(ui->leftPanel, SIGNAL(dragCopy()), SLOT(copy()));
    connect(ui->rightPanel, SIGNAL(dragCopy()), SLOT(copy()));
}

QStringList MainWindow::createFullPath(QString path, QStringList files, QString singleFile) const
{
    QStringList list;
    if (source->selectedFiles().isEmpty()) {
        list.append(path + QDir::separator() + singleFile);
    } else {
        for (QString &file : files) {
            list.append(path + QDir::separator() + file);
        }
    }
    return list;
}

int MainWindow::ask(QString icon, QString msg) const
{
    QMessageBox msgBox(QMessageBox::Question,
                       "File Manager",
                       msg,
                       QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setWindowIcon(QIcon(icon));
    return msgBox.exec();
}

void MainWindow::copy()
{
    FSUtils::copy(createFullPath(source->currentDir(),
                                 source->selectedFiles(),
                                 source->cuttentFile()),
                  target->currentDir());
    source->selectedListClear();
}

void MainWindow::rename()
{
    FSUtils::move(createFullPath(source->currentDir(),
                                 source->selectedFiles(),
                                 source->cuttentFile()),
                  target->currentDir());
    source->selectedListClear();
}

void MainWindow::mkdir()
{
    QString path = source->currentDir();
    if (path.isEmpty())
        return;
    MakeDirDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        QString dirName = dialog.dirName();
        if (dirName.isEmpty())
            return;
        FSUtils::mkdir(path + QDir::separator() + dirName);
    }
}

void MainWindow::remove()
{
    if (ask(":/icons/app.png", "Удалить выбранные файлы/каталоги?") == QMessageBox::Ok) {
        FSUtils::remove(
            createFullPath(source->currentDir(), source->selectedFiles(), source->cuttentFile()));
        source->selectedListClear();
    }
}

void MainWindow::setSourcePanel()
{
    if (source != sender()) {
        target = source;
        source = static_cast<PanelView *>(sender());
    }
}
