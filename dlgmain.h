#ifndef DLGMAIN_H
#define DLGMAIN_H

#include <QSystemTrayIcon>
#include <QDialog>
#include <QMenu>
#include <QAction>
#include <QTranslator>

extern QTranslator     *poTransApp;
extern QTranslator     *poTransQT;
extern QApplication    *apMainApp;

#include "dlgtimer.h"

namespace Ui { class dlgMain; }

class dlgMain : public QDialog
{
    Q_OBJECT

public:
    explicit dlgMain(QWidget *parent = 0);
    ~dlgMain();
    void setTimerLength( int p_nTimerLength );
    void setExtendedLength( int p_nExtendedLength );
    void setTimerStart( int p_nTimerStart );

protected:
    void timerEvent( QTimerEvent *p_poEvent );
    void mousePressEvent ( QMouseEvent *p_poEvent );
    void mouseReleaseEvent ( QMouseEvent *p_poEvent );
    void mouseMoveEvent ( QMouseEvent *p_poEvent );

public slots:
    void slotSettings();
    void slotLogoff();
    void slotShutdown();
    void slotReboot();

signals:
    void signalStartCounter(int p_nTimerLength, int p_nExtendedLength);
    void signalStopCounter();
    void signalResetCounter(int p_nTimerLength, int p_nExtendedLength);

private slots:
    void on_pbLogin_clicked();
    void on_pbModifyPassword_clicked();
    void on_pbStart_clicked();
    void on_Timer_finished();
    void on_pbHide_clicked();
    void on_pbStop_clicked();
    void on_pbReset_clicked();
    void on_pbCancel_clicked();
    void on_actionShow_triggered();

    void on_pbExit_clicked();

private:
    dlgTimer            *poDlgTimer;
    Ui::dlgMain         *ui;
    QSystemTrayIcon     *trayIcon;
    QMenu               *trayIconMenu;
    QAction             *actionSettings;
    QAction             *actionShow;
    QAction             *actionStart;
    QAction             *actionStop;
    QAction             *actionReset;
    QAction             *actionLogoff;
    QAction             *actionReboot;
    QAction             *actionShutdown;
    QAction             *actionExit;
    bool                 m_bAdminIdentified;
    int                  m_nTimer;
    bool                 m_bMousePressed;
    int                  m_nMouseX;
    int                  m_nMouseY;

    void                _setActions();
    void                _setMenu();
    void                _setControlsEnabled( bool p_bEnabled );
};

#endif // DLGMAIN_H
