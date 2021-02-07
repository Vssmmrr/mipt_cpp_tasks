#include <iostream>
#include "time.h"
#include "date.h"
//#include "datatime.h"
#include "exceptions.h"

int main() {
    Date date;
    try {
        date.SetDay(30);
        date.SetMonth(2);
    } catch (InvalidArgument& ex) {
        std::cout << ex.What() << '\n';
    }
    try {
        ReadDate(date);
    } catch (InvalidArgument& ex) {
        std::cout << ex.What() << '\n';
    }
    PrintDate(date);

    Time time;
    try {
        ReadTime(time);
    } catch (InvalidArgument& ex) {
        std::cout << ex.What() << '\n';
    }
    PrintTime(time);
    try {
        time.SetHours(27);
    } catch (InvalidArgument& ex) {
        std::cout << ex.What() << '\n';
    }
//    PrintTime(time);
//    Datatime dt;
//    ReadDataTime(dt);
//    PrintDataTime(dt);
    return 0;
}
