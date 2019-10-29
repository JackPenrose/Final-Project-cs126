#include "ofApp.h"

/*
	All components instantiated within a gui
	https://github.com/braitsch/ofxDatGui @braitsch
*/

using namespace midnight::http;

size_t  ofApp::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

void ofApp::onScrollViewEvent(ofxDatGuiScrollViewEvent e)
{
	int index = e.target->getIndex();// the index of the item that was selected (zero based)
	ofxDatGuiButton* button = e.target; // a pointer to the button that was selected // 
	ofxDatGuiScrollView* parent = e.parent; // a pointer to the scrollview that dispatched the event //
	cout << button->getLabel() << " [index " << e.target->getIndex() << "] selected in [" << parent->getName() << "]" << endl;
	playerIndex = e.target->getIndex();
	name = button->getLabel();
	xCoord.clear();
	yCoord.clear();
	xmCoord.clear();
	ymCoord.clear();
	ofApp::getShots(playerIndex);
	vector < pair<int, int>> made = locationPair.first;
	for (std::vector<pair<int, int>>::const_iterator i = made.begin(); i != made.end(); ++i) {
		xCoord.push_back(2 * (*i).first);
		yCoord.push_back(-2 * (*i).second);
	}
	vector < pair<int, int>> missed = locationPair.second;
	for (std::vector<pair<int, int>>::const_iterator i = missed.begin(); i != missed.end(); ++i) {
		xmCoord.push_back(2 * (*i).first);
		ymCoord.push_back(-2 * (*i).second);
	}
	pic = true;
}

string ofApp::getplayerid(string data) {
	std::size_t found = data.find(',');
	int jsonNameSize = 14;
	return data.substr(jsonNameSize, (found - jsonNameSize));
}

void ofApp::onAddNewItemButtonClick(ofxDatGuiButtonEvent e)
{
}

string ofApp::getNameToString(string name) {
	string field = "api_key=58SKpLBrAfnHyj4eihxwOEC2WzIP7cVq&first_name=";
	std::size_t found = name.find(' ');
	string firstName = name.substr(0, found);
	string lastName = name.substr(found);
	field += firstName;
	field += "&last_name=";
	field += lastName.substr(1);
	return field;
}

std::vector<string> ofApp::getShotsToVector(string json) {
	std::vector<string> shotVector;
	int num = 0;
	for (int i = 0; i < json.length() - 10; i = num) {
		std::size_t found1 = json.find("{", i);
		std::size_t found2 = json.find("}", i);
		string player = json.substr(found1 + 1, (found2 - found1) - 1);
		shotVector.push_back(player);
		num = json.find("{", found2);
	}
	return shotVector;
}

void ofApp::setup()
{
	mHttpClient = HttpClient::make();
	CURL *curl = curl_easy_init();
	string readBuffer;
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://probasketballapi.com/players");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "api_key=58SKpLBrAfnHyj4eihxwOEC2WzIP7cVq");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ofApp::WriteCallback);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		playerVector = ofApp::getPlayersTovector(readBuffer);
	}
	ofBackground(ofColor::lightGray);
	// instantiate and position the gui //
	gui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
	gui->addBreak();
	gui->addBreak();
	ofSetWindowShape(WINDOW_WIDTH, WINDOW_HEIGHT);
	ofSetWindowPosition(ofGetScreenWidth() / 2 - ofGetWidth() / 2, 0);
	width = 540;
	theme = new ofxDatGuiThemeMidnight();
	ofApp::makeScrollView();
	// and a couple of simple buttons //
	gui->addToggle("toggle fullscreen", true);
	// adding the optional header allows you to drag the gui around //
	gui->addHeader(":: drag me to reposition ::");
	// adding the optional footer allows you to collapse/expand the gui //
	gui->addFooter();
	// launch the app //
	mFullscreen = true;
	refreshWindow();
}

