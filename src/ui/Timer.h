#include <iostream>
#include <thread>
#include <chrono>

class Timer
{
    bool clear = false;

public:
    void setTimeout(void (*function)(), int delay);
    void setInterval(void (*function)(), int interval);
    
    void stop();
};
