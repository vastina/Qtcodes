#include <QApplication>
#include <QAudioDevice>
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QMediaDevices>
#include <QPushButton>
#include <QFileDialog>
#include <QVideoWidget>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListView>

int main( int argc, char** argv )
{
  QApplication app( argc, argv );

  auto wd = QWidget( nullptr );
  {
    wd.resize( 1280, 750 );
  }
  auto player = new QMediaPlayer( &wd );
  auto ao = new QAudioOutput( &wd );
  auto vw = new QVideoWidget( &wd );
  {
    player->setAudioOutput( ao );
    vw->resize( 1280, 720 );
    player->setVideoOutput( vw );
  }

  QBoxLayout* layout = new QVBoxLayout( &wd );
  QBoxLayout* displayLayout = new QHBoxLayout;
  QHBoxLayout* hLayout = new QHBoxLayout;
  QBoxLayout* controlLayout = new QHBoxLayout;
  auto slider = new QSlider( Qt::Horizontal, &wd );
  auto openButton = new QPushButton( "open", &wd );
  {
    displayLayout->addWidget( vw );
    layout->addLayout( displayLayout );

    hLayout->addWidget( slider );
    layout->addLayout( hLayout );

    controlLayout->addWidget( openButton );
    controlLayout->addStretch( 1 );
    layout->addLayout( controlLayout );
  }

  QObject::connect( openButton, &QPushButton::clicked, &wd, [&wd, player, slider] {
    QFileDialog dialog( &wd );
    dialog.setWindowTitle( "Open Videos" );
    dialog.setFileMode( QFileDialog::ExistingFile );
    dialog.setNameFilter( QObject::tr( "*.mp4" ) );
    QStringList filePaths;
    if ( dialog.exec() ) {
      filePaths = dialog.selectedFiles();
    } else
      return;

    player->setSource( filePaths.at( 0 ) );
    player->play();
  } );
  QObject::connect( player, &QMediaPlayer::durationChanged, [slider]( int64_t duration ) {
    slider->setRange( 0, duration );
  } );
  QObject::connect(
    slider, &QSlider::sliderMoved, &wd, [player]( int msSec ) { player->setPosition( msSec ); } );

  wd.show();

  return QApplication::exec();
}
