#ifndef DLGMAIN_H
#define DLGMAIN_H

#include <QSystemTrayIcon>
#include <QDialog>
#include <QMenu>
#include <QAction>

namespace Ui { class dlgMain; }

class dlgMain : public QDialog
{
    Q_OBJECT

public:
    explicit dlgMain(QWidget *parent = 0);
    ~dlgMain();

public slots:

    void                 slotSettings();
    void                 slotLogoff();
    void                 slotShutdown();
    void                 slotReboot();

private:
    Ui::dlgMain         *ui;
    QSystemTrayIcon     *trayIcon;
    QMenu               *trayIconMenu;
    QAction             *actionSettings;
    QAction             *actionLogoff;
    QAction             *actionReboot;
    QAction             *actionShutdown;
    QAction             *actionExit;

    void                _setActions();
    void                _setMenu();
};

#endif // DLGMAIN_H
