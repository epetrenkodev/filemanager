#ifndef PANELVIEW_H
#define PANELVIEW_H

#include <QFileSystemModel>
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
    void chFocus();

private:
    Ui::PanelView *ui;
    QFileSystemModel *model = nullptr;

    void initModel();
    void initView();
    void initConnect();
    void changeDir(const QModelIndex &index);

private slots:
    void action(const QModelIndex &index);
    void on_homeClicked();
    void loaded();
};

#endif // PANELVIEW_H
