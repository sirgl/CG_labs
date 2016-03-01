#include "MainWindow.h"
#include <QApplication>
#include <QDebug>
#include "CoreApplication.h"

const auto USAGE = "usage: <config file> [<output file>]";

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    argc = 3;
    char* argv1[] = {"", "1.json", "1.png"};
    argv = argv1;
    if (1 == argc) {
        MainWindow w;
        w.show();
        return a.exec();
    }
    if (3 == argc) {
        CoreApplication core;
        return core.exec(argc, argv);
    }
    qWarning() << USAGE;
    return EXIT_FAILURE;
}
