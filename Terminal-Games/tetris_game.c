#include <pthread.h>
#include "../Game_Engine/2D_terminal_engine.h" 

unsigned n = 0, x = 0, y = 0, dir = 0, state = 1, type = 1, num_rights = 0, num_lefts = 0, num_bottoms = 0, rot_checks = 0, screen_height = 25, screen_width = 34;
unsigned signal_new = 0, signal_rotate = 0, signal_down = 0, signal_move = 0, valid_move = 1, signal_row = 0, gameover = 0, bad_moves = 0, filled_rows = 0;
int loop_count = 0, score = 0;
unsigned* rows_filled;

struct point2D* lefts;
struct point2D* rights;
struct point2D* bottoms;
struct point2D* rot_pts;

void generate_new(){
	y = 1;
	x = 16;
	++n;
	time_t t;
   	srand((unsigned) time(&t));
	type = (rand() % 4) + 1;
	state = 1;
	struct point2D check_pt = {.x = x, .y = y};
		switch(type){
			case 1:
				if(read_screen(check_pt)){gameover = 1; return;}
				++check_pt.x;
				if(read_screen(check_pt)){gameover = 1; return;}
				++check_pt.x;
				if(read_screen(check_pt)){gameover = 1; return;}
				--check_pt.x; --check_pt.y;
				if(read_screen(check_pt)){gameover = 1; return;}

				control_buf[4*n].x = x; control_buf[4*n].y = y;
				control_buf[(4*n) + 1].x = (x+1); control_buf[(4*n) + 1].y = y;
				control_buf[(4*n) + 2].x = (x+1); control_buf[(4*n) + 2].y = (y-1);
				control_buf[(4*n) + 3].x = (x+2); control_buf[(4*n) + 3].y = y;

				num_lefts = 2; num_rights = 2; num_bottoms = 3; rot_checks = 4;

				lefts[0].x = control_buf[4*n].x - 1; lefts[0].y = control_buf[4*n].y;
				lefts[1].x = control_buf[4*n].x; lefts[1].y = control_buf[4*n].y - 1;

				rights[0].x = control_buf[(4*n) + 2].x + 1; rights[0].y = control_buf[(4*n) + 2].y;
				rights[1].x = control_buf[(4*n) + 2].x + 2; rights[1].y = control_buf[(4*n) + 2].y + 1;

				bottoms[0].x = control_buf[(4*n) + 1].x - 1; bottoms[0].y = control_buf[(4*n) + 1].y + 1;
				bottoms[1].x = control_buf[(4*n) + 1].x; bottoms[1].y = control_buf[(4*n) + 1].y + 1;
				bottoms[2].x = control_buf[(4*n) + 1].x + 1; bottoms[2].y = control_buf[(4*n) + 1].y + 1;

				rot_pts[0].x = control_buf[(4*n) + 1].x + 1; rot_pts[0].y = control_buf[(4*n) + 1].y - 1;
				rot_pts[1].x = control_buf[(4*n) + 1].x - 1; rot_pts[1].y = control_buf[(4*n) + 1].y - 1;
				rot_pts[2].x = control_buf[(4*n) + 1].x - 1; rot_pts[2].y = control_buf[(4*n) + 1].y + 1;
				rot_pts[3].x = control_buf[(4*n) + 1].x; rot_pts[3].y = control_buf[(4*n) + 1].y + 1;
				break;
			case 2:
				if(read_screen(check_pt)){gameover = 1; return;}
				++check_pt.x;
				if(read_screen(check_pt)){gameover = 1; return;}
				++check_pt.x;
				if(read_screen(check_pt)){gameover = 1; return;}
				++check_pt.x;
				if(read_screen(check_pt)){gameover = 1; return;}

				control_buf[4*n].x = x; control_buf[4*n].y = y;
				control_buf[(4*n) + 1].x = (x+1); control_buf[(4*n) + 1].y = y;
				control_buf[(4*n) + 2].x = (x+2); control_buf[(4*n) + 2].y = y;
				control_buf[(4*n) + 3].x = (x+3); control_buf[(4*n) + 3].y = y;

				num_lefts = 1; num_rights = 1; num_bottoms = 4, rot_checks = 7;

				lefts[0].x = control_buf[4*n].x - 1; lefts[0].y = control_buf[4*n].y;

				rights[0].x = control_buf[(4*n) + 3].x + 1; rights[0].y = control_buf[(4*n) + 3].y;

				bottoms[0].x = control_buf[4*n].x; bottoms[0].y = control_buf[4*n].y + 1;
				bottoms[1].x = control_buf[(4*n) + 1].x; bottoms[1].y = control_buf[(4*n) + 1].y + 1;
				bottoms[2].x = control_buf[(4*n) + 2].x; bottoms[2].y = control_buf[(4*n) + 2].y + 1;
				bottoms[3].x = control_buf[(4*n) + 3].x; bottoms[3].y = control_buf[(4*n) + 3].y + 1;

				rot_pts[0].x = control_buf[(4*n) + 3].x; rot_pts[0].y = control_buf[(4*n) + 3].y - 1;
				rot_pts[1].x = control_buf[(4*n) + 2].x; rot_pts[1].y = control_buf[(4*n) + 2].y - 1;
				rot_pts[2].x = control_buf[(4*n) + 2].x; rot_pts[2].y = control_buf[(4*n) + 2].y - 2;
				rot_pts[3].x = control_buf[(4*n) + 1].x; rot_pts[3].y = control_buf[(4*n) + 1].y - 1;
				rot_pts[4].x = control_buf[(4*n) + 1].x; rot_pts[4].y = control_buf[(4*n) + 1].y - 2;
				rot_pts[5].x = control_buf[(4*n) + 1].x; rot_pts[5].y = control_buf[(4*n) + 1].y + 1;
				rot_pts[6].x = control_buf[4*n].x; rot_pts[6].y = control_buf[4*n].y + 1;

				break;
			case 3:
				if(read_screen(check_pt)){gameover = 1; return;}
				++check_pt.x;
				if(read_screen(check_pt)){gameover = 1; return;}
				++check_pt.x;
				if(read_screen(check_pt)){gameover = 1; return;}
				--check_pt.y;
				if(read_screen(check_pt)){gameover = 1; return;}

				control_buf[4*n].x = x; control_buf[4*n].y = y;
				control_buf[(4*n) + 1].x = (x+1); control_buf[(4*n) + 1].y = y;
				control_buf[(4*n) + 2].x = (x+2); control_buf[(4*n) + 2].y = y;
				control_buf[(4*n) + 3].x = (x+2); control_buf[(4*n) + 3].y = y-1;

				num_lefts = 2; num_rights = 2; num_bottoms = 3, rot_checks = 7;

				lefts[0].x = x-1; lefts[0].y = y;
				lefts[1].x = x+1; lefts[1].y = y-1;

				rights[0].x = x+3; rights[0].y = y;
				rights[1].x = x+3; rights[1].y = y-1;

				bottoms[0].x = x; bottoms[0].y = y+1;
				bottoms[1].x = x+1; bottoms[1].y = y+1;
				bottoms[2].x = x+2; bottoms[2].y = y+1;

				rot_pts[0].x = x; rot_pts[0].y = y+1;
				rot_pts[1].x = x+1; rot_pts[1].y = y+1;
				rot_pts[2].x = x; rot_pts[2].y = y-1;
				rot_pts[3].x = x; rot_pts[3].y = y-2;
				rot_pts[4].x = x+1; rot_pts[4].y = y-1;
				rot_pts[5].x = x+1; rot_pts[5].y = y-2;
				rot_pts[6].x = x+2; rot_pts[6].y = y-2;

				break;
			case 4:
				if(read_screen(check_pt)){gameover = 1; return;}
				++check_pt.x;
				if(read_screen(check_pt)){gameover = 1; return;}
				++check_pt.y;
				if(read_screen(check_pt)){gameover = 1; return;}
				--check_pt.x;
				if(read_screen(check_pt)){gameover = 1; return;}

				control_buf[4*n].x = x; control_buf[4*n].y = y;
				control_buf[(4*n) + 1].x = (x+1); control_buf[(4*n) + 1].y = y;
				control_buf[(4*n) + 2].x = (x+1); control_buf[(4*n) + 2].y = y+1;
				control_buf[(4*n) + 3].x = (x); control_buf[(4*n) + 3].y = y+1;

				num_lefts = 2; num_rights = 2; num_bottoms = 2, rot_checks = 0;

				lefts[0].x = x-1; lefts[0].y = y;
				lefts[1].x = x-1; lefts[1].y = y+1;

				rights[0].x = x+2; rights[0].y = y;
				rights[1].x = x+2; rights[1].y = y+1;

				bottoms[0].x = x; bottoms[0].y = y+2;
				bottoms[1].x = x+1; bottoms[1].y = y+2;
				break;
			default:
				break;
		}
}