void ofApp::getShots(int index) {
	string readBuffer1;
	CURL *curl1 = curl_easy_init();
	string field = ofApp::getNameToString(playerVector.at(index));
	curl_easy_setopt(curl1, CURLOPT_URL, "https://probasketballapi.com/players");
	curl_easy_setopt(curl1, CURLOPT_POSTFIELDS, field.c_str());
	curl_easy_setopt(curl1, CURLOPT_WRITEFUNCTION, ofApp::WriteCallback);
	curl_easy_setopt(curl1, CURLOPT_SSL_VERIFYPEER, false);
	curl_easy_setopt(curl1, CURLOPT_WRITEDATA, &readBuffer1);
	res = curl_easy_perform(curl1);
	curl_easy_cleanup(curl1);
	CURL *curl2 = curl_easy_init();
	string readBufferr;
	string pfield = "api_key=58SKpLBrAfnHyj4eihxwOEC2WzIP7cVq&player_id=" + ofApp::getplayerid(readBuffer1);
	curl_easy_setopt(curl2, CURLOPT_URL, "https://probasketballapi.com/shotcharts");
	curl_easy_setopt(curl2, CURLOPT_POSTFIELDS, pfield.c_str());
	curl_easy_setopt(curl2, CURLOPT_WRITEFUNCTION, ofApp::WriteCallback);
	curl_easy_setopt(curl2, CURLOPT_SSL_VERIFYPEER, false);
	curl_easy_setopt(curl2, CURLOPT_WRITEDATA, &readBufferr);
	res = curl_easy_perform(curl2);
	curl_easy_cleanup(curl2);
	shotVector = ofApp::getShotsToVector(readBufferr);
	locationPair = getShotsToPair(shotVector);
	vector<pair<int, int>> made = locationPair.second;
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
	cout << "onButtonEvent: " << e.target->getLabel() << endl;
}

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)
{
	if (e.target->is("toggle fullscreen")) toggleFullscreen();
	cout << "onToggleEvent: " << e.target->getLabel() << " " << e.checked << endl;
}

void ofApp::draw() {
	if (pic) {
		drawCourt();
		int bitMapWidth = 1050;
		int bitMapHeight = 50;
		ofDrawBitmapString(name, bitMapWidth, bitMapHeight);
		drawMissed();
		drawMade();
	}
	view->draw();
	addItem->draw();
}
void ofApp::update() {
	view->update();
	addItem->update();
}

void ofApp::drawMade()
{
	for (int i = 0; i < xCoord.size(); i++) {
		ofFill();
		ofSetColor(ofColor::blue);
		int courtX = 1100;
		int courtY = 1000;
		int width = 5;
		ofCircle(xCoord.at(i) + courtX, yCoord.at(i) + courtY, width);
	}
}

void ofApp::drawMissed()
{
	for (int i = 0; i < xmCoord.size(); i++) {
		ofNoFill();
		ofSetColor(ofColor::red);
		int courtX = 1100;
		int courtY = 1000;
		int width = 5;
		ofCircle(xmCoord.at(i) + courtX, ymCoord.at(i) + courtY, width);
	}
}

void ofApp::makeScrollView()
{
	int numePlayersDisplayed = 8;
	// create a scroll view that displays eight items at a time //
	addItem = new ofxDatGuiButton("select a player");
	addItem->setStripeVisible(false);
	addItem->setWidth(width);
	addItem->setPosition(0, 0);
	addItem->setLabelAlignment(ofxDatGuiAlignment::CENTER);
	addItem->onButtonEvent(this, &ofApp::onAddNewItemButtonClick);
	view = new ofxDatGuiScrollView("ScrollView #1", numePlayersDisplayed);
	view->setWidth(width);
	view->setPosition(addItem->getX(), addItem->getY() + addItem->getHeight() + 1);
	view->onScrollViewEvent(this, &ofApp::onScrollViewEvent);

	for (std::vector<string>::const_iterator i = playerVector.begin(); i != playerVector.end(); ++i) {
		view->add(*i);
	}

}

