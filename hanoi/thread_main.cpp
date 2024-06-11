#include "hanoi.hpp"
#include "key.hpp"

#include <QApplication>

int main( int argc, char* argv[] )
{
  QApplication a( argc, argv );
  vas::hanoi w;
  KeyPressFilter filter( w.getStepBtn() );
  a.installEventFilter( &filter );
  w.init( 7 );
  w.show();
  return a.exec();
}
