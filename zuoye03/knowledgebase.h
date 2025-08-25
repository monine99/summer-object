#ifndef KNOWLEDGEBASE_H
#define KNOWLEDGEBASE_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class KnowledgeBase : public QObject
{
    Q_OBJECT

public:
    explicit KnowledgeBase(QObject *parent = nullptr);
    
    // 添加问答对
    void addQA(const QString &question, const QString &answer);
    
    // 搜索答案
    QString searchAnswer(const QString &question);
    
    // 从文件加载知识库
    bool loadFromFile(const QString &filename);
    
    // 保存知识库到文件
    bool saveToFile(const QString &filename);
    
    // 获取所有问题
    QStringList getAllQuestions() const;
    
    // 清空知识库
    void clear();

private:
    QMap<QString, QString> qaMap;
    
    // 初始化默认知识库
    void initializeDefaultKnowledge();
    
    // 计算字符串相似度
    double calculateSimilarity(const QString &str1, const QString &str2);
};

#endif // KNOWLEDGEBASE_H
