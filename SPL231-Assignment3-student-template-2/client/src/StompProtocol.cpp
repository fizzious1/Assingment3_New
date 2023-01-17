//
// Created by User on 15/01/2023.
#import <stdio.h>
using namespace std;
#include "../include/StompProtocol.h"

//Start of Class
//Start of Rule of Five Implementation
//Constructor
StompProtocol::StompProtocol(ConnectionHandler* ch, string username):
connectionHandler(ch),username(username),connected(false), receiptId(0), subscriptionId(0), userToGames(map<string , vector<string>>()),
receiptIdToData(map<string, vector<string>>()), userToSubscriptionId(map<string, string>()), subscriptionIdToUser(map<string , string>()) , gameNameToGameObject(map<string,Game*>()), keyboardInput(vector<string>()){}
//Destructor
StompProtocol::~StompProtocol() {
    if(connectionHandler!=nullptr)
        delete connectionHandler;
    if(!gameNameToGameObject.empty()){
        for (auto it = gameNameToGameObject.begin(); it != gameNameToGameObject.end(); ++it) {
            delete it->second;
        }
        gameNameToGameObject.clear();
    }
}
//Copy Constructor
StompProtocol::StompProtocol(const StompProtocol& other): connectionHandler(other.connectionHandler), username(other.username), connected(other.connected),
receiptId(other.receiptId), subscriptionId(other.subscriptionId), userToGames(other.userToGames), receiptIdToData(other.receiptIdToData), userToSubscriptionId(other.userToSubscriptionId), subscriptionIdToUser(other.subscriptionIdToUser), gameNameToGameObject(other.gameNameToGameObject){}

//Move Constructor
StompProtocol::StompProtocol( StompProtocol&& other) : connectionHandler(other.connectionHandler), username(other.username), connected(other.connected),
receiptId(other.receiptId), subscriptionId(other.subscriptionId), userToGames(other.userToGames), receiptIdToData(other.receiptIdToData), userToSubscriptionId(other.userToSubscriptionId), subscriptionIdToUser(other.subscriptionIdToUser){
    other.connectionHandler = nullptr;
    other.username = nullptr;
    other.userToGames.clear();
    other.receiptIdToData.clear();
    other.userToSubscriptionId.clear();
    other.subscriptionIdToUser.clear();
    other.gameNameToGameObject.clear();
}
//Copy Operator
StompProtocol& StompProtocol::operator=(const StompProtocol& other){
    if(this != &other){
        connectionHandler = other.connectionHandler;
        username = other.username;
        connected = other.connected;
        receiptId = other.receiptId;
        subscriptionId = other.subscriptionId;
        userToGames = other.userToGames;
        receiptIdToData = other.receiptIdToData;
        userToSubscriptionId = other.userToSubscriptionId;
        subscriptionIdToUser = other.subscriptionIdToUser;
        gameNameToGameObject = other.gameNameToGameObject;
    }
    return *this;
}
//Move Operator
StompProtocol& StompProtocol::operator=(StompProtocol&& other){
    if(this != &other){
        connectionHandler = other.connectionHandler;  other.connectionHandler = nullptr;
        username = other.username;  other.username = nullptr;
        connected = other.connected;
        receiptId = other.receiptId;
        subscriptionId = other.subscriptionId;
        userToGames = other.userToGames;  other.userToGames.clear();
        receiptIdToData = other.receiptIdToData; other.receiptIdToData.clear();
        userToSubscriptionId = other.userToSubscriptionId; other.userToSubscriptionId.clear();
        subscriptionIdToUser = other.subscriptionIdToUser; other.subscriptionIdToUser.clear();
        gameNameToGameObject = other.gameNameToGameObject; other.gameNameToGameObject.clear();
    }
    return *this;
}
//End of Rule of Five Implementation

