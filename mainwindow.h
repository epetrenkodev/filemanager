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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QShortcut *keyF3;
    QShortcut *keyF4;
    QShortcut *keyF5;
    QShortcut *keyF6;
    QShortcut *keyF7;
    QShortcut *keyF8;
    QShortcut *keyDel;

    PanelView *source;
    PanelView *target;

    bool copyDir(const QDir srcDir, const QDir dstDir);
    bool removeDir(QDir dir);

private slots:
    void view();
    void edit();
    void copy();
    void rename();
    void mkdir();
    void remove();
    void setSource();
};
#endif // MAINWINDOW_H
