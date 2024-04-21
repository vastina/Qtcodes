#include <QApplication>
#include <QThread>



#include "client.hpp"
#include "loger.hpp"

constexpr unsigned short PORT{1689};

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    

    vastina::logtest("start");

    vastina::client *client_ = new vastina::client();
    
    client_->setClientSock(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    client_->Connect(AF_INET, PORT);
    client_->Init(); 

    client_->setBaseSize(800, 600);
    client_->show();
    return app.exec();
}

// #include "box.hpp"

// int main(int argc, char *argv[]){
//     QApplication app(argc, argv);
//     ChatBox chatbox;
//     chatbox.show();
//     return app.exec();
// }