void rotate(){
	switch(type){
		case 1:
			switch(state){
				case 1:
					for(size_t i = 0; i < rot_checks; ++i){
						if(read_screen(rot_pts[i])){valid_move = 0; break;}
					}
					if(valid_move){
						state = 2;
						for(size_t i = 0; i < 4; ++i){
							write_to_screen(control_buf[(4*n) + i], ' ');
						}
						control_buf[4*n].x = x+1; control_buf[4*n].y = y+1;
						control_buf[(4*n) + 1].x = (x+1); control_buf[(4*n) + 1].y = y;
						control_buf[(4*n) + 2].x = x; control_buf[(4*n) + 2].y = y;
						control_buf[(4*n) + 3].x = (x+1); control_buf[(4*n) + 3].y = y-1;

						num_lefts = 3; num_rights = 3; num_bottoms = 2, rot_checks = 4;

						lefts[0].x = control_buf[(4*n) + 2].x; lefts[0].y = control_buf[(4*n) + 2].y - 1;
						lefts[1].x = control_buf[(4*n) + 2].x - 1; lefts[1].y = control_buf[(4*n) + 2].y;
						lefts[2].x = control_buf[(4*n) + 2].x; lefts[2].y = control_buf[(4*n) + 2].y + 1;

						rights[0].x = x + 2; rights[0].y = y + 1;
						rights[1].x = x + 2; rights[1].y = y;
						rights[2].x = x + 2; rights[2].y = y - 1;

						bottoms[0].x = control_buf[4*n].x - 1; bottoms[0].y = control_buf[4*n].y;
						bottoms[1].x = control_buf[4*n].x; bottoms[1].y = control_buf[4*n].y + 1;

						rot_pts[0].x = control_buf[(4*n) + 1].x + 1; rot_pts[0].y = control_buf[(4*n) + 1].y;
						rot_pts[1].x = control_buf[(4*n) + 1].x - 1; rot_pts[1].y = control_buf[(4*n) + 1].y - 1;
						rot_pts[2].x = control_buf[(4*n) + 1].x - 1; rot_pts[2].y = control_buf[(4*n) + 1].y + 1;
						rot_pts[3].x = control_buf[(4*n) + 1].x + 1; rot_pts[3].y = control_buf[(4*n) + 1].y + 1;
						break;
					}
					else{valid_move = 1; break;}
	
				case 2:
					for(size_t i = 0; i < rot_checks; ++i){
						if(read_screen(rot_pts[i])){valid_move = 0; break;}
					}
					if(valid_move){
						state = 3;
						for(size_t i = 0; i < 4; ++i){
							write_to_screen(control_buf[(4*n) + i], ' ');
						}
						control_buf[4*n].x = x+2; control_buf[4*n].y = y;
						control_buf[(4*n) + 1].x = (x+1); control_buf[(4*n) + 1].y = y;
						control_buf[(4*n) + 2].x = (x+1); control_buf[(4*n) + 2].y = (y+1);
						control_buf[(4*n) + 3].x = x; control_buf[(4*n) + 3].y = y;

						num_lefts = 2; num_rights = 2; num_bottoms = 3, rot_checks = 4;

						lefts[0].x = control_buf[(4*n) + 3].x - 1; lefts[0].y = control_buf[(4*n) + 3].y;
						lefts[1].x = control_buf[(4*n) + 3].x; lefts[1].y = control_buf[(4*n) + 3].y + 1;

						rights[0].x = control_buf[4*n].x + 1; rights[0].y = control_buf[4*n].y;
						rights[1].x = control_buf[4*n].x; rights[1].y = control_buf[4*n].y + 1;

						bottoms[0].x = control_buf[(4*n) + 2].x - 1; bottoms[0].y = control_buf[(4*n) + 2].y;
						bottoms[1].x = control_buf[(4*n) + 2].x; bottoms[1].y = control_buf[(4*n) + 2].y + 1;
						bottoms[2].x = control_buf[(4*n) + 2].x + 1; bottoms[2].y = control_buf[(4*n) + 2].y;

						rot_pts[0].x = control_buf[(4*n) + 1].x + 1; rot_pts[0].y = control_buf[(4*n) + 1].y + 1;
						rot_pts[1].x = control_buf[(4*n) + 1].x + 1; rot_pts[1].y = control_buf[(4*n) + 1].y - 1;
						rot_pts[2].x = control_buf[(4*n) + 1].x; rot_pts[2].y = control_buf[(4*n) + 1].y - 1;
						rot_pts[3].x = control_buf[(4*n) + 1].x - 1; rot_pts[3].y = control_buf[(4*n) + 1].y + 1;
						break;
					}
					else{valid_move = 1; break;}
				case 3:
					for(size_t i = 0; i < rot_checks; ++i){
						if(read_screen(rot_pts[i])){valid_move = 0; break;}
					}
					if(valid_move){
						state = 4;
						for(size_t i = 0; i < 4; ++i){
							write_to_screen(control_buf[(4*n) + i], ' ');
						}
						control_buf[4*n].x = x+1; control_buf[4*n].y = y-1;
						control_buf[(4*n) + 1].x = (x+1); control_buf[(4*n) + 1].y = y;
						control_buf[(4*n) + 2].x = (x+2); control_buf[(4*n) + 2].y = y;
						control_buf[(4*n) + 3].x = (x+1); control_buf[(4*n) + 3].y = y+1;

						num_lefts = 3; num_rights = 3; num_bottoms = 2, rot_checks = 4;

						lefts[0].x = control_buf[(4*n) + 1].x - 1; lefts[0].y = control_buf[(4*n) + 1].y - 1;
						lefts[1].x = control_buf[(4*n) + 1].x - 1; lefts[1].y = control_buf[(4*n) + 1].y;
						lefts[2].x = control_buf[(4*n) + 1].x - 1; lefts[2].y = control_buf[(4*n) + 1].y + 1;

						rights[0].x = x + 2; rights[0].y = y + 1;
						rights[1].x = x + 3; rights[1].y = y;
						rights[2].x = x + 2; rights[2].y = y - 1;

						bottoms[0].x = control_buf[(4*n) + 3].x; bottoms[0].y = control_buf[(4*n) + 3].y + 1;
						bottoms[1].x = control_buf[(4*n) + 3].x + 1; bottoms[1].y = control_buf[(4*n) + 3].y;

						rot_pts[0].x = control_buf[(4*n) + 1].x + 1; rot_pts[0].y = control_buf[(4*n) + 1].y + 1;
						rot_pts[1].x = control_buf[(4*n) + 1].x + 1; rot_pts[1].y = control_buf[(4*n) + 1].y - 1;
						rot_pts[2].x = control_buf[(4*n) + 1].x - 1; rot_pts[2].y = control_buf[(4*n) + 1].y - 1;
						rot_pts[3].x = control_buf[(4*n) + 1].x - 1; rot_pts[3].y = control_buf[(4*n) + 1].y;
						break;
					}
					else{valid_move = 1; break;}
				case 4:
					for(size_t i = 0; i < rot_checks; ++i){
						if(read_screen(rot_pts[i])){valid_move = 0; break;}
					}
					if(valid_move){
						state = 1;
						for(size_t i = 0; i < 4; ++i){
							write_to_screen(control_buf[(4*n) + i], ' ');
						}
						control_buf[4*n].x = x; control_buf[4*n].y = y;
						control_buf[(4*n) + 1].x = (x+1); control_buf[(4*n) + 1].y = y;
						control_buf[(4*n) + 2].x = (x+1); control_buf[(4*n) + 2].y = (y-1);
						control_buf[(4*n) + 3].x = (x+2); control_buf[(4*n) + 3].y = y;

						num_lefts = 2; num_rights = 2; num_bottoms = 3, rot_checks = 4;

						lefts[0].x = control_buf[4*n].x - 1; lefts[0].y = control_buf[4*n].y;
						lefts[1].x = control_buf[4*n].x; lefts[1].y = control_buf[4*n].y - 1;

						rights[0].x = control_buf[(4*n) + 2].x + 1; rights[0].y = control_buf[(4*n) + 2].y;
						rights[1].x = control_buf[(4*n) + 2].x + 2; rights[1].y = control_buf[(4*n) + 2].y - 1;

						bottoms[0].x = control_buf[(4*n) + 1].x - 1; bottoms[0].y = control_buf[(4*n) + 1].y + 1;
						bottoms[1].x = control_buf[(4*n) + 1].x; bottoms[1].y = control_buf[(4*n) + 1].y + 1;
						bottoms[2].x = control_buf[(4*n) + 1].x + 1; bottoms[2].y = control_buf[(4*n) + 1].y + 1;

						rot_pts[0].x = control_buf[(4*n) + 1].x + 1; rot_pts[0].y = control_buf[(4*n) + 1].y - 1;
						rot_pts[1].x = control_buf[(4*n) + 1].x - 1; rot_pts[1].y = control_buf[(4*n) + 1].y - 1;
						rot_pts[2].x = control_buf[(4*n) + 1].x - 1; rot_pts[2].y = control_buf[(4*n) + 1].y + 1;
						rot_pts[3].x = control_buf[(4*n) + 1].x; rot_pts[3].y = control_buf[(4*n) + 1].y + 1;
						break;
					}
					else{valid_move = 1; break;}
				default:
					break;
			} break;
		case 2:
			switch(state){
				case 1:
					for(size_t i = 0; i < rot_checks; ++i){
						if(read_screen(rot_pts[i])){valid_move = 0; break;}
					}
					if(valid_move){
						state = 2;
						for(size_t i = 0; i < 4; ++i){
							write_to_screen(control_buf[(4*n) + i], ' ');
						}
						control_buf[4*n].x = (x+1); control_buf[4*n].y = (y+1);
						control_buf[(4*n) + 1].x = (x+1); control_buf[(4*n) + 1].y = y;
						control_buf[(4*n) + 2].x = (x+1); control_buf[(4*n) + 2].y = (y-1);
						control_buf[(4*n) + 3].x = (x+1); control_buf[(4*n) + 3].y = (y-2);

						num_lefts = 4; num_rights = 4; num_bottoms = 1, rot_checks = 7;

						lefts[0].x = x; lefts[0].y = y+1;
						lefts[1].x = x; lefts[1].y = y;
						lefts[2].x = x; lefts[2].y = y-1;
						lefts[3].x = x; lefts[3].y = y-2;

						rights[0].x = x+2; rights[0].y = y+1;
						rights[1].x = x+2; rights[1].y = y;
						rights[2].x = x+2; rights[2].y = y-1;
						rights[3].x = x+2; rights[3].y = y-2;

						bottoms[0].x = x+1; bottoms[0].y = y+2;

						rot_pts[0].x = x+2; rot_pts[0].y = y+1;
						rot_pts[1].x = x+2; rot_pts[1].y = y;
						rot_pts[2].x = x; rot_pts[2].y = y;
						rot_pts[3].x = x; rot_pts[3].y = y-1;
						rot_pts[4].x = x; rot_pts[4].y = y-2;
						rot_pts[5].x = x-1; rot_pts[5].y = y;
						rot_pts[6].x = x-1; rot_pts[6].y = y-1;
						break;
					}
					else{valid_move = 1; break;}
				case 2:
					for(size_t i = 0; i < rot_checks; ++i){
						if(read_screen(rot_pts[i])){valid_move = 0; break;}
					}
					if(valid_move){
						state = 3;
						for(size_t i = 0; i < 4; ++i){
							write_to_screen(control_buf[(4*n) + i], ' ');
						}
						control_buf[4*n].x = (x+2); control_buf[4*n].y = y;
						control_buf[(4*n) + 1].x = (x+1); control_buf[(4*n) + 1].y = y;
						control_buf[(4*n) + 2].x = x; control_buf[(4*n) + 2].y = y;
						control_buf[(4*n) + 3].x = (x-1); control_buf[(4*n) + 3].y = y;

						num_lefts = 1; num_rights = 1; num_bottoms = 4, rot_checks = 7;

						lefts[0].x = x-2; lefts[0].y = y;

						rights[0].x = x+3; rights[0].y = y;

						bottoms[0].x = x-1; bottoms[0].y = y+1;
						bottoms[1].x = x; bottoms[1].y = y+1;
						bottoms[2].x = x+1; bottoms[2].y = y+1;
						bottoms[3].x = x+2; bottoms[3].y = y+1;

						rot_pts[0].x = x+2; rot_pts[0].y = y-1;
						rot_pts[1].x = x+1; rot_pts[1].y = y-2;
						rot_pts[2].x = x-1; rot_pts[2].y = y+1;
						rot_pts[3].x = x; rot_pts[3].y = y+1;
						rot_pts[4].x = x; rot_pts[4].y = y+2;
						rot_pts[5].x = x+1; rot_pts[5].y = y+1;
						rot_pts[6].x = x+1; rot_pts[6].y = y+2;
						break;
					}
					else{valid_move = 1; break;}
				case 3:
					for(size_t i = 0; i < rot_checks; ++i){
						if(read_screen(rot_pts[i])){valid_move = 0; break;}
					}
					if(valid_move){
						state = 4;
						for(size_t i = 0; i < 4; ++i){
							write_to_screen(control_buf[(4*n) + i], ' ');
						}
						control_buf[4*n].x = (x+1); control_buf[4*n].y = y-1;
						control_buf[(4*n) + 1].x = (x+1); control_buf[(4*n) + 1].y = y;
						control_buf[(4*n) + 2].x = x+1; control_buf[(4*n) + 2].y = y+1;
						control_buf[(4*n) + 3].x = (x+1); control_buf[(4*n) + 3].y = y+2;

						num_lefts = 4; num_rights = 4; num_bottoms = 1, rot_checks = 7;

						lefts[0].x = x; lefts[0].y = y-1;
						lefts[0].x = x; lefts[0].y = y;
						lefts[0].x = x; lefts[0].y = y+1;
						lefts[0].x = x; lefts[0].y = y+2;

						rights[0].x = x+2; rights[0].y = y-1;
						rights[0].x = x+2; rights[0].y = y;
						rights[0].x = x+2; rights[0].y = y+1;
						rights[0].x = x+2; rights[0].y = y+2;

						bottoms[0].x = x+1; bottoms[0].y = y+3;

						rot_pts[0].x = x; rot_pts[0].y = y-1;
						rot_pts[1].x = x; rot_pts[1].y = y;
						rot_pts[2].x = x+2; rot_pts[2].y = y;
						rot_pts[3].x = x+3; rot_pts[3].y = y;
						rot_pts[4].x = x+2; rot_pts[4].y = y+1;
						rot_pts[5].x = x+3; rot_pts[5].y = y+1;
						rot_pts[6].x = x+2; rot_pts[6].y = y+2;
						break;
					}
					else{valid_move = 1; break;}
				case 4:
					for(size_t i = 0; i < rot_checks; ++i){
						if(read_screen(rot_pts[i])){valid_move = 0; break;}
					}
					if(valid_move){
						state = 1;
						for(size_t i = 0; i < 4; ++i){
							write_to_screen(control_buf[(4*n) + i], ' ');
						}
						control_buf[4*n].x = x; control_buf[4*n].y = y;
						control_buf[(4*n) + 1].x = (x+1); control_buf[(4*n) + 1].y = y;
						control_buf[(4*n) + 2].x = x+2; control_buf[(4*n) + 2].y = y;
						control_buf[(4*n) + 3].x = (x+3); control_buf[(4*n) + 3].y = y;

						num_lefts = 1; num_rights = 1; num_bottoms = 4, rot_checks = 7;

						lefts[0].x = x-1; lefts[0].y = y;

						rights[0].x = x+4; rights[0].y = y;

						bottoms[0].x = x; bottoms[0].y = y+1;
						bottoms[1].x = x+1; bottoms[1].y = y+1;
						bottoms[2].x = x+2; bottoms[2].y = y+1;
						bottoms[3].x = x+3; bottoms[3].y = y+1;

						rot_pts[0].x = x; rot_pts[0].y = y+1;
						rot_pts[1].x = x+1; rot_pts[1].y = y+1;
						rot_pts[2].x = x+1; rot_pts[2].y = y-1;
						rot_pts[3].x = x+1; rot_pts[3].y = y-2;
						rot_pts[4].x = x+2; rot_pts[4].y = y-1;
						rot_pts[5].x = x+2; rot_pts[5].y = y-2;
						rot_pts[6].x = x+3; rot_pts[6].y = y-1;
						break;
					}
					else{valid_move = 1; break;}
				default:
					break;
			} break;
		case 3:
			switch(state){
				case 1:
					for(size_t i = 0; i < rot_checks; ++i){
						if(read_screen(rot_pts[i])){valid_move = 0; break;}
					}
					if(valid_move){
						state = 2;
						for(size_t i = 0; i < 4; ++i){
							write_to_screen(control_buf[(4*n) + i], ' ');
						}
						control_buf[4*n].x = x+1; control_buf[4*n].y = y+1;
						control_buf[(4*n) + 1].x = (x+1); control_buf[(4*n) + 1].y = y;
						control_buf[(4*n) + 2].x = x+1; control_buf[(4*n) + 2].y = y-1;
						control_buf[(4*n) + 3].x = x; control_buf[(4*n) + 3].y = y-1;

						num_lefts = 3; num_rights = 3; num_bottoms = 2, rot_checks = 7;

						lefts[0].x = x-1; lefts[0].y = y-1;
						lefts[1].x = x; lefts[1].y = y;
						lefts[2].x = x; lefts[2].y = y+1;

						rights[0].x = x+2; rights[0].y = y-1;
						rights[1].x = x+2; rights[1].y = y;
						rights[2].x = x+2; rights[2].y = y+1;

						bottoms[0].x = x; bottoms[0].y = y;
						bottoms[1].x = x+1; bottoms[1].y = y+2;

						rot_pts[0].x = x+2; rot_pts[0].y = y;
						rot_pts[1].x = x+2; rot_pts[1].y = y+1;
						rot_pts[2].x = x; rot_pts[2].y = y;
						rot_pts[3].x = x; rot_pts[3].y = y+1;
						rot_pts[4].x = x-1; rot_pts[4].y = y-1;
						rot_pts[5].x = x-1; rot_pts[5].y = y;
						rot_pts[6].x = x-1; rot_pts[6].y = y+1;
						break;
					}
					else{valid_move = 1; break;}
				case 2:
					for(size_t i = 0; i < rot_checks; ++i){
						if(read_screen(rot_pts[i])){valid_move = 0; break;}
					}
					if(valid_move){
						state = 3;
						for(size_t i = 0; i < 4; ++i){
							write_to_screen(control_buf[(4*n) + i], ' ');
						}
						control_buf[4*n].x = x+2; control_buf[4*n].y = y;
						control_buf[(4*n) + 1].x = (x+1); control_buf[(4*n) + 1].y = y;
						control_buf[(4*n) + 2].x = x; control_buf[(4*n) + 2].y = y;
						control_buf[(4*n) + 3].x = x; control_buf[(4*n) + 3].y = y+1;

						num_lefts = 2; num_rights = 2; num_bottoms = 3, rot_checks = 7;

						lefts[0].x = x-1; lefts[0].y = y;
						lefts[1].x = x-1; lefts[1].y = y+1;

						rights[0].x = x+3; rights[0].y = y;
						rights[1].x = x+1; rights[1].y = y+1;

						bottoms[0].x = x; bottoms[0].y = y+2;
						bottoms[1].x = x+1; bottoms[1].y = y+1;
						bottoms[2].x = x+2; bottoms[2].y = y+1;

						rot_pts[0].x = x; rot_pts[0].y = y+2;
						rot_pts[1].x = x+1; rot_pts[1].y = y+1;
						rot_pts[2].x = x+1; rot_pts[2].y = y+2;
						rot_pts[3].x = x+2; rot_pts[3].y = y+1;
						rot_pts[4].x = x+2; rot_pts[4].y = y+2;
						rot_pts[5].x = x+1; rot_pts[5].y = y-1;
						rot_pts[6].x = x+2; rot_pts[6].y = y-1;
						break;
					}
					else{valid_move = 1; break;}
				case 3:
					for(size_t i = 0; i < rot_checks; ++i){
						if(read_screen(rot_pts[i])){valid_move = 0; break;}
					}
					if(valid_move){
						state = 4;
						for(size_t i = 0; i < 4; ++i){
							write_to_screen(control_buf[(4*n) + i], ' ');
						}
						control_buf[4*n].x = x+1; control_buf[4*n].y = y-1;
						control_buf[(4*n) + 1].x = (x+1); control_buf[(4*n) + 1].y = y;
						control_buf[(4*n) + 2].x = x+1; control_buf[(4*n) + 2].y = y+1;
						control_buf[(4*n) + 3].x = x+2; control_buf[(4*n) + 3].y = y+1;

						num_lefts = 3; num_rights = 3; num_bottoms = 2, rot_checks = 7;

						lefts[0].x = x; lefts[0].y = y-1;
						lefts[1].x = x; lefts[1].y = y;
						lefts[2].x = x; lefts[2].y = y+1;

						rights[0].x = x+2; rights[0].y = y-1;
						rights[1].x = x+2; rights[1].y = y;
						rights[2].x = x+3; rights[2].y = y+1;

						bottoms[0].x = x+1; bottoms[0].y = y+2;
						bottoms[1].x = x+2; bottoms[1].y = y+2;

						rot_pts[0].x = x; rot_pts[0].y = y;
						rot_pts[1].x = x; rot_pts[1].y = y-1;
						rot_pts[2].x = x+2; rot_pts[2].y = y-1;
						rot_pts[3].x = x+2; rot_pts[3].y = y;
						rot_pts[4].x = x+3; rot_pts[4].y = y-1;
						rot_pts[5].x = x+3; rot_pts[5].y = y;
						rot_pts[6].x = x+3; rot_pts[6].y = y+1;
						break;
					}
					else{valid_move = 1; break;}
				case 4:
					for(size_t i = 0; i < rot_checks; ++i){
						if(read_screen(rot_pts[i])){valid_move = 0; break;}
					}
					if(valid_move){
						state = 1;
						for(size_t i = 0; i < 4; ++i){
							write_to_screen(control_buf[(4*n) + i], ' ');
						}
						control_buf[4*n].x = x; control_buf[4*n].y = y;
						control_buf[(4*n) + 1].x = (x+1); control_buf[(4*n) + 1].y = y;
						control_buf[(4*n) + 2].x = (x+2); control_buf[(4*n) + 2].y = y;
						control_buf[(4*n) + 3].x = (x+2); control_buf[(4*n) + 3].y = y-1;

						num_lefts = 2; num_rights = 2; num_bottoms = 3, rot_checks = 7;

						lefts[0].x = x-1; lefts[0].y = y;
						lefts[1].x = x+1; lefts[1].y = y-1;

						rights[0].x = x+3; rights[0].y = y;
						rights[1].x = x+3; rights[1].y = y-1;

						bottoms[0].x = x; bottoms[0].y = y+1;
						bottoms[1].x = x+1; bottoms[1].y = y+1;
						bottoms[2].x = x+2; bottoms[2].y = y+1;

						rot_pts[0].x = x; rot_pts[0].y = y+1;
						rot_pts[1].x = x+1; rot_pts[1].y = y+1;
						rot_pts[2].x = x; rot_pts[2].y = y-1;
						rot_pts[3].x = x; rot_pts[3].y = y-2;
						rot_pts[4].x = x+1; rot_pts[4].y = y-1;
						rot_pts[5].x = x+1; rot_pts[5].y = y-2;
						rot_pts[6].x = x+2; rot_pts[6].y = y-2;

						break;
					}
					else{valid_move = 1; break;}
				default:
					break;
			} break;
		case 4:
			break;
		  default:
			break;
			
	}
}

