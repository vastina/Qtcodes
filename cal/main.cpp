#include "cal_ui.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    vastina::Calculator calc = vastina::Calculator(nullptr);
    calc.show();
    return app.exec();
}