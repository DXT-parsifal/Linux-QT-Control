#include "Curtain.h"
#include <string.h>

Curtain::Curtain()
{
    effect.type = FF_RUMBLE;
    effect.id = -1;
    effect.u.rumble.strong_magnitude = 0xFFFF; //调节振动强度
    effect.u.rumble.weak_magnitude = 0;
    effect.replay.length = 3000; //调节振动时长 ms
    effect.replay.delay = 0;
}

void Curtain::dev_start(){
    int fd;
    fd = open(dev_path,O_RDWR);
    int num_effects;
    ioctl(fd,EVIOCGEFFECTS,&num_effects);
    ioctl(fd, EVIOCSFF, &effect);
    write(fd, (const void*) &play, sizeof(play));
    sleep(2);
}
