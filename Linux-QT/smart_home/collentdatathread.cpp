#include "collentdatathread.h"
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <QDebug>
CollentdataThread::CollentdataThread()
{

}

void CollentdataThread::run()
{
    int temp_raw = 0;
    int temp_offset = 0;
    float temp_scale = 0;
    int hum_raw = 0;
    int hum_offset = 0;
    float hum_scale = 0;
    float tem_float =0;
    float hum_float =0;
    float ill_float =0;
    QString hum;
    QString tem;
    QString ill;
    const char *device1 ="iio:device0";//温湿度
    const char *device2 ="iio:device1";//光照
    while (1)
    {
        /*read temp data*/
        read_sysfs_int(device1, "in_temp_raw", &temp_raw);
        read_sysfs_int(device1, "in_temp_offset", &temp_offset);
        read_sysfs_float(device1, "in_temp_scale", &temp_scale);
        tem_float =(temp_raw + temp_offset) * temp_scale / 1000;
        tem =QString::number(tem_float,'f', 2);
        read_sysfs_int(device1, "in_humidityrelative_raw", &hum_raw);
        read_sysfs_int(device1, "in_humidityrelative_offset", &hum_offset);
        read_sysfs_float(device1, "in_humidityrelative_scale", &hum_scale);
        hum_float = (hum_raw + hum_offset) * hum_scale / 1000;
        hum =QString::number(hum_float,'f', 2);
        read_sysfs_float(device2, "in_illuminance_input", &ill_float);
        ill =QString::number(ill_float,'f', 2);
        emit send(tem,hum,ill);
        sleep(2);
    }
}

int CollentdataThread::read_sysfs_float(const char *device, const char *filename, float *val)
{
    int ret = 0;
    FILE *sysfsfp;
    char temp[128];
    memset(temp, '0', 128);
    ret = sprintf(temp, "/sys/bus/iio/devices/%s/%s", device, filename);
    if (ret < 0)
        goto error;
    sysfsfp = fopen(temp, "r");
    if (!sysfsfp)
    {
        ret = -errno;
        goto error;
    }
    errno = 0;
    if (fscanf(sysfsfp, "%f\n", val) != 1)
    {
        ret = errno ? -errno : -ENODATA;
        if (fclose(sysfsfp))
            perror("read_sysfs_float(): Failed to close dir");
        goto error;
    }
    if (fclose(sysfsfp))
        ret = -errno;
error:
    return ret;
}

int CollentdataThread::read_sysfs_int(const char *device, const char *filename, int *val)
{
    int ret = 0;
    FILE *sysfsfp;
    char temp[128];
    memset(temp, '0', 128);
    ret = sprintf(temp, "/sys/bus/iio/devices/%s/%s", device, filename);
    if (ret < 0)
        goto error;
    sysfsfp = fopen(temp, "r");
    if (!sysfsfp)
    {
        ret = -errno;
        goto error;
    }
    errno = 0;
    if (fscanf(sysfsfp, "%d\n", val) != 1)
    {
        ret = errno ? -errno : -ENODATA;
        if (fclose(sysfsfp))
            perror("read_sysfs_float(): Failed to close dir");
        goto error;
    }
    if (fclose(sysfsfp))
        ret = -errno;
error:
    return ret;
}
