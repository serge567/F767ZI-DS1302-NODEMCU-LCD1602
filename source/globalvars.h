// messdateandtime is share object between threads

#include <string>
typedef struct {
            unsigned char day;
            unsigned char mth;
            unsigned char year;
            unsigned char dow;
            unsigned char hr;
            unsigned char minu;
            unsigned char sec;
            std::string smonth;
            std::string weekday;
} userdateandtime;

extern userdateandtime messdateandtime;
