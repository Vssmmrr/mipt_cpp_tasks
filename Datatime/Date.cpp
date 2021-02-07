#include <iostream>

#include "date.h"
#include "exceptions.h"

void Date::SetDay(int day) {
    if (IsLeap() && month_ == 2 && day > 0 && day < 30){
        day_ = day;
    } else if (month_ == 2 && day > 0 && day < 29){
        day_ = day;
    } else if (month_ != 2 && day > 0 && day < 32){
        day_ = day;
    } else {
        throw InvalidArgument(std::string ("Wrong Data"));
    }
}

void Date::SetMonth(int month) {
    if (IsLeap() && month == 2 && day_ > 29){
        throw InvalidArgument(std::string ("Wrong Data"));
    } else if (!IsLeap() && month == 2 && day_ > 28){
        throw InvalidArgument(std::string ("Wrong Data"));
    } else if (month > 0 && month < 13) {
        month_ = month;
    }
}

void Date::SetYear(int year) {
    year_ = year;
}

bool Date::IsLeap() const{
    return !(year_ % 4 !=0 || (year_ % 100 == 0 && year_ % 400 != 0));
}

int Date::GetDay() const {
    return day_;
}

int Date::GetMonth() const {
    return month_;
}

int Date::GetYear() const {
    return year_;
}

void ReadDate(Date& date){
    int day, month, year;
    std::cin >> day >> month >> year;
    date.SetDay(day);
    date.SetMonth(month);
    date.SetYear(year);
}

void PrintDate(const Date& date){
    std::cout << date.GetDay() << '.' << date.GetMonth() << '.' << date.GetYear() << '\n';
}


