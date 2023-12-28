//A simple game were you control the jerk, of a jerk, so that you can bully people.
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
using namespace std;
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
//Bully Class
class Jerk {
	private:
		//setup variables
		int acceleration = 0;
		SDL_Surface * image = IMG_Load("Images/jerk.png");
		SDL_Rect pos;
	public:
		Jerk(SDL_Window* win) {
			pos.x = 0;
			pos.y = 0;
			pos.w = 50;
			pos.h = 100;
		}
		void display(SDL_Window* win) {
			SDL_BlitSurface(image, NULL, SDL_GetWindowSurface(win), &pos);
		}

		SDL_Rect getPosition() {
			return pos;
		}
};
//function to move the bully
//function to display the bully
//Homescreen mainloop
const char* home(SDL_Window * win) {
	SDL_Event ev;
	Jerk jerk(win);
	SDL_Surface * home_page = IMG_Load("Images/homepage.png");
	SDL_Rect home_page_pos;
	home_page_pos.x = 0;
	home_page_pos.y = 0;
	home_page_pos.w = 1360;
	home_page_pos.h = 660;
	while (true){
		SDL_Surface * win_surface = SDL_GetWindowSurface(win);
		SDL_BlitSurface(home_page, NULL, win_surface, &home_page_pos);
		jerk.display(win);
		SDL_UpdateWindowSurface(win);
		//get events
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) {
				return "Quit";
			}
			else if (ev.type == SDL_MOUSEBUTTONDOWN) {
				if (pointRectCollision(ev.button.x, ev.button.y, jerk.getPosition())) {
					cout << "hey, that tickles" << endl;
				}
				else {
					cout << "Naa na na boo boo" << endl;
				}
			}
		}
	}
}
//	get events
//		cursor (can be used to move to the start location)
//		display the window
//Game mainloop
const char* game(SDL_Window* win) {
	return "NOT READY";
//	get events
//		arrows(move)
//		space(bully if your close enough to someone who has your item or, if their is nobody, but their is a bully, then you can knock them down before they bully you)
//		a (add item to your bulliers inventory if your close enough to something)
//		cursor (can be used to exit back to homescreen)
//	display all characters
//	move all characters (acording to a set speed as pixels/second)
//	randomly spawn an item
//	randomly spawn a character
//	randomly spawn a bully
}
//main function(will call the mainloop functions)
int main(int argc, char** args) {
	//Setup
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* win = SDL_CreateWindow("Control the Jerk", 0, 0, 1360, 660, 0);
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
