#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    //---Qt warning для быстрой отладки
    qputenv("QT_FATAL_WARNINGS", "1");

    MainWindow w;
    w.show();
    return app.exec();
}
