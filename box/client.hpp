#ifndef _CLIENT_SOCK_H_
#define _CLIENT_SOCK_H_

#include <cstdio>
#include <qobject.h>
#include <qthread.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <arpa/inet.h>
#include <mutex>

#include <QListWidget>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QPushButton>

#include "Taskpool.hpp"

namespace vastina {

static const char* localaddr = "127.0.0.1";

class client : public QWidget
{
    Q_OBJECT
private:
    int clientsock_;
    char readbuffer_[BUFSIZ];
    //std::string sendbuffer_;
    //char sendbuffer_[BUFSIZ];
    Taskpool taskpool_;

    typedef struct stop{
        bool stop{false};
        std::mutex smutex;
    } stop;
    stop stop_;

private slots:
    // 发送按钮点击事件的槽函数
    void sendMessage();
    // 处理输入框中按下回车键的槽函数
    void onInputReturnPressed();

private:
    QVBoxLayout *layout;
    QListWidget *chatListWidget;
    QLineEdit *inputLineEdit;
    QPushButton *sendButton;

    QThread *t_read;
    //QThread *t_send;

public:
    explicit client(QWidget *parent = nullptr);
    ~client(){
        taskpool_.setstoped();
        taskpool_.lastwork();
        ::close(clientsock_);
    }

    void Init();
    void setClientSock(int domain,int type,int protocol, short port = -1);
    void Connect(int damain, unsigned short port);
    template<typename F, typename...Args>
        void Submit(F&& f, Args&&... args, IMPORTANCE level);

    void SendMsg(const QString&);
    bool ReciveMsg();
    void DealMsg();
    void DrawMsg(std::string);
    void addMessage(const QString&);
    
    void sender();
    void reader();

};

}

#endif