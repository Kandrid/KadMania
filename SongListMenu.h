#ifndef SONG_LIST_MENU_H
#define SONG_LIST_MENU_H

#include "Menu.h"
#include "PackMenu.h"

#include <filesystem>

class SongListMenu : public Menu {
private:
	olc::Sprite* default_background;
	olc::Sprite* background;
public:
	SongListMenu(const char name[], olc::Sprite* background) {
		this->nested = true;
		this->name = name;
		this->default_background = background;
		this->background = default_background;
	}

	void loadPackList() {
		options.clear();

		for (const auto& entry : std::filesystem::directory_iterator("./Songs")) {
			options.push_back(new PackMenu(entry.path().filename().string()));
		}
	}

	void start() override {
		loadPackList();
	}

	Menu* select() override {
		return nullptr;
	}

	void draw(olc::PixelGameEngine* engine) override {
		engine->SetPixelMode(olc::Pixel::Mode::ALPHA);
		engine->DrawSprite(0, 0, background);

		uint32_t spacing = engine->ScreenHeight() / 20;

		for (size_t i = 0; i < options.size(); i++) {
			engine->DrawString(spacing, spacing / 2 + i * spacing, options[i]->getName(), selection == i ? olc::WHITE : olc::GREY, engine->ScreenHeight() / 200);
		}

		engine->SetPixelMode(olc::Pixel::Mode::NORMAL);
	}

	void exit() override {
		for (size_t i = 0; i < options.size(); i++) {
			delete options[i];
			options[i] = nullptr;
		}
	}
};

#endif