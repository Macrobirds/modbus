#include<Timer.h>

MyTimer::MyTimer(ULONG time){
    currenttime=0;
    lasttime=0;
    timerstate=false;
    timeout=time;
}

MyTimer::~MyTimer(){

}

void MyTimer::timerenable(){
    timerstate=true;


}

void MyTimer::timerdisable(){
    timerstate=false;

}

bool MyTimer::timerpoll(){
    if(timerstate){
        currenttime=GetTickCount();
        if((currenttime-lasttime)>timeout){
            return true;
        }
    }else{
        return false;
    }
}

void MyTimer::settimerstate(bool state){
    timerstate=state;
}

void MyTimer::updatetimer(){
    lasttime=GetTickCount();
}
