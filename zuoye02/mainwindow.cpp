#include "mainwindow.h"
#include <QApplication>
#include <QHostAddress>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , tcpServer(nullptr)
    , tcpSocket(nullptr)
    , isServer(false)
    , isConnected(false)
{
    setupUI();
    setupConnections();
    
    portSpinBox->setValue(8080);
    ipInput->setText("127.0.0.1");
    
    setWindowTitle("Qt网络聊天程序");
    resize(600, 500);
}

MainWindow::~MainWindow()
{
    if (tcpSocket) {
        tcpSocket->disconnectFromHost();
        delete tcpSocket;
    }
    if (tcpServer) {
        tcpServer->close();
        delete tcpServer;
    }
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    
    statusLabel = new QLabel("状态: 未连接", this);
    statusLabel->setStyleSheet("QLabel { color: #cc0000; font-weight: bold; font-size: 14px; padding: 8px; background-color: #ffe6e6; border-radius: 5px; }");
    mainLayout->addWidget(statusLabel);
    
    connectionLayout = new QHBoxLayout();
    
    QLabel *ipLabel = new QLabel("IP地址:", this);
    ipInput = new QLineEdit(this);
    
    QLabel *portLabel = new QLabel("端口:", this);
    portSpinBox = new QSpinBox(this);
    portSpinBox->setRange(1024, 65535);
    
    startServerButton = new QPushButton("启动服务器", this);
    startServerButton->setStyleSheet("QPushButton { background-color: #28a745; color: white; border: none; padding: 8px 16px; border-radius: 5px; font-weight: bold; } QPushButton:hover { background-color: #218838; } QPushButton:disabled { background-color: #cccccc; }");
    
    connectButton = new QPushButton("连接到服务器", this);
    connectButton->setStyleSheet("QPushButton { background-color: #17a2b8; color: white; border: none; padding: 8px 16px; border-radius: 5px; font-weight: bold; } QPushButton:hover { background-color: #138496; } QPushButton:disabled { background-color: #cccccc; }");
    
    connectionLayout->addWidget(ipLabel);
    connectionLayout->addWidget(ipInput);
    connectionLayout->addWidget(portLabel);
    connectionLayout->addWidget(portSpinBox);
    connectionLayout->addWidget(startServerButton);
    connectionLayout->addWidget(connectButton);
    
    mainLayout->addLayout(connectionLayout);
    
    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);
    chatDisplay->setStyleSheet("QTextEdit { background-color: #ffffff; border: 2px solid #e0e0e0; border-radius: 8px; padding: 10px; font-family: 'Microsoft YaHei', Arial; font-size: 12px; }");
    mainLayout->addWidget(chatDisplay);
    
    inputLayout = new QHBoxLayout();
    
    messageInput = new QLineEdit(this);
    messageInput->setPlaceholderText("输入聊天消息...");
    messageInput->setStyleSheet("QLineEdit { padding: 8px; border: 2px solid #e0e0e0; border-radius: 5px; font-size: 12px; } QLineEdit:focus { border-color: #0066cc; }");
    
    sendButton = new QPushButton("发送", this);
    sendButton->setEnabled(false);
    sendButton->setStyleSheet("QPushButton { background-color: #0066cc; color: white; border: none; padding: 8px 16px; border-radius: 5px; font-weight: bold; } QPushButton:hover { background-color: #0052a3; } QPushButton:disabled { background-color: #cccccc; }");
    
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);
    
    mainLayout->addLayout(inputLayout);
    
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);
    
    QLabel *helpLabel = new QLabel("使用说明: 先启动服务器，然后另一个程序连接到该服务器进行聊天", this);
    helpLabel->setStyleSheet("QLabel { color: #6c757d; background-color: #f8f9fa; padding: 10px; border-radius: 5px; border-left: 4px solid #0066cc; font-size: 11px; }");
    helpLabel->setWordWrap(true);
    mainLayout->addWidget(helpLabel);
}

void MainWindow::setupConnections()
{
    connect(startServerButton, &QPushButton::clicked, this, &MainWindow::startServer);
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::connectToServer);
    connect(sendButton, &QPushButton::clicked, this, &MainWindow::sendMessage);
    connect(messageInput, &QLineEdit::returnPressed, this, &MainWindow::sendMessage);
}

void MainWindow::startServer()
{
    if (isServer) {
        QMessageBox::information(this, "提示", "服务器已经在运行中！");
        return;
    }
    
    if (tcpServer) {
        delete tcpServer;
    }
    
    tcpServer = new QTcpServer(this);
    
    if (tcpServer->listen(QHostAddress::Any, portSpinBox->value())) {
        isServer = true;
        statusLabel->setText("状态: 服务器运行中，等待连接...");
        statusLabel->setStyleSheet("QLabel { color: #28a745; font-weight: bold; font-size: 14px; padding: 8px; background-color: #d4edda; border-radius: 5px; }");
        
        startServerButton->setEnabled(false);
        connectButton->setEnabled(false);
        
        QString serverMessage = QString("<div style='color: #008000; font-weight: bold; margin: 5px 0;'>[%1] 服务器启动成功，监听端口: %2</div>")
                               .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
                               .arg(portSpinBox->value());
        chatDisplay->append(serverMessage);
        
        connect(tcpServer, &QTcpServer::newConnection, this, &MainWindow::handleNewConnection);
    } else {
        QMessageBox::critical(this, "错误", "无法启动服务器！");
        delete tcpServer;
        tcpServer = nullptr;
    }
}

