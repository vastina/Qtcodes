#include "mainwindow.hpp"
#include <qpushbutton.h>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent )
{
  this->setWindowTitle( "????" );
  this->resize( 600, 800 );
  QPalette palette;
  palette.setColor( QPalette::Window, QPalette::Dark );
  this->setPalette( palette );

  start = new QPushButton( "run", this );
  // no move needed
  step = new QPushButton( "next", this );
  step->move( 500, 0 );

  poles = new pole_t[3];
  for ( int i = 0; i < 3; ++i ) {
    poles[i].pole = new QPushButton( " ", this );
    poles[i].pole->resize( 10, 400 );
    poles[i].pole->setText( QString( (QChar)( '1' + i ) ) );
    poles[i].pole->move( 150 - 10 / 2 + 150 * i, 400 );
  }
}

MainWindow::~MainWindow() {}

void MainWindow::init( unsigned short level )
{
  // connect(start, &QPushButton::clicked, [this, level](){
  //   this->stepflag = (1<<level) - 1;
  //   currentstep = 0;
  //   poles[0].flag = level;
  //   poles[1].flag = poles[2].flag = 0;
  //   this->run( level - 1, pole::ONE, pole::THREE );
  // });
  Level = level;
  connect( step, &QPushButton::clicked, [this]() {
    if ( Level <= 0 )
      return;
    if ( ++stepflag >= ( 1UL << Level ) )
      reset();
    currentstep = 0;
    poles[0].flag = Level;
    poles[1].flag = poles[2].flag = 0;
    this->run( Level - 1, pole::ONE, pole::THREE );
  } );

  QPalette palette;
  palette.setColor( QPalette::Window, QPalette::Light );

  disks.resize( Level );
  for ( unsigned short i = 0; i < Level; ++i ) {
    disks[i] = new QPushButton( QString( (QChar)( '1' + i ) ), this );
    disks[i]->resize( 20 * i + 20, 20 );
    disks[i]->move( 150 - 10 * i - 10, 800 - 20 * ( Level - i ) );
    ++poles[0].flag;
  }
  reset();
}

void MainWindow::reset()
{
  stepflag = 0;
  for ( unsigned short i = 0; i < Level; ++i )
    disks[i]->move( 150 - 10 * i - 10, 800 - 20 * ( Level - i ) );
}

void MainWindow::run( unsigned short level, unsigned short from, unsigned short to )
{
  if ( level == 0 ) {
    moveone( 0, from, to );
    return;
  }
  run( level - 1, from, total - from - to );
  moveone( level, from, to );
  run( level - 1, total - from - to, to );
}

void MainWindow::moveone( unsigned short level, unsigned short from, unsigned short to )
{
  if ( stepflag <= currentstep )
    return;
  ++currentstep;

  --poles[from].flag;
  ++poles[to].flag;
  disks[level]->move( 150 * ( to + 1 ) - disks[level]->size().width() / 2, 800 - poles[to].flag * 20 );
}
