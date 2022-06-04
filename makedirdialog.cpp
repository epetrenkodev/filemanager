#include "makedirdialog.h"
#include "ui_makedirdialog.h"

MakeDirDialog::MakeDirDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MakeDirDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();
}

MakeDirDialog::~MakeDirDialog()
{
    delete ui;
}

QString MakeDirDialog::dirName()
{
    return ui->lineEdit->text();
}