void move(){
	switch (dir){
		case 1:
			for(size_t i = 0; i < num_rights; ++i){
				if(read_screen(rights[i])){valid_move = 0; ++bad_moves; break;}
			}
			if(valid_move){
				++loop_count;
				++x;
				for(size_t i = 0; i < 4; ++i){
					write_to_screen(control_buf[(4*n) + i], ' ');
				}
				++control_buf[4*n].x;
				++control_buf[(4*n) + 1].x;
				++control_buf[(4*n) + 2].x;
				++control_buf[(4*n) + 3].x;
				for(size_t i = 0; i < num_bottoms; ++i){++bottoms[i].x;}
				for(size_t i = 0; i < num_lefts; ++i){++lefts[i].x;}
				for(size_t i = 0; i < num_rights; ++i){++rights[i].x;}
				for(size_t i = 0; i < rot_checks; ++i){++rot_pts[i].x;}
				break;
			}
			else{valid_move = 1; break;}
		case 2:
			for(size_t i = 0; i < num_lefts; ++i){
				if(read_screen(lefts[i])){valid_move = 0; break;}
			}
			if(valid_move){
				--x;
				for(size_t i = 0; i < 4; ++i){
					write_to_screen(control_buf[(4*n) + i], ' ');
				}
				--control_buf[4*n].x;
				--control_buf[(4*n) + 1].x;
				--control_buf[(4*n) + 2].x;
				--control_buf[(4*n) + 3].x;
				for(size_t i = 0; i < num_bottoms; ++i){--bottoms[i].x;}
				for(size_t i = 0; i < num_lefts; ++i){--lefts[i].x;}
				for(size_t i = 0; i < num_rights; ++i){--rights[i].x;}
				for(size_t i = 0; i < rot_checks; ++i){--rot_pts[i].x;}
				break;
			}
			else{valid_move = 1; break;}
		default:
			break;
	}
}

