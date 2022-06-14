#ifndef PANELVIEW_H
#define PANELVIEW_H

#include "action.h"
#include "selectdelegate.h"
#include "sortproxy.h"
#include <QFileSystemModel>
#include <QMenu>
#include <QTime>
#include <QWidget>

namespace Ui {
class PanelView;
}

class PanelView : public QWidget
{
    Q_OBJECT

public:
    explicit PanelView(QWidget *parent = nullptr);
    ~PanelView();

    QString currentDir();
    QString cuttentFile();
    QStringList selectedFiles() const;

    void selectedListClear();

private:
    Ui::PanelView *ui;
    QPoint m_dragStart;
    QStringList selectedList;
    QFileSystemModel *model = nullptr;
    SortProxy *proxy;
    SelectDelegate *delegate;
    int prevDirRow = 0;
    QTime contextMenuPressTime;
    QMenu *menu;
    Action *action;

    void initModel();
    void initView();
    void initMenu();
    void initConnect();
    void changeCurrentPath(const QModelIndex &index);
    virtual void focusInEvent(QFocusEvent *focusEvent) override;

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void setSource();
    void dragCopy();

private slots:
    void activated(const QModelIndex &index);
    void on_homeClicked();
    void dirLoaded();
    void selectFile(QModelIndex);
    void copyToClipboard();
};

#endif // PANELVIEW_H
