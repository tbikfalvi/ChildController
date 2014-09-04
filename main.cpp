#include <QApplication>
#include <QtGui>

#include "dlgmain.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(childcontroller);

    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        QMessageBox::critical( 0, QObject::tr("Systray"),
                               QObject::tr("I couldn't detect any system tray on this system."));
        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);

    dlgMain obDlgMain;

//    obDlgMain.show();

    return app.exec();
}
