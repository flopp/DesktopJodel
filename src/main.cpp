#include "mainWindow.h"
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setApplicationName("DesktopJodel");
    app.setOrganizationName("flopp.net");
    app.setOrganizationDomain("flopp.net");

    MainWindow w;
    w.show();

    return app.exec();
}
