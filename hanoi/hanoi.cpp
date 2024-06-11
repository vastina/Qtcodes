#include "hanoi.hpp"
#include <mutex>
#include <qpushbutton.h>

namespace vas {

void content_::reset( u16 level )
{
  poles[0].len = level;
  poles[1].len = poles[2].len = 0;
  for ( u16 i = 0; i < level; ++i ) {
    disks[i]->move( wWidth / 4 - poleWidth * ( i + 1 ), wHeight - diskLen * ( level - i ) );
  }
}

void Hanoier::moveone( u16 level, u16 from, u16 to )
{
  current++;
  --ctnt->poles[from].len;
  ++ctnt->poles[to].len;
  ctnt->disks[level]->move( ( wWidth / 4 ) * ( to + 1 ) - ctnt->disks[level]->width() / 2,
                            wHeight - ctnt->poles[to].len * diskLen );
}

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
      ;
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
    hanoier.Level,
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
  connect( content.step, &QPushButton::clicked, [this] { controler.cv = false; } );

  worker = CreateWorker();
  worker->start();
}
}
