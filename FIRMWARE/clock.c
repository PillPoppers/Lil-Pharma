#include "clock.h"

void InitClock()
{
    time.sec = 0;
    time.min = 0;
    time.hour = 0;

    unsigned int i;
    for (i = 0; i < ALARMNUMBER; i++)
        alarms[i].ID = -1;          // Slot is empty
}

void SetTime(Time t)
{
    time = t;
}

Time GetTime()
{
    return time;
}

void Tick()
{
    time.sec++;

    if (time.sec == 60)
    {
        time.sec = 0;
        time.min++;

        if (time.min == 60)
        {
            time.min = 0;
            time.hour = (time.hour+1)%24;
        }

        CheckAlarms();          // Checks alarms every minute
    }
}

void AddAlarm(Alarm a)
{
    unsigned short i;
    for (i = 0; i < ALARMNUMBER; i++)
        if (alarms[i].ID == -1) // Assigns the first empty slot an alarm
        {
            alarms[i] = a;
            break;
        }
}

void DeleteAlarm(short ID)
{
    unsigned short i;
    for (i = 0; i < ALARMNUMBER; i++)
        if (alarms[i].ID == ID) // Assigns the first empty slot an alarm
        {
            alarms[i].ID = -1;  // ID is set to -1.. Indicates that slot is empty
                                // Technically, we can break here, but we check to see if there's
                                // any other alarm with same ID
        }
}

// Alarms will be checked for every minute
void CheckAlarms()
{
    unsigned short i;
      for (i = 0; i < ALARMNUMBER; i++)
          if (alarms[i].ID != -1)           // Only checks assigned alarms
              if (alarms[i].time.hour == time.hour && alarms[i].time.min == time.min)   // Time matches (hours and minutes)
                  TriggerAlarm(alarms[i]);  // Trigger any alarm set for that time
}

// This method sets off any given alarm... needs to coordinate with motors and rest of code
void TriggerAlarm(Alarm a)
{
        // TODO: Implement whatever is necessary.. flashing LEDS, buzzer, rotating carousel, etc
}
