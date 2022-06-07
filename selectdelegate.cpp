#include "selectdelegate.h"

SelectDelegate::SelectDelegate(const QStringList &selectedList, QObject *parent)
    : QItemDelegate{parent}
    , selectedList(selectedList)
{}

void SelectDelegate::paint(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    QStyleOptionViewItem new_option = option;
    if (selectedList.contains(index.data().toString())) {
        QPalette palette;
        palette.setColor(QPalette::HighlightedText, QColor(Qt::red));
        palette.setColor(QPalette::Text, QColor(Qt::red));
        new_option.palette = palette;
    }
    QItemDelegate::paint(painter, new_option, index);
}
