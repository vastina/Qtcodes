#include "co.hpp"
#include <generator>
#include <qpushbutton.h>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent )
{
  this->setWindowTitle( "hnaoi" );
  this->resize( 600, 800 );
  QPalette palette;
  palette.setColor( QPalette::Window, QPalette::Dark );
  this->setPalette( palette );

  content.start = new QPushButton( "run", this );
  // todo, the run btn
  content.step = new QPushButton( "next", this );
  content.step->move( 500, 0 );

  content.poles = new pole_t[3];
  for ( int i = 0; i < 3; ++i ) {
    content.poles[i].pole = new QPushButton( " ", this );
    content.poles[i].pole->resize( 10, 400 );
    content.poles[i].pole->setText( QString( (QChar)( '1' + i ) ) );
    content.poles[i].pole->move( 150 - 10 / 2 + 150 * i, 400 );
  }
}

MainWindow::~MainWindow() {}

void MainWindow::init( u16 level )
{
  hanoier = Hanoier { 1, 0, level, &content };
  connect( content.step, &QPushButton::clicked, [this]() {
    // so todo, this is not what I want when use coroutine
    content.reset( hanoier.Level );
    if ( hanoier.totalstep >= 1UL << hanoier.Level ) {
      hanoier = { 1, 0, hanoier.Level, &content };
      return;
    }
    for ( auto b : hanoier() )
      if ( b )
        break;
    hanoier.totalstep++;
    hanoier.current = 0;
  } );

  // QPalette palette;
  // palette.setColor( QPalette::Window, QPalette::Light );

  content.disks.resize( level );
  for ( u16 i = 0; i < level; ++i ) {
    content.disks[i] = new QPushButton( QString( (QChar)( '1' + i ) ), this );
    content.disks[i]->resize( 20 * i + 20, 20 );
    ++content.poles[0].len;
  }
  content.reset( level );
}
