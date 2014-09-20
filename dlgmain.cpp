
#include <windows.h>
#include <QMessageBox>
#include <QSettings>
#include <QCryptographicHash>
#include <QDir>

#include "dlgmain.h"
#include "ui_dlgmain.h"
#include "dlgtimer.h"

dlgMain::dlgMain(QWidget *parent) : QDialog(parent), ui(new Ui::dlgMain)
{
    ui->setupUi(this);

    m_nTimer            = 0;
    m_bMousePressed     = false;

    setWindowFlags( Qt::Dialog | Qt::FramelessWindowHint );

    QSettings   obPref( "c:/ProgramData/ChildController/childcontroller.inf", QSettings::IniFormat );

    QDir    qdBackup( "c:/ProgramData/ChildController" );

    if( !qdBackup.exists() )
    {
        if( !qdBackup.mkpath( "c:/ProgramData/ChildController" ) )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("The following directory can not be created:\n\n"
                                     "c:/ProgramData/ChildController\n\n"
                                     "Please create the directory manually and restart process.") );
            close();
        }
    }

    QString     qsPassword = obPref.value( "AdminPassword", "" ).toString();

    trayIconMenu = new QMenu(this);
    trayIcon = new QSystemTrayIcon(this);

    _setActions();
    _setMenu();

    trayIcon->setIcon( QIcon( ":/childcontroller.png" ) );
    trayIcon->show();

    poDlgTimer = new dlgTimer();

    connect( poDlgTimer, SIGNAL(timerFinished()), this, SLOT(on_Timer_finished()) );
    connect( this, SIGNAL(signalStartCounter(int,int)), poDlgTimer, SLOT(counterStart(int,int)) );
    connect( this, SIGNAL(signalStopCounter()), poDlgTimer, SLOT(counterStop()) );
    connect( this, SIGNAL(signalResetCounter(int,int)), poDlgTimer, SLOT(counterReset(int,int)) );

    m_bAdminIdentified = false;

    _setControlsEnabled( m_bAdminIdentified );

    if( qsPassword.length() == 0 )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Please set the administrator password!") );
        ui->ledPassword2->setEnabled( true );
        show();
    }
}

dlgMain::~dlgMain()
{
    delete ui;
}

void dlgMain::setTimerLength(int p_nTimerLength)
{
    ui->ledTimerLength->setText( QString::number( p_nTimerLength ) );
}

void dlgMain::setExtendedLength(int p_nExtendedLength)
{
    ui->ledExtendedLength->setText( QString::number( p_nExtendedLength ) );
}

void dlgMain::setTimerStart(int p_nTimerStart)
{
    m_nTimer = startTimer( p_nTimerStart*1000 );
}

void dlgMain::timerEvent(QTimerEvent *)
{
    killTimer( m_nTimer );
    m_nTimer = 0;
    on_pbStart_clicked();
}

void dlgMain::mousePressEvent ( QMouseEvent *p_poEvent )
{
    m_bMousePressed = true;

    m_nMouseX = p_poEvent->pos().x();
    m_nMouseY = p_poEvent->pos().y();

    p_poEvent->accept();
}
void dlgMain::mouseReleaseEvent ( QMouseEvent *p_poEvent )
{
    m_bMousePressed = false;
    p_poEvent->accept();
}

void dlgMain::mouseMoveEvent ( QMouseEvent *p_poEvent )
{
    if( m_bMousePressed )
    {
        move( x() + p_poEvent->pos().x() - m_nMouseX,
              y() + p_poEvent->pos().y() - m_nMouseY );
    }
    p_poEvent->accept();
}

