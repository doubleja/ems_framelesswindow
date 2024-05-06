#ifndef HTTP_H
#define HTTP_H

#include <QObject>
#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class Http : public QObject
{
    Q_OBJECT
public:
    explicit Http(QObject *parent = nullptr);
    // 函数声明
    void onenetRetData(const QString& id, const QDateTime& sendTime);
    void onNetworkReply(QNetworkReply* reply);

    QNetworkAccessManager *m_manager;
signals:

};

#endif // HTTP_H