void move_down(){
	for(unsigned i = 0; i < num_bottoms; ++i){
		if(read_screen(bottoms[i])){valid_move = 0; break;}
	}
	if(valid_move){
		++y;
		for(size_t i = 0; i < 4; ++i){
			write_to_screen(control_buf[(4*n) + i], ' ');
			++control_buf[(4*n) + i].y;
			
		}
		for(size_t i = 0; i < num_bottoms; ++i){++bottoms[i].y;}
		for(size_t i = 0; i < num_lefts; ++i){++lefts[i].y;}
		for(size_t i = 0; i < num_rights; ++i){++rights[i].y;}
		for(size_t i = 0; i < rot_checks; ++i){++rot_pts[i].y;}
	}
	else{
		char filled_pts = 0;
		struct point2D check_pt;
		for(int i = screen_height - 1; i >= 0; --i){
			check_pt.y = i;
			for(unsigned j = 0; j < screen_width; ++j){
				check_pt.x = j;
				if(read_screen(check_pt)){++filled_pts;}
				else{break;} 
			}
			if(filled_pts == screen_width){
				rows_filled[filled_rows] = check_pt.y;
				++filled_rows;
			}
			filled_pts = 0;
		}
		if(filled_rows > 0){signal_row = 1; valid_move = 1; return;}
		else{signal_new = 1; valid_move = 1; return;}
	}
}

