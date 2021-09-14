// 20210914
// DEMO project with Multi Threading, user object (messdateandtime in globalvars.h) sharing between threads
//
// *HARDWARE*
//
// NUCLEO-F767ZI with connected:
// * LCD 1602 with I2C (PCF8574), D14 <-> SDA; D15 <-> SCL
// * NODEMCU DEVKIT V0.9, WIFI; D1,TX <-> D9,RX; D0,RX <-> D10,TX; D2 <-> RST
// * RTC Module based on DS1302; D8 <-> RST; D12 <-> DATA; D13 <-> CLK; 
// 
// 

#include "globalvars.h"
#include "user-server-socket.h"
#include "uuclock.h"
#include "mbed.h"


Thread thread;
Thread thread2;
userdateandtime messdateandtime;

unsigned char tmpsec;

void user_server()
{
    userserversock();
}
void user_clock()
{
    uclock();
}

int main(void)
{
    thread.start(user_server);
    thread2.start(user_clock);
    bool init = false;
     while (true) 
    {
        if (tmpsec != messdateandtime.sec)
        {
            if (init)
                printf("Date and time: %s, %s, %d, %d  %d:%d:%d .\r\n", 
                            messdateandtime.weekday.c_str(), 
                            messdateandtime.smonth.c_str(), 
                            messdateandtime.day, 
                            messdateandtime.year + 2000, 
                            messdateandtime.hr, 
                            messdateandtime.minu, 
                            messdateandtime.sec);  
            init  = true;
        }
        tmpsec = messdateandtime.sec;
    }
}
