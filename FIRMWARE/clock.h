#ifndef CLOCK
#define CLOCK

typedef struct Time {
    unsigned short sec;
    unsigned short min;
    unsigned short hour;
} Time;

Time time;

void InitClock();
void SetTime(Time t);
Time GetTime();
// Triggered by 1 Hz square wave
void Tick();

typedef struct Alarm {
    short ID;               // each alarm will have a corresponding ID
                            //(this will be used to know which slots in the carousel should be moved for any given alarm)
                            // An ID of -1 means that the alarm is not assigned
    Time time;              // Time assigned to alarm
                            // Add other properties as necessary
} Alarm;

#define ALARMNUMBER 10      // 10 alarms max for now
Alarm alarms[ALARMNUMBER];

void AddAlarm(Alarm a);
void DeleteAlarm(short ID);
// Alarms will be checked for every minute
void CheckAlarms();
// This method sets off any given alarm... needs to coordinate with motors and rest of code
void TriggerAlarm(Alarm a);


#endif
