#include "tableview.h"
#include <QApplication>
#include <QMouseEvent>

TableView::TableView(QWidget *parent)
    : QTableView(parent)
{}

void TableView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::RightButton) {
        emit selectFile(indexAt(event->pos()));
    } else {
        QTableView::mousePressEvent(event);
    }
}

void TableView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{}
