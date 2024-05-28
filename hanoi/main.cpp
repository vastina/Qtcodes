#include "mainwindow.hpp"

#include <QApplication>

int main( int argc, char* argv[] )
{
  QApplication a( argc, argv );
  MainWindow w;
  KeyPressFilter filter( w.getStepBtn() );
  a.installEventFilter( &filter );
  w.init( 9 );
  w.show();
  return a.exec();
}
