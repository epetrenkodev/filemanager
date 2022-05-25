#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>

class TableView : public QTableView
{
    Q_OBJECT

public:
    int prevDirRow = 0;
    TableView(QWidget *parent = nullptr);

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;

signals:
    void selectFile(QModelIndex);
};

#endif // TABLEVIEW_H
