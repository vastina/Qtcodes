#include "lightsoff.hpp"

#include <random>

namespace vas {

GameWindow::GameWindow( const std::vector<std::array<std::array<bool, 5>, 5>>& puzzles, QWidget* parent )
  : QWidget( parent ), puzzles_( puzzles )
{
  // this is by copilot
  // 创建主布局
  QVBoxLayout* mainLayout = new QVBoxLayout( this );
  // 创建水平布局包含关卡按钮、关卡显示和菜单按钮
  QHBoxLayout* headerLayout = new QHBoxLayout();

  whichPuzzle_->setAlignment( Qt::AlignCenter );

  // 将元素添加到水平布局中
  headerLayout->addWidget( changePuzzle_ );
  headerLayout->addStretch(); // 添加拉伸元素使关卡显示标签居中
  headerLayout->addWidget( whichPuzzle_ );
  headerLayout->addStretch(); // 添加拉伸元素使积分和菜单按钮靠右
  headerLayout->addWidget( timeLabel_ );
  headerLayout->addWidget( menuBtn_ );
  // 将水平布局添加到主布局中
  mainLayout->addLayout( headerLayout );

  // 创建5x5网格按钮布局
  QGridLayout* gridLayout = new QGridLayout();
  gridLayout->setSpacing( 5 ); // 设置格子之间的间距
  // 添加网格布局到主布局
  InitBoard( gridLayout );
  mainLayout->addLayout( gridLayout );

  // 设置窗口大小和标题
  setFixedSize( 400, 400 );
  setWindowTitle( "Lights Off" );
}

void GameWindow::InitCallback( const int x, const int y )
{
  QObject::connect( board_[x][y], &QPushButton::clicked, [this, x, y] {
    //
    timeLabel_->setText( QString::number( timeLabel_->text().toInt() + 1 ) );
    //
    color_[x][y] = !color_[x][y];
    board_[x][y]->setStyleSheet( color_[x][y] ? redstyle.data() : bluestyle.data() );
    if ( x > 0 ) {
      color_[x - 1][y] = !color_[x - 1][y];
      board_[x - 1][y]->setStyleSheet( color_[x - 1][y] ? redstyle.data() : bluestyle.data() );
    }
    if ( x < 4 ) {
      color_[x + 1][y] = !color_[x + 1][y];
      board_[x + 1][y]->setStyleSheet( color_[x + 1][y] ? redstyle.data() : bluestyle.data() );
    }
    if ( y > 0 ) {
      color_[x][y - 1] = !color_[x][y - 1];
      board_[x][y - 1]->setStyleSheet( color_[x][y - 1] ? redstyle.data() : bluestyle.data() );
    }
    if ( y < 4 ) {
      color_[x][y + 1] = !color_[x][y + 1];
      board_[x][y + 1]->setStyleSheet( color_[x][y + 1] ? redstyle.data() : bluestyle.data() );
    }
    //
    if ( Success() ) {
      Nextpuzzle();
    }
  } );
}

void GameWindow::InitBoard( QGridLayout* gridLayout )
{
  for ( int i = 0; i < 5; ++i ) {
    for ( int j = 0; j < 5; ++j ) {
      board_[i][j] = new QPushButton( this );
      board_[i][j]->setFixedSize( 50, 50 );
      color_[i][j] = puzzles_[puzzleIndex_][i][j];
      board_[i][j]->setStyleSheet( color_[i][j] ? redstyle.data() : bluestyle.data() );
      InitCallback( i, j );
      gridLayout->addWidget( board_[i][j], i, j );
    }
  }
}

std::mt19937 gen { std::random_device {}() };
std::uniform_int_distribution<int> dist( 0, 4 );
auto rd { std::bind( dist, gen ) };
static void GenRandomPuzzle( bool puzzle[5][5] )
{
  int step = rd() * 5 + 1;
  for(int k = 0; k < step; ++k) {
    int x = rd();
    int y = rd();
    puzzle[x][y] = !puzzle[x][y];
    if ( x > 0 ) {
      puzzle[x - 1][y] = !puzzle[x - 1][y];
    }
    if ( x < 4 ) {
      puzzle[x + 1][y] = !puzzle[x + 1][y];
    }
    if ( y > 0 ) {
      puzzle[x][y - 1] = !puzzle[x][y - 1];
    }
    if ( y < 4 ) {
      puzzle[x][y + 1] = !puzzle[x][y + 1];
    }
  }
}

void GameWindow::Nextpuzzle()
{
  if ( ( ++puzzleIndex_ ) >= puzzles_.size() ) {
    GenRandomPuzzle( color_ );
  } else {
    for ( int i = 0; i < 5; ++i )
      for ( int j = 0; j < 5; ++j )
        color_[i][j] = puzzles_[puzzleIndex_][i][j];
  }
  for ( int i = 0; i < 5; ++i ) {
    for ( int j = 0; j < 5; ++j ) {
      board_[i][j]->setStyleSheet( color_[i][j] ? redstyle.data() : bluestyle.data() );
    }
  }
  whichPuzzle_->setText( QString( "Puzzle " ).append( QString( std::to_string( puzzleIndex_ + 1 ).data() ) ) );
  timeLabel_->setText( "0" );
}

void GameWindow::menuBtnCallback()
{
  QObject::connect( menuBtn_, &QPushButton::clicked, [] {
    // do something
  } );
}

void GameWindow::changePuzzleCallback()
{
  QObject::connect( changePuzzle_, &QPushButton::clicked, [] {
    // do something
  } );
}

bool GameWindow::Success()
{
  for ( int i = 0; i < 5; ++i ) {
    for ( int j = 0; j < 5; ++j ) {
      if ( !color_[i][j] ) {
        return false;
      }
    }
  }
  return true;
}

} // namespace vas
