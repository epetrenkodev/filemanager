#ifndef PANELVIEW_H
#define PANELVIEW_H

#include "fsmodel.h"

#include <QWidget>

namespace Ui {
class PanelView;
}

class PanelView : public QWidget
{
    Q_OBJECT

public:
    FSModel *model = nullptr;

    explicit PanelView(QWidget *parent = nullptr);
    ~PanelView();

    QModelIndex selectIndex();
    QString currentPath();

private:
    Ui::PanelView *ui;
    QPoint m_dragStart;

    void initModel();
    void initView();
    void initConnect();
    void changeCurrentPath(const QModelIndex &index);

    virtual void focusInEvent(QFocusEvent *focusEvent) override;

signals:
    void setSource();
    void dragCopy();

private slots:
    void action(const QModelIndex &index);
    void on_homeClicked();
    void dirLoaded();
    void selectFile(QModelIndex);

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

    // QWidget interface
protected:
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;
};

#endif // PANELVIEW_H
