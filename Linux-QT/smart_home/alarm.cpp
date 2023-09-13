#include "alarm.h"

Alarm::Alarm()
{
    event.type = EV_SND;
    event.code = SND_TONE;
    event.value = 500;
    time.tv_sec = 1;
    time.tv_usec = 0;
    event.time = time;

}

void Alarm::alarm_control()
{
    fd = open(device_path,O_RDWR);

    ret = write(fd, &event, sizeof(struct input_event));
    sleep(1);

    event.value = 1000;
    ret = write(fd, &event, sizeof(struct input_event));
    sleep(1);

    event.value = 1500;
    ret = write(fd, &event, sizeof(struct input_event));
    sleep(1);

    event.value = 2000;
    ret = write(fd, &event, sizeof(struct input_event));
    sleep(1);

    event.value = 1500;
    ret = write(fd, &event, sizeof(struct input_event));
    sleep(1);

    event.value = 1000;
    ret = write(fd, &event, sizeof(struct input_event));
    sleep(1);

    event.value = 500;
    ret = write(fd, &event, sizeof(struct input_event));
    sleep(1);

    event.code = SND_BELL;
    event.value = 0000;
    ret = write(fd, &event, sizeof(struct input_event));
    close(fd);
}
