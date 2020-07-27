#ifndef CONFIG_MENU_H
#define CONFIG_MENU_H

#include "Menu.h"

class ConfigMenu : public Menu {
private:

	uint32_t* value;
	uint32_t floor, ceiling;

public:

	ConfigMenu(const char name[], uint32_t* ref, uint32_t floor, uint32_t ceiling) {
		this->nested = true;
		this->name = name;
		this->floor = floor;
		this->ceiling = ceiling;
		this->value = ref;
	}

	bool moveUp() override {
		if (*value < ceiling) {
			(*value)++;

			return true;
		}

		return false;
	}

	bool moveDown() override {
		if (*value > floor) {
			(*value)--;

			return true;
		}

		return false;
	}

	void start() override {

	}

	std::shared_ptr<Menu> select() override {
		return nullptr;
	}

	void draw(olc::PixelGameEngine* engine) override {
		engine->Clear(olc::BACK);

		engine->DrawString(10, 10, name, olc::WHITE, 5);
		engine->DrawString(10, 50, std::to_string(*value), olc::BLUE, 6);
	}

	void exit() override {

	}
};

#endif
