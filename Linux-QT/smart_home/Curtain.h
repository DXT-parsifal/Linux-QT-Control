#ifndef CURTAIN_H
#define CURTAIN_H

#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/ioctl.h>
//设备文件
#define dev_path  "/dev/input/event1"

using namespace std;

class Curtain{
public:
    Curtain();
    void dev_start();
private:
    struct ff_effect effect;
    struct input_event play = {
        .type = EV_FF,
        .code = effect.id,
        .value = 1
    };
};

#endif // CURTAIN_H
