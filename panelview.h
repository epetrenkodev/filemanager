#ifndef PANELVIEW_H
#define PANELVIEW_H

#include "selectdelegate.h"
#include <QFileSystemModel>
#include <QWidget>

namespace Ui {
class PanelView;
}

class PanelView : public QWidget
{
    Q_OBJECT

public:
    QFileSystemModel *model = nullptr;
    int prevDirRow = 0;

    explicit PanelView(QWidget *parent = nullptr);
    ~PanelView();

    QModelIndex selectIndex();
    QString currentDir();
    QString cuttentFile();
    QStringList selectedFiles() const;

    void selectedListClear();

    virtual bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::PanelView *ui;
    QPoint m_dragStart;
    QStringList selectedList;
    SelectDelegate *delegate;

    void initModel();
    void initView();
    void initConnect();
    void changeCurrentPath(const QModelIndex &index);
    virtual void focusInEvent(QFocusEvent *focusEvent) override;

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;

signals:
    void setSource();
    void dragCopy();

private slots:
    void action(const QModelIndex &index);
    void on_homeClicked();
    void dirLoaded();
    void selectFile(QModelIndex);

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // PANELVIEW_H
