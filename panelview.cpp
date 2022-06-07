#include "panelview.h"
#include "mainwindow.h"
#include "ui_panelview.h"

#include <QDebug>
#include <QDesktopServices>
#include <QDrag>
#include <QFileSystemModel>
#include <QMimeData>
#include <QMouseEvent>
#include <QUrl>

PanelView::PanelView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PanelView)
{
    ui->setupUi(this);

    delegate = new SelectDelegate(selectedList, this);

    setAcceptDrops(true);

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

QString PanelView::currentDir()
{
    return ui->currentPath->text();
}

QString PanelView::cuttentFile()
{
    return ui->tableView->currentIndex().data().toString();
}

QStringList PanelView::selectedFiles() const
{
    return selectedList;
}

void PanelView::selectedListClear()
{
    selectedList.clear();
    ui->tableView->viewport()->repaint();
}

void PanelView::initModel()
{
    model = new QFileSystemModel;
    model->setRootPath(QDir::rootPath());
    model->setFilter(QDir::AllEntries | QDir::NoDot);
}

void PanelView::initView()
{
    ui->tableView->setModel(model);
    ui->tableView->setItemDelegate(delegate);

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

    ui->tableView->viewport()->installEventFilter(this);
}

void PanelView::initConnect()
{
    connect(ui->tableView, SIGNAL(activated(QModelIndex)), SLOT(action(QModelIndex)));

    connect(ui->rootButton, SIGNAL(clicked()), ui->tableView, SLOT(reset()));
    connect(ui->homeButton, SIGNAL(clicked()), SLOT(on_homeClicked()));

    connect(model, SIGNAL(directoryLoaded(QString)), this, SLOT(dirLoaded()));
}

void PanelView::changeCurrentPath(const QModelIndex &index)
{
    QString dirName = model->fileInfo(index).fileName();
    QModelIndex newIndex;
    if (dirName == "..") {
        newIndex = index.parent().parent();
        prevDirRow = index.parent().row();
    } else {
        newIndex = index;
        prevDirRow = 0;
    }

    ui->tableView->setRootIndex(newIndex);
    ui->tableView->selectRow(prevDirRow);

    ui->currentPath->setText(model->filePath(ui->tableView->rootIndex()));

    selectedList.clear();
}

void PanelView::focusInEvent(QFocusEvent *focusEvent)
{
    Q_UNUSED(focusEvent);
    emit setSource();
    ui->tableView->setFocus();
}

void PanelView::action(const QModelIndex &index)
{
    QFileInfo fileinfo = model->fileInfo(index);
    if (fileinfo.isDir()) {
        changeCurrentPath(index);
    } else if (fileinfo.isFile()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileinfo.filePath()));
    }
}

void PanelView::on_homeClicked()
{
    ui->tableView->setRootIndex(model->index(QDir::homePath()));
    ui->currentPath->setText(model->filePath(ui->tableView->rootIndex()));
}

void PanelView::dirLoaded()
{
    ui->tableView->selectRow(prevDirRow);
}

void PanelView::selectFile(QModelIndex index)
{
    QString fileName = index.data().toString();
    if (fileName != "..") {
        if (selectedList.contains(fileName))
            selectedList.removeAll(fileName);
        else
            selectedList.append(fileName);
        ui->tableView->selectRow(index.row());
        ui->tableView->viewport()->repaint();
    }
}

bool PanelView::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->tableView->viewport()) {
        switch (event->type()) {
        case QEvent::MouseButtonPress: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            switch (mouseEvent->button()) {
            case Qt::MouseButton::LeftButton:
                m_dragStart = mouseEvent->pos();
                break;
            case Qt::MouseButton::RightButton:
                selectFile(ui->tableView->indexAt(mouseEvent->pos()));
                return true;
                break;
            default:
                break;
            }
        } break;
        case QEvent::MouseMove: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if ((mouseEvent->buttons() & Qt::LeftButton)
                && QApplication::startDragDistance()
                       <= (mouseEvent->pos() - m_dragStart).manhattanLength()) {
                QDrag *drag = new QDrag(this);
                QMimeData *mimeData = new QMimeData;
                // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                QList<QUrl> list;
                list.append(QUrl::fromLocalFile(model->fileInfo(selectIndex()).filePath()));
                // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                mimeData->setUrls(list);
                drag->setMimeData(mimeData);
                drag->exec(Qt::MoveAction);
            }
        } break;
        default:
            break;
        }
    }
    return false;
}

void PanelView::dragEnterEvent(QDragEnterEvent *event)
{
    QStringList formats = event->mimeData()->formats();
    if (formats.contains("text/uri-list")) {
        event->acceptProposedAction();
    }
}

void PanelView::dropEvent(QDropEvent *event)
{
    emit dragCopy();
    event->acceptProposedAction();
}

void PanelView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Insert) {
        selectFile(ui->tableView->currentIndex());
        int currentRow = ui->tableView->currentIndex().row();
        int countRow = model->rowCount(ui->tableView->currentIndex().parent());
        if (currentRow < countRow) {
            ui->tableView->selectRow(currentRow + 1);
        }
        ui->tableView->viewport()->repaint();
    }
}
