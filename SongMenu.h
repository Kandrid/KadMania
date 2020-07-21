#ifndef SONG_MENU_H
#define SONG_MENU_H

#include "Menu.h"

class SongMenu : public Menu {
public:
	Menu* select() override {
		return nullptr;
	}

	void draw(olc::PixelGameEngine* engine) override {

	}

	void exit() override {

	}
};

#endif