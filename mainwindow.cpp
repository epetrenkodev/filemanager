#include "mainwindow.h"
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

void MainWindow::copy()
{
    source->model->copyFiles(source->selectIndex(), target->currentPath());
}

void MainWindow::rename()
{
    source->model->renameFiles(source->selectIndex(), target->currentPath());
}

void MainWindow::mkdir()
{
    MakeDirDialog dialog;
    if (dialog.exec() == QDialog::Accepted)
        source->model->mkDir(source->selectIndex().parent(), dialog.dirName());
}

void MainWindow::remove()
{
    QMessageBox msgBox(QMessageBox::Information,
                       "File Manager",
                       "Удалить выбранные файлы/каталоги?",
                       QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setWindowIcon(QIcon(":/icons/app.png"));
    if (msgBox.exec() == QMessageBox::Ok)
        source->model->removeFiles(source->selectIndex());
}

void MainWindow::setSourcePanel()
{
    if (source != sender()) {
        target = source;
        source = static_cast<PanelView *>(sender());
    }
}
