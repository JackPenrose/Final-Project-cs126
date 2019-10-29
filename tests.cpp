
#include	"src/ofApp.h"
#define		CATCH_CONFIG_MAIN 
#include	"catch.hpp"

ofApp jack;
string name = "Jack Penrose";
string playerData = "[{{{""player_id"":979,";

TEST_CASE("Checking if getting the name to a string works", "[getNameToString]") {
	REQUIRE(jack.getNameToString(name) == "api_key=58SKpLBrAfnHyj4eihxwOEC2WzIP7cVq&first_name=Jack&last_name=Penrose");
}

TEST_CASE("checking if I can get the player id from a string","[getPlayerId]") {
	REQUIRE(jack.getplayerid(playerData) == "979");
}

TEST_CASE("checking to see if finding out if the shot is actually made", "[checkIfMade]") {
	REQUIRE_FALSE(jack.checkIfMade("asddasshot_madesdaadsasddasadsasdsda"));
	REQUIRE(jack.checkIfMade("shot_made==1,"));
}
