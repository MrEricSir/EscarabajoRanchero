#ifndef DISPATCHKEYHANDLER_H
#define DISPATCHKEYHANDLER_H

namespace Escarabajo
{

class DispatchKeyHandler
{
public:
    DispatchKeyHandler();
    virtual ~DispatchKeyHandler();

    // Keyboard functions

    virtual void pressLeft();
    virtual void releaseLeft();

    virtual void pressRight();
    virtual void releaseRight();


    virtual void pressUp();
    virtual void releaseUp();

    virtual void pressDown();
    virtual void releaseDown();

    virtual void pressEscape();
    virtual void pressPlus();
    virtual void pressMinus();
    virtual void pressR();
    virtual void pressP();

private:
};

}


#endif // DISPATCHKEYHANDLER_H
