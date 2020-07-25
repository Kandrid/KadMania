#ifndef BASIC_MENU_H
#define BASIC_MENU_H

#include "Menu.h"

class BasicMenu : public Menu {
private:
public:
	olc::Sprite* background;

	BasicMenu(const char name[], olc::Sprite* background) {
		nested = true;
		this->name = name;
		this->options = options;
		this->background = background;
	}

	void start() override {

	}

	Menu* select() override {

		return options[selection];
	}

	void draw(olc::PixelGameEngine* engine) override {
		engine->SetPixelMode(olc::Pixel::Mode::ALPHA);
		engine->DrawSprite(0, 0, background);

		uint32_t spacing = engine->ScreenHeight() / 10;

		for (size_t i = 0; i < options.size(); i++) {
			engine->DrawString(spacing, spacing / 2 + i * spacing, options[i]->getName(), i == selection ? olc::WHITE : olc::PixelF(100, 100, 100, 0.5f), engine->ScreenHeight() / 100);
		}

		engine->SetPixelMode(olc::Pixel::Mode::NORMAL);
	}

	void exit() override {

	}
};

#endif