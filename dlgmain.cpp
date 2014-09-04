
#include <windows.h>

#include "dlgmain.h"
#include "ui_dlgmain.h"

dlgMain::dlgMain(QWidget *parent) : QDialog(parent), ui(new Ui::dlgMain)
{
    ui->setupUi(this);

    trayIconMenu = new QMenu(this);
    trayIcon = new QSystemTrayIcon(this);

    _setActions();
    _setMenu();

    trayIcon->setIcon( QIcon( ":/childcontroller.png" ) );
    trayIcon->show();
}

dlgMain::~dlgMain()
{
    delete ui;
}

void dlgMain::_setActions()
{
    actionSettings = new QAction(tr("&Settings"), this);
    actionSettings->setIcon( QIcon( ":/settings.png" ) );
    connect( actionSettings, SIGNAL(triggered()), this, SLOT(slotSettings()) );

    actionLogoff = new QAction(tr("&Log off"), this);
    actionLogoff->setIcon( QIcon( ":/logout.png" ) );
    connect( actionLogoff, SIGNAL(triggered()), this, SLOT(slotLogoff()) );

    actionShutdown = new QAction(tr("&Shutdown"), this);
    actionShutdown->setIcon( QIcon( ":/shutdown.png" ) );
    connect( actionShutdown, SIGNAL(triggered()), this, SLOT(slotShutdown()) );

    actionReboot = new QAction(tr("&Reboot"), this);
    actionReboot->setIcon( QIcon( ":/reboot.png" ) );
    connect( actionReboot, SIGNAL(triggered()), this, SLOT(slotReboot()) );

    actionExit = new QAction(tr("&Exit application"), this);
    actionExit->setIcon( QIcon( ":/exit.png" ) );
    connect( actionExit, SIGNAL(triggered()), qApp, SLOT(quit()) );
}

void dlgMain::_setMenu()
{
    trayIconMenu->addAction( actionSettings );

    trayIconMenu->addSeparator();

    trayIconMenu->addAction( actionLogoff );
//    trayIconMenu->addAction( actionShutdown );
//    trayIconMenu->addAction( actionReboot );

    trayIconMenu->addSeparator();

    trayIconMenu->addAction( actionExit );

    trayIcon->setContextMenu(trayIconMenu);
}

void dlgMain::slotSettings()
{
    show();
}

void dlgMain::slotLogoff()
{
    ExitWindowsEx( EWX_LOGOFF, 1 );
}

void dlgMain::slotShutdown()
{
    ExitWindowsEx( EWX_SHUTDOWN, 1 );
}

void dlgMain::slotReboot()
{
    ExitWindowsEx( EWX_REBOOT, 1 );
}
