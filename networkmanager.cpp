#include "networkmanager.h"

NetworkManager::NetworkManager(QObject *parent) :
    QNetworkAccessManager(parent),
    m_textAnalysisAPI(nullptr)
{}

NetworkManager::NetworkManager(const QString id, const QString secret,
                               QObject *parent)
    :QNetworkAccessManager(parent)
{
    QString accessToken = getAccessToken(id, secret);
    if (accessToken != "") setAccessToken(accessToken);
}

QString NetworkManager::getAccessToken(const QString id, const QString secret)
{
     QNetworkRequest *pAccessToken
             = new QNetworkRequest
             (QUrl("https://aip.baidubce.com/oauth/2.0/token?"
            "grant_type=client_credentials"
            "&client_id=" + id +
            "&client_secret="+secret));
     QNetworkReply *reply = get(*pAccessToken);
     QEventLoop eventLoop;
     QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
     eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
     if (reply->error() == QNetworkReply::NoError)
     {
         QByteArray dataArray = reply->readAll();
         QJsonParseError jsonParseErr;
         QJsonDocument json = QJsonDocument::fromJson(dataArray, &jsonParseErr);
         QJsonObject jsonObj = json.object();
         if (jsonObj.contains("access_token")){
             emit available(true);
             return jsonObj["access_token"].toString();
         }else if (jsonObj.contains("error")){
             QString errMsg = QString("error_description:%1, error:%2")
                     .arg(jsonObj["error_description"].toString(),
                     jsonObj["error"].toString());
             emit available(false);
             emit errors(errMsg);
         }
     }else{
         emit available(false);
         emit errors("Get Access Token Failed!");
     }
     return "";
}

void NetworkManager::setAccessToken(const QString &accessToken)
{
    m_textAnalysisAPI = new QNetworkRequest
            (QUrl("https://aip.baidubce.com/rest/2.0/ocr/v1/general_basic"
                  "?access_token=" + accessToken));
    m_textAnalysisAPI->setHeader(QNetworkRequest::ContentTypeHeader,
                                 "application/x-www-form-urlencoded");

}

QString NetworkManager::getString(QByteArray &src)
{
    if (m_textAnalysisAPI == nullptr) {
        emit errors("Get access token first.");
        return "";
    }
    QString result = "";
    QNetworkReply *reply = post(*m_textAnalysisAPI, src);
    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray dataArray = reply->readAll();
        QJsonParseError jsonParseErr;
        QJsonDocument json = QJsonDocument::fromJson(dataArray, &jsonParseErr);
        QJsonObject jsonObj = json.object();
        if (jsonObj.contains("words_result")){
            QJsonArray wordsArray = jsonObj["words_result"].toArray();
            QJsonObject eachLine;
            auto iterator = wordsArray.begin();
            while (iterator != wordsArray.end()){
                eachLine = iterator->toObject();
                result += eachLine["words"].toString() + '\n';
                ++iterator;
            }
        }else if (jsonObj.contains("error_code")){
            QString errMsg = QString("error_code:%1, error_msg:%2")
                    .arg(jsonObj["error_code"].toInt())
                    .arg(jsonObj["error_msg"].toString());
            emit errors(errMsg);
        }
    }else
        emit errors("Get Text Failed!");
    return result;
}

