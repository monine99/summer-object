#ifndef QASYSTEM_H
#define QASYSTEM_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "knowledgebase.h"

class QASystem : public QObject
{
    Q_OBJECT

public:
    explicit QASystem(QObject *parent = nullptr);
    ~QASystem();
    
    // 获取答案
    void getAnswer(const QString &question);
    
    // 添加新的问答对
    void addNewQA(const QString &question, const QString &answer);
    
    // 设置API密钥
    void setApiKey(const QString &key);
    
    // 设置是否使用在线API
    void setUseOnlineAPI(bool use);
    
    // 获取知识库
    KnowledgeBase* getKnowledgeBase() const;

signals:
    void answerReceived(const QString &answer);
    void errorOccurred(const QString &error);
    void knowledgeUpdated();

private slots:
    void onNetworkReplyFinished();
    void onNetworkError(QNetworkReply::NetworkError error);

private:
    KnowledgeBase *knowledgeBase;
    QNetworkAccessManager *networkManager;
    QString apiKey;
    bool useOnlineAPI;
    
    // 调用在线API
    void callOnlineAPI(const QString &question);
    
    // 处理API响应
    void processAPIResponse(const QByteArray &data);
    
    // 模拟在线API响应（当没有真实API时）
    QString simulateOnlineResponse(const QString &question);
};

#endif // QASYSTEM_H
