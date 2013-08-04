#ifndef KEYBOARDHANDLER_H_
#define KEYBOARDHANDLER_H_

namespace Escarabajo
{

class DispatchEventHandler
{
public:
    DispatchEventHandler( unsigned intervalMs );
    virtual ~DispatchEventHandler();

    // Quit.
    // If this function returns true, the loop continues.
    // If it returns false, the loop ends, thus exiting the program.
    virtual bool quit();


    // Timer.
    virtual void tick();
    unsigned getInterval();

private:
    DispatchEventHandler(); // unused

    unsigned intervalMs;
};

}

#endif /* KEYBOARDHANDLER_H_ */
