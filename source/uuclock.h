// Connects NODEMCU WIFI to AP with Internet
// Gets date and time from worldtimeapi.org as JSON object
// Synchronizes date and time with RTC Module
// Reads date and time from RTC Module and displays it on LCD 1602 
// Copies date and time from RTC Module to messdateandtime share object between threads

int uclock(void);