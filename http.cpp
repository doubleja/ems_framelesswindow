#include "http.h"

Http::Http(QObject *parent)
    : QObject{parent}
{
    // 初始化网络访问管理器
    m_manager = new QNetworkAccessManager(this);
    // 连接信号和槽
    connect(m_manager, &QNetworkAccessManager::finished, this, &Http::onNetworkReply);

}

// 获取onenet数据流最新数据
void Http::onenetRetData( const QDateTime& sendTime)
{
    qDebug() << "send_time_str" << sendTime.toString();

    // 获取当前时间
    QDateTime nowTime = QDateTime::currentDateTime();

    // 确保查询的开始时间晚于sendTime
    QDateTime time10MinAfter = sendTime.addSecs(-10);

    // 格式化开始时间
    QString startTime = time10MinAfter.toString(Qt::ISODate);

    // 构建请求URL
    QString url = "http://api.heclouds.com/devices/" + id + "/datapoints?datastream_id=BIN_DATA&sort=DESC&limit=1&start=" + startTime;

    // 发送GET请求
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("api-key", "R218Q5jpynJPvh8ow9b8LBXFNr0=");
    m_manager->get(request);
}

// 处理网络响应
void Http::onNetworkReply(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        qDebug()<<"onNetworkReply";
        // 读取响应数据
        QByteArray data = reply->readAll();
        qDebug()<<"onNetworkReply";
        qDebug() << "Data:" << data;
        // 解析JSON
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();

        if (obj.contains("data") && obj["data"].isObject())
        {
            QJsonObject dataObj = obj["data"].toObject();
            if (dataObj.contains("datastreams") && dataObj["datastreams"].isArray())
            {
                QJsonArray streamsArray = dataObj["datastreams"].toArray();
                if (!streamsArray.isEmpty())
                {
                    QJsonObject streamObj = streamsArray.at(0).toObject();
                    if (streamObj.contains("datapoints") && streamObj["datapoints"].isArray())
                    {
                        QJsonArray pointsArray = streamObj["datapoints"].toArray();
                        if (!pointsArray.isEmpty())
                        {
                            QJsonObject pointObj = pointsArray.at(0).toObject();
                            if (pointObj.contains("value") && pointObj["value"].isString())
                            {
                                QString bindata = pointObj["value"].toString();
                                qDebug() << "获取onenet数据流最新数据:" << bindata;
                                // 执行其他操作
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        qDebug() << "Error:" << reply->errorString();
    }
    // 释放网络响应对象
    reply->deleteLater();
}



void Http::cmd_send(const QString& cmd)
{


    QString url = "http://api.heclouds.com/cmds?device_id=" + id;
    // 发送GET请求
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("api-key", "R218Q5jpynJPvh8ow9b8LBXFNr0=");
    // QString testData = "0a79c6f2";
    m_manager->post(request, cmd.toUtf8());
    // QNetworkReply* reply = m_manager->post(request, testData.toUtf8());


}
void Http::requestFinished(QNetworkReply* reply) {
    // 获取http状态码
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
        qDebug() << "status code=" << statusCode.toInt();

    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if(reason.isValid())
        qDebug() << "reason=" << reason.toString();

    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError) {
        qDebug() << "Failed: " << reply->errorString();
    }
    else {
        // 获取返回内容
        qDebug() << reply->readAll();
    }
}
