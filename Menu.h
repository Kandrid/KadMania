#ifndef MENU_H
#define MENU_H

class Menu {
protected:
	std::string name;
	std::vector<Menu*> options;
	olc::Sprite background;
	bool nested = false;
public:
	size_t selection = 0;

	bool isNested() { return nested; }
	std::string getName() { return name; }
	Menu* getOption(size_t index) { return options[index]; }
	size_t optionCount() { return options.size(); }

	void addOption(Menu* option) {
		options.push_back(option);
	}

	void moveUp() {
		if (selection > 0) {
			selection--;
		}
	}

	void moveDown() {
		if (selection < options.size() - 1) {
			selection++;
		}
	}

	void free() {
		for (size_t i = 0; i < options.size(); i++) {
			options[i]->free();
			delete options[i];
		}
	}

	virtual Menu* select() = 0;
	virtual void draw(olc::PixelGameEngine* engine) = 0;
	virtual void exit() = 0;
};

#endif