void dlgMain::_setActions()
{
    actionSettings = new QAction(tr("&Settings"), this);
    actionSettings->setIcon( QIcon( ":/settings.png" ) );
    connect( actionSettings, SIGNAL(triggered()), this, SLOT(slotSettings()) );

    actionShow = new QAction(tr("Show counter"), this);
    actionShow->setIcon( QIcon( ":/show.png" ) );
    connect( actionShow, SIGNAL(triggered()), this, SLOT(on_actionShow_triggered()) );

    actionStart = new QAction(tr("&Start"), this);
    actionStart->setIcon( QIcon( ":/start.png" ) );
    connect( actionStart, SIGNAL(triggered()), this, SLOT(on_pbStart_clicked()) );

    actionStop = new QAction(tr("S&top"), this);
    actionStop->setIcon( QIcon( ":/stop.png" ) );
    connect( actionStop, SIGNAL(triggered()), this, SLOT(on_pbStop_clicked()) );

    actionReset = new QAction(tr("&Reset"), this);
    actionReset->setIcon( QIcon( ":/reset.png" ) );
    connect( actionReset, SIGNAL(triggered()), this, SLOT(on_pbReset_clicked()) );

    actionLogoff = new QAction(tr("&Log off"), this);
    actionLogoff->setIcon( QIcon( ":/logout.png" ) );
    actionLogoff->setToolTip( tr("Log off Windows with forcing applications to close.") );
    connect( actionLogoff, SIGNAL(triggered()), this, SLOT(slotLogoff()) );

    actionShutdown = new QAction(tr("S&hutdown"), this);
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
    trayIconMenu->addAction( actionShow );
    trayIconMenu->addSeparator();

    trayIconMenu->addAction( actionStart );
    trayIconMenu->addAction( actionStop );
    trayIconMenu->addAction( actionReset );

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
    m_bAdminIdentified = false;
    _setControlsEnabled( m_bAdminIdentified );
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

void dlgMain::on_pbLogin_clicked()
{
    QSettings   obPref( "c:/ProgramData/ChildController/childcontroller.inf", QSettings::IniFormat );

    if( ui->ledPassword2->isEnabled() )
    {
        if( ui->ledPassword1->text().compare( ui->ledPassword2->text() ) != 0 )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("The two password did not match!") );
            return;
        }
        QByteArray  obPwdHash = QCryptographicHash::hash( ui->ledPassword1->text().toAscii(), QCryptographicHash::Sha1 );
        obPref.setValue( "AdminPassword", QString( obPwdHash.toHex() ) );
        m_bAdminIdentified = true;
    }
    else
    {
        QByteArray  obPwdHash = QCryptographicHash::hash( ui->ledPassword1->text().toAscii(), QCryptographicHash::Sha1 );
        QString     qsPassword = obPref.value( "AdminPassword", "7c01fcbe9cab6ae14c98c76cf943a7b2be6a7922" ).toString();

        if( qsPassword != QString( obPwdHash.toHex() ) )
        {
            QMessageBox::critical( this, tr("Error"),
                                   tr("Incorrect password!") );
        }
        else
        {
            m_bAdminIdentified = true;
        }
    }
    ui->ledPassword1->setText( "" );
    ui->ledPassword2->setText( "" );
    _setControlsEnabled( m_bAdminIdentified );
}

void dlgMain::on_pbModifyPassword_clicked()
{
    ui->ledPassword1->setText( "" );
    ui->ledPassword2->setText( "" );
    ui->ledPassword2->setEnabled( true );
    ui->pbCancel->setEnabled( true );
    ui->pbCancel->setVisible( true );
}

void dlgMain::on_pbStart_clicked()
{
    if( atoi( ui->ledTimerLength->text().toStdString().c_str() ) < 1 )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Length of counter must be greater than zero!"));
        return;
    }

    emit signalStartCounter( ui->ledTimerLength->text().toInt(),
                             ui->ledExtendedLength->text().toInt() );
    poDlgTimer->show();
}

void dlgMain::on_Timer_finished()
{
    m_bAdminIdentified = false;
    _setControlsEnabled( m_bAdminIdentified );
    if( ui->rbActionLogoffWindows->isChecked() )
    {
        slotLogoff();
    }
    else if( ui->rbActionMessageBox->isChecked() )
    {
        QMessageBox::information( this, tr("Information"), ui->ledMessage->text() );
    }
    else
    {
        QMessageBox::information( this, tr("Information"), tr("Your time is over!") );
    }
}

void dlgMain::on_pbHide_clicked()
{
    hide();
}

void dlgMain::on_pbStop_clicked()
{
    emit signalStopCounter();
}

void dlgMain::on_pbReset_clicked()
{
    if( atoi( ui->ledTimerLength->text().toStdString().c_str() ) < 1 )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Length of counter must be greater than zero!"));
        return;
    }

    emit signalResetCounter( ui->ledTimerLength->text().toInt(),
                             ui->ledExtendedLength->text().toInt() );
}

void dlgMain::_setControlsEnabled(bool p_bEnabled)
{
    ui->ledPassword2->setEnabled( false );
    ui->pbCancel->setEnabled( false );
    ui->pbCancel->setVisible( false );
    ui->pbModifyPassword->setEnabled( p_bEnabled );
    ui->ledTimerLength->setEnabled( p_bEnabled );
    ui->ledExtendedLength->setEnabled( p_bEnabled );
    ui->pbStart->setEnabled( p_bEnabled );
    ui->pbStop->setEnabled( p_bEnabled );
    ui->pbReset->setEnabled( p_bEnabled );
    ui->rbActionNothing->setEnabled( p_bEnabled );
    ui->rbActionLogoffWindows->setEnabled( p_bEnabled );
    ui->rbActionMessageBox->setEnabled( p_bEnabled );
    ui->ledMessage->setEnabled( p_bEnabled );

    actionStart->setEnabled( p_bEnabled );
    actionStop->setEnabled( p_bEnabled );
    actionReset->setEnabled( p_bEnabled );
    actionLogoff->setEnabled( p_bEnabled );
    actionExit->setEnabled( p_bEnabled );
}

void dlgMain::on_pbCancel_clicked()
{
    ui->ledPassword1->setText( "" );
    ui->ledPassword2->setText( "" );
    _setControlsEnabled( m_bAdminIdentified );
}

void dlgMain::on_actionShow_triggered()
{
    poDlgTimer->show();
}
