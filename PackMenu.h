#ifndef PACK_MENU_H
#define PACK_MENU_H
#pragma warning(disable : 4996)

#include "Menu.h"

#include <fstream>
#include <winsock.h>

class PackMenu : public Menu {
private:
	const uint32_t BANNER_WIDTH = 512;
	const uint32_t BANNER_HEIGHT = 160;

	std::shared_ptr<olc::Sprite> banner;
	std::shared_ptr<olc::Sprite> background;

	std::string* musicPath;
public:
	PackMenu(std::string name, std::shared_ptr<olc::Sprite> background, std::string* musicPath) {
		this->nested = true;
		this->name = name;
		this->background = background;
		this->banner = nullptr;
		this->musicPath = musicPath;
	}

	void loadSongList() {
		options.clear();

		for (const auto& entry : std::filesystem::directory_iterator("./Songs/" + name)) {
			if (entry.is_directory()) {
				options.push_back(std::make_shared<TempMenu>(entry.path().filename().string().c_str()));
			}
		}
	}

	/* portions derived from IJG code */

#define readbyte(a,b) do if(((a)=getc((b))) == EOF) return 0; while (0)
#define readword(a,b) do { int cc_=0,dd_=0; \
                          if((cc_=getc((b))) == EOF \
        		  || (dd_=getc((b))) == EOF) return 0; \
                          (a) = (cc_<<8) + (dd_); \
                          } while(0)


	int scanhead(FILE* infile, int* image_width, int* image_height) {
		int marker = 0;
		int dummy = 0;
		if (getc(infile) != 0xFF || getc(infile) != 0xD8)
			return 0;

		for (;
			;) {


			int discarded_bytes = 0;
			readbyte(marker, infile);
			while (marker != 0xFF) {
				discarded_bytes++;
				readbyte(marker, infile);
			}
			do readbyte(marker, infile); while (marker == 0xFF);

			if (discarded_bytes != 0) return 0;

			switch (marker) {
			case 0xC0:
			case 0xC1:
			case 0xC2:
			case 0xC3:
			case 0xC5:
			case 0xC6:
			case 0xC7:
			case 0xC9:
			case 0xCA:
			case 0xCB:
			case 0xCD:
			case 0xCE:
			case 0xCF: {
				readword(dummy, infile);	/* usual parameter length count */
				readbyte(dummy, infile);
				readword((*image_height), infile);
				readword((*image_width), infile);
				readbyte(dummy, infile);

				return 1;
				break;
			}
			case 0xDA:
			case 0xD9:
				return 0;
			default: {
				int length;

				readword(length, infile);

				if (length < 2)
					return 0;
				length -= 2;
				while (length > 0) {
					readbyte(dummy, infile);
					length--;
				}
			}
				   break;
			}
		}
	}

	bool GetImageSize(const char* fn, int* x, int* y)
	{
		FILE* f = fopen(fn, "rb"); if (f == 0) return false;
		if (scanhead(f, x, y)) {
			fclose(f);
			return true;
		}
		fseek(f, 0, SEEK_END); long len = ftell(f); fseek(f, 0, SEEK_SET);
		if (len < 24) { fclose(f); return false; }

		// Strategy:
		// reading GIF dimensions requires the first 10 bytes of the file
		// reading PNG dimensions requires the first 24 bytes of the file
		// reading JPEG dimensions requires scanning through jpeg chunks
		// In all formats, the file is at least 24 bytes big, so we'll read that always
		unsigned char buf[24]; fread(buf, 1, 24, f);

		// For JPEGs, we need to read the first 12 bytes of each chunk.
		// We'll read those 12 bytes at buf+2...buf+14, i.e. overwriting the existing buf.
		if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF && buf[3] == 0xE0 && buf[6] == 'J' && buf[7] == 'F' && buf[8] == 'I' && buf[9] == 'F')
		{
			long pos = 2;
			while (buf[2] == 0xFF)
			{
				if (buf[3] == 0xC0 || buf[3] == 0xC1 || buf[3] == 0xC2 || buf[3] == 0xC3 || buf[3] == 0xC9 || buf[3] == 0xCA || buf[3] == 0xCB) break;
				pos += 2 + (buf[4] << 8) + buf[5];
				if (pos + 12 > len) break;
				fseek(f, pos, SEEK_SET); fread(buf + 2, 1, 12, f);
			}
		}

		fclose(f);

		// GIF: first three bytes say "GIF", next three give version number. Then dimensions
		if (buf[0] == 'G' && buf[1] == 'I' && buf[2] == 'F')
		{
			*x = buf[6] + (buf[7] << 8);
			*y = buf[8] + (buf[9] << 8);
			return true;
		}

		// PNG: the first frame is by definition an IHDR frame, which gives dimensions
		if (buf[0] == 0x89 && buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G' && buf[4] == 0x0D && buf[5] == 0x0A && buf[6] == 0x1A && buf[7] == 0x0A
			&& buf[12] == 'I' && buf[13] == 'H' && buf[14] == 'D' && buf[15] == 'R')
		{
			*x = (buf[16] << 24) + (buf[17] << 16) + (buf[18] << 8) + (buf[19] << 0);
			*y = (buf[20] << 24) + (buf[21] << 16) + (buf[22] << 8) + (buf[23] << 0);
			return true;
		}

		return false;
	}

	void loadAssets() {
		bool bannerLoaded = false;
		*musicPath = "";
		std::string path = "./Songs/" + name + "/" + options[selection]->getName();
		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			if (entry.is_regular_file()) {
				path = entry.path().generic_string();
				if (*musicPath == "" && path.size() >= 3 && path.substr(path.size() - 3) == "ogg") {
					*musicPath = path;
				}
				else if (!bannerLoaded) {
					int width, height;
					if (GetImageSize(path.c_str(), &width, &height)) {
						if (width <= BANNER_WIDTH && height <= BANNER_HEIGHT) {
							banner = std::make_shared<olc::Sprite>(path);
							bannerLoaded = true;
						}
					}
				}
			}
		}
	}

	bool moveUp() override {
		if (selection > 0) {
			selection--;
			loadAssets();
			return true;
		}
		return false;
	}

	bool moveDown() override {
		if (selection < options.size() - 1) {
			selection++;
			loadAssets();
			return true;
		}
		return false;
	}

	void start() override {
		loadSongList();
		loadAssets();
	}

	std::shared_ptr<Menu> select() override {
		return nullptr;
	}

	void draw(olc::PixelGameEngine* engine) override {
		engine->DrawSprite(0, 0, background.get());

		if (banner->width == BANNER_WIDTH && banner->height == BANNER_HEIGHT) {
			engine->DrawSprite(engine->ScreenWidth() - BANNER_WIDTH, 0, banner.get());
		}
		else {
			engine->FillRect(engine->ScreenWidth() - BANNER_WIDTH, 0, BANNER_WIDTH, BANNER_HEIGHT, olc::DARK_GREY);
		}

		engine->SetPixelMode(olc::Pixel::Mode::ALPHA);

		uint32_t spacing = engine->ScreenHeight() / options.size();
		uint32_t scaling = spacing / 9;

		if (spacing > 50) spacing = 50;

		if (scaling > 4) scaling = 4;
		if (scaling < 1) scaling = 1;

		for (size_t i = 0; i < options.size(); i++) {
			engine->DrawString(spacing, 5 + i * spacing, options[i]->getName(), selection == i ? olc::WHITE : olc::PixelF(100, 100, 100, 0.5f), scaling);
		}

		engine->SetPixelMode(olc::Pixel::Mode::NORMAL);
	}

	void exit() override {
		*musicPath = "";
	}
};

#endif
