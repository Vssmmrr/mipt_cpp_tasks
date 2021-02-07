#include <iostream>
#include "time.h"
#include "exceptions.h"

void Time::SetHours(int hours) {
    if (hours >= 0 && hours < 24) {
        hours_ = hours;
    } else {
        throw InvalidArgument(std::string("Invalid Hours"));
    }
}

void Time::SetMinutes(int minutes) {
    if (minutes >= 0 && minutes < 60){
        minutes_ = minutes;
    } else {
        throw InvalidArgument(std::string("Invalid Minutes"));
    }
}

void Time::SetSeconds(int seconds) {
    if (seconds >= 0 && seconds < 60){
        seconds_ = seconds;
    } else {
        throw InvalidArgument(std::string("Invalid Seconds"));
    }
}

int Time::GetHours() const {
    return hours_;
}

int Time::GetMinutes() const {
    return minutes_;
}

int Time::GetSeconds() const {
    return seconds_;
}

void ReadTime(Time& time){
    int hours, minutes, seconds;
    std::cin >> hours >> minutes >> seconds;
    time.SetHours(hours);
    time.SetMinutes(minutes);
    time.SetSeconds(seconds);
}

void PrintTime(const Time& time){
    std::cout << time.GetHours() << ':' << time.GetMinutes() << ':' << time.GetSeconds() << '\n';
}
