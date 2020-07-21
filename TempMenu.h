#ifndef TEMP_MENU_H
#define TEMP_MENU_H

#include "Menu.h"

class TempMenu : public Menu {
public:
	TempMenu(const char name[]) {
		this->name = name;
	}

	Menu* select() override {
		return nullptr;
	}

	void draw(olc::PixelGameEngine* engine) override {

	}

	void exit() override {

	}
};

#endif