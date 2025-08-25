#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QListWidget>
#include <QSplitter>
#include <QProgressBar>
#include "qasystem.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAskButtonClicked();
    void onAddKnowledgeButtonClicked();
    void onClearButtonClicked();
    void onLoadKnowledgeButtonClicked();
    void onSaveKnowledgeButtonClicked();
    void onAnswerReceived(const QString &answer);
    void onErrorOccurred(const QString &error);
    void onKnowledgeUpdated();
    void onQuestionEnterPressed();
    void onUseOnlineAPIToggled(bool checked);

private:
    Ui::MainWindow *ui;
    QASystem *qaSystem;
    
    // UI组件
    QTextEdit *chatDisplay;
    QLineEdit *questionInput;
    QPushButton *askButton;
    QPushButton *addKnowledgeButton;
    QPushButton *clearButton;
    QPushButton *loadKnowledgeButton;
    QPushButton *saveKnowledgeButton;
    QCheckBox *useOnlineAPICheckBox;
    QLineEdit *apiKeyInput;
    QListWidget *knowledgeList;
    QProgressBar *progressBar;
    
    void setupUI();
    void setupConnections();
    void addMessageToChat(const QString &message, bool isUser = false);
    void updateKnowledgeList();
    void showStatusMessage(const QString &message);
};

#endif // MAINWINDOW_H
