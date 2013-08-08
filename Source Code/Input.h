#ifndef _INPUT_
#define _INPUT_

class Input{

public:
	Input(){}
	virtual ~Input(){}

	virtual void updateInput() = 0;

	virtual bool isKeyPressed(int KeyID) = 0;
	virtual bool isMouseButtonPressed(int MouseButtonID) = 0;
	virtual int	getXPos() = 0;
	virtual int getXPosDiff() = 0;
	virtual int getYPos() = 0;
	virtual int getYPosDiff() = 0;
	virtual int getMouseWheel() = 0;

	virtual bool isKeyPressedOnce(int KeyID) = 0;
	virtual bool isMouseButtonPressedOnce(int MouseButtonID) = 0;

	virtual void setMousePos(int xpos, int ypos) = 0;
	virtual void setMouseWheel(int pos) = 0;
};


#endif