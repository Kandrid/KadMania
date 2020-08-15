#ifndef BASIC_MENU_H
#define BASIC_MENU_H

#include "Menu.h"

class BasicMenu : public Menu {
public:

	std::shared_ptr<olc::Sprite> background;

	BasicMenu(const char name[], std::shared_ptr<olc::Sprite> background) {
		this->nested = true;
		this->name = name;
		this->options = options;
		this->background = background;
	}

	void start() override {

	}

	std::shared_ptr<Menu> select() override {
		return options[selection];
	}

	void draw(olc::PixelGameEngine* engine) override {
		engine->DrawSprite(0, 0, background.get());

		engine->SetPixelMode(olc::Pixel::Mode::ALPHA);

		int32_t spacing = engine->ScreenHeight() / static_cast<int32_t>(options.size());
		int32_t scaling = spacing / 9;

		if (spacing > 50) spacing = 50;

		if (scaling > 4) scaling = 4;
		if (scaling < 1) scaling = 1;

		for (size_t i = 0; i < options.size(); i++) {
			engine->DrawString(spacing, 5 + static_cast<int32_t>(i) * spacing, options[i]->getName(), selection == i ? olc::WHITE : olc::PixelF(100, 100, 100, 0.5f), scaling);
		}

		engine->SetPixelMode(olc::Pixel::Mode::NORMAL);
	}

	void exit() override {

	}
};

#endif