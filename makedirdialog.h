#ifndef MAKEDIRDIALOG_H
#define MAKEDIRDIALOG_H

#include <QDialog>

namespace Ui {
class MakeDirDialog;
}

class MakeDirDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MakeDirDialog(QWidget *parent = nullptr);
    ~MakeDirDialog();

    QString dirName();

private:
    Ui::MakeDirDialog *ui;
};

#endif // MAKEDIRDIALOG_H
