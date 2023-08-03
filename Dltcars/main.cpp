#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create the main window
    MainWindow w;
    // Show the login dialog first
    w.hide(); // Hide the main window initially
    w.onLoginAction();

    return a.exec();
}
