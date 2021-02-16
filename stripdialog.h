#ifndef STRIPDIALOG_H
#define STRIPDIALOG_H

#include <QDialog>

namespace Ui {
class StripDialog;
}

class StripDialog : public QDialog
{
    Q_OBJECT

public:
    typedef struct{
        QKeySequence keySequence;
        QString content;
    } stripData;
    explicit StripDialog(QWidget *parent = nullptr);
    StripDialog(const QString &text, QKeySequence keys, QWidget *parent=nullptr);
    stripData dialogValues();
    QKeySequence keys();
    QString content();


    ~StripDialog();

private:
    Ui::StripDialog *ui;
};

#endif // STRIPDIALOG_H
