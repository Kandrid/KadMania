#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <SFML/Audio.hpp>
#include "MenuManager.h"
#include <iostream>
#include <memory>

class KadMania : public olc::PixelGameEngine
{
private:
	MenuManager mManager;
	sf::Music jukebox;
	sf::SoundBuffer tick, select, exit;
	sf::Sound soundbox;
	olc::Sprite* default_background;
	uint32_t volume = 30;
public:
	KadMania()
	{
		sAppName = "KadMania";
	}

	bool OnUserCreate() override
	{
		default_background = new olc::Sprite("images/fallback.png");

		BasicMenu* mainMenu = new BasicMenu("Main", default_background);
		mainMenu->addOption(new SongListMenu("Songs", default_background));
		BasicMenu* optionMenu = new BasicMenu("Options", default_background);
		optionMenu->addOption(new TempMenu("Volume"));
		optionMenu->addOption(new TempMenu("Vsync"));
		mainMenu->addOption(optionMenu);
		mainMenu->addOption(new TempMenu("Exit"));

		mManager = MenuManager(mainMenu, this);

		if (!jukebox.openFromFile("audio/ambience.ogg") || !tick.loadFromFile("audio/tick.ogg") || !select.loadFromFile("audio/select.ogg") || !exit.loadFromFile("audio/exit.ogg")) {
			return false;
		}

		soundbox.setVolume(100.0f * volume / 100.0f);

		jukebox.setVolume(25.0f * volume / 100.0f);
		jukebox.setLoop(true);
		jukebox.play();
		jukebox.setPlayingOffset(sf::milliseconds(1800));

		return true;
	}

	bool OnUserDestroy() override {
		mManager.cleanup();
		delete default_background;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetKey(olc::Key::ESCAPE).bPressed) {
			if (mManager.exitMenu()) {
				soundbox.setBuffer(exit);
				soundbox.play();
				soundbox.setPlayingOffset(sf::milliseconds(200));
			}
		}
		else if (mManager.getCurrentMenu()->getName() == "Song") {
			// In-Song logic
		}
		else if (GetKey(olc::Key::ENTER).bPressed) {
			if (mManager.selectMenu()) {
				soundbox.setBuffer(select);
				soundbox.play();
			}
			if (mManager.getCurrentMenu()->getSelection()) {
				if (mManager.getCurrentMenu()->getSelection()->getName() == "Exit") {
					return false;
				}
			}
		}
		else if (GetKey(olc::Key::UP).bPressed) {
			if (mManager.upMenu()) {
				soundbox.setBuffer(tick);
				soundbox.play();
			}
		}
		else if (GetKey(olc::Key::DOWN).bPressed) {
			if (mManager.downMenu()) {
				soundbox.setBuffer(tick);
				soundbox.play();
			}
		}

		mManager.drawMenu();

		return true;
	}
};

int main()
{
	KadMania program;

	if (program.Construct(1280, 720, 1, 1)) {
		if (!program.Start()) {
			return -2;
		}
	}
	else {
		return -1;
	}

	return 0;
}