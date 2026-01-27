/* TIMECONT.H - Use Date & Time functions with strings instead of repetitive structures */
#ifndef TIMECONT_H
#define TIMECONT_H

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <dos.h>

#define TIME_FORMAT 6
#define DATE_FORMAT 11
#define LARGE_DATE_FORMAT 27

void get_time(char *time_buffer);
void get_date(char *date_buffer);
void get_date_time(char *time_buffer, char *date_buffer);
short verify_time_change(char *curr_time, char *curr_date);
void get_large_date(char *large_date);

void get_time(char *time_buffer) {
    time_t t;
    struct tm *local_time;
    
    time(&t);
    local_time = localtime(&t);

    sprintf(time_buffer, "%02d:%02d",
        local_time->tm_hour, 
        local_time->tm_min,
        local_time->tm_sec);
}

void get_date(char *date_buffer) {
    time_t t;
    struct tm *local_time;
    
    time(&t);
    local_time = localtime(&t);

    sprintf(date_buffer, "%02d/%02d/%d",
       local_time->tm_mday,
       local_time->tm_mon + 1,
       local_time->tm_year + 1900);
}

void get_date_time(char *time_buffer, char *date_buffer) {
    time_t t;
    struct tm *local_time;
    
    time(&t);
    local_time = localtime(&t);

    sprintf(time_buffer, "%02d:%02d",
        local_time->tm_hour, 
        local_time->tm_min,
        local_time->tm_sec);

    sprintf(date_buffer, "%02d/%02d/%d",
       local_time->tm_mday,
       local_time->tm_mon + 1,
       local_time->tm_year + 1900);
}

short verify_time_change(char *curr_time, char *curr_date) {
    char time_new[TIME_FORMAT];
    char date_new[DATE_FORMAT];
    short change = 0;

    get_time(time_new);
    get_date(date_new);

    if (strcmp(curr_time, time_new) != 0) {
        strcpy(curr_time, time_new);
        change = 1;
    }

    if (strcmp(curr_date, date_new) != 0) {
        strcpy(curr_date, date_new);
        change = 1;
    }

    return change;
}

void get_large_date(char *large_date) {
    time_t t;
    struct tm *local_time;

    char *week_days[] = {
        "Domingo", "Lunes", "Martes", "Miercoles", 
        "Jueves", "Viernes", "Sabado"
    };
    
    char *months[] = {
        "Enero", "Febrero", "Marzo", "Abril", 
        "Mayo", "Junio", "Julio", "Agosto",
        "Septiembre", "Octubre", "Noviembre", "Diciembre"
    };

    time(&t);
    local_time = localtime(&t);

    sprintf(
        large_date,
        "%s, %d de %s",
        week_days[local_time->tm_wday],
        local_time->tm_mday,
        months[local_time->tm_mon]
    );
}
#endif