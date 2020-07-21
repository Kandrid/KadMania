#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "MenuManager.h"

class KadMania : public olc::PixelGameEngine
{
private:
	MenuManager mManager;
public:
	KadMania()
	{
		sAppName = "KadMania";
	}

	bool OnUserCreate() override
	{
		BasicMenu* mainMenu = new BasicMenu("Main");
		mainMenu->addOption(new TempMenu("Play"));
		BasicMenu* optionMenu = new BasicMenu("Options");
		optionMenu->addOption(new TempMenu("Volume"));
		optionMenu->addOption(new TempMenu("Resolution"));
		optionMenu->addOption(new TempMenu("Framerate"));
		optionMenu->addOption(new TempMenu("Vsync"));
		mainMenu->addOption(optionMenu);

		mManager = MenuManager(mainMenu, this);

		return true;
	}

	bool OnUserDestroy() override {
		mManager.cleanup();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetKey(olc::Key::ESCAPE).bPressed) {
			mManager.exitMenu();
		}
		else if (mManager.getCurrentMenu()->getName() == "Song") {
			// In-Song logic
		}
		else if (GetKey(olc::Key::ENTER).bPressed) {
			mManager.selectMenu();
		}
		else if (GetKey(olc::Key::UP).bPressed) {
			mManager.upMenu();
		}
		else if (GetKey(olc::Key::DOWN).bPressed) {
			mManager.downMenu();
		}

		Clear(olc::BLACK);
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