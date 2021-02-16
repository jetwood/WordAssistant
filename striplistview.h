#ifndef STRIPLISTVIEW_H
#define STRIPLISTVIEW_H

#include <QListWidget>
#include <QSettings>
#include <QMouseEvent>
#include <QInputDialog>
#include <QDebug>

#include "stripdialog.h"

namespace Ui {
class StripListView;
}

class StripListView : public QListWidget
{
    Q_OBJECT

public:
    explicit StripListView(QWidget *parent = nullptr);
    void readSettings();
    void writeSettings();
    ~StripListView();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

signals:
    void doubleClickTrigger(QString text);

public slots:
    void addNewStrip();
    void deleteCurrentStrip();

private:
    Ui::StripListView *ui;
};

#endif // STRIPLISTVIEW_H
