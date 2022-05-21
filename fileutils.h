#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QFileInfo>
#include <QObject>
#include <QProgressDialog>

class FileUtils : public QObject
{
    Q_OBJECT
public:
    explicit FileUtils(QObject *parent = nullptr);

    void remove(QStringList files);

private:
    qint64 total = 0;
    QList<QFileInfo> entryList;
    QProgressDialog *progress;

    void getList(QStringList files);
};

#endif // FILEUTILS_H
