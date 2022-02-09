#ifndef TIMER_H
#define TIMER_H


#include<windows.h>
typedef unsigned long ULONG;



class MyTimer{
private:
    ULONG currenttime;
    ULONG lasttime;
    bool timerstate;
    ULONG timeout;
public:
    MyTimer(ULONG);
    ~MyTimer();
    bool timerpoll();
    void timerenable();
    void timerdisable();
    void settimerstate(bool state);
    void updatetimer();


};

#endif // TIMER_H
