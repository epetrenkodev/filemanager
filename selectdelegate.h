#ifndef SELECTDELEGATE_H
#define SELECTDELEGATE_H

#include <QItemDelegate>

class SelectDelegate : public QItemDelegate
{
public:
    explicit SelectDelegate(const QStringList &selectedList, QObject *parent = nullptr);
    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const override;

private:
    const QStringList &selectedList;
};

#endif // SELECTDELEGATE_H
