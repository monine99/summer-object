#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QMessageBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startServer();
    void connectToServer();
    void sendMessage();
    void handleNewConnection();
    void handleReadyRead();
    void handleDisconnected();

private:
    void setupUI();
    void setupConnections();

    // 网络组件
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    
    // UI组件
    QTextEdit *chatDisplay;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QPushButton *startServerButton;
    QPushButton *connectButton;
    QSpinBox *portSpinBox;
    QLineEdit *ipInput;
    QLabel *statusLabel;
    
    // 布局
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *inputLayout;
    QHBoxLayout *connectionLayout;
    
    bool isServer;
    bool isConnected;
};

#endif // MAINWINDOW_H
