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
#include <QLabel>

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
  auto labelDuration = new QLabel( &wd );
  auto openButton = new QPushButton( "open", &wd );
  {
    displayLayout->addWidget( vw );
    layout->addLayout( displayLayout );

    hLayout->addWidget( slider );
    labelDuration->setText( "00:00 / 00:00" );
    labelDuration->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );
    hLayout->addWidget( labelDuration );
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
  int64_t currentDuration {};
  QObject::connect(
    player, &QMediaPlayer::durationChanged, [slider, &currentDuration]( int64_t duration ) {
      slider->setRange( 0, duration );
      slider->setValue( 0 );

      currentDuration = duration;
    } );
  QObject::connect(
    slider, &QSlider::sliderMoved, &wd, [player]( int msSec ) { player->setPosition( msSec ); } );
  QObject::connect(
    player,
    &QMediaPlayer::positionChanged,
    &wd,
    [slider, labelDuration, &currentDuration]( int64_t MS ) {
      if ( not slider->isSliderDown() )
        slider->setValue( MS );

      int64_t Sec = MS / 1000;
      int64_t currentDurationSec = currentDuration / 1000;
      labelDuration->setText(
        QTime( ( Sec / 3600 ), ( Sec / 60 ) % 60, Sec % 60 )
          .toString( currentDurationSec > 3600 ? "hh:mm:ss" : "mm:ss" )
        + " / "
        + QTime( ( currentDurationSec / 3600 ), ( currentDurationSec / 60 ) % 60, currentDurationSec % 60 )
            .toString( currentDurationSec > 3600 ? "hh:mm:ss" : "mm:ss" ) );
    } );

  wd.show();

  return QApplication::exec();
}
