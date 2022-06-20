#include "propertiesdialog.h"
#include "ui_propertiesdialog.h"

#include <QDateTime>
#include <QDebug>
#include <QFileInfo>

extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;

PropertiesDialog::PropertiesDialog(QString file, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PropertiesDialog)
{
    ui->setupUi(this);

    qt_ntfs_permission_lookup++;

    QFileInfo fileInfo(file);
    setWindowTitle("Свойства: " + fileInfo.fileName());

    ui->type->setText(fileInfo.isDir() ? "Папка" : "Файл");

    ui->location->setText(fileInfo.path());
    ui->size->setText(QString::number(fileInfo.size()));

    ui->ctime->setText(fileInfo.birthTime().toString());
    ui->mtime->setText(fileInfo.lastModified().toString());
    ui->atime->setText(fileInfo.lastRead().toString());

    ui->owner->setText(fileInfo.owner());
    ui->group->setText(fileInfo.group());

    QString perm("%1%2%3 %4%5%6 %7%8%9");
    ui->attr->setText(perm.arg(fileInfo.permission(QFile::ReadOwner) ? "r" : "-")
                          .arg(fileInfo.permission(QFile::WriteOwner) ? "w" : "-")
                          .arg(fileInfo.permission(QFile::ExeOwner) ? "x" : "-")

                          .arg(fileInfo.permission(QFile::ReadGroup) ? "r" : "-")
                          .arg(fileInfo.permission(QFile::WriteGroup) ? "w" : "-")
                          .arg(fileInfo.permission(QFile::ExeGroup) ? "x" : "-")

                          .arg(fileInfo.permission(QFile::ReadOther) ? "r" : "-")
                          .arg(fileInfo.permission(QFile::WriteOther) ? "w" : "-")
                          .arg(fileInfo.permission(QFile::ExeOther) ? "x" : "-"));
}

PropertiesDialog::~PropertiesDialog()
{
    delete ui;
}
