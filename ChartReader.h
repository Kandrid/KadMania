#ifndef CHART_READER_H
#define CHART_READER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class ChartReader {
public:
	std::vector<std::string> chartData;
	std::string title, subtitle, artist, music, background, banner;

	ChartReader() { 
		chartData = std::vector<std::string>();
		title = subtitle = artist = music = background = banner = "";
	}

	bool loadChartData(std::string path) {
		if (path.size() >= 3 && path.substr(path.size() - 2) == "sm" || path.substr(path.size() - 3) == "ssc") {
			std::ifstream file = std::ifstream(path);
			std::string str;

			if (file.is_open()) {
				chartData.clear();

				title = subtitle = artist = music = background = banner = "";

				while (!file.eof()) {
					std::getline(file, str);

					if (str.find('#') != std::string::npos) {
						str = str.substr(str.find_first_of('#'));

						str.pop_back();

						if (str.substr(1, 6) == "TITLE:") {
							title = str.substr(7);
						}
						else if (str.substr(1, 9) == "SUBTITLE:") {
							subtitle = str.substr(10);
						}
						else if (str.substr(1, 7) == "ARTIST:") {
							artist = str.substr(8);
						} 
						else if (str.substr(1, 6) == "MUSIC:") {
							music = str.substr(7);
						}
						else if (str.substr(1, 11) == "BACKGROUND:") {
							background = str.substr(12);
						}
						else if (str.substr(1, 7) == "BANNER:") {
							banner = str.substr(8);
						}
					} else if (str.size() == 4 && str.find_first_not_of("01234M") == std::string::npos || str == ",") {
						do {
							chartData.push_back(str);
							std::getline(file, str);
						} while (!file.eof());
					}
				}

				file.close();

				return true;
			}
		}

		return false;
	}
};

#endif