#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <cstdint>
#include <iostream>
#include <generator>
#include <ranges>
#include <coroutine>
#include <qevent.h>
#include <qpushbutton.h>

using u16 = std::uint16_t;

enum pole
{
  ONE = 0,
  TWO,
  THREE
};
constexpr u16 total = pole::ONE + pole::TWO + pole::THREE;

class MainWindow : public QMainWindow
{
  Q_OBJECT

private:
  typedef struct pole_t
  {
    QPushButton* pole = nullptr;
    u16 len = 0;
  } pole_t;
  struct content_
  {
    pole_t* poles { nullptr };
    QList<QPushButton*> disks;
    QPushButton* start;
    QPushButton* step;

    void reset( u16 level )
    {
      poles[0].len = level;
      poles[1].len = poles[2].len = 0;
      for ( u16 i = 0; i < level; ++i ) {
        disks[i]->move( 150 - 10 * i - 10, 800 - 20 * ( level - i ) );
      }
    }
    
  };
  content_ content {};

  struct Hanoier
  {
    unsigned long long totalstep {};
    unsigned long long current {};
    u16 Level = -1;
    content_* ctnt;

    void moveone( u16 level, u16 from, u16 to )
    {
      current++;
      --ctnt->poles[from].len;
      ++ctnt->poles[to].len;
      ctnt->disks[level]->move( 150 * ( to + 1 ) - ctnt->disks[level]->size().width() / 2,
                                800 - ctnt->poles[to].len * 20 );
    }
    std::generator<bool> hanoi( u16 level, u16 from, u16 to )
    {
      if ( level == 0 ) {
        moveone( level, from, to );
        co_yield current >= totalstep;
        co_return;
      }

      co_yield std::ranges::elements_of(hanoi( level - 1, from, total - from - to ));

      moveone( level, from, to );
      co_yield current >= totalstep;

      co_yield std::ranges::elements_of(hanoi( level - 1, total - from - to, to ));

      if ( Level - 1 == level ) {
        co_yield true;
        co_return;
      }
    }
    auto operator()( pole from = pole::ONE, pole to = pole::THREE )
      { return hanoi( Level - 1, from, to ); }
  };
  Hanoier hanoier;

public:
  MainWindow( QWidget* parent = nullptr );
  ~MainWindow();
  void init( u16 level );
  inline QPushButton* getStepBtn() { return content.step; }
};

#endif // MAINWINDOW_H
