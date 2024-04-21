#include <QApplication>
#include <QWidget>
#include <QObject>
#include <iostream>


QEvent::Type t1=(QEvent::Type)QEvent::registerEventType(1333);
QEvent e1(t1); //使用 QEvent 的构造函数创建自定义事件

//t2 的值与 t1 重复，使用 registerEventType 会自动产生一个合法的值
QEvent::Type t2=(QEvent::Type)QEvent::registerEventType(20000);
QEvent e2(t2);

class A:public QWidget{
public:

    inline bool event(QEvent* e){
        if(e->type()==t1) {
            std::cout<<"A event "<<e->type()<<",";
            f1((QEvent*)e);
            return true;
        }

        if(e->type()==t2) {
            std::cout<<"BE"<<e->type()<<",";
            f2((QEvent*)e);
            return true;
        }

        return QWidget::event(e);
    } //event 结束

    inline void f1(QEvent*){std::cout<<"f1 deal event"<<std::endl;}
    inline void f2(QEvent*){std::cout<<"F2 deal event"<<std::endl;}
};
class B:public QObject{
public:

    inline bool eventFilter(QObject*, QEvent *e){
        if(e->type()==t1){
            std::cout<<"A"<<std::endl;
            return 0;
        }// 0 is valid

        if(e->type()==t2){
            std::cout<<"B"<<std::endl;
            return 1;
        }//send 1 will be ignored

        return 0;
    }
};

int main(int argc, char *argv[]){ 
    QApplication aa(argc,argv);
    A ma; B mb;
    ma.installEventFilter(&mb); //安装事件过滤器
    aa.sendEvent(&ma,&e1);
    aa.sendEvent(&ma,&e2);
    ma.resize(333,222);
    ma.show();
    aa.exec();
    return 0; }