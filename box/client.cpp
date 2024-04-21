#include "client.hpp"
#include "tools.h"
#include "loger.hpp"
#include "loger.hpp"

#include <errno.h>
#include <format>
#include <QThread>
#include <qwidget.h>
#include <string>
#include <cstring>

#include <QListWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

namespace vastina {

client::client(QWidget *parent): QWidget(parent)
{
    // 设置布局为垂直布局
    layout = new QVBoxLayout(this);
    setLayout(layout);
    // 创建聊天列表控件
    chatListWidget = new QListWidget(this);
    layout->addWidget(chatListWidget);
    // 创建输入框和发送按钮
    inputLineEdit = new QLineEdit(this);
    
    sendButton = new QPushButton("Send", this);
    // 连接发送按钮的点击事件到槽函数
    connect(sendButton, &QPushButton::clicked, this, &client::sendMessage);
    connect(inputLineEdit, &QLineEdit::returnPressed, this, &client::onInputReturnPressed);
    // 将输入框和发送按钮添加到布局中
    layout->addWidget(inputLineEdit);
    layout->addWidget(sendButton);
    // 设置滚动条策略为总是显示
    chatListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    // 设置不可编辑
    chatListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 设置选择模式为不可选中
    chatListWidget->setSelectionMode(QAbstractItemView::NoSelection);

    t_read = QThread::create([this]{this->reader();});
    //t_send = QThread::create([this]{this->sender();});
    t_read->start();
    //t_send->start();
}

void client::sender(){
    // while(true){
    //     print("\nsend message> ");
    //     std::getline(std::cin, sendbuffer_);
    //     {
    //         if(strcmp(sendbuffer_.data(), EXIT_STR.data()) == 0)
    //         {
    //             std::unique_lock<std::mutex> lock(stop_.smutex);
    //             stop_.stop = true;
    //         }
    //     }
    //     if(stop_.stop) return;
    //     SendMsg(sendbuffer_.data());
    //     sendbuffer_.clear();
    //     showtime();
    // }    
}

void client::reader(){
    while (true) {
        if(ReciveMsg()){
            DealMsg();
        }
        {
            std::unique_lock<std::mutex> lock(stop_.smutex);
            if(stop_.stop) break;
        }
    }
}



void client::SendMsg(const QString& msg){
//qDebug() << msg <<' ' <<msg.size() <<'\n';
    const QByteArray data = msg.toUtf8(); // 或者使用 toLocal8Bit()
    if (-1 == write(clientsock_, data.constData(), data.size()))
    {
        Submit([]{
            vastina::logtest(std::format("fail to send with code: {}\n", errno).c_str());
        }, IMPORTANCE::common);
    } 
}

bool client::ReciveMsg(){
    int bufferlen = -1;
    {
        if( (bufferlen = read(clientsock_, (void*)readbuffer_, sizeof(readbuffer_))) > 0)
        {
            readbuffer_[bufferlen] = '\0';
        }
    }
    if(bufferlen < 0){
        Submit([]{
            vastina::logtest(std::format("read error, {}\n", errno).c_str());
        }, IMPORTANCE::common);
        return false;
    }
    return true;
}

void client::DealMsg(){
    DrawMsg(std::format("recive message: {} \t", readbuffer_));
    memset(readbuffer_, 0, sizeof(readbuffer_));
}

void client::DrawMsg(std::string message){
    gettimebystring(message);
    addMessage(message.data());
}

void client::addMessage(const QString& message){
    // 创建新的 QListWidgetItem
    auto item = new QListWidgetItem(message);
    item->setTextAlignment(Qt::AlignLeft); // 文字左对齐
    // 添加到聊天框
    chatListWidget->addItem(item);
    // 滚动到底部
    chatListWidget->scrollToBottom();
}

void client::sendMessage(){
    QString message = inputLineEdit->text();
    SendMsg(message);
    if (!message.isEmpty())
    {
        //addMessage(message);
        inputLineEdit->clear();
    }
}

void client::onInputReturnPressed()
{
    sendButton->click();
}

void client::Init(){
    memset(readbuffer_, 0, sizeof(readbuffer_));
    //memset(sendbuffer_, 0, sizeof(sendbuffer_));
    //sendbuffer_.clear();
    taskpool_.start();
}

void client::setClientSock(int domain,int type,int protocol, short port){
    clientsock_ = CreateClientSocket(domain, type, protocol, port);

    if(clientsock_ == -1){
        errorhandling("clientsock_ init error, code: {}\n", errno);
        Submit([]{
            vastina::logtest("set sock fail");
        }, IMPORTANCE::common);
    }   
}

void client::Connect(int domain , unsigned short port){
    struct sockaddr_in serveaddr;
    memset(&serveaddr, 0, sizeof(serveaddr));
    serveaddr.sin_family = domain;
    serveaddr.sin_addr.s_addr = inet_addr(localaddr);
	serveaddr.sin_port = htons(port) ;
    if(::connect(clientsock_, (struct sockaddr*)&serveaddr, sizeof(serveaddr)) == -1){
        Submit([]{
            vastina::logtest(std::format("connect fail {}\n", errno).c_str());
        }, IMPORTANCE::common);
    }
}

template<typename F, typename...Args>
void client::Submit(F&& f, Args&&... args, IMPORTANCE level){
    if(level == IMPORTANCE::maintk){
        //todo
    }
    else taskpool_.submittask(f, args..., level);
}

};