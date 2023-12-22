//A simple game were you control the jerk, of a jerk, so that you can bully people.
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
using namespace std;
//Homescreen mainloop
const char* Home(SDL_Window* win) {
	SDL_Event ev;
	while (true){
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) {
				return "Quit";
			}
		}
	}
}
//	get events
//		cursor (can be used to move to the start location)
//		display the window
//Game mainloop
const char* Game(SDL_Window* win) {
	return "NOT READY";
}
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
//main function(will call the mainloop functions)
int main(int argc, char** args) {
	//Setup
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* win = SDL_CreateWindow("Hello World!", 0, 0, 500, 500, 0);
	//Mainloop
	try {
		while (true) {
			if (Home(win) == "Game") {
				if (Game(win) != "Home") {
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
