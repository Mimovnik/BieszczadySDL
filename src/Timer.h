#ifndef TIMER_H
#define TIMER_H
class Timer {
   public:
    double time;
    double coolDown;

    Timer() { this->time = 0; }

    void start(double realTime) { time = realTime + coolDown; }

    bool isUp(double realTime) { return realTime > time; }
};
#endif
