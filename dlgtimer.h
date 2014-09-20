#ifndef DLGTIMER_H
#define DLGTIMER_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
class dlgTimer;
}

class dlgTimer : public QDialog
{
    Q_OBJECT

public:
    explicit dlgTimer(QWidget *parent = 0);
    ~dlgTimer();

public slots:
    void counterStop();
    void counterReset(int p_nTimerLength, int p_nExtendedLength);
    void counterStart(int p_nTimerLength = 0 , int p_nExtendedLength = 0);

protected:
    void timerEvent( QTimerEvent *p_poEvent );
    void mousePressEvent ( QMouseEvent *p_poEvent );
    void mouseReleaseEvent ( QMouseEvent *p_poEvent );
    void mouseMoveEvent ( QMouseEvent *p_poEvent );
    void mouseDoubleClickEvent ( QMouseEvent *p_poEvent );

signals:
    void timerFinished();

private:
    Ui::dlgTimer *ui;

    bool    m_bMousePressed;
    int     m_nMouseX;
    int     m_nMouseY;
    int     m_nTimer;
    int     m_nTimerLength;
    int     m_nExtendedLength;

    void formatTimerString( int timer );
};

#endif // DLGTIMER_H