void clear_rows(){
	char filled_pts = 0;
	struct point2D control_pt, check_pt;
	label1:
	for(unsigned i = 0; i < filled_rows; ++i){
		control_pt.y = rows_filled[i];
		for(unsigned j = 0; j < screen_width; ++j){
			control_pt.x = j;
			write_to_screen(control_pt, ' ');
		}
		for(int j = control_pt.y - 1; j >= 0; --j){
			control_pt.y = j;
			for(unsigned k = 0; k < screen_width; ++k){
				control_pt.x = k;
				if(read_screen(control_pt)){
					write_to_screen(control_pt, ' ');
					++control_pt.y;
					write_to_screen(control_pt, '0');
					--control_pt.y;
				}
			}
		}
		for(size_t j = 0; j < filled_rows; ++j){
			++rows_filled[i];
		}
	}
	score += (filled_rows * screen_width);
	filled_rows = 0;
	for(int i = screen_height - 1; i >= 0; --i){
		check_pt.y = i;
		for(unsigned j = 0; j < screen_width; ++j){
			check_pt.x = j;
			if(read_screen(check_pt)){++filled_pts;}
			else{break;} 
		}
		if(filled_pts == screen_width){
			rows_filled[filled_rows] = check_pt.y;
			++filled_rows;
		}
		filled_pts = 0;
	}
	if(filled_rows > 0){goto label1;}
	else{signal_new = 1; valid_move = 1; filled_rows = 0; return;}
}

void* start_rendering(){
	char* dummy = "s";
	useconds_t timer = 100000;
	while(1){
		if(signal_down){move_down(); signal_down = 0;}
		if(signal_row){clear_rows(); signal_row = 0;}
		if(signal_new){generate_new(); signal_new = 0;}

		if(gameover){return (void*)dummy;}

		if(signal_move){move(); signal_move = 0;}
		if(signal_rotate){rotate(); signal_rotate = 0;}
		
		for(size_t i = 0; i < 4; ++i){
			write_to_screen(control_buf[(4*n) + i], '0');
		}
		system("clear");
		draw_screen();
		printf("Score: %d\n", score);
		usleep(timer);	
	}
	return (void*)dummy;
}

void* keyboard_listener(){
	char* dummy = "s";
	while(1){
		keypress(0);
		switch(key_pressed){
			case 'D':
			case 'd':
				dir = 1; signal_move = 1; break;
			case 'A':
			case 'a':
				dir = 2; signal_move = 1; break;
			case 'W':
			case 'w':
				signal_rotate = 1; break;
			default:
				break;
		}
	}
	return (void*)dummy;
}

int main(){
	initialize_screen(screen_height, screen_width);

	if(posix_memalign((void*)&rows_filled, 64, 4*(sizeof(unsigned))) != 0){printf("Memory allocation error.\n");}
	if(posix_memalign((void*)&rot_pts, 64, 16*(sizeof(struct point2D))) != 0){printf("Memory allocation error.\n");}
	if(posix_memalign((void*)&lefts, 64, 4*(sizeof(struct point2D))) != 0){printf("Memory allocation error.\n");}
	if(posix_memalign((void*)&rights, 64, 4*(sizeof(struct point2D))) != 0){printf("Memory allocation error.\n");}
	if(posix_memalign((void*)&bottoms, 64, 4*(sizeof(struct point2D))) != 0){printf("Memory allocation error.\n");}

	useconds_t timer_main = 750000;

	pthread_t thread_id_keyboard;
	pthread_create(&thread_id_keyboard, NULL, keyboard_listener, NULL);

	pthread_t thread_id_renderer;
	pthread_create(&thread_id_renderer, NULL, start_rendering, NULL);

	signal_new = 1;

	while(!gameover){
		usleep(timer_main);
		signal_down = 1;
	}

	pthread_cancel(thread_id_keyboard);
	pthread_cancel(thread_id_renderer);

	free(rows_filled); free(rot_pts); free(lefts); free(rights); free(bottoms);

	tcsetattr(STDIN_FILENO, TCSANOW, &savedState);

	system("clear");
	printf("GAME OVER\n");

	return 0;
}






