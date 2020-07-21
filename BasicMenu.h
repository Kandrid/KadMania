#ifndef BASIC_MENU_H
#define BASIC_MENU_H

#include "Menu.h"

class BasicMenu : public Menu {
private:
	olc::Sprite selectionBox;
public:
	BasicMenu(const char name[]) {
		nested = true;
		this->name = name;
		this->options = options;
	}

	Menu* select() override {
		return options[selection];
	}

	void draw(olc::PixelGameEngine* engine) override {
		if (options.size() > 0) {
			uint32_t spacing = engine->ScreenHeight() / 12;
			for (size_t i = 0; i < options.size(); i++) {
				engine->DrawString(spacing, 10 + i * spacing, options[i]->getName(), i == selection ? olc::WHITE : olc::DARK_GREY, engine->ScreenHeight() / 100);
			}
		}
	}

	void exit() override {

	}
};

#endif