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

    void initModel();
    void initView();
    void initConnect();
    void changeCurrentPath(const QModelIndex &index);

    virtual void focusInEvent(QFocusEvent *focusEvent) override;

signals:
    void setSource();

private slots:
    void action(const QModelIndex &index);
    void on_homeClicked();
    void dirLoaded();
    void selectFile(QModelIndex);
};

#endif // PANELVIEW_H
