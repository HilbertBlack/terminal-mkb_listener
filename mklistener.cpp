#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

struct termios pre_term_sett;

const char* const CLICK 			= "\033[?1000h";
const char* const CLICK_DRAG     	= "\033[?1002h";
const char* const CLICK_DRAG_MOVE   = "\033[?1003h";
const char* const MOUSE_FORMAT      = "\033[?1006h";

unsigned char EXIT_KEY = 0x03;
unsigned char key = 0;


int PRESS 		= 'M';
int RELEASE	 	= 'm';
int WHEEL_UP	= 64;
int WHEEL_DOWN	= 65;
int MOVE		= 35;
int DRAG		= 30;
int LEFT_BUTTON = 0;
int MIDDLE_BUTTON = 1;
int RIGHT_BUTTON  = 2;


class MOUSE_EVENT
{
	public:
		int x;
		int y;

		int button;
		int event ;
};

class KEYBOARD_MOUSE_LISTENER
{
	public:
			int start_getting_mouse_input();
			int stop_getting_mouse_input();
			int change_terminal_settings();
			int return_terminal_settings();

	int listen(const char* const mouse_event = CLICK_DRAG_MOVE);

	virtual void mousePressed(class MOUSE_EVENT e)
	{
		printf("mouse pressed\n");
	}
	virtual void mouseReleased(class MOUSE_EVENT e)
	{
		printf("mouse released\n");
	}
	virtual void mouseMoved(class MOUSE_EVENT e)
	{
		printf("mouse moved event\n");
	}
	virtual void mouseDraged(class MOUSE_EVENT e)
	{
		printf("mouse draged event\n");
	}
	virtual void mouseWheel(class MOUSE_EVENT e)
	{
		printf("Mouse wheel event\n");
	}

	virtual void keyPressed(unsigned char key)
	{
		printf("key pressed: %c\n",key );
	}
	
};

// int main()
// {
// 	KEYBOARD_MOUSE_LISTENER mouse;

// // CHANGING THE TERMINAL SETTINGS TO
// // CANONICAL MODE - GETTING INPUT WITHOUT ENTER
// 	mouse.change_terminal_settings();

// // START getting the mouse input
// 	mouse.start_getting_mouse_input();

// 	mouse.listen();

// // STOP getting the mouse input
// 	mouse.stop_getting_mouse_input();

// // changing the terminal settings back to normal
// 	mouse.return_terminal_settings();
	
// 	return 0;
// }


int KEYBOARD_MOUSE_LISTENER::listen(const char* const mouse_event)
{
	unsigned char buf_[16];
	int count = 0;
	while(1)
	{
		fflush(stdout);	
		int status = read(1,&buf_[count],1);
		
		if(status == 0)
		{
			continue;
		}
	
		// if control C is pressed, listening will be stoped
		if (buf_[count] == EXIT_KEY)
		{
			stop_getting_mouse_input();
			return_terminal_settings();
		//	printf("---Existing---");
			return 0;
		}		
		
		else if (buf_[count] == '\033')
		{
		 //	printf("Encounter escape sequnece !!");	
			count++;
			
		}
		else if(buf_[count] == '[' and count == 1 )
		{
			count++;
			continue;
		}
		else if(buf_[count] == '<' and count == 2)
		{
		 //	printf("Encounted mouse point!!\n");
			unsigned int mouse_data[3] = {0,0,0};
			unsigned int X=0, Y=0;
			unsigned char dummy = 0, press_or_release = 1;
		 // mouse motion is inerpreted			
			for (int i=0;i< 3;i++)
			{
				while(1)
				{
					read(1, &dummy, 1);
					if(dummy == ';')
						break;
					else if(dummy == 'm' || dummy == 'M')
					{
						press_or_release = dummy;
						break;
					}
					mouse_data[i] *= 10;
					mouse_data[i] += dummy - 48;
				}
			}
			X = mouse_data[1] - 1;
			Y = mouse_data[2] - 1;
			press_or_release = dummy;
	    //printf("X:%i Y:%i %c:%i\n",mouse_data[1]-1,mouse_data[2]-1,dummy,mouse_data[0]);

			MOUSE_EVENT me;
			me.x = X;
			me.y = Y;

			
			switch(mouse_data[0])
			{
				case 64:
					//Wheel up detected
					me.event = WHEEL_UP;
					mouseWheel(me);
					break;
				case 65:
					//Wheel down detected
					me.event = WHEEL_DOWN;
					mouseWheel(me);
					break;
				case 35:
					//Motion detected
					me.event = MOVE;
					mouseMoved(me);
					break;
				case 34:
					//DRAG for the right clic
					me.button = RIGHT_BUTTON;
					me.event = DRAG;
					mouseDraged(me);
					break;
				case 33:
					// DRAG for the MIDDLE cliCK
					me.button = MIDDLE_BUTTON;
					me.event = DRAG;
					mouseDraged(me);
					break;
				case 32:
					//DRAG for left click
					me.button = LEFT_BUTTON;
					me.event = DRAG;
					mouseDraged(me);
					break;
				case 2:
					//right button event
					me.button = RIGHT_BUTTON;
					me.event  = press_or_release;
					(press_or_release == PRESS)? mousePressed(me):mouseReleased(me); 
					break;
				case 1:
					//Middle button event
					me.button = MIDDLE_BUTTON;
					me.event = press_or_release;
					(press_or_release == PRESS)? mousePressed(me):mouseReleased(me);
					break;
				case 0:
					//left button event
					me.button = LEFT_BUTTON;
					me.event = press_or_release;
					(press_or_release == PRESS)? mousePressed(me):mouseReleased(me);
					break; 
			}
	    count = 0;
		}
		else
		{
			for (int i=0; i<=count; i++)
			{
				keyPressed(buf_[i]);	
			}
			count =0;
		
			//printf("That unsigned char: %d |%c|\n", *buf_, *buf_);
	
		}
	}
	return 0;
}

int KEYBOARD_MOUSE_LISTENER::start_getting_mouse_input()
{

	int status   = write(0, CLICK_DRAG_MOVE, 8);
	//int status = printf("\033[?1002l\033[?1006l");
	//int status = system("echo -n '\033[?1002h\033[?1006h'");

	write(0, MOUSE_FORMAT, 8); // setting the output format to the standards -> 1006h
		
	//printf("status : %d\n", status);
	return status;
}

int KEYBOARD_MOUSE_LISTENER::stop_getting_mouse_input()
{
	
	int status = write(0,"\033[?1000l\033[?1002l\033[?1003l\033[?1006l", 32);
	//int status = printf("\033[?1000l\033[?1002l\033[?1003l\033[?1006l");
	//int status = system("echo -n '\033[?1000l\033[?1002l\033[?1003l\033[?1006l'");

	return status;
}

/*--------------------------------------------------.
| This function sets the terminal in CANONICAL mode.|
| Such that each byte is red if inputed. If no bytes|
| are avilable then, on each READ call it returns 0.|
`--------------------------------------------------*/

int KEYBOARD_MOUSE_LISTENER::change_terminal_settings()
{
	tcgetattr(1,&pre_term_sett);

	struct termios new_term_sett = pre_term_sett;
		
	new_term_sett.c_lflag = (~ICANON);
	new_term_sett.c_cc[VMIN] = 0;
	new_term_sett.c_cc[VTIME]= 0;
	
	tcsetattr(1, TCSANOW, &new_term_sett);

	return 1;
}


int KEYBOARD_MOUSE_LISTENER::return_terminal_settings()
{
	tcsetattr(1, TCSANOW, &pre_term_sett);
	return 0;
}
