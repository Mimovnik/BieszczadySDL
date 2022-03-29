#ifndef TIMER_H
#define TIMER_H
class Timer {
    double deadline;

    double coolDown;
    bool waiting;

   public:
    Timer() {
        this->deadline = 0;
        this->waiting = false;
    }

    void reset(){
        deadline = 0;
        waiting = false;
    }

    void start(double realTime) {
        if (!waiting) {
            waiting = true;
            deadline = realTime + coolDown;
        }
    }

    bool isUp(double realTime) { 
        if(realTime > deadline){
            waiting = false;
        }
        return realTime > deadline; }

    void setCooldown(double cooldown) { this->coolDown = cooldown; }
};
#endif
