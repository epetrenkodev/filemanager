#ifndef FSUTILS_H
#define FSUTILS_H

#include <QFileInfoList>
#include <QObject>

class FSUtils : public QObject
{
    Q_OBJECT
public:
    explicit FSUtils(QObject *parent = nullptr);

    static QFileInfoList getList(QStringList paths);
    static bool copy(QStringList paths, QString dest);
    static bool move(QStringList paths, QString dest);

private:
    static void warning(QString message);

signals:
};

#endif // FSUTILS_H