//Listen and process START
string StompProtocol::readFromKeyboard(){ // took it from echo-client
    const short bufsize = 1024;
    char buf[bufsize];
    std::cin.getline(buf, bufsize); //Reads from the keyboard into buf
    std::string line(buf); //Converts the char array to a string
    return line;
}
void StompProtocol::processFromKeyboard(string msg) { //Assume inital login already happend
    //First, get the command
    keyboardInput = stringToVector(msg);
    string command = keyboardInput[0];
    //Start of possible cases
    if (command == "login") { //DONE
        if (connected)
            cout << "user is already connected!" << endl;
        else { //user is not connected yet
            string frameToSend = StompProtocol::buildConnectFrame(keyboardInput); //build CONNECT frame
            vector<string> data; //Create vector {"join","username"}
            data.push_back(command);
            data.push_back(keyboardInput[2]);
            if (connectionHandler->sendLine(frameToSend)) { //if sending successful
                receiptId++;
                receiptIdToData[to_string(receiptId)] = data; //Map:receiptId->{"join","username"}
            } else
                receiptId--;


        }
    } else if (command == "join") { //DONE
        if (connected) {
            string frameToSend = buildSubscribeFrame(keyboardInput); //build the frame and receiptId++
            vector<string> data; //Create vector {"join","gameName"}
            data.push_back(command);
            data.push_back(keyboardInput[1]);
            if (connectionHandler->sendLine(frameToSend)) {
                receiptIdToData[to_string(receiptId)] = data;  //Map:receiptId->{"join","gameName"}

            }
            else //if frame not sent, then receiptID should be as before
                receiptId--;
        }
    } else if (command == "exit") { //Unsubscribe {gameName}
        if (connected) {
            string frameToSend = buildUnsubscribeFrame(keyboardInput); //receiptId++ inside build
            vector<string> data;
            data.push_back(command);
            data.push_back(keyboardInput[1]);
            if (connectionHandler->sendLine(frameToSend))
                receiptIdToData[to_string(receiptId)] = data;
            //userToSubscriptionId[username]
            else {
                receiptId--;
                subscriptionId--;
            }
        }
    } else if (command == "report") {
        if (isConnected()) {
            string fileToParse = keyboardInput[1];
            names_and_events gameEvents = parseEventsFile(fileToParse);
            vector<Event> events = gameEvents.events;
            vector<string> data;
            data.push_back(command);
            data.push_back(events[0].get_team_a_name() + "_" + events[0].get_team_b_name());
            sortEventsByTime(events);
            string frameToSend = buildSendFrame(events);
            if (connectionHandler->sendLine(frameToSend)) {
                receiptId++;
                receiptIdToData[to_string(receiptId)] = data;
            }
            else
                cout << "Error in sending message" << endl;
        }
    } else if (command == "summary") {
        if (connected) {
            string gameName = keyboardInput[1];
            int index = Game::findChar(gameName, '_');
            string teamA = gameName.substr(0, index);
            string teamB = gameName.substr(index + 1, gameName.length());
            Game gameToSum(gameName);
            map<string, map<string, string>> allUpdates = gameNameToGameObject[gameName]->getTopicToUpdates();
            map<string, string> generalUpdates = allUpdates.at("general game updates");
            map<string, string> teamAUpdates = allUpdates.at("team a updates");
            map<string, string> teamBUpdates = allUpdates.at("team b updates");
            //build SummaryFrame
            string frameToSend(teamA + " vs " + teamB + "\n");
            frameToSend.append("Game stats:\n");
            frameToSend.append("General stats:\n");
            for (auto it = generalUpdates.begin(); it != generalUpdates.end(); ++it)//append general updates
                frameToSend.append(it->first + ": " + it->second + "\n");
            frameToSend.append("team a stats:\n");
            for (auto it = teamAUpdates.begin(); it != teamAUpdates.end(); ++it)//append teamA updates
                frameToSend.append(it->first + ": " + it->second + "\n");
            frameToSend.append("team b stats:\n");
            for (auto it = teamBUpdates.begin(); it != teamBUpdates.end(); ++it)//append teamB updates
                frameToSend.append(it->first + ": " + it->second + "\n");
            //TODO: figure out how to parse it to existing file (or create new file and parse it)
        }
//End of possible cases
    }
}
    void StompProtocol::processFromServer() { //here we get frames from the server and process them!
        //Get frame from server
        string frameToRead;
        connectionHandler->getLine(frameToRead); //get frame from server
        serverInputFrame = stringToVector(frameToRead); //break frame to array, delim is ' '
        if (serverInputFrame[0] == "CONNECTED") {//connection successful
            cout << "Login Successful:DD" << endl;
            subscriptionId++; //new user gets a new subscriptionID!
        }
        else if (serverInputFrame[0] == "RECEIPT") { //RECEIPT frame is what we get back in most of the requests, break to cases:
            if (connected) {
                if (receiptIdToData["receipt-id"][0] ==
                    "join") { //if a channel join request sent, add game to user's map
                    string gameName = receiptIdToData["receipt-id"][1]; //get the game name
                    if (userToGames.find(gameName) == userToGames.end()) { //game not found in user's map
                        userToGames[username].push_back(gameName); //Map:username->{spain_germany,....,gameName}
                        if (gameNameToGameObject[gameName] == nullptr) //case of first subscription, needs to create new Game(gameName) Object
                            gameNameToGameObject[gameName] = new Game(gameName); //Create new Game object of that game
                        gameNameToGameObject[gameName]->addNumOfActiveSubscribers(1); //update current subscribed users
                        userToSubscriptionId[username] = subscriptionId; //Map:username->subID
                        subscriptionIdToUser[to_string(subscriptionId)] = username; //Map: subID->username
                    } else //already subscribed to the game
                        cout << "Already joined" << gameName << "channel!" << endl;

                } else if (receiptIdToData["receipt-id"][0] == "exit") { //unsubscribe a game channel
                    string gameName = receiptIdToData["receipt-id"][1]; //get the game name
                    if (userToGames.find(gameName) == userToGames.end()) //if game not found in user's map, print error msg
                        cout << username << "not subscribed to " << gameName << "channel" << endl;
                    else { //game found in user's map
                        for (std::vector<std::string>::iterator it = userToGames[username].begin();
                             it != userToGames[username].end(); it++) { //find the index of that game in user's map
                            if (*it == gameName) {
                                userToGames[username].erase(it); //delete it
                                break;
                            }
                        }
                            gameNameToGameObject[gameName]->addNumOfActiveSubscribers(-1); //update current subscribed users
                            if (gameNameToGameObject[gameName]->getNumOfActiveSubscribers() == 0) //if no active users, delete the game
                                delete gameNameToGameObject[gameName]; //delete the Game object
                        }


                } else if (receiptIdToData["receipt-id"][0] == "logout") { //unsubscribe all channels and disconnect from server
                    for (int i = 0; i < userToGames[username].size(); i++) { //extract all games the user subscribed to
                        string gameName = userToGames[username].at(i); //save the gameName
                        gameNameToGameObject[gameName]->addNumOfActiveSubscribers(
                                -1); //update current active subscribers to the Game(gameName)
                        if (gameNameToGameObject[gameName]->getNumOfActiveSubscribers() == 0) //if no active users
                            delete gameNameToGameObject[gameName]; //delete the Game object
                    }
                        userToGames[username].clear(); //unsubscribe all channels
                        setConnected(false);
                        connectionHandler->close(); //close the connection and wait for keyboard input


                }
            }
        }
        //End of RECEIPT cases
        else if(serverInputFrame[0] == "MESSAGE"){ //got a message from a channel!!
        bool found = false; //assume subscription not found
        string subID; //the subscriptionID we want to find
        for(int i = 1 ; i < 4 && !found ; i++) { //find the right header out of 3 options sIF[1]/sIF[2]/sIF[3]
            int index =
                    serverInputFrame[i].find("subscription:") + 14;// if "dest" not found, then index == string.length
            if (index < serverInputFrame[i].length()) { //found
                subID = serverInputFrame[i].substr(index, serverInputFrame[i].length());
                found = true;
            }
        }
        string user = subscriptionIdToUser[subID];
        //Message is line 0, headers are lines 1,2,3 , empty row is 4 , ^@ at row serverInput.length-1 , therefore body is in line 5-serverInput.length-2
        //Extract info from body!!, we dont know where info is stored in the array, so we will do it from frameToSend


        }
        }

    //Fields Methods
    void StompProtocol::setConnected(bool condition){
        connected = condition;
    }
