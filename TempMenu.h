#ifndef TEMP_MENU_H
#define TEMP_MENU_H

#include "Menu.h"

class TempMenu : public Menu {
public:

	TempMenu(const char name[]) {
		this->name = name;
	}

	void start() override {

	}

	std::shared_ptr<Menu> select() override {
		return nullptr;
	}

	void draw(olc::PixelGameEngine* engine) override {

	}

	void exit() override {

	}
};

#endif