void MainWindow::connectToServer()
{
    if (isConnected) {
        QMessageBox::information(this, "提示", "已经连接到服务器！");
        return;
    }
    
    if (tcpSocket) {
        delete tcpSocket;
    }
    
    tcpSocket = new QTcpSocket(this);
    
    connect(tcpSocket, &QTcpSocket::connected, [this]() {
        isConnected = true;
        statusLabel->setText("状态: 已连接到服务器");
        statusLabel->setStyleSheet("QLabel { color: #28a745; font-weight: bold; font-size: 14px; padding: 8px; background-color: #d4edda; border-radius: 5px; }");
        
        startServerButton->setEnabled(false);
        connectButton->setEnabled(false);
        sendButton->setEnabled(true);
        
        QString connectMessage = QString("<div style='color: #008000; font-weight: bold; margin: 5px 0;'>[%1] 成功连接到服务器</div>")
                                .arg(QDateTime::currentDateTime().toString("hh:mm:ss"));
        chatDisplay->append(connectMessage);
    });
    
    connect(tcpSocket, &QTcpSocket::readyRead, this, &MainWindow::handleReadyRead);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &MainWindow::handleDisconnected);
    
    tcpSocket->connectToHost(ipInput->text(), portSpinBox->value());
    
    if (!tcpSocket->waitForConnected(5000)) {
        QMessageBox::critical(this, "错误", "连接超时！");
        delete tcpSocket;
        tcpSocket = nullptr;
    }
}

void MainWindow::sendMessage()
{
    if (!isConnected && !tcpSocket) {
        QMessageBox::warning(this, "警告", "请先连接到服务器！");
        return;
    }
    
    QString message = messageInput->text().trimmed();
    if (message.isEmpty()) {
        return;
    }
    
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString fullMessage = QString("[%1] 我: %2").arg(timestamp).arg(message);
    
    // 使用HTML格式设置发送消息的样式
    QString htmlMessage = QString("<div style='color: #000000; font-weight: bold; margin: 5px 0;'>[%1] 我: %2</div>").arg(timestamp).arg(message);
    chatDisplay->append(htmlMessage);
    
    if (tcpSocket && tcpSocket->state() == QAbstractSocket::ConnectedState) {
        tcpSocket->write(message.toUtf8());
    }
    
    messageInput->clear();
}

void MainWindow::handleNewConnection()
{
    if (tcpSocket) {
        delete tcpSocket;
    }
    
    tcpSocket = tcpServer->nextPendingConnection();
    
    connect(tcpSocket, &QTcpSocket::readyRead, this, &MainWindow::handleReadyRead);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &MainWindow::handleDisconnected);
    
    isConnected = true;
    sendButton->setEnabled(true);
    
            statusLabel->setText("状态: 客户端已连接");
        statusLabel->setStyleSheet("QLabel { color: #28a745; font-weight: bold; font-size: 14px; padding: 8px; background-color: #d4edda; border-radius: 5px; }");
    
            QString clientMessage = QString("<div style='color: #008000; font-weight: bold; margin: 5px 0;'>[%1] 客户端已连接</div>")
                               .arg(QDateTime::currentDateTime().toString("hh:mm:ss"));
        chatDisplay->append(clientMessage);
}

void MainWindow::handleReadyRead()
{
    if (tcpSocket) {
        QByteArray data = tcpSocket->readAll();
        QString message = QString::fromUtf8(data);
        QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
        
        // 使用HTML格式设置接收消息的样式
        QString htmlMessage = QString("<div style='color: #0066cc; font-weight: bold; margin: 5px 0;'>[%1] 对方: %2</div>").arg(timestamp).arg(message);
        chatDisplay->append(htmlMessage);
    }
}

void MainWindow::handleDisconnected()
{
    isConnected = false;
    sendButton->setEnabled(false);
    
    if (isServer) {
        statusLabel->setText("状态: 服务器运行中，等待连接...");
        statusLabel->setStyleSheet("QLabel { color: #28a745; font-weight: bold; font-size: 14px; padding: 8px; background-color: #d4edda; border-radius: 5px; }");
        
        startServerButton->setEnabled(false);
        connectButton->setEnabled(false);
    } else {
        statusLabel->setText("状态: 连接已断开");
        statusLabel->setStyleSheet("QLabel { color: #dc3545; font-weight: bold; font-size: 14px; padding: 8px; background-color: #f8d7da; border-radius: 5px; }");
        
        startServerButton->setEnabled(true);
        connectButton->setEnabled(true);
    }
    
            QString disconnectMessage = QString("<div style='color: #cc0000; font-weight: bold; margin: 5px 0;'>[%1] 连接已断开</div>")
                                   .arg(QDateTime::currentDateTime().toString("hh:mm:ss"));
        chatDisplay->append(disconnectMessage);
    
    if (tcpSocket) {
        tcpSocket->deleteLater();
        tcpSocket = nullptr;
    }
}
