#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollBar>
#include <QDateTime>
#include <QApplication>
#include <QStyle>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , qaSystem(new QASystem(this))
{
    ui->setupUi(this);
    setupUI();
    setupConnections();
    
    setWindowTitle("智能问答系统 - AI问答助手");
    resize(1000, 700);
    
    addMessageToChat("欢迎使用智能问答系统！我是您的AI助手，请问有什么可以帮助您的吗？", false);
    showStatusMessage("系统就绪，可以开始提问");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    mainLayout->addWidget(splitter);
    
    // 左侧聊天区域
    QWidget *chatWidget = new QWidget;
    QVBoxLayout *chatLayout = new QVBoxLayout(chatWidget);
    
    chatDisplay = new QTextEdit;
    chatDisplay->setReadOnly(true);
    chatDisplay->setStyleSheet(
        "QTextEdit {"
        "    background-color: #f8f9fa;"
        "    border: 1px solid #dee2e6;"
        "    border-radius: 8px;"
        "    padding: 10px;"
        "    font-family: 'Microsoft YaHei', Arial, sans-serif;"
        "    font-size: 14px;"
        "}"
    );
    chatLayout->addWidget(new QLabel("智能问答对话"));
    chatLayout->addWidget(chatDisplay);
    
    QHBoxLayout *inputLayout = new QHBoxLayout;
    questionInput = new QLineEdit;
    questionInput->setPlaceholderText("请输入您的问题...");
    questionInput->setStyleSheet(
        "QLineEdit {"
        "    padding: 8px;"
        "    border: 2px solid #007bff;"
        "    border-radius: 6px;"
        "    font-size: 14px;"
        "}"
    );
    
    askButton = new QPushButton("提问");
    askButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #007bff;"
        "    color: white;"
        "    border: none;"
        "    padding: 8px 16px;"
        "    border-radius: 6px;"
        "    font-weight: bold;"
        "}"
    );
    
    inputLayout->addWidget(questionInput);
    inputLayout->addWidget(askButton);
    chatLayout->addLayout(inputLayout);
    
    progressBar = new QProgressBar;
    progressBar->setVisible(false);
    chatLayout->addWidget(progressBar);
    
    splitter->addWidget(chatWidget);
    
    // 右侧控制面板
    QWidget *controlWidget = new QWidget;
    QVBoxLayout *controlLayout = new QVBoxLayout(controlWidget);
    
    QGroupBox *apiGroup = new QGroupBox("API设置");
    QVBoxLayout *apiLayout = new QVBoxLayout(apiGroup);
    
    useOnlineAPICheckBox = new QCheckBox("使用在线API");
    apiKeyInput = new QLineEdit;
    apiKeyInput->setPlaceholderText("输入API密钥（可选）");
    apiKeyInput->setEchoMode(QLineEdit::Password);
    
    apiLayout->addWidget(useOnlineAPICheckBox);
    apiLayout->addWidget(new QLabel("API密钥:"));
    apiLayout->addWidget(apiKeyInput);
    
    controlLayout->addWidget(apiGroup);
    
    QGroupBox *knowledgeGroup = new QGroupBox("知识库管理");
    QVBoxLayout *knowledgeLayout = new QVBoxLayout(knowledgeGroup);
    
    addKnowledgeButton = new QPushButton("添加知识");
    addKnowledgeButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #28a745;"
        "    color: white;"
        "    border: none;"
        "    padding: 8px;"
        "    border-radius: 4px;"
        "    margin: 2px;"
        "}"
    );
    
    loadKnowledgeButton = new QPushButton("加载知识库");
    loadKnowledgeButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #17a2b8;"
        "    color: white;"
        "    border: none;"
        "    padding: 8px;"
        "    border-radius: 4px;"
        "    margin: 2px;"
        "}"
    );
    
    saveKnowledgeButton = new QPushButton("保存知识库");
    saveKnowledgeButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #ffc107;"
        "    color: #212529;"
        "    border: none;"
        "    padding: 8px;"
        "    border-radius: 4px;"
        "    margin: 2px;"
        "}"
    );
    
    clearButton = new QPushButton("清空对话");
    clearButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #dc3545;"
        "    color: white;"
        "    border: none;"
        "    padding: 8px;"
        "    border-radius: 4px;"
        "    margin: 2px;"
        "}"
    );
    
    knowledgeLayout->addWidget(addKnowledgeButton);
    knowledgeLayout->addWidget(loadKnowledgeButton);
    knowledgeLayout->addWidget(saveKnowledgeButton);
    knowledgeLayout->addWidget(clearButton);
    
    controlLayout->addWidget(knowledgeGroup);
    
    QGroupBox *listGroup = new QGroupBox("知识库内容");
    QVBoxLayout *listLayout = new QVBoxLayout(listGroup);
    
    knowledgeList = new QListWidget;
    knowledgeList->setMaximumHeight(200);
    listLayout->addWidget(knowledgeList);
    
    controlLayout->addWidget(listGroup);
    
    splitter->addWidget(controlWidget);
    splitter->setSizes(QList<int>() << 700 << 300);
    
    updateKnowledgeList();
}

