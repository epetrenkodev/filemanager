#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "panelview.h"
#include <QMainWindow>
#include <QShortcut>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QShortcut *keyF3;
    QShortcut *keyF5;
    QShortcut *keyF6;
    QShortcut *keyF7;
    QShortcut *keyF8;
    QShortcut *keyDel;

    PanelView *source;
    PanelView *target;

    void intiShortcut();
    void initConnect();
    QStringList createFullPath(QString path, QStringList files, QString singleFile) const;
    int ask(QString icon, QString msg) const;

private slots:
    void view();
    void copy();
    void rename();
    void mkdir();
    void remove();
    void setSourcePanel();
};
#endif // MAINWINDOW_H
