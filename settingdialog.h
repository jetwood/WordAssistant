#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QCloseEvent>
#include <networkmanager.h>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

signals:
    void errorMsg(const QString&err);

private slots:
    void doneEvents(int r);

private:
    Ui::SettingDialog *ui;
    NetworkManager *m_pNetworkManager;
};

#endif // SETTINGDIALOG_H