void ofApp::drawCourt()
{
	int courtLeftSidelineX = 600;
	int courtLeftSideliney = 1065;
	int courtLength = 1000;
	int courtHeight = 940;
	int threeFeet = 60;
	int midrangeHeight = 785;
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectangle(courtLeftSidelineX, courtLeftSideliney, courtLength, -courtHeight);
	ofDrawLine(courtLeftSidelineX + threeFeet, courtLeftSideliney, courtLeftSidelineX + threeFeet, midrangeHeight);
	ofDrawLine(courtLeftSidelineX + courtLength - threeFeet, courtLeftSideliney, courtLeftSidelineX + courtLength - threeFeet, midrangeHeight);
	ofPath threePointArc;
	int theta1 = 25;
	int theta2 = 155;
	int middleCourt = 1100;
	int top3ptArc = 985;
	int threePointlength = -485;
	int threePointheight = -475;
	threePointArc.arc(middleCourt, top3ptArc, threePointlength, threePointheight, theta1, theta2);
	threePointArc.setFillColor(ofColor::black);
	threePointArc.draw();
	ofPath noFill;
	int threeArclength2 = -483;
	int threePointheight2 = -473;
	noFill.arc(middleCourt, top3ptArc, threeArclength2, threePointheight2, theta1, theta2);
	noFill.setFillColor(ofColor::lightGray);
	noFill.draw();
	ofNoFill();
	ofSetColor(ofColor::black);
	int lengthRestricted = 320;
	int heightRestricted = -380;
	int twoFeet = 40;
	int cornerToThree = 240;
	ofDrawRectangle(courtHeight, courtLeftSideliney, lengthRestricted, heightRestricted);
	ofDrawRectangle(courtHeight + twoFeet, courtLeftSideliney, cornerToThree, heightRestricted);
	ofCircle(1100, courtLength, 14);
	int backBoardHeight = 1070;
	int backBoardLength = 1014;
	int backboardWidth = 5;
	ofSetLineWidth(backboardWidth);
	ofDrawLine(backBoardHeight, backBoardLength, backBoardHeight + threeFeet, backBoardLength);
	ofSetLineWidth(1);
	int hoopHeight = 685;
	ofDrawCircle(middleCourt, hoopHeight, cornerToThree/2);
}

std::pair<vector<pair<int, int>>, vector<pair<int, int>>> ofApp::getShotsToPair(std::vector<string> shots)
{
	vector<pair<int, int>> madeShots;
	vector<pair<int, int>> missedShots;
	for (std::vector<string>::const_iterator i = shots.begin(); i != shots.end(); ++i) {
		if (ofApp::checkIfMade(*i)) {
			madeShots.push_back(ofApp::getLocation(*i));
		}
		else {
			missedShots.push_back(ofApp::getLocation(*i));
		}
	}
	return pair< vector<pair<int, int>>, vector<pair<int, int>>>(madeShots, missedShots);
}

pair<int,int> ofApp::getLocation(string shot) {
	std::size_t xfound1 = shot.find("loc_x");
	std::size_t xfound2 = shot.find(",", xfound1);
	std::size_t yfound1 = shot.find("loc_y");
	std::size_t yfound2 = shot.find(",", yfound1);
	int locJsonSize = 7;
	string xLoc = shot.substr(xfound1 + locJsonSize, (xfound2 - xfound1) - locJsonSize);
	int x = std::stoi(xLoc);
	string yLoc = shot.substr(yfound1 + locJsonSize, (yfound2 - yfound1) - locJsonSize);
	int y = std::stoi(yLoc);
	return pair<int, int> (x,y);
}

bool ofApp::checkIfMade(string shot)
{
	std::size_t found = shot.find("shot_made");
	int outcomeJsonSize = 11;
	string outcome = shot.substr(found + outcomeJsonSize, 1);
	return (outcome == "1");
}

std::vector<string> ofApp::getPlayersTovector(string json)
{
	std::vector<string> playerVector;
	int num = 0;
	int jsonLength = 100;
	for (int i = 0; i < json.length() - jsonLength; i = num) {
		std::size_t found1 = json.find("player_name", i);
		std::size_t found2 = json.find("birth", i);
		int jsonPlayerName = 14;
		string player = json.substr(found1 + jsonPlayerName, (found2 - found1) - jsonPlayerName - 3);
		playerVector.push_back(player);
		num = json.find("{", found2);
	}
	return playerVector;
}

void ofApp::keyPressed(int key)
{
	if (key == 'f') {
		toggleFullscreen();
	}
}

void ofApp::toggleFullscreen()
{
	mFullscreen = !mFullscreen;
	gui->getToggle("toggle fullscreen")->setChecked(mFullscreen);
	refreshWindow();
}

void ofApp::refreshWindow()
{
	//generated with ofxDatGUi
	ofSetFullscreen(mFullscreen);
	if (!mFullscreen) {
		int width = ofGetScreenWidth() * .8;
		int height = ofGetScreenHeight() * .8;
		ofSetWindowShape(width, height);
		ofSetWindowPosition((ofGetScreenWidth() / 2) - (width / 2), 0);
	}
}
