#include "dlgtimer.h"
#include "ui_dlgtimer.h"

dlgTimer::dlgTimer(QWidget *parent) : QDialog(parent), ui(new Ui::dlgTimer)
{
    ui->setupUi(this);

    setWindowFlags( Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );

    m_bMousePressed     = false;
    m_nTimerLength      = 0;
    m_nExtendedLength   = 0;
    m_nTimer            = 0;

    setStyleSheet( "background-color: rgb(0, 170, 0);" );
}

dlgTimer::~dlgTimer()
{
    delete ui;
}

void dlgTimer::counterStop()
{
    killTimer( m_nTimer );
    m_nTimer = 0;
}

void dlgTimer::counterReset( int p_nTimerLength, int p_nExtendedLength )
{
    m_nTimerLength      = p_nTimerLength * 60;
    m_nExtendedLength   = p_nExtendedLength;
    formatTimerString( m_nTimerLength );
}

void dlgTimer::counterStart( int p_nTimerLength, int p_nExtendedLength )
{
    if( m_nTimerLength == 0 && m_nExtendedLength == 0 )
    {
        m_nTimerLength      = p_nTimerLength * 60;
        m_nExtendedLength   = p_nExtendedLength;
        formatTimerString( m_nTimerLength );
    }
    if( m_nTimer == 0 )
    {
        m_nTimer = startTimer( 1000 );
    }
}

void dlgTimer::timerEvent(QTimerEvent *)
{
    if( m_nTimerLength > 0 )
    {
        m_nTimerLength--;
        formatTimerString( m_nTimerLength );

        if( m_nTimerLength == 0 )
        {
            setStyleSheet( "background-color: rgb(170, 0, 0);" );
            formatTimerString( m_nExtendedLength );
        }
    }
    else
    {
        if( m_nExtendedLength > 0 )
        {
            m_nExtendedLength--;
            formatTimerString( m_nExtendedLength );
        }
        else
        {
            killTimer( m_nTimer );
            m_nTimer = 0;
            hide();
            emit timerFinished();
        }
    }
}

void dlgTimer::mousePressEvent ( QMouseEvent *p_poEvent )
{
    m_bMousePressed = true;

    m_nMouseX = p_poEvent->pos().x();
    m_nMouseY = p_poEvent->pos().y();

    p_poEvent->accept();
}
void dlgTimer::mouseReleaseEvent ( QMouseEvent *p_poEvent )
{
    m_bMousePressed = false;
    p_poEvent->accept();
}

void dlgTimer::mouseMoveEvent ( QMouseEvent *p_poEvent )
{
    if( m_bMousePressed )
    {
        move( x() + p_poEvent->pos().x() - m_nMouseX,
              y() + p_poEvent->pos().y() - m_nMouseY );
    }
    p_poEvent->accept();
}

void dlgTimer::mouseDoubleClickEvent ( QMouseEvent *p_poEvent )
{
//    hide();
    p_poEvent->accept();
}

void dlgTimer::formatTimerString(int timer)
{
    if( m_nTimerLength > 0 )
    {
        if( m_nTimerLength > 59 )
        {
            setStyleSheet( "background-color: rgb(0, 170, 0);" );
        }
        else
        {
            setStyleSheet( "background-color: rgb(255, 128, 0);" );
        }
    }
    else
    {
        setStyleSheet( "background-color: rgb(170, 0, 0);" );
    }

    ui->lblCounter->setText( QString( QString( "%1:%2:%3" ).arg( timer / 3600, 2, 10, QChar( '0' ) )
                                                           .arg( ( timer % 3600 ) / 60, 2, 10, QChar( '0' ) )
                                                           .arg( ( timer % 3600 ) % 60, 2, 10, QChar( '0' ) ) ) );
}
