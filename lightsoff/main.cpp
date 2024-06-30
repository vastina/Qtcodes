#include "lightsoff.hpp"

#include <QApplication>
#include <fstream>

auto readPuzzleConfig( const char* filename )
{
  std::vector<std::array<std::array<bool, 5>, 5>> puzzles {};

  std::ifstream ifs { filename };
  if ( ifs )
    while ( !ifs.eof() ) {
      std::array<std::array<bool, 5>, 5> puzzle {};
      for ( int i = 0; i < 5; ++i ) {
        for ( int j = 0; j < 5; ++j ) {
          ifs >> puzzle[i][j];
          // should have check here
        }
      }
      puzzles.push_back( puzzle );
    }
  ifs.close();

  return puzzles;
}

int main( int argc, char* argv[] )
{
  QApplication app( argc, argv );
  auto puzzles { readPuzzleConfig( argc > 1 ? argv[1] : "LightsOffPuzzles.txt" ) };
  vas::GameWindow window { puzzles, nullptr };
  window.show();
  return app.exec();
}
