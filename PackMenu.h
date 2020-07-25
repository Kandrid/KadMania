#ifndef PACK_MENU_H
#define PACK_MENU_H

#include "Menu.h"

class PackMenu : public Menu {
public:
	PackMenu(std::string name) {
		this->name = name;
	}

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
