#ifndef COLLENTDATATHREAD_H
#define COLLENTDATATHREAD_H

#include <QThread>

class CollentdataThread : public QThread
{
    Q_OBJECT
public:
    CollentdataThread();
    virtual void run();
signals:
    void send(QString tem,QString hum,QString ill);
private:
    int read_sysfs_float(const char *device,const char *filename,float *val);
    int read_sysfs_int(const char *device, const char *filename, int *val);
};

#endif // COLLENTDATATHREAD_H
