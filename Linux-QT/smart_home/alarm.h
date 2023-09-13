#ifndef ALARM_H
#define ALARM_H
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <unistd.h>
#define device_path "/dev/input/event0"

class Alarm
{
public:
    Alarm();
    void alarm_control();//pwm控制
private:
    int fd, version, ret;
    struct input_event event;
    struct timeval time;
};

#endif // ALARM_H
