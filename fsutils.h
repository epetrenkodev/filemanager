#ifndef FSUTILS_H
#define FSUTILS_H

#include <QFileInfoList>
#include <QObject>

class FSUtils : public QObject
{
    Q_OBJECT

public:
    explicit FSUtils(QObject *parent = nullptr);

    static bool copy(QStringList paths, QString dest);
    static bool move(QStringList paths, QString dest);
    static bool mkdir(QString newDir);
    static bool remove(QStringList paths);

private:
    static void warning(QString message);
    static QFileInfoList getList(QStringList paths);
};

#endif // FSUTILS_H
