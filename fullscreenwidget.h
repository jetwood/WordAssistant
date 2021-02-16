#ifndef FULLSCREENWIDGET_H
#define FULLSCREENWIDGET_H

/* offical libs */
#include <QWidget>

/* developer's libs */
#include "imageviewer.h"

namespace Ui {
class FullScreenWidget;
}

class FullScreenWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FullScreenWidget(QWidget *parent = nullptr);
    void setScreen(QScreen *screen);
    void grabScreen();
    QScreen *onScreen();
    ImageViewer *imageViewer() const;
    ~FullScreenWidget();

private:

    Ui::FullScreenWidget *ui;
};

#endif // FULLSCREENWIDGET_H
