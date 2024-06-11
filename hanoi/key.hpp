#ifndef _KEY_PRESS_H_
#define _KEY_PRESS_H_

#include <QObject>
#include <QPushButton>
#include <QEvent>
#include <QKeyEvent>
#include <qobject.h>

class KeyPressFilter : public QObject
{
public:
  explicit KeyPressFilter( QPushButton* button ) : QObject(nullptr), button( button ) {}
  virtual ~KeyPressFilter() {};

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

#endif