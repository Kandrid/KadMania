#ifndef PACK_LIST_MENU_H
#define PACK_LIST_MENU_H

#include "Menu.h"
#include "PackMenu.h"

#include <filesystem>

class PackListMenu : public Menu {
private:
	std::shared_ptr<olc::Sprite> background;
	std::string* musicPath;
public:
	PackListMenu(const char name[], std::shared_ptr<olc::Sprite> background, std::string* musicPath) {
		this->nested = true;
		this->name = name;
		this->background = background;
		this->musicPath = musicPath;
	}

	void loadPackList() {
		options.clear();

		for (const auto& entry : std::filesystem::directory_iterator("./Songs")) {
			if (entry.is_directory()) {
				options.push_back(std::make_shared<PackMenu>(entry.path().filename().string(), background, musicPath));
			}
		}
	}

	void start() override {
		loadPackList();
	}

	std::shared_ptr<Menu> select() override {
		return options[selection];
	}

	void draw(olc::PixelGameEngine* engine) override {
		engine->DrawSprite(0, 0, background.get());
		engine->SetPixelMode(olc::Pixel::Mode::ALPHA);

		uint32_t spacing = engine->ScreenHeight() / options.size();
		uint32_t scaling = spacing / 9;

		if (spacing > 50) spacing = 50;

		if (scaling > 4) scaling = 4;
		if (scaling < 1) scaling = 1;

		for (size_t i = 0; i < options.size(); i++) {
			engine->DrawString(spacing, 5 + i * spacing, options[i]->getName(), selection == i ? olc::WHITE : olc::PixelF(100, 100, 100, 0.5f), scaling);
		}

		engine->SetPixelMode(olc::Pixel::Mode::NORMAL);
	}

	void exit() override {

	}
};

#endif