#include "action.h"

Action::Action(QObject *parent)
    : QObject{parent}
{
    copy = new QAction("Копировать", this);
    copy->setAutoRepeat(false);
    copy->setIcon(QIcon(":/copy.ico"));
    copy->setShortcut(QKeySequence::Copy);

    properties = new QAction("Свойства", this);
    properties->setAutoRepeat(false);
    properties->setIcon(QIcon(":/properties.ico"));
    //properties->setShortcut(QKeySequence::);
}
