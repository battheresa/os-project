#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MIN_LENGTH 10
#define SUB_LENGTH 25
#define CMD_LENGTH 80
#define ORD_LENGTH 200

char raw_path[] = "_resources/raw.txt";
char order_path[] = "_resources/orders.txt";
char invalid_path[] = "_resources/invalids.txt";

// --------------------------------------------------------------------------------

int indexOf(char str[], char c, int from, int to) {
    for (int i = from; i < to; i++)
        if (str[i] == c)
            return i;
    
    return -10;
}

void substring(char source[], char dest[], int from, int to) {    
    if (to < 0)
        to = strlen(source);
    
    memcpy(dest, &source[from], to);
    dest[to] = 0;
}

int split(char source[], char *dest[], char dex[]) {
    int count = 0;
    char *token = strtok(source, dex);
    
    while (token != NULL) {
        memcpy(dest[count++], token, strlen(token));
        token = strtok(NULL, dex);
    }
    
    return count;
}

// --------------------------------------------------------------------------------

typedef struct Date {
    int day;
    int month;
    int year;
} Date;

Date start_period, end_period;

Date constructDate(char str[]) {
    Date d;
    char temp[SUB_LENGTH];
    
    substring(str, temp, 0, 4);
    d.year = atoi(&temp[0]);
    
    substring(str, temp, 5, 7);
    d.month = atoi(&temp[0]);
    
    substring(str, temp, 8, strlen(str));
    d.day = atoi(&temp[0]);
    
    return d;
}

void dateToString(Date date, char str[]) {
    char month[3];
    sprintf(month, (date.month >= 10) ? "%d" : "0%d", date.month);
    
    char day[3];
    sprintf(day, (date.day >= 10) ? "%d" : "0%d", date.day);
    
    sprintf(str, "%d-%s-%s", date.year, month, day);
}

// --------------------------------------------------------------------------------

int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void setPeriod(Date start, Date end) {
    start_period = start;
    end_period = end;
}

bool isValidDate(Date d) {
    // if leap year, Feberuary have 29 days
    if ((d.year % 4 == 0 && d.year % 100 != 0) || d.year % 400 == 0)
        days_in_month[1] = 29;
    
    // if day is more than days in each month, return date invalid
    if (d.day > days_in_month[d.month - 1])
        return false;
    
    return true;
}

// --------------------------------------------------------------------------------

bool isBefore(Date from, Date to, bool overlap) {
    if (to.year < from.year)
        return false;
    
    if (to.year > from.year)
        return true;
    
    if (to.month < from.month)
        return false;
    
    if (to.month > from.month)
        return true;
    
    if (overlap && to.day < from.day)
        return false;
    
    if (!overlap && to.day <= from.day)
        return false;
    
    return true;
}

// --------------------------------------------------------------------------------

int dateToDays(Date from, Date to) {
    if (isBefore(from, to, false))  // if to is before from, return -1;
        return -1;
    
    int difference = 0;
    days_in_month[1] = 28;  // reset February to 28 days (isValidDate might have changed it)
    
    if (from.year == to.year && from.month == to.month) {   // same year and same month
        // days from 'from' to 'to'
        difference += to.day - from.day;
    }
    else if (from.year == to.year && from.month != to.month) {  // same year but different month
        // if leap year, February have 29 days
        if ((from.year % 4 == 0 && from.year % 100 != 0) || from.year % 400 == 0)
            days_in_month[1] = 29;
        
        // days from 'from' to end of the month
        difference += days_in_month[from.month - 1] - from.day;
        
        // days of the months between 'from' and 'to'
        for (int i = from.month + 1; i < to.month; i++)
            difference += days_in_month[i - 1];
        
        // days from begining of the month to 'to'
        difference += to.day;
    }
    else {
        // if leap year, February have 29 days
        if ((from.year % 4 == 0 && from.year % 100 != 0) || from.year % 400 == 0)
            days_in_month[1] = 29;
        
        // days from 'from' to end of the year
        difference += days_in_month[from.month - 1] - from.day;
        for (int i = from.month + 1; i <= 12; i++)
            difference += days_in_month[i - 1];
        
        // days of the years between 'from' and 'to'
        for (int i = from.year + 1; i < to.year; i++)
            difference += ((i % 4 == 0 && i % 100 != 0) || i % 400 == 0) ? 366 : 365;
        
        // if not leap year, February have 28 days
        if ((to.year % 4 != 0 || to.year % 100 == 0) && to.year % 400 != 0)
            days_in_month[1] = 28;
        
        // days from begining of the year to 'to'
        difference += to.day;
        for (int j = 0; j < to.month - 1; j++)
            difference += days_in_month[j - 1];
    }
    
    return difference;
}

// --------------------------------------------------------------------------------

Date daysToDate(int num) {
    int diff_month = 0;
    int diff_year = 0;
    
    int this_month = start_period.month - 1;
    int this_year = start_period.year;
    
    days_in_month[1] = 28;
    
    if (num > days_in_month[this_month] - start_period.day) {
        num -= days_in_month[this_month] - start_period.day;
        diff_month++;
        this_month++;
        
        while (num > days_in_month[this_month]) {
            if ((this_year % 4 == 0 && this_year % 100 != 0) || this_year % 400 == 0)
                days_in_month[1] = 29;
            
            num -= days_in_month[this_month];
            diff_month++;
            this_month++;
            
            if (diff_month >= 12) {     // difference between month reaches a year
                diff_month = 0;
                diff_year++;
            }
            
            if (this_month >= 12) {     // loop back to January
                days_in_month[1] = 28;
                this_month = 0;
                diff_year++;
                this_year++;
            }
        }
    }
        
    Date d;
    d.day = (diff_month > 0 || diff_year > 0) ? start_period.day + num - 1 : start_period.day + num;
    d.month = start_period.month + diff_month;
    d.year = start_period.year + diff_year;
    return d;
}
