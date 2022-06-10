#ifndef SORTPROXY_H
#define SORTPROXY_H

#include <QSortFilterProxyModel>

class SortProxy : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit SortProxy(QObject *parent = nullptr);
};

#endif // SORTPROXY_H
