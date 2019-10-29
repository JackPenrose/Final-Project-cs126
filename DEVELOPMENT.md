# DEVELOPMENT
## Check in 1
### Initial Work
Started off by exploring the ofxHttp library and ofxJson trying to get them to work. I was unsuccessful in getting the example projects to work so instead i switched over to using the ofxCurl (https://github.com/morphogencc/ofxCurl) library and was able to get the example to load. I also looked into the Nba Stats APi and found some documentation on using the api with user agent and endpoint documentation.
### Next Steps
Started trying to access the NBA stats api with using curl and changing the user agent and endpoints that I was using. Spent many hours trying to figure out the problem with this. After some research I found out that the NBA Stats api needed verification as of a month ago, so I found the api proBasketball api (https://probasketballapi.com/) which utilizes the NBA stats api and puts it in a much easier to work with format. The only downside is that I will have to continue to make free acccounts with different emails because after the first week it becomes $20 a month.
### Using the API
I began to see if I could get data from the api, and experimented with finding players profiels and shot charts. I constructed a small open frameworks program that is able to find a players shot chart based on the user inputted name.
### Next Steps
* work on the application interface 
* work on compiling the player data
## Check in 2
### Utilizing datGUI
This week I started to create the user interface for the simple application. I started by making a new openframeworks project and making sure the example works. After figuring out how it works for my computer I began editing the preset gui. After figuring out I needed to make a scroll view I used the example and edited it to fit my needs. 
### Combining API and datGUI
Next I made a couple of methods in order to get all the availabble players from the website and find their name and their id and store their name in a vector. This vector is what the scrollview uses and scrolls through. So each player is their own button. I will use this to make the shot charts for the players. I then found out how to load images into the screen and uses them.
### Next Steps
* create the player shot charts
* clean up the interface and make it more user freindly
## FINAL WEEK
### Finishing up
In the final week I implemented the drawing of all the shots, and the court itself. I also cleaned and refractored the programs and removed magic numbers. I also added in some tests for the helper methods.
