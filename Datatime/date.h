#ifndef DATATIME_DATE_H
#define DATATIME_DATE_H

class Date {
    int day_ = 1;
    int month_ = 1;
    int year_ = 1970;

public:
    void SetDay(int day);
    void SetMonth(int month);
    void SetYear(int year);

    int GetDay() const;
    int GetMonth() const;
    int GetYear() const;

    bool IsLeap() const;
};

void ReadDate(Date& date);
void PrintDate(const Date& date);


#endif //DATATIME_DATE_H
