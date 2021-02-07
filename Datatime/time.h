#ifndef DATATIME_TIME_H
#define DATATIME_TIME_H

class Time {
    int hours_ = 1;
    int minutes_ = 23;
    int seconds_ = 45;

public:
    void SetHours(int hours);
    void SetMinutes(int minutes);
    void SetSeconds(int seconds);

    int GetHours() const;
    int GetMinutes() const;
    int GetSeconds() const;

};

void ReadTime(Time& time);
void PrintTime(const Time& time);


#endif //DATATIME_TIME_H
