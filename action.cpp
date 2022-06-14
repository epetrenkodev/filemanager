#include "action.h"

Action::Action(QObject *parent)
    : QObject{parent}
{
    copy = new QAction("Копировать", this);
    copy->setAutoRepeat(false);
    copy->setIcon(QIcon(":/copy.ico"));
    copy->setShortcut(QKeySequence::Copy);
}
