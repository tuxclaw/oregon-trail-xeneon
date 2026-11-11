#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Oregon Trail - Xeneon Edge");
    app.setOrganizationName("tuxclaw");

    MainWindow window;
    window.show();

    return app.exec();
}