bool StompProtocol::isConnected(){
    return connected;
}

    //Frame Builders start
    string buildConnectFrame(vector<string> &input) {
        if (input.size() == 4) {
            std::string hostAndPort = input[1];
            int port = stoi(hostAndPort.substr(0, 4)); //test that it works
            if (port < 1000 || port > 9999) //to make sure we get 4-digit number port
                std::cerr << "port is not valid " << ":" << port << std::endl;
            string host = hostAndPort.substr(6, hostAndPort.length());
            //Create frame to send
            //Create COMMAND
            string frame = "CONNECT\n";
            //Create headers
            frame.append("accept-version: 1.2\n"); //accept-version
            frame.append("host:" + host + "\n"); // host
            frame.append("login:" + input[2] + "\n");
            frame.append("passcode:" + input[3] + "\n\n");
            frame.append("^@");
            return frame;


        }
    }
string StompProtocol::buildSubscribeFrame(vector<string>& input){
    //first, get unique ID from the server to join this channel(topic)
    string channelToJoin = keyboardInput[1];
    //Create SUBSCRIBE Frame
    //Create frame COMMAND
    string frame = "SUBSCRIBE\n";
    //Create frame Headers
    frame.append("destination:/" + keyboardInput[1] + "\n");
    subscriptionId++;
    frame.append("id:"+ to_string(subscriptionId) + "\n");
    receiptId++;
    frame.append("receipt: " + to_string(receiptId) + "\n\n");
    frame.append("^@");
    return frame;
}
string StompProtocol::buildUnsubscribeFrame(vector<string>& input){
    string frame = "UNSUCSCRIBE";
    frame.append("id:"+ to_string(subscriptionId) + "\n");
    receiptId++;
    frame.append("receipt: "+ to_string(receiptId) + "\n");
    frame.append("^@");
    return frame;

}
string StompProtocol::buildSendFrame(vector<Event>& event){ //get vector of events, event[i]={teamA,teamB,updates,description}
    string gameName = event[0].get_team_a_name()+"_"+event[0].get_team_b_name();
    string frame = "SEND\n";
    frame.append("destination:/"+gameName + "\n\n");
    frame.append("user: "+ username + "\n");
    frame.append("team a: " + event[0].get_team_a_name() + "\n");
    frame.append("team b: "+event[0].get_team_b_name() + "\n");
    for(int i = 0 ; i < event.size() ; i++){ //iterate through all events
    frame.append("event name: "+ event[i].get_name() + "\n");
    frame.append("time: "+ to_string(event[i].get_time()) + "\n");
        frame.append("general game updates: \n");
        for (const auto& element : event[i].get_game_updates()){
           frame.append(element.first +": " + element.second+"\n"); //append general updates
           gameNameToGameObject[gameName]->addUpdatesToTopic(username,"general upadtes", element.first+": "+element.second);
            //Game::getTopicToUpdates()[username]["general updates"].append(element.first+": "+element.second);
            //Map1:username->Map2:general-updates->the update itself
        }
        frame.append("team a updates: \n");
        for (const auto& element : event[i].get_team_a_updates()){
            frame.append(element.first +": " + element.second+"\n"); //append teamA updates
            gameNameToGameObject[gameName]->addUpdatesToTopic(username,"team a updates", element.first+": "+element.second);
            //Map1:username->Map2:teamA->the update itself
        }
        frame.append("team b updates: \n");
        for (const auto& element : event[i].get_team_b_updates()){
            frame.append(element.first +": " + element.second+"\n");
            gameNameToGameObject[gameName]->addUpdatesToTopic(username,"team b updates", element.first+": "+element.second);
        }
        frame.append("description :" + event[i].get_discription() + "\n");
        gameNameToGameObject[gameName]->addUpdatesToTopic(username,"description", event[i].get_discription());
        if(gameNameToGameObject[gameName]!=nullptr) //if the game exists
        gameNameToGameObject[gameName]->addEvent(username, frame); // add events to the game

    }
        frame.append("^@");


}
//Frame Builders end
void StompProtocol::sortEventsByTime(vector<Event>& events) {
    for (int i = 0; i < events.size() - 1; i++) {
        for (int j = 0; j < events.size() - i - 1; j++) {
            if (events[j].get_time() > events[j + 1].get_time())
                swap(events[j], events[j + 1]);
        }
    }
}
vector<string> StompProtocol::stringToVector(string str) {
    vector<string> temp;
    std::stringstream lines(str);
    string line;
    while (std::getline(lines, line, '\n'))
        temp.push_back(line);
}
map<string, map<string,string>> extractInfo(string message){
    map<string, map<string,string>> infoExtracted;
    string generalGameUpdates = "general game updates: ";
    string teamAUpdates = "team a updates: ";
    string teamBUpdates = "team b updates";
    string description = "description: ";
    int generalUpdatesIndex = message.find(generalGameUpdates) + generalGameUpdates.length();
    int teamAUpdatesIndex = message.find(teamAUpdates) + teamAUpdates.length();
    int teamBUpdatesIndex = message.find(teamBUpdates) + teamBUpdates.length();
    int descriptionIndex = message.find(description) + description.length();
    generalGameUpdates = message.substr(generalUpdatesIndex,teamAUpdatesIndex);
    teamAUpdates = message.substr(teamAUpdatesIndex,teamBUpdatesIndex);
    teamBUpdates = message.substr(teamBUpdatesIndex , descriptionIndex);
    description = message.substr(descriptionIndex,message.length()-1);
    vector<string> genUpd = StompProtocol::stringToVector(generalGameUpdates);
    vector<string> teamAupd = StompProtocol::stringToVector(teamAUpdates);
    vector<string> teamBupd = StompProtocol::stringToVector(teamBUpdates);
    vector<string> desUpd = StompProtocol::stringToVector(description);
    //insert info to map
    //general info
    for(int i = 0 ; i < genUpd.size() ; i++){
        int index = Game::findChar(genUpd[i], ':'); //take only the part after the ":" from current row
        string currHeader = genUpd[i].substr(0,index);
        string relevantInfo = genUpd[i].substr(index+1,genUpd[i].length());
        infoExtracted["general game updates"][currHeader] = relevantInfo;
    }
    for(int i = 0 ; i < teamAupd.size() ; i++){
        int index = Game::findChar(teamAupd[i], ':'); //take only the part after the ":" from current row
        string currHeader = teamAupd[i].substr(0,index);
        string relevantInfo = teamAupd[i].substr(index+1,genUpd[i].length());
        infoExtracted["general game updates"][currHeader] = relevantInfo;
    }
    for(int i = 0 ; i < teamBupd.size() ; i++){
        int index = Game::findChar(teamBupd[i], ':'); //take only the part after the ":" from current row
        string currHeader = teamBupd[i].substr(0,index);
        string relevantInfo = teamBupd[i].substr(index+1,teamBupd[i].length());
        infoExtracted["general game updates"][currHeader] = relevantInfo;
    }
    for(int i = 0 ; i < desUpd.size() ; i++){
        int index = Game::findChar(desUpd[i], ':'); //take only the part after the ":" from current row
        string currHeader = desUpd[i].substr(0,index);
        string relevantInfo = desUpd[i].substr(index+1,desUpd[i].length());
        infoExtracted["general game updates"][currHeader] = relevantInfo;
    }

return infoExtracted;

}


