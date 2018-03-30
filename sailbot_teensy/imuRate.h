#ifndef IMURATE_H
#define IMURATE_H

class imuRate {
public:
    imuRate(int);
    bool needsRun();
    void sleep();
private:
    int periodUs;
    unsigned long lastUs;
};


#endif
