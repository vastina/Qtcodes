#ifndef CHATBOX_H
#define CHATBOX_H

#include <QListWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

class ChatBox : public QWidget
{
    Q_OBJECT
public:
    explicit ChatBox(QWidget *parent = nullptr);

    // 添加一条消息到聊天框
    void addMessage(const QString& message);

private slots:
    // 发送按钮点击事件的槽函数
    void sendMessage();

private:
    QVBoxLayout *layout;
    QListWidget *chatListWidget;
    QLineEdit *inputLineEdit;
    QPushButton *sendButton;

};

#endif // CHATBOX_H
