#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>

const size_t screen_siz = 2266;
unsigned snake_len = 3;
unsigned char direction = 'd', gameover = 0, key_input_locked = 0;

//Input a character and read it instantly, without having to wait for Enter to be pressed.
char keypress(unsigned char echo)
{
    struct termios savedState, newState;
    char c;

    if (-1 == tcgetattr(STDIN_FILENO, &savedState)){
        return EOF;     /* error on tcgetattr */
    }

    newState = savedState;

    if ((echo = !echo)){ 
        echo = ECHO;    /* echo bit to disable echo */
    }

    /* disable canonical input and disable echo.  set minimal input to 1. */
    newState.c_lflag &= ~(echo | ICANON);
    newState.c_cc[VMIN] = 1;

    if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &newState)){
        return EOF;     /* error on tcsetattr */
    }

    c = getchar();      /* block (withot spinning) until we get a keypress */

    /* restore the saved state */
    if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &savedState)){
        return EOF;     /* error on tcsetattr */
    }
  
    //Change the snake's direction of movement according to key pressed.
	switch(c){
		case 'a':
		case 'A':
			if(direction != 'l' && direction != 'r'){direction = 'l';} break;
		case 'w':
		case 'W':
			if(direction != 'u' && direction != 'd'){direction = 'u';} break;
		case 'd':
		case 'D':
			if(direction != 'l' && direction != 'r'){direction = 'r';} break;
		case 's':
		case 'S':
			if(direction != 'u' && direction != 'd'){direction = 'd';} break;
		default:
			break;
	}
    return c;
}

struct vec2{
	char x, y;
};

char* screen_buf; char** screen_ptr = &screen_buf;
struct vec2* control_buf;
int appl_spawned = 0;

void renew_apple(){
	//Rewrite where the old apple was back to blank.
	char valid = 1;
	size_t appl_pos = 102 + (control_buf[2047].y * 103) + (control_buf[2047].x + 1 + 1);
	screen_buf[appl_pos] = ' ';
	appl_spawned = 0;
	
	//Generate new random apple coordinates.
        time_t t;
   	srand((unsigned) time(&t));

	control_buf[2047].x = rand() % 100;
	control_buf[2047].y = rand() % 20;
	
	//Check if it would spawn on top of the snake.
	for(size_t i = 0; i < snake_len; ++i){
		if((control_buf[i].x == control_buf[2047].x) && (control_buf[i].y == control_buf[2047].y)){
			valid = 0;
			break;
		}
	}
	
	//If it would spawn outside of snake, spawn it.
	if(valid){
		appl_pos = 102 + (control_buf[2047].y * 103) + (control_buf[2047].x + 1 + 1); 
		screen_buf[appl_pos] = '*';
		appl_spawned = 1;
		return;
	}
	else{return;}
	
}

void* keyboard_listen(){
	char* dummy_ptr = "d";
	while(!gameover){
		if(!key_input_locked){
			//Discard any characaters that were written to input and waiting to be entered AND characters
			// that were already entered from std input, to prevent multiple moves in a single frame.
			tcflush(STDIN_FILENO, TCIOFLUSH);
			keypress(0);
			key_input_locked = 1;
		}
	}
	return dummy_ptr;
}

void update(){
	//Move snake body points.
	size_t position = 102 + (control_buf[snake_len - 1].y * 103) + (control_buf[snake_len - 1].x + 1 + 1);

	struct vec2 tail_coords = {.x = control_buf[snake_len - 1].x, .y = control_buf[snake_len - 1].y};

	screen_buf[position] = ' ';
	for(size_t i = 0; i < snake_len-1; ++i){
		control_buf[snake_len - (i+1)].x = control_buf[snake_len - (i+2)].x;
		control_buf[snake_len - (i+1)].y = control_buf[snake_len - (i+2)].y;
	}
	//Compute new head point.
	switch (direction){
		case 'd':
			++control_buf[0].y; break;
		case 'u':
			--control_buf[0].y; break;
		case 'r':
			++control_buf[0].x; break;
		case 'l':
			--control_buf[0].x; break;
		default: 
			break;	
	}

	//Attempt to spawn a new apple.
	if(!appl_spawned){
		renew_apple();
	}
	else if((appl_spawned) && (control_buf[0].x == control_buf[2047].x) && (control_buf[0].y == control_buf[2047].y)){
		control_buf[snake_len].x = tail_coords.x;
		control_buf[snake_len].y = tail_coords.y;
		++snake_len; 
		renew_apple();
	}
	
	//Check for hitting a wall.
	if((control_buf[0].x > 99) || (control_buf[0].x < 0) || (control_buf[0].y < 0) || (control_buf[0].y > 19)){
		gameover = 1;	
		return;
	}

	//Check for hitting self.
	for(size_t i = 1; i < snake_len - 1; ++i){
		if((control_buf[0].x == control_buf[i].x) && control_buf[0].y == control_buf[i].y){
			gameover = 1;
			return;
		}
	}
	//Apply the new points to the screen buffer.
	for(size_t i = 0; i < snake_len; ++i){
		position = 102 + (control_buf[i].y * 103) + (control_buf[i].x + 1 + 1);
		screen_buf[position] = 'o';
	}

	//Print the new frame.
	system("clear");
	for(size_t i = 0; i < screen_siz; ++i){
		printf("%c", screen_buf[i]);
	}
	printf("Points: %u\n", (snake_len - 3));
	key_input_locked = 0;
	return;
}


int main(){
	//Initialize game parameters.
	if(posix_memalign((void*)&control_buf, 64, 2048*(sizeof(struct vec2))) != 0){printf("Memory allocation fail.\n");}

	//Create initial 3-long snake.
	control_buf[0].x = 0; control_buf[0].y = 0;
	control_buf[1].x = 0; control_buf[1].y = 1;
	control_buf[2].x = 0; control_buf[2].y = 2;


	//Populate the byte array for the game screen.
	*screen_ptr = calloc(2266, sizeof(char));
	size_t pos = 0;

	//The upper wall
	while(pos < 102){
		screen_buf[pos] = '=';
		++pos;
	}
	screen_buf[pos] = '\n'; 
	++pos;

	//The horizontal walls + empty spaces
	for(size_t i = 0; i < 20; ++i){
		for(size_t j = 0; j < 102; ++j){
			if(j == 0 || j == 101){screen_buf[pos] = '|';}
			else{screen_buf[pos] = ' ';}
			++pos;
		}
		screen_buf[pos] = '\n';
		++pos;
	}
	
	//The bottom wall
	for(int i = 0; i < 102; ++i){
		screen_buf[pos] = '=';
		++pos;
	}
	screen_buf[pos] = '\n';

	//Generate first apple at (10, 10)
	control_buf[2047].x = 10; control_buf[2047].y = 10;
	size_t first_appl_pos = 102 + (control_buf[2047].y * 103) + (control_buf[2047].x + 1 + 1);
	screen_buf[first_appl_pos] = '*';
	appl_spawned = 1;

	//Print the game screen for the first time
	for(size_t i = 0; i < screen_siz; ++i){
		printf("%c", screen_buf[i]);
	}

	//Start keyboard event listener thread	
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, keyboard_listen, NULL);	
	
	//Begin main game rendering loop
	useconds_t timer = 200000;
	while(!gameover){
		usleep(timer);
		update();
	}	
	printf("GAME OVER\n");
	return 0;
}
