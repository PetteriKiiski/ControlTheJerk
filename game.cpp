//A simple game were you control the jerk, of a jerk, so that you can bully people.
#define JERK_PER_SECOND 0.001
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace chrono;
//to check if a point collides with a rect
bool pointRectCollision (int x, int y, SDL_Rect rect) {
	if (x >= rect.x and x <= rect.x + rect.w and y >= rect.y and y <= rect.y + rect.h) {
		return true;
	}
	return false;
}
//to check if a rect collides with another rect
bool RectCollision (SDL_Rect rect1, SDL_Rect rect2) {
	if (rect1.x < rect2.x + rect2.w or rect1.y < rect2.y + rect2.h or rect2.x < rect1.x + rect1.w or rect2.y < rect1.y + rect1.h) {
		return true;
	}	
	return false;
}

uint64_t epochTime() {
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
SDL_Rect makeRect(int x, int y, int w, int h) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
}
//Jerk Class
class Jerk {
	private:
		//setup variables
		int jerk[2] = {0, 0}; //Paired: [x-axis, y-axis]
		int acceleration[2] = {0, 0};
		int velocity[2] = {0, 0};
		SDL_Surface * image = IMG_Load("Images/jerk.png");
		SDL_Rect pos;
		SDL_Window* win;
		uint64_t last_move = epochTime();
		uint64_t left_duration = 0;
		uint64_t right_duration = 0;
		uint64_t up_duration = 0;
		uint64_t down_duration = 0;
	public:
		Jerk(SDL_Window* window) {
			pos.x = 475;
			pos.y = 280;
			pos.w = 50;
			pos.h = 100;
			win = window;
		}
		//function to display the bully
		void display() {
			SDL_BlitSurface(image, NULL, SDL_GetWindowSurface(win), &pos);
		}
		//function to move the bully
		void move() {
			uint64_t t = epochTime() - last_move;
			pos.y += t * velocity[1] + 0.5 * t * pow(acceleration[1], 2) + (1/6) * t * pow(jerk[1], 3);
			pos.x += t * velocity[0] + 0.5 * t * pow(acceleration[0], 2) + (1/6) * t * pow(jerk[0], 3);
			velocity[0] += acceleration[0] * t + 0.5 * t * pow(jerk[0], 2);
			velocity[1] += acceleration[1] * t + 0.5 * t * pow(jerk[1], 2);
			acceleration[0] += jerk[0] * t;
			acceleration[1] += jerk[1] * t;
			last_move = epochTime();
			cout << "Position: " << pos.x << ", " << pos.y << endl;
			cout << "Jerk: " << jerk[0] << ", " << jerk[1] << endl;
		}
		void left() {
			if (right_duration != 0) {
				right_duration = 0;
			}
			if (left_duration == 0) {
				left_duration = epochTime();
			}
			else {
				jerk[0] -= ((epochTime() - left_duration) / 1000) * JERK_PER_SECOND;
				left_duration = epochTime();
			}
		}
		void right() {
			if (left_duration != 0) {
				left_duration = 0;
			}
			if (right_duration == 0) {
				right_duration = epochTime();
			}
			else {
				jerk[0] += ((epochTime() - right_duration) / 1000) * JERK_PER_SECOND;
			}
		}
		void up() {
			if (down_duration != 0) {
				down_duration = 0;
			}
			if (up_duration == 0) {
				up_duration = epochTime();
			}
			else {
				jerk[1] -= (epochTime() - up_duration) * JERK_PER_SECOND; 
			}
		}
		void down() {
			if (up_duration != 0) {
				up_duration = 0;
			}
			if (down_duration == 0) {
				down_duration = epochTime();
			}
			else {
				jerk[1] += (epochTime() - up_duration) * JERK_PER_SECOND;
			}
		}
};
//Button class
class Button {
	private:
		SDL_Surface * image;
		SDL_Rect pos;
		SDL_Window* win;
	public:		
		Button(const char* file, SDL_Rect position, SDL_Window* window) {
			image = IMG_Load(file);
			pos = position;
			win = window;
		}
		void display() {
			SDL_BlitSurface(image, NULL, SDL_GetWindowSurface(win), &pos);
		}
		bool clicked(int x, int y) {
			return pointRectCollision(x, y, pos);
		}
};
//Homescreen mainloop
const char* home(SDL_Window * win) {
	//Setup images and rects
	SDL_Rect home_page_pos = makeRect(0, 0, 1000, 660);
	SDL_Event ev;
	Button button("Images/start.png", makeRect(350, 510, 300, 75), win);
	SDL_Surface* home_page = IMG_Load("Images/homepage.png");
	//No animation, no need to do this inside the mainloop
	SDL_Surface* win_surface = SDL_GetWindowSurface(win);
	SDL_BlitSurface(home_page, NULL, win_surface, &home_page_pos);
	button.display();
	SDL_UpdateWindowSurface(win);
	while (true) {
		//get events
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) {
				return "Quit";
			}
			//cursor (can be used to start the game)
			else if (ev.type == SDL_MOUSEBUTTONDOWN) {
				if (button.clicked(ev.button.x, ev.button.y)) {
					return "Game";
				}
			}
		}
	}
}
//Game mainloop
const char* game(SDL_Window* win) {
	Jerk jerk(win);
	SDL_Event ev;
	SDL_Surface* bg = IMG_Load("Images/bg.png");
	SDL_Rect bg_rect = makeRect(0, 0, 1000, 600);
	bool going_up = false;
	bool going_down = false;
	bool going_right = false;
	bool going_left = false;
	while (true) {
		SDL_BlitSurface(bg, NULL, SDL_GetWindowSurface(win), &bg_rect);
		jerk.display();
		jerk.move();
		SDL_UpdateWindowSurface(win);
		//get events
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) {
				return "Quit";
			}
			//arrows keys(move via jerk)
			if (ev.type == SDL_KEYDOWN) {
				if (ev.key.keysym.sym == SDLK_LEFT) {
					going_right = false;
					going_left = true;
				}
				if (ev.key.keysym.sym == SDLK_RIGHT) {
					going_left = false;
					going_right = true;
				}
				if (ev.key.keysym.sym == SDLK_UP) {
					going_down = false;
					going_up = true;
				}
				if (ev.key.keysym.sym == SDLK_DOWN) {
					going_up = false;
					going_down = true;
				}
			}
			if (ev.type == SDL_KEYUP) {
				if (ev.key.keysym.sym == SDLK_LEFT) {
					going_left = false;
				}
				if (ev.key.keysym.sym == SDLK_RIGHT) {
					going_right = false;
				}
				if (ev.key.keysym.sym == SDLK_UP) {
					going_up = false;
				}
				if (ev.key.keysym.sym == SDLK_DOWN) {
					going_down = false;
				}
			}
		}
		if (going_left) {
			jerk.left();
		}
		if (going_right) {
			jerk.right();
		}
		if (going_up) {
			jerk.up();
		}
		if (going_down) {
			jerk.down();
		}
	}
//		space(bully if your close enough to someone who has your item or, if their is nobody, but their is a bully, then you can knock them down before they bully you)
//		a (add item to your bulliers inventory if your close enough to something)
//		cursor (can be used to exit back to homescreen)
//	display all characters
//	move all characters (according to a set speed as pixels/second)
//	randomly spawn an item
//	randomly spawn a character
//	randomly spawn a bully
}
//main function(will call the mainloop functions)
int main(int argc, char** args) {
	//Setup
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* win = SDL_CreateWindow("Control the Jerk", 0, 0, 1000, 660, 0);
	//Mainloop
	try {
		while (true) {
			if (home(win) == "Game") {
				if (game(win) != "Home") {
					SDL_Quit();
					return 0;
				}
			}
			//Exit call
			else {
				SDL_Quit();
				return 0;
			}
		}
	}
	catch (...) {
		cout << "error" << endl;
		SDL_Quit();
		return 0;
	}
	SDL_Quit();
	return 0;
}
