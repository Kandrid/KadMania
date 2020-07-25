#ifndef CONFIG_MENU_H
#define CONFIG_MENU_H

#include "Menu.h"

class ConfigMenu : public Menu {
public:
	void start() override {

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
