#include "stripdialog.h"
#include "ui_stripdialog.h"

StripDialog::StripDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StripDialog)
{
    ui->setupUi(this);

}

StripDialog::StripDialog(const QString &text, QKeySequence keys, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StripDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setText(text);
    ui->keySequenceEdit->setKeySequence(keys);

}

StripDialog::stripData StripDialog::dialogValues()
{
    return {ui->keySequenceEdit->keySequence(), ui->lineEdit->text()};
}

QKeySequence StripDialog::keys()
{
    return ui->keySequenceEdit->keySequence();
}

QString StripDialog::content()
{
    return ui->lineEdit->text();
}

StripDialog::~StripDialog()
{
    delete ui;
}
