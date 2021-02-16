#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    m_pNetworkManager = new NetworkManager(this);
    QSettings settings;
    settings.beginGroup("apidata");
    ui->lineEditAPI->setText(settings.value("client_id").toString());
    ui->lineEditSecret->setText(settings.value("client_secret").toString());
    settings.endGroup();
    connect(this, &SettingDialog::finished, this, &SettingDialog::doneEvents);
}

void SettingDialog::doneEvents(int r)
{
    if (r == QDialog::Accepted){
        QString accessToken = m_pNetworkManager->getAccessToken(
                    ui->lineEditAPI->text(),
                    ui->lineEditSecret->text());
        if (accessToken != ""){
            QSettings settings;
            settings.beginGroup("apidata");
            settings.setValue("client_id", ui->lineEditAPI->text());
            settings.setValue("client_secret", ui->lineEditSecret->text());
            settings.setValue("accesstoken", accessToken);
            settings.endGroup();
        }else{
            emit errorMsg("Get Access Token Failed!");
        }
    }
}

SettingDialog::~SettingDialog()
{
    delete ui;
}
