//A simple game were you control the jerk, of a jerk, so that you can bully people.
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
	public:
		Jerk(SDL_Window* window) {
			pos.x = 475;
			pos.y = 280;
			pos.w = 50;
			pos.h = 100;
			win = window;
		}
		void display() {
			SDL_BlitSurface(image, NULL, SDL_GetWindowSurface(win), &pos);
		}
		//function to move the bully
		void move() {
			
		}
		//function to display the bully
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
	while (true) {
		SDL_BlitSurface(bg, NULL, SDL_GetWindowSurface(win), &bg_rect);
		jerk.display();
		SDL_UpdateWindowSurface(win);
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) {
				return "Quit";
			}
			//arrows(move)
		}
	}
//	get events
//		arrows(move)
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
	cout << epochTime() << endl;
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
