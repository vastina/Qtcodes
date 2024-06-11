#ifndef _THREAD_HANOI_H_
#define _THREAD_HANOI_H_

#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QThread>
#include <array>
#include <atomic>
#include <cstdint>
#include <qalgorithms.h>
#include <qmainwindow.h>
#include <qobject.h>
#include <qtmetamacros.h>

namespace vas {

using u16 = std::uint16_t;
using u64 = std::uint64_t;

// struct config {
constexpr std::array<u16, 3> POLE { { 0, 1, 2 } };
constexpr u16 TOTAL { POLE[0] + POLE[1] + POLE[2] };
constexpr u16 wWidth { 600 };  // win width
constexpr u16 wHeight { 800 }; // win height
constexpr u16 poleWidth { 10 };
constexpr u16 poleLen { 400 };
constexpr u16 diskLen { 20 };
// }

typedef struct pole_t
{
  QPushButton* pole = nullptr;
  u16 len = 0;
} pole_t;

struct content_
{
  pole_t* poles { nullptr };
  QList<QPushButton*> disks {};
  QPushButton* start { nullptr };
  QPushButton* step { nullptr };

  void reset( u16 );
};

typedef struct Hanoier
{
  u64 aimstep {};
  u64 current {};
  content_* ctnt;
  u16 Level = -1;

  void moveone( u16 level, u16 from, u16 to );
} Hanoier;

class hanoi : public QMainWindow
{
  Q_OBJECT

private:
  content_ content {};
  Hanoier hanoier {};
  struct controler_
  {
    std::atomic_bool cv { true };
    // std::mutex mtx {};
  };
  controler_ controler;
  QThread* worker { nullptr };

public:
  hanoi( QWidget* parent = nullptr );
  ~hanoi();
  void init( u16 level );
  void reset( u16 );
  QThread* CreateWorker();
  inline QPushButton* getStepBtn() { return content.step; }
};
}

#endif