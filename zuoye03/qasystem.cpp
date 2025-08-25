#include "qasystem.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QEventLoop>
#include <QTimer>
#include <QDebug>
#include <QRandomGenerator>

QASystem::QASystem(QObject *parent)
    : QObject(parent)
    , knowledgeBase(new KnowledgeBase(this))
    , networkManager(new QNetworkAccessManager(this))
    , useOnlineAPI(false)
{
    connect(networkManager, &QNetworkAccessManager::finished, 
            this, &QASystem::onNetworkReplyFinished);
}

QASystem::~QASystem()
{
}

void QASystem::getAnswer(const QString &question)
{
    if (question.trimmed().isEmpty()) {
        emit errorOccurred("问题不能为空");
        return;
    }
    
    if (useOnlineAPI && !apiKey.isEmpty()) {
        callOnlineAPI(question);
    } else {
        // 使用本地知识库
        QString answer = knowledgeBase->searchAnswer(question);
        emit answerReceived(answer);
    }
}

void QASystem::addNewQA(const QString &question, const QString &answer)
{
    knowledgeBase->addQA(question, answer);
    emit knowledgeUpdated();
}

void QASystem::setApiKey(const QString &key)
{
    apiKey = key;
}

void QASystem::setUseOnlineAPI(bool use)
{
    useOnlineAPI = use;
}

KnowledgeBase* QASystem::getKnowledgeBase() const
{
    return knowledgeBase;
}

void QASystem::callOnlineAPI(const QString &question)
{
    // 检查API密钥是否配置
    if (apiKey.isEmpty()) {
        emit errorOccurred("API密钥未配置，请先输入API密钥");
        return;
    }
    
    // 构建阿里云通义千问API请求
    QNetworkRequest request;
    request.setUrl(QUrl("https://dashscope.aliyuncs.com/api/v1/services/aigc/text-generation/generation"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(apiKey).toUtf8());
    
    // 构建请求数据
    QJsonObject data;
    data["model"] = "qwen-turbo";  // 使用通义千问模型
    
    QJsonObject input;
    QJsonArray messages;
    QJsonObject message;
    message["role"] = "user";
    message["content"] = question;
    messages.append(message);
    input["messages"] = messages;
    data["input"] = input;
    
    QJsonDocument doc(data);
    QByteArray postData = doc.toJson();
    
    // 发送POST请求到阿里云API
    QNetworkReply *reply = networkManager->post(request, postData);
    
    // 连接完成信号 - 这是关键修复！
    connect(reply, &QNetworkReply::finished, this, &QASystem::onNetworkReplyFinished);
    
    // 连接错误信号
    connect(reply, &QNetworkReply::errorOccurred, this, &QASystem::onNetworkError);
    
    // 显示进度条
    emit answerReceived("正在调用阿里云AI服务，请稍候...");
}

void QASystem::processAPIResponse(const QByteArray &data)
{
    qDebug() << "开始处理API响应，数据长度:" << data.size();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        QString errorMsg = "API响应格式错误，无法解析JSON";
        qDebug() << errorMsg << "原始数据:" << data;
        emit errorOccurred(errorMsg);
        return;
    }
    
    QJsonObject root = doc.object();
    qDebug() << "API响应JSON对象:" << root.keys();
    
    QString answer;
    
    // 解析阿里云通义千问API响应格式
    if (root.contains("output")) {
        QJsonObject output = root["output"].toObject();
        qDebug() << "找到output字段，包含:" << output.keys();
        
        if (output.contains("text")) {
            answer = output["text"].toString();
            qDebug() << "从text字段获取答案:" << answer;
        } else if (output.contains("choices")) {
            QJsonArray choices = output["choices"].toArray();
            qDebug() << "找到choices数组，长度:" << choices.size();
            
            if (!choices.isEmpty()) {
                QJsonObject choice = choices.first().toObject();
                if (choice.contains("message")) {
                    QJsonObject message = choice["message"].toObject();
                    if (message.contains("content")) {
                        answer = message["content"].toString();
                        qDebug() << "从choices.message.content获取答案:" << answer;
                    }
                }
            }
        }
    }
    
    // 如果阿里云格式不匹配，尝试其他常见格式
    if (answer.isEmpty()) {
        qDebug() << "阿里云格式不匹配，尝试其他格式...";
        if (root.contains("answer")) {
            answer = root["answer"].toString();
            qDebug() << "从answer字段获取答案:" << answer;
        } else if (root.contains("response")) {
            answer = root["response"].toString();
            qDebug() << "从response字段获取答案:" << answer;
        } else if (root.contains("content")) {
            answer = root["content"].toString();
            qDebug() << "从content字段获取答案:" << answer;
        } else if (root.contains("message")) {
            answer = root["message"].toString();
            qDebug() << "从message字段获取答案:" << answer;
        }
    }
    
    // 如果仍然没有找到答案，显示原始响应用于调试
    if (answer.isEmpty()) {
        answer = QString("API响应格式不匹配。原始响应：%1").arg(QString::fromUtf8(data));
        qDebug() << "未找到答案，显示原始响应";
    }
    
    qDebug() << "最终答案:" << answer;
    emit answerReceived(answer);
}

void QASystem::onNetworkReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            qDebug() << "API响应数据:" << responseData;
            processAPIResponse(responseData);
        } else {
            QString errorMsg = QString("网络请求失败: %1").arg(reply->errorString());
            qDebug() << errorMsg;
            emit errorOccurred(errorMsg);
        }
        reply->deleteLater();
    }
}

void QASystem::onNetworkError(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error)
    emit errorOccurred("网络连接错误");
}

QString QASystem::simulateOnlineResponse(const QString &question)
{
    // 模拟在线AI的智能回答
    QString lowerQuestion = question.toLower();
    
    if (lowerQuestion.contains("天气") || lowerQuestion.contains("温度")) {
        return "抱歉，我目前无法获取实时天气信息。建议您查看天气预报应用或网站获取准确的天气数据。";
    }
    
    if (lowerQuestion.contains("时间") || lowerQuestion.contains("几点")) {
        return "我无法获取实时时间信息。请查看您的设备时钟或系统时间。";
    }
    
    if (lowerQuestion.contains("计算") || lowerQuestion.contains("数学")) {
        return "我可以帮助解答数学概念问题，但无法进行实时计算。建议使用计算器或数学软件。";
    }
    
    if (lowerQuestion.contains("新闻") || lowerQuestion.contains("时事")) {
        return "我无法获取最新的新闻信息。建议您查看新闻网站或应用程序获取最新资讯。";
    }
    
    if (lowerQuestion.contains("翻译") || lowerQuestion.contains("英文")) {
        return "我可以帮助解释语言概念，但无法进行实时翻译。建议使用专业的翻译工具。";
    }
    
    // 通用智能回答
    QStringList responses = {
        "这是一个很有趣的问题。基于我的理解，",
        "让我来分析一下这个问题。",
        "根据我的知识，",
        "这个问题涉及到",
        "我可以为您解释一下，"
    };
    
    QString response = responses[QRandomGenerator::global()->bounded(responses.size())];
    
    if (lowerQuestion.contains("为什么")) {
        response += "这通常是因为多种因素共同作用的结果。具体情况需要根据具体问题来分析。";
    } else if (lowerQuestion.contains("怎么") || lowerQuestion.contains("如何")) {
        response += "解决这个问题通常需要分步骤进行，首先明确目标，然后制定计划，最后执行并评估结果。";
    } else if (lowerQuestion.contains("什么")) {
        response += "这涉及到概念定义和分类。需要从不同角度来理解和分析。";
    } else {
        response += "这个问题可以从多个角度来思考，建议您提供更多具体信息，这样我可以给出更准确的回答。";
    }
    
    return response;
}
