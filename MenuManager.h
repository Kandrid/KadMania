#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "TempMenu.h"
#include "BasicMenu.h"
#include "ConfigMenu.h"
#include "PackMenu.h"
#include "PackListMenu.h"

#include <stack>

class MenuManager {
private:

	olc::PixelGameEngine* engine = nullptr;

	std::stack<std::shared_ptr<Menu>> menuStack;

	std::shared_ptr<Menu> root;

public:

	MenuManager() {}

	MenuManager(std::shared_ptr<Menu> root, olc::PixelGameEngine* engine) {
		this->engine = engine;
		this->root = root;
		menuStack.push(root);
	}

	std::shared_ptr<Menu> getCurrentMenu() { return menuStack.top(); }
	std::shared_ptr<Menu> getRootMenu() { return root; }

	bool upMenu() {
		return menuStack.top()->moveUp();
	}

	bool downMenu() {
		return menuStack.top()->moveDown();
	}

	bool selectMenu() {
		std::shared_ptr<Menu> newMenu = menuStack.top()->select();

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
};

#endif