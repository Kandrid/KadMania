#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "BasicMenu.h"
#include "ConfigMenu.h"
#include "SongListMenu.h"
#include "TempMenu.h"

#include <stack>

class MenuManager {
private:
	olc::PixelGameEngine* engine = nullptr;
	std::stack<Menu*> menuStack;
	Menu* root = nullptr;
public:
	MenuManager() {}

	MenuManager(Menu* root, olc::PixelGameEngine* engine) {
		this->engine = engine;
		this->root = root;
		menuStack.push(root);
	}

	Menu* getCurrentMenu() { return menuStack.top(); }
	Menu* getRootMenu() { return root; }

	bool upMenu() {
		return menuStack.top()->moveUp();
	}

	bool downMenu() {
		return menuStack.top()->moveDown();
	}

	bool selectMenu() {
		Menu* newMenu = menuStack.top()->select();

		if (newMenu) {
			newMenu->start();
			if (newMenu->isNested()) {
				menuStack.push(newMenu);
				return true;
			}
		}

		return false;
	}

	bool exitMenu() {
		if (menuStack.size() > 1) {
			menuStack.top()->exit();
			menuStack.pop();
			return true;
		}
		return false;
	}

	void drawMenu() {
		menuStack.top()->draw(engine);
	}

	void cleanup() {
		root->free();
	}
};

#endif