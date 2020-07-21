#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "BasicMenu.h"
#include "ConfigMenu.h"
#include "SongMenu.h"
#include "TempMenu.h"

#include <stack>
#include <vector>

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

	void upMenu() {
		menuStack.top()->moveUp();
	}

	void downMenu() {
		menuStack.top()->moveDown();
	}

	void selectMenu() {
		Menu* newMenu = menuStack.top()->select();

		if (newMenu->isNested()) {
			menuStack.push(newMenu);
		}
	}

	void exitMenu() {
		if (menuStack.size() > 1) {
			menuStack.top()->exit();
			menuStack.pop();
		}
	}

	void drawMenu() {
		menuStack.top()->draw(engine);
	}

	void cleanup() {
		root->free();
	}
};

#endif