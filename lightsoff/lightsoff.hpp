#ifndef _LIGHTS_OFF_H_
#define _LIGHTS_OFF_H_

#include <QPushButton>
#include <QLabel>
#include <QGridLayout>

#include <array>
#include <vector>

namespace vas {

constexpr static std::string_view bluestyle { "background-color: darkblue;"
                                              "width: 50px;"
                                              "height: 50px;" };
constexpr static std::string_view redstyle { "background-color: red;"
                                             "width: 50px;"
                                             "height: 50px;" };

class GameWindow : public QWidget
{
  Q_OBJECT

private:
  QPushButton* menuBtn_ { new QPushButton( "☰", this ) };
  QPushButton* changePuzzle_ { new QPushButton( "Change Puzzle", this ) };
  QLabel* whichPuzzle_ { new QLabel( "Puzzle 1", this ) };
  QLabel* timeLabel_ { new QLabel( "0", this ) }; // how many times you click in one puzzle
private:
  void menuBtnCallback();
  void changePuzzleCallback();

private:
  QPushButton* board_[5][5] {};
  bool color_[5][5] {};
  void InitCallback( const int, const int );
  void InitBoard( QGridLayout* );

private:
  unsigned puzzleIndex_ { 0 };
  const std::vector<std::array<std::array<bool, 5>, 5>>& puzzles_;
  bool Success();
  void Nextpuzzle();
// I need to define a save game method

public:
  GameWindow( const std::vector<std::array<std::array<bool, 5>, 5>>&, QWidget* = nullptr );
};

}

#endif