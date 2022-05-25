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

QModelIndex PanelView::selectIndex()
{
    return ui->tableView->currentIndex();
}

QString PanelView::currentPath()
{
    return ui->currentPath->text();
}

// Init
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
    connect(ui->tableView, SIGNAL(selectFile(QModelIndex)), SLOT(selectFile(QModelIndex)));

    connect(ui->rootButton, SIGNAL(clicked()), ui->tableView, SLOT(reset()));
    connect(ui->homeButton, SIGNAL(clicked()), SLOT(on_homeClicked()));

    connect(model, SIGNAL(directoryLoaded(QString)), this, SLOT(dirLoaded()));
}

// Private function
void PanelView::changeCurrentPath(const QModelIndex &index)
{
    QString dirName = model->fileInfo(index).fileName();
    QModelIndex newIndex;
    if (dirName == "..") {
        newIndex = index.parent().parent();
        ui->tableView->prevDirRow = index.parent().row();
    } else {
        newIndex = index;
        ui->tableView->prevDirRow = 0;
    }

    ui->tableView->setRootIndex(newIndex);
    ui->tableView->selectRow(ui->tableView->prevDirRow);

    ui->currentPath->setText(model->filePath(ui->tableView->rootIndex()));

    model->clearSelect();
}

// Events
void PanelView::focusInEvent(QFocusEvent *focusEvent)
{
    Q_UNUSED(focusEvent);
    emit setSource();
    ui->tableView->setFocus();
}

// Slots
void PanelView::action(const QModelIndex &index)
{
    QFileInfo fileinfo = model->fileInfo(index);
    if (fileinfo.isDir()) {
        changeCurrentPath(index);
    }
}

void PanelView::on_homeClicked()
{
    ui->tableView->setRootIndex(model->index(QDir::homePath()));
    ui->currentPath->setText(model->filePath(ui->tableView->rootIndex()));
}

void PanelView::dirLoaded()
{
    ui->tableView->selectRow(ui->tableView->prevDirRow);
}

void PanelView::selectFile(QModelIndex index)
{
    model->toggleIndex(index);
    ui->tableView->selectRow(index.row());
}
