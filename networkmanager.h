#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>

class NetworkManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);
    NetworkManager(const QString id, const QString sercet,
                                  QObject *parent=nullptr);
    QString getString(QByteArray &src);
    QString getAccessToken(const QString id, const QString secret);
    void setAccessToken(const QString& accessToken);

signals:
    void errors(const QString &msg);
    void available(bool flag);

private:
    QNetworkRequest *m_textAnalysisAPI;
};

#endif // NETWORKMANAGER_H
