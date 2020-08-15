#define OLC_PGE_APPLICATION

#include "olcPixelGameEngine.h"
#include "MenuManager.h"

#include <SFML/Audio.hpp>

#include <chrono>
#include <iostream>
#include <memory>

class KadMania : public olc::PixelGameEngine
{
private:
	const double HELD_START_DELAY = 0.3;
	const double HELD_DELAY = 0.07;

	const std::string DEFAULT_MUSIC = "audio/ambience.ogg";

	MenuManager mManager;

	sf::Music jukebox;

	enum Sound {TICK, SELECT, EXIT, SOUND_COUNT};

	sf::SoundBuffer sounds[SOUND_COUNT];

	sf::Sound soundbox;

	sf::Time themeOffset = sf::milliseconds(1800);

	std::shared_ptr<olc::Sprite> default_background;

	uint32_t volume = 30;

	std::chrono::high_resolution_clock clock;
	std::chrono::high_resolution_clock::time_point t1, t2;

	bool buttonHeld = false;

	std::string musicPathBuffer = DEFAULT_MUSIC;
	std::string musicPath = DEFAULT_MUSIC;

public:
	KadMania()
	{
		sAppName = "KadMania";
	}

	bool OnUserCreate() override
	{
		t1 = t2 = clock.now();

		default_background = std::make_shared<olc::Sprite>("images/fallback.png");

		std::shared_ptr<BasicMenu> mainMenu = std::make_shared<BasicMenu>("Main", default_background);
		mainMenu->addOption(std::make_shared<PackListMenu>("Songs", default_background, &musicPathBuffer));

		std::shared_ptr<BasicMenu> optionMenu = std::make_shared<BasicMenu>("Options", default_background);
		optionMenu->addOption(std::make_shared<ConfigMenu>("Volume", &volume, 0, 100));

		mainMenu->addOption(optionMenu);
		mainMenu->addOption(std::make_shared<TempMenu>("Exit"));

		mManager = MenuManager(mainMenu, this);

		if (!jukebox.openFromFile("audio/ambience.ogg") || !sounds[TICK].loadFromFile("audio/tick.ogg") || !sounds[SELECT].loadFromFile("audio/select.ogg") || !sounds[EXIT].loadFromFile("audio/exit.ogg")) {
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
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (musicPath != musicPathBuffer) {
			if (musicPathBuffer == "") {
				if (musicPath != DEFAULT_MUSIC) {
					if (!jukebox.openFromFile(DEFAULT_MUSIC)) {
						return false;
					}

					jukebox.setPlayingOffset(themeOffset);
					jukebox.setLoop(true);
					jukebox.play();
					musicPath = DEFAULT_MUSIC;
				}
				else {
					themeOffset = jukebox.getPlayingOffset();
				}
			}
			else {
				if (!jukebox.openFromFile(musicPathBuffer)) {
					if (!jukebox.openFromFile(DEFAULT_MUSIC)) {
						return false;
					}

					jukebox.setPlayingOffset(themeOffset);
					jukebox.setLoop(true);
					jukebox.play();
					musicPath = DEFAULT_MUSIC;
					musicPathBuffer = "";

					return true;
				}

				jukebox.setLoop(true);
				jukebox.play();

				musicPath = musicPathBuffer;
			}
		}

		soundbox.setVolume(100.0f * volume / 100.0f);
		jukebox.setVolume(25.0f * volume / 100.0f);


		if (GetKey(olc::Key::ESCAPE).bPressed) {
			if (mManager.exitMenu()) {
				soundbox.setBuffer(sounds[EXIT]);
				soundbox.play();
				soundbox.setPlayingOffset(sf::milliseconds(200));
			}
		}
		else if (mManager.getCurrentMenu()->getName() == "Song") {
			// In-Song logic
		}
		else if (GetKey(olc::Key::ENTER).bPressed) {
			if (mManager.selectMenu()) {
				soundbox.setBuffer(sounds[SELECT]);
				soundbox.play();
			}
			if (mManager.getCurrentMenu()->getSelection()) {
				if (mManager.getCurrentMenu()->getSelection()->getName() == "Exit") {
					return false;
				}
			}
		}
		else if (GetKey(olc::Key::UP).bPressed) {
			t1 = clock.now();

			if (mManager.upMenu()) {
				soundbox.setBuffer(sounds[TICK]);
				soundbox.play();
			}
		}
		else if (GetKey(olc::Key::DOWN).bPressed) {
			t1 = clock.now();

			if (mManager.downMenu()) {
				soundbox.setBuffer(sounds[TICK]);
				soundbox.play();
			}
		}
		else {
			t2 = clock.now();
			double duration = std::chrono::duration<double>(t2 - t1).count();

			if (duration >= HELD_START_DELAY|| (buttonHeld && duration >= HELD_DELAY)) {
				if (GetKey(olc::Key::UP).bHeld) {
					buttonHeld = true;
					t1 = clock.now();

					if (mManager.upMenu()) {
						soundbox.setBuffer(sounds[TICK]);
						soundbox.play();
					}
				}
				else if (GetKey(olc::Key::DOWN).bHeld) {
					buttonHeld = true;
					t1 = clock.now();

					if (mManager.downMenu()) {
						soundbox.setBuffer(sounds[TICK]);
						soundbox.play();
					}
				}
				else {
					t1 = t2 = clock.now();
					buttonHeld = false;
				}
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