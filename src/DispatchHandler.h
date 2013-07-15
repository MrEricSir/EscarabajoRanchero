#ifndef KEYBOARDHANDLER_H_
#define KEYBOARDHANDLER_H_

namespace Escarabajo
{

class DispatchHandler
{
public:
	DispatchHandler( unsigned intervalMs );
	virtual ~DispatchHandler();

	// Quit.
	// If this function returns true, the loop continues.
	// If it returns false, the loop ends, thus exiting the program.
	virtual bool quit();


	// Timer.
	virtual void tick();
	unsigned getInterval();



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
	DispatchHandler(); // unused

	unsigned intervalMs;
};

}

#endif /* KEYBOARDHANDLER_H_ */
