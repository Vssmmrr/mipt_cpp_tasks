#include <iostream>
#include <string>
#include "datatime.h"

void ReadDataTime(Datatime& datatime){
    int day, month, year, hours, minutes, seconds;
    std::cin >> day >> month >> year >> hours >> minutes >> seconds;
    datatime.date.SetDay(day);
    datatime.date.SetMonth(month);
    datatime.date.SetYear(year);
    datatime.time.SetHours(hours);
    datatime.time.SetMinutes(minutes);
    datatime.time.SetSeconds(seconds);
}

void PrintDataTime(const Datatime& datatime){
    std::cout << datatime.date.GetDay() << '.' << datatime.date.GetMonth() << '.' <<
    datatime.date.GetYear() << ' ' << datatime.time.GetHours() << ':' << datatime.time.GetMinutes()
    << ':' << datatime.time.GetSeconds() << '\n';
}
