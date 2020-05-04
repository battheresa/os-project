#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MIN_LENGTH 10
#define SUB_LENGTH 25
#define CMD_LENGTH 80

// --------------------------------------------------------------------------------

int indexOf(char str[], char c, int from, int to) {
    for (int i = from; i < to; i++)
        if (str[i] == c)
            return i;
    
    return -1;
}

void substring(char source[], char dest[], int from, int to) {
    if (to == -1)
        to = strlen(source);
    
    memcpy(dest, &source[from], to);
    dest[to] = 0;
}

// --------------------------------------------------------------------------------

struct Date {
    int day;
    int month;
    int year;
};

struct Date constructDate(char str[]) {
    struct Date d;
    char temp[SUB_LENGTH];
    
    substring(str, temp, 0, 4);
    d.year = atoi(&temp[0]);
    
    substring(str, temp, 5, 7);
    d.month = atoi(&temp[0]);
    
    substring(str, temp, 8, strlen(str));
    d.day = atoi(&temp[0]);
    
    return d;
}

// --------------------------------------------------------------------------------

int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool isValidDate(struct Date d) {
    // if leap year, Feberuary have 29 days
    if ((d.year % 4 == 0 && d.year % 100 != 0) || d.year % 400 == 0)
        days_in_month[1] = 29;
    
    // if day is more than days in each month, return date invalid
    if (d.day > days_in_month[d.month - 1])
        return false;
    
    return true;
}

int dateToDays(struct Date from, struct Date to) {
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