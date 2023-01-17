//
// Created by User on 17/01/2023.
//

#include "../include/Game.h"

using namespace std;

Game::Game(string teamNames):teamA(""),teamB(""),userToEvents(map<string, vector<string>>()),topicToUpdates(map<string, map<string,string>>()){
    int index = findChar(teamNames,'_');
    teamA = teamNames.substr(0,index);
    teamB = teamNames.substr(index+1,teamNames.length());
}
//Start of Getters Implementation
string Game::getNames(){
    return teamA+"_"+teamB;
}
int Game::getNumOfActiveSubscribers(){
    return numOfActiveSubscribers;
}
map<string , vector<string>> Game::getUserToEvents(){
    return userToEvents;
}
map<string, map<string,string>> Game::getTopicToUpdates() {
    return topicToUpdates;
}
//map<string, map<string,string>> Game::getTopicToUpdates(string username, string typeOfUpdate){
//    return topicToUpdates[username];

vector<string> Game::getEventsFromUser(string username){
    return userToEvents[username];
}
map<string,string> Game::getUpdatesFromUsername(string username){
    return topicToUpdates[username];
}
//End of Getters Implementation

//Start of Adders Implementation
void Game::addNumOfActiveSubscribers(int toAdd){
    numOfActiveSubscribers = numOfActiveSubscribers+toAdd;
}
void Game::addEvent(string username, string event) {
    userToEvents[username].push_back(event);
}
void Game::addUpdatesToTopic(string username, string topic, string updates){
    topicToUpdates[username][topic].append("\n"+updates);

}
//End of Adders Implementation

//Utilities
int findChar(string teamNames, char delim){
    bool found = false;
    int index;
    for(int i = 0 ; i < teamNames.length() &&!found ; i++){
        if(teamNames.at(i) == delim)
            index = i;
        found = true;
    }
}