void MainWindow::setupConnections()
{
    connect(qaSystem, &QASystem::answerReceived, this, &MainWindow::onAnswerReceived);
    connect(qaSystem, &QASystem::errorOccurred, this, &MainWindow::onErrorOccurred);
    connect(qaSystem, &QASystem::knowledgeUpdated, this, &MainWindow::onKnowledgeUpdated);
    
    connect(askButton, &QPushButton::clicked, this, &MainWindow::onAskButtonClicked);
    connect(questionInput, &QLineEdit::returnPressed, this, &MainWindow::onQuestionEnterPressed);
    connect(addKnowledgeButton, &QPushButton::clicked, this, &MainWindow::onAddKnowledgeButtonClicked);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onClearButtonClicked);
    connect(loadKnowledgeButton, &QPushButton::clicked, this, &MainWindow::onLoadKnowledgeButtonClicked);
    connect(saveKnowledgeButton, &QPushButton::clicked, this, &MainWindow::onSaveKnowledgeButtonClicked);
    connect(useOnlineAPICheckBox, &QCheckBox::toggled, this, &MainWindow::onUseOnlineAPIToggled);
    
    connect(apiKeyInput, &QLineEdit::textChanged, [this](const QString &text) {
        qaSystem->setApiKey(text);
    });
}

void MainWindow::onAskButtonClicked()
{
    QString question = questionInput->text().trimmed();
    if (question.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入问题");
        return;
    }
    
    addMessageToChat(question, true);
    questionInput->clear();
    
    progressBar->setVisible(true);
    progressBar->setRange(0, 0);
    
    qaSystem->getAnswer(question);
}

void MainWindow::onQuestionEnterPressed()
{
    onAskButtonClicked();
}

void MainWindow::onAddKnowledgeButtonClicked()
{
    bool ok1, ok2;
    QString question = QInputDialog::getText(this, "添加知识", "请输入问题:", QLineEdit::Normal, "", &ok1);
    if (ok1 && !question.trimmed().isEmpty()) {
        QString answer = QInputDialog::getText(this, "添加知识", "请输入答案:", QLineEdit::Normal, "", &ok2);
        if (ok2 && !answer.trimmed().isEmpty()) {
            qaSystem->addNewQA(question.trimmed(), answer.trimmed());
            QMessageBox::information(this, "成功", "知识添加成功！");
        }
    }
}

void MainWindow::onClearButtonClicked()
{
    int ret = QMessageBox::question(this, "确认", "确定要清空所有对话记录吗？", 
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        chatDisplay->clear();
        addMessageToChat("对话记录已清空，请问有什么可以帮助您的吗？", false);
    }
}

void MainWindow::onLoadKnowledgeButtonClicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "加载知识库", "", "JSON文件 (*.json)");
    if (!filename.isEmpty()) {
        if (qaSystem->getKnowledgeBase()->loadFromFile(filename)) {
            QMessageBox::information(this, "成功", "知识库加载成功！");
            updateKnowledgeList();
        } else {
            QMessageBox::critical(this, "错误", "知识库加载失败！");
        }
    }
}

void MainWindow::onSaveKnowledgeButtonClicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "保存知识库", "knowledge_base.json", "JSON文件 (*.json)");
    if (!filename.isEmpty()) {
        if (qaSystem->getKnowledgeBase()->saveToFile(filename)) {
            QMessageBox::information(this, "成功", "知识库保存成功！");
        } else {
            QMessageBox::critical(this, "错误", "知识库保存失败！");
        }
    }
}

void MainWindow::onAnswerReceived(const QString &answer)
{
    progressBar->setVisible(false);
    addMessageToChat(answer, false);
    showStatusMessage("回答完成");
}

void MainWindow::onErrorOccurred(const QString &error)
{
    progressBar->setVisible(false);
    addMessageToChat("错误: " + error, false);
    showStatusMessage("发生错误: " + error);
}

void MainWindow::onKnowledgeUpdated()
{
    updateKnowledgeList();
}

void MainWindow::onUseOnlineAPIToggled(bool checked)
{
    qaSystem->setUseOnlineAPI(checked);
    apiKeyInput->setEnabled(checked);
    
    if (checked) {
        showStatusMessage("已启用在线API模式");
    } else {
        showStatusMessage("已切换到本地知识库模式");
    }
}

void MainWindow::addMessageToChat(const QString &message, bool isUser)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString formattedMessage;
    
    if (isUser) {
        formattedMessage = QString("<div style='margin: 10px 0;'>"
                                 "<div style='text-align: right;'>"
                                 "<span style='background-color: #007bff; color: white; padding: 8px 12px; "
                                 "border-radius: 15px; display: inline-block; max-width: 70%;'>"
                                 "%1</span>"
                                 "<br><small style='color: #6c757d;'>%2 (您)</small>"
                                 "</div></div>")
                                 .arg(message.toHtmlEscaped(), timestamp);
    } else {
        formattedMessage = QString("<div style='margin: 10px 0;'>"
                                 "<div style='background-color: #e9ecef; color: #212529; padding: 8px 12px; "
                                 "border-radius: 15px; display: inline-block; max-width: 70%;'>"
                                 "%1</span>"
                                 "<br><small style='color: #6c757d;'>%2 (AI助手)</small>"
                                 "</div>")
                                 .arg(message.toHtmlEscaped(), timestamp);
    }
    
    chatDisplay->append(formattedMessage);
    
    QScrollBar *scrollBar = chatDisplay->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void MainWindow::updateKnowledgeList()
{
    knowledgeList->clear();
    QStringList questions = qaSystem->getKnowledgeBase()->getAllQuestions();
    for (const QString &question : questions) {
        knowledgeList->addItem(question);
    }
}

void MainWindow::showStatusMessage(const QString &message)
{
    statusBar()->showMessage(message, 3000);
}
