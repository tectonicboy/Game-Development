#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

unsigned short SCREEN_HEIGHT, SCREEN_LENGTH;
unsigned int screen_buf_siz;

char* screen_buf; 
char** screen_ptr = &screen_buf;
char key_pressed;

struct point2D{
	unsigned int x, y; 
};

struct point2D* control_buf; 

int initialize_screen(unsigned short height, unsigned short length){
	if(length > 200 || height > 64){return 0;}
	SCREEN_HEIGHT = height; 
	SCREEN_LENGTH = length;
	screen_buf_siz = (SCREEN_HEIGHT + 2) * (SCREEN_LENGTH + 3);
	*screen_ptr = calloc(screen_buf_siz, sizeof(char));
	size_t pos = 0;

	//The upper wall
	while(pos < (SCREEN_LENGTH + 2)){
		screen_buf[pos] = '=';
		++pos;
	}
	screen_buf[pos] = '\n'; 
	++pos;

	//The horizontal walls + empty spaces
	for(size_t i = 0; i < (SCREEN_HEIGHT); ++i){
		for(size_t j = 0; j < (SCREEN_LENGTH + 2); ++j){
			if(j == 0 || j == (SCREEN_LENGTH + 1)){screen_buf[pos] = '|';}
			else{screen_buf[pos] = ' ';}
			++pos;
		}
		screen_buf[pos] = '\n';
		++pos;
	}
	
	//The bottom wall
	for(int i = 0; i < (SCREEN_LENGTH + 2); ++i){
		screen_buf[pos] = '=';
		++pos;
	}
	screen_buf[pos] = '\n';
	if(posix_memalign((void*)&control_buf, 64,((SCREEN_HEIGHT * SCREEN_LENGTH) + 256)*sizeof(struct point2D)) != 0){
		printf("Aligned mem allocation fail.");
		return 0;
	}
	return 1;
}

int write_to_screen(struct point2D point, char c){
	//Check that the desired 2D coordinate is within game screen boundaries.
	if((point.x > (SCREEN_LENGTH - 1)) || (point.y > (SCREEN_HEIGHT - 1))){
		return 0;
		printf("Point to write to is outside screen bounds.\n");
	}

	//Draw it to the game screen by computing and editing the appropriate index in the screen byte array.
	const size_t index = (SCREEN_LENGTH + 3) + (point.y * (SCREEN_LENGTH + 3)) + (point.x + 1);
	screen_buf[index] = c;
	return 1;
}

int read_screen(struct point2D point){
	//Check that the desired 2D coordinate is within game screen boundaries.
	if((point.x > (SCREEN_LENGTH - 1)) || (point.y > (SCREEN_HEIGHT - 1))){
		return 1;
	}
	
	//Compute index and check screen_array[index] if it's ' ' or not.
	const size_t index = (SCREEN_LENGTH + 3) + (point.y * (SCREEN_LENGTH + 3)) + (point.x + 1);
	if(screen_buf[index] == ' '){return 0;}
	else{return 1;}
}

void draw_screen(void){
	for(size_t i = 0; i < screen_buf_siz; ++i){
		printf("%c", screen_buf[i]);
	}
}

//Input a character and read it instantly, without having to wait for Enter to be pressed.
struct termios savedState, newState;
char keypress(unsigned char echo)
{
    	tcflush(STDIN_FILENO, TCIOFLUSH);
    	//struct termios savedState, newState;

    	if (-1 == tcgetattr(STDIN_FILENO, &savedState)){
        	return EOF;     /* error on tcgetattr */
    	}

    	newState = savedState;

    	if ((echo = !echo)){ /* yes i'm doing an assignment in an if clause */
        	echo = ECHO;    /* echo bit to disable echo */
    	}

    	/* disable canonical input and disable echo.  set minimal input to 1. */
    	newState.c_lflag &= ~(echo | ICANON);
    	newState.c_cc[VMIN] = 1;

    	if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &newState)){
        	return EOF;     /* error on tcsetattr */
    	}

    	key_pressed = getchar();      /* block (withot spinning) until we get a keypress */

    	/* restore the saved state */
    	if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &savedState)){
        	return EOF;     /* error on tcsetattr */
    	}

    	return key_pressed;
}
