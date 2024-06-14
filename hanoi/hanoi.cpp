#include "hanoi.hpp"
#include <chrono>
#include <qpushbutton.h>
#include <qthread.h>
#include <thread>

namespace vas {

using namespace std::chrono_literals;

void content_::reset( u16 level )
{
  poles[0].len = level;
  poles[1].len = poles[2].len = 0;
  for ( u16 i = 0; i < level; ++i ) {
    disks[i]->move( wWidth / 4 - poleWidth * ( i + 1 ), wHeight - diskLen * ( level - i ) );
  }
}

void content_::moveone( u16 level, u16 from, u16 to )
{
  --poles[from].len;
  ++poles[to].len;
  disks[level]->move( ( wWidth / 4 ) * ( to + 1 ) - disks[level]->width() / 2,
                      wHeight - poles[to].len * diskLen );
}

void Hanoier::moveone( u16 level, u16 from, u16 to )
{
  current++;
  ctnt->moveone( level, from, to );
}

// QThread* Hanoier::CreateWorker( std::function<void( u16 level, u16 from, u16 to )> mover )
// {
//   return QThread::create(
//     [mover]( this auto self, u16 level, u16 from, u16 to ) {
//       if ( level == 0 ) {
//         mover( level, from, to );
//         return;
//       }
//       self( level - 1, from, TOTAL - from - to );
//       mover( level, from, to );
//       self( level - 1, TOTAL - from - to, to );
//     },
//     Level - 1,
//     POLE[0],
//     POLE[2] );
// }

hanoi::hanoi( QWidget* parent ) : QMainWindow( parent ), controler {}
{
  this->setWindowTitle( "hnaoi" );
  this->resize( wWidth, wHeight );

  QPalette palette;
  palette.setColor( QPalette::Window, QPalette::Dark );
  this->setPalette( palette );

  content.start = new QPushButton( "run", this );
  content.step = new QPushButton( "next", this );
  content.step->move( wWidth - 100, 0 );

  content.poles = new pole_t[3];
  for ( int i = 0; i < 3; ++i ) {
    content.poles[i].pole = new QPushButton( " ", this );
    content.poles[i].pole->resize( poleWidth, poleLen );
    content.poles[i].pole->setText( QString( (QChar)( '1' + i ) ) );
    content.poles[i].pole->move( ( wWidth / 4 ) * ( i + 1 ) - poleWidth / 2, poleLen );
  }
}

hanoi::~hanoi() {}

QThread* hanoi::CreateWorker()
{
  const auto move_and_stop { [this]( u16 level, u16 from, u16 to ) {
    while ( controler.cv )
      std::this_thread::sleep_for( 20ms );
    hanoier.moveone( level, from, to );
    controler.cv = true;
  } };
  return QThread::create(
    [move_and_stop]( this auto self, u16 level, u16 from, u16 to ) {
      if ( level == 0 ) {
        move_and_stop( level, from, to );
        return;
      }
      self( level - 1, from, TOTAL - from - to );
      move_and_stop( level, from, to );
      self( level - 1, TOTAL - from - to, to );
    },
    hanoier.Level - 1,
    POLE[0],
    POLE[2] );
}

void hanoi::init( u16 level )
{
  content.disks.resize( level );
  for ( u16 i = 0; i < level; ++i ) {
    content.disks[i] = new QPushButton( QString( (QChar)( '1' + i ) ), this );
    content.disks[i]->resize( diskLen * ( i + 1 ), diskLen );
  }
  content.reset( level );

  hanoier = Hanoier { 1, 0, &content, level };
  connect( content.step, &QPushButton::clicked, [this] {
    if ( runner )
      if ( runner->isRunning() )
        runner->quit();
    if ( hanoier.finished() ) {
      hanoier.reset();
      content.reset( hanoier.Level );
      worker = CreateWorker();
      worker->start();
      return;
    }
    controler.cv = false;
  } );
  connect( content.start, &QPushButton::clicked, [this] {
    // if ( runner )
    //   if ( runner->isRunning() )
    //     return;
    // it's not a bug, it's a feature
    // click this button multiple times will create multiple threads and speed up the process
    runner = QThread::create( [this] {
      while ( !hanoier.finished() ) {
        controler.cv = false;
        std::this_thread::sleep_for( 100ms );
      }
      return;
    } );
    runner->start();
  } );

  worker = CreateWorker();
  worker->start();
}
}
