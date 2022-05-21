#include "panelview.h"
#include "ui_panelview.h"

#include <QMainWindow>

PanelView::PanelView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PanelView)
{
    ui->setupUi(this);

    initModel();
    initView();
    initConnect();
}

PanelView::~PanelView()
{
    delete ui;
}

void PanelView::initModel()
{
    model = new FSModel(this);
    model->setRootPath(QDir::rootPath());
    model->setFilter(QDir::AllEntries | QDir::NoDot);
}

void PanelView::initView()
{
    ui->tableView->setModel(model);

    ui->tableView->setShowGrid(false);
    ui->tableView->setIconSize(QSize(14, 14));

    ui->tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableView->horizontalHeader()->hideSection(2);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->verticalHeader()->setMinimumSectionSize(16);
    ui->tableView->verticalHeader()->setDefaultSectionSize(16);
}

void PanelView::initConnect()
{
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), SLOT(action(QModelIndex)));
    connect(ui->tableView, SIGNAL(activated(QModelIndex)), SLOT(action(QModelIndex)));
    connect(ui->tableView, SIGNAL(selectFile(QModelIndex)), SLOT(select(QModelIndex)));

    connect(ui->rootButton, SIGNAL(clicked()), ui->tableView, SLOT(reset()));
    connect(ui->homeButton, SIGNAL(clicked()), SLOT(on_homeClicked()));

    connect(model, SIGNAL(directoryLoaded(QString)), this, SLOT(loaded()));
}

void PanelView::changeDir(const QModelIndex &index)
{
    QFileInfo fileinfo(model->fileInfo(index));
    if (fileinfo.isDir()) {
        if (fileinfo.fileName() == "..") {
            QDir dir = fileinfo.dir();
            dir.cdUp();
            QModelIndex parent = model->index(dir.absolutePath());
            ui->tableView->setRootIndex(parent);
            ui->tableView->selectRow(0);
        } else {
            ui->tableView->setRootIndex(index);
            ui->tableView->selectRow(0);
        }
        ui->lineEdit->setText(model->filePath(ui->tableView->rootIndex()));
    }
}

void PanelView::focusInEvent(QFocusEvent *focusEvent)
{
    Q_UNUSED(focusEvent);
    emit setSource();
    ui->tableView->setFocus();
}

QFileInfo PanelView::selectedFileInfo()
{
    return model->fileInfo(ui->tableView->currentIndex());
}

QFileInfo PanelView::currentDirInfo()
{
    return model->fileInfo(ui->tableView->currentIndex().parent());
}

void PanelView::action(const QModelIndex &index)
{
    QFileInfo fileinfo = model->fileInfo(index);
    if (fileinfo.isDir()) {
        changeDir(index);
    }
}

void PanelView::on_homeClicked()
{
    ui->tableView->setRootIndex(model->index(QDir::homePath()));
}

void PanelView::loaded()
{
    ui->tableView->selectRow(0);
}

void PanelView::select(QModelIndex index)
{
    model->toggleIndex(index);
    ui->tableView->selectRow(index.row());
}
