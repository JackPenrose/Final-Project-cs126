#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "HttpClient.h"
#include <vector>

class ofApp : public ofBaseApp {

public:
	int WINDOW_WIDTH = 1920;
	int WINDOW_HEIGHT = 1080;
	bool pic = false;
	void setup();
	void getShots(int index);
	void draw();
	void update();
	void drawMade();
	void drawMissed();
	void makeScrollView();
	string name;
	int playerIndex;
	vector<int> xCoord;
	vector<int> yCoord;
	vector<int> xmCoord;
	vector<int> ymCoord;
	void drawCourt();
	CURLcode res;
	std::vector<string> playerVector;
	std::vector<string> shotVector;
	std::pair<vector<pair<int, int>>, vector<pair<int, int>>> locationPair;
	std::pair<vector<pair<int, int>>, vector<pair<int, int>>> getShotsToPair(std::vector<string> shots);
	pair<int, int> getLocation(string shot);
	bool checkIfMade(string shot);
	std::vector<string> getPlayersTovector(string json);
	std::vector<string> getShotsToVector(string json);
	ofxDatGui* gui;
	bool mFullscreen;
	void refreshWindow();
	void toggleFullscreen();
	void keyPressed(int key);
	void onButtonEvent(ofxDatGuiButtonEvent e);
	void onToggleEvent(ofxDatGuiToggleEvent e);
	size_t tIndex;
	vector<ofxDatGuiTheme*> themes;
	int width;
	ofxDatGuiTheme* theme;
	ofxDatGuiScrollView* view;
	ofxDatGuiButton* addItem;
	std::shared_ptr<midnight::http::HttpClient> mHttpClient;
	static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
	void onScrollViewEvent(ofxDatGuiScrollViewEvent e);
	string getplayerid(string data);
	void onAddNewItemButtonClick(ofxDatGuiButtonEvent e);
	string getNameToString(string name);
};