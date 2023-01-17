//
// Created by User on 17/01/2023.
//
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <map>

using namespace std;
class Game{
private:
    string teamA;
    string teamB;
    int numOfActiveSubscribers;
    map<string, vector<string>> userToEvents; //Map:username->events vector
     map<string, map<string,string>> topicToUpdates; //Map1:username->Map2:(general/teamA/teamB)->update
    //Example: teamA update--> (update no.3-->the update itself)
public:
    Game(string teamNames); //Constructor
    //Getters
    string getNames(); //get teamA_teamB
    int getNumOfActiveSubscribers();
     map<string , vector<string>> getUserToEvents(); //get userToEvents
    // map<string, map<string,string>> getTopicToUpdates(string username, string typeOfUpdate); //get topicToUpdates
    vector<string> getEventsFromUser(string username); //get map[user]
    map<string,string> getUpdatesFromUsername(string username); // get map[topic]
    //Adders
    void addNumOfActiveSubscribers(int toAdd);
    void addEvent(string username, string event); //add event to map[user]
    void addUpdatesToTopic(string username, string topic, string updates);



    //Utilities
    static int findChar(string teamNames, char delim);
    //void sortEventsOfUser(string username);



    map<string, map<string, string>> getTopicToUpdates();
};