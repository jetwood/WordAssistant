#include "fullscreenwidget.h"
#include "ui_fullscreenwidget.h"

FullScreenWidget::FullScreenWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullScreenWidget)
{
    ui->setupUi(this);
}

FullScreenWidget::~FullScreenWidget()
{
    delete ui;
}
