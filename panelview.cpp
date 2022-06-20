#include "panelview.h"
#include "mainwindow.h"
#include "propertiesdialog.h"
#include "ui_panelview.h"

#include <QClipboard>
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

    setAcceptDrops(true);

    initModel();
    initView();
    action = new Action(this);
    initMenu();
    initConnect();
}

PanelView::~PanelView()
{
    delete ui;
}

QString PanelView::currentDir()
{
    return ui->currentPath->text();
}

QString PanelView::currentFile()
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
    model = new QFileSystemModel(this);
    proxy = new SortProxy(this);
    delegate = new SelectDelegate(selectedList, this);

    model->setRootPath(QDir::rootPath());
    model->setFilter(QDir::AllEntries | QDir::NoDot);
    proxy->setSourceModel(model);
}

void PanelView::initView()
{
    ui->tableView->setModel(proxy);
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
    ui->tableView->sortByColumn(2, Qt::SortOrder::AscendingOrder);
}

void PanelView::initMenu()
{
    menu = new QMenu(this);
    menu->addAction(action->copy);
    menu->addAction(action->properties);
}

void PanelView::initConnect()
{
    connect(ui->tableView, &QAbstractItemView::activated, this, &PanelView::activated);
    connect(ui->rootButton, &QAbstractButton::clicked, ui->tableView, &QAbstractItemView::reset);
    connect(ui->homeButton, &QAbstractButton::clicked, this, &PanelView::on_homeClicked);
    connect(model, &QFileSystemModel::directoryLoaded, this, &PanelView::dirLoaded);
    connect(action->copy, &QAction::triggered, this, &PanelView::copyToClipboard);
    connect(action->properties, &QAction::triggered, this, &PanelView::properties);
}

void PanelView::changeCurrentPath(const QModelIndex &index)
{
    QString dirName = index.data(QFileSystemModel::FileNameRole).toString();
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

    ui->currentPath->setText(
        ui->tableView->rootIndex().data(QFileSystemModel::FilePathRole).toString());

    selectedList.clear();
}

void PanelView::focusInEvent(QFocusEvent *focusEvent)
{
    Q_UNUSED(focusEvent);
    emit setSource();
    ui->tableView->setFocus();
}

void PanelView::activated(const QModelIndex &index)
{
    QFileInfo fileinfo = QFileInfo(index.data(QFileSystemModel::FilePathRole).toString());
    if (fileinfo.isDir()) {
        changeCurrentPath(index);
    } else if (fileinfo.isFile()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileinfo.filePath()));
    }
}

void PanelView::on_homeClicked()
{
    ui->tableView->setRootIndex(proxy->mapFromSource(model->index(QDir::homePath())));
    ui->currentPath->setText(
        ui->tableView->rootIndex().data(QFileSystemModel::FilePathRole).toString());
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

void PanelView::copyToClipboard()
{
    qDebug() << "copy clipboard";
    QClipboard *clipboard = QApplication::clipboard();
    QMimeData *mimeData = new QMimeData();
    QList<QUrl> list;
    for (QString &file : selectedList) {
        list.append(QUrl::fromLocalFile(currentDir() + QDir::separator() + file));
    }
    mimeData->setUrls(list);
    clipboard->setMimeData(mimeData);
}

void PanelView::properties()
{
    PropertiesDialog dialog(currentDir() + QDir::separator() + currentFile());
    dialog.exec();
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
                return false;
                break;
            case Qt::MouseButton::RightButton:
                selectFile(ui->tableView->indexAt(mouseEvent->pos()));
                contextMenuPressTime = QTime::currentTime();
                return true;
                break;
            default:
                return false;
                break;
            }
        } break;
        case QEvent::MouseButtonRelease: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::MouseButton::RightButton
                && contextMenuPressTime.msecsTo(QTime::currentTime()) > 500) {
                menu->exec(mouseEvent->globalPos());
            }
        } break;
        case QEvent::MouseMove: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if ((mouseEvent->buttons() & Qt::LeftButton)
                && QApplication::startDragDistance()
                       <= (mouseEvent->pos() - m_dragStart).manhattanLength()) {
                QDrag *drag = new QDrag(this);
                QMimeData *mimeData = new QMimeData;
                QList<QUrl> list;
                for (QString &file : selectedList) {
                    list.append(QUrl::fromLocalFile(currentDir() + QDir::separator() + file));
                }
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
        int countRow = proxy->rowCount(ui->tableView->currentIndex().parent());
        if (currentRow < countRow) {
            ui->tableView->selectRow(currentRow + 1);
        }
        ui->tableView->viewport()->repaint();
    }
}
