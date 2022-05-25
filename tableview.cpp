#include "tableview.h"
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
