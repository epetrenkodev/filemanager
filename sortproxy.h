#ifndef SORTPROXY_H
#define SORTPROXY_H

#include <QSortFilterProxyModel>

class SortProxy : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit SortProxy(QObject *parent = nullptr);

    // QSortFilterProxyModel interface
protected:
    virtual bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};

#endif // SORTPROXY_H
