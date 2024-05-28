#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <qevent.h>
#include <qpushbutton.h>

enum pole
{
  ONE = 0,
  TWO,
  THREE
};
constexpr unsigned short total = pole::ONE + pole::TWO + pole::THREE;

class KeyPressFilter : public QObject
{
  Q_OBJECT

public:
  explicit KeyPressFilter( QPushButton* button ) : button( button ) {}

protected:
  bool eventFilter( QObject* obj, QEvent* event ) override
  {
    if ( event->type() == QEvent::KeyPress ) {
      QKeyEvent* keyEvent = static_cast<QKeyEvent*>( event );
      if ( keyEvent->key() == Qt::Key_Plus ) {
        Q_EMIT button->clicked();
        return true;
      }
    }
    return QObject::eventFilter( obj, event );
  }

private:
  QPushButton* button;
};

class MainWindow : public QMainWindow
{
  Q_OBJECT

private:
  unsigned long long stepflag = 0;
  unsigned long long currentstep = 0;
  unsigned short Level = -1;

  typedef struct pole_t
  {
    QPushButton* pole = nullptr;
    unsigned short flag = 0;
  } pole_t;
  pole_t* poles = nullptr;

  QList<QPushButton*> disks;
  QPushButton* start;
  QPushButton* step;

public:
  MainWindow( QWidget* parent = nullptr );
  ~MainWindow();
  void init( unsigned short level );
  inline QPushButton* getStepBtn() { return step; }

private:
  void run( unsigned short level, unsigned short from, unsigned short to );
  void moveone( unsigned short level, unsigned short from, unsigned short to );
  void reset();
};

#endif // MAINWINDOW_H
