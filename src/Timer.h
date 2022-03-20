#ifndef TIMER_H
#define TIMER_H
class Timer {
    double deadline;

    double coolDown;

   public:
    Timer() { this->deadline = 0; }

    void start(double realTime) { deadline = realTime + coolDown; }

    bool isUp(double realTime) { return realTime > deadline; }

    void setCooldown(double cooldown) { this->coolDown = cooldown; }
};
#endif
