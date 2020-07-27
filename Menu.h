#ifndef MENU_H
#define MENU_H

class Menu {
protected:

	std::string name;

	std::vector<std::shared_ptr<Menu>> options;

	bool nested = false;

public:

	size_t selection = 0;

	bool isNested() { return nested; }

	std::string getName() { return name; }

	std::shared_ptr<Menu> getOption(size_t index) { return options[index]; }

	std::shared_ptr<Menu> getSelection() { return options.size() > 0 ? options[selection] : nullptr; }

	size_t optionCount() { return options.size(); }

	void addOption(std::shared_ptr<Menu> option) {
		options.push_back(option);
	}

	virtual bool moveUp() {
		if (selection > 0) {
			selection--;

			return true;
		}

		return false;
	}

	virtual bool moveDown() {
		if (selection < options.size() - 1) {
			selection++;

			return true;
		}

		return false;
	}

	virtual std::shared_ptr<Menu> select() = 0;

	virtual void start() = 0;
	virtual void draw(olc::PixelGameEngine* engine) = 0;
	virtual void exit() = 0;
};

#endif