#include "knowledgebase.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

KnowledgeBase::KnowledgeBase(QObject *parent)
    : QObject(parent)
{
    initializeDefaultKnowledge();
}

void KnowledgeBase::addQA(const QString &question, const QString &answer)
{
    if (!question.trimmed().isEmpty() && !answer.trimmed().isEmpty()) {
        qaMap[question.trimmed().toLower()] = answer.trimmed();
    }
}

QString KnowledgeBase::searchAnswer(const QString &question)
{
    QString query = question.trimmed().toLower();
    
    // 直接匹配
    if (qaMap.contains(query)) {
        return qaMap[query];
    }
    
    // 模糊匹配
    QString bestMatch;
    double bestSimilarity = 0.0;
    
    for (auto it = qaMap.begin(); it != qaMap.end(); ++it) {
        double similarity = calculateSimilarity(query, it.key());
        if (similarity > bestSimilarity && similarity > 0.6) {
            bestSimilarity = similarity;
            bestMatch = it.value();
        }
    }
    
    if (!bestMatch.isEmpty()) {
        return bestMatch;
    }
    
    // 关键词匹配
    QStringList keywords = query.split(QRegularExpression("\\s+"));
    for (const QString &keyword : keywords) {
        if (keyword.length() > 2) {
            for (auto it = qaMap.begin(); it != qaMap.end(); ++it) {
                if (it.key().contains(keyword, Qt::CaseInsensitive)) {
                    return it.value();
                }
            }
        }
    }
    
    return "抱歉，我暂时无法回答这个问题。请尝试换个方式提问，或者添加新的知识。";
}

bool KnowledgeBase::loadFromFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件:" << filename;
        return false;
    }
    
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (doc.isNull()) {
        qDebug() << "JSON格式错误";
        return false;
    }
    
    QJsonObject root = doc.object();
    if (root.contains("qa_pairs")) {
        QJsonArray qaArray = root["qa_pairs"].toArray();
        for (const QJsonValue &value : qaArray) {
            QJsonObject qa = value.toObject();
            if (qa.contains("question") && qa.contains("answer")) {
                addQA(qa["question"].toString(), qa["answer"].toString());
            }
        }
    }
    
    return true;
}

bool KnowledgeBase::saveToFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "无法创建文件:" << filename;
        return false;
    }
    
    QJsonObject root;
    QJsonArray qaArray;
    
    for (auto it = qaMap.begin(); it != qaMap.end(); ++it) {
        QJsonObject qa;
        qa["question"] = it.key();
        qa["answer"] = it.value();
        qaArray.append(qa);
    }
    
    root["qa_pairs"] = qaArray;
    
    QJsonDocument doc(root);
    file.write(doc.toJson());
    
    return true;
}

QStringList KnowledgeBase::getAllQuestions() const
{
    return qaMap.keys();
}

void KnowledgeBase::clear()
{
    qaMap.clear();
    initializeDefaultKnowledge();
}

void KnowledgeBase::initializeDefaultKnowledge()
{
    // 添加一些默认的问答对
    addQA("你好", "你好！我是智能问答助手，很高兴为您服务！");
    addQA("你是谁", "我是一个基于Qt开发的智能问答系统，可以帮助您解答各种问题。");
    addQA("你能做什么", "我可以回答问题、提供信息、帮助学习等。如果您有具体问题，请随时提问！");
    addQA("如何添加新知识", "您可以在界面中输入问题和答案，然后点击添加按钮来扩充我的知识库。");
    addQA("谢谢", "不客气！如果还有其他问题，随时可以问我。");
    addQA("再见", "再见！祝您生活愉快！");
    addQA("什么是人工智能", "人工智能（AI）是计算机科学的一个分支，旨在创建能够执行通常需要人类智能的任务的系统。");
    addQA("什么是机器学习", "机器学习是人工智能的一个子集，它使计算机能够在没有明确编程的情况下学习和改进。");
    addQA("什么是深度学习", "深度学习是机器学习的一个分支，使用多层神经网络来模拟人脑的学习过程。");
    addQA("Qt是什么", "Qt是一个跨平台的C++应用程序开发框架，广泛用于开发桌面应用程序和嵌入式系统。");
}

double KnowledgeBase::calculateSimilarity(const QString &str1, const QString &str2)
{
    if (str1 == str2) return 1.0;
    if (str1.isEmpty() || str2.isEmpty()) return 0.0;
    
    // 简单的相似度计算（基于共同字符）
    QStringList words1 = str1.split(QRegularExpression("\\s+"));
    QStringList words2 = str2.split(QRegularExpression("\\s+"));
    
    int commonWords = 0;
    for (const QString &word1 : words1) {
        for (const QString &word2 : words2) {
            if (word1 == word2) {
                commonWords++;
            }
        }
    }
    
    int totalWords = words1.size() + words2.size();
    if (totalWords == 0) return 0.0;
    
    return (2.0 * commonWords) / totalWords;
}
