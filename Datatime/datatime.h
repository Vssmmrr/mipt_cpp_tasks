#ifndef DATATIME_DATATIME_H
#define DATATIME_DATATIME_H

#include "date.h"
#include "time.h"

struct Datatime {
    Date date;
    Time time;
};

void ReadDataTime(Datatime& datatime);
void PrintDataTime(const Datatime& datatime);

#endif //DATATIME_DATATIME_H
