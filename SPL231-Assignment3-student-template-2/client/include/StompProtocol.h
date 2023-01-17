#pragma once

#include "../include/ConnectionHandler.h"
#include <thread>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include "../include/event.h"
#include "../include/Game.h"
// TODO: implement the STOMP protocol
class StompProtocol
{
private:
    //Fields
    ConnectionHandler* connectionHandler;
    string username;
    bool connected;
    int receiptId;
    int subscriptionId;
    //Frames to arrays
    vector<std::string> keyboardInput;
    vector<std::string> serverInput;
    vector<std::string> serverInputFrame;
    //Maps to track activity
    map<string , vector<string>> userToGames; //Map:user->gameName
    map<string, vector<string>> receiptIdToData; //Map:receiptID->keyboardInputCommand(login,join,exit,etc....)
    map<string, string> userToSubscriptionId; //Map:user->subscriptionIDs
    map<string , string> subscriptionIdToUser; //Map:subscriptionIDs->user
    map<string , Game*> gameNameToGameObject; //Map:gameName->Game
public:
    //Rule of Five
    StompProtocol(ConnectionHandler*, string username);// Constructor
    virtual ~StompProtocol(); //Destructor
    StompProtocol(const StompProtocol& other); //Copy Constructor
    StompProtocol(StompProtocol&& other); //Move Constructor
    StompProtocol& operator=(const StompProtocol& other); //Copy Assignment Operator
    StompProtocol& operator= (StompProtocol&& other); //Move Assignment Operator
    //Process methods
    string readFromKeyboard(); //get the frame from server
    void processFromKeyboard(std::string msg); //process the frame from server
    void processFromServer(); // get the frame from server + process it
    //Fields Methods
    void setConnected(bool condition);
    bool isConnected();
    bool shouldTerminate();
    //FrameBuilders
    static string buildConnectFrame(vector<string> &input);
    string buildSubscribeFrame(vector<string>& input);
    string buildUnsubscribeFrame(vector<string>& input);
    string buildSendFrame(vector<Event>& input);
    //Utilities
    static vector<string> stringToVector(string str);
    void sortEventsByTime(vector<Event>& events);
    map<string, map<string,string>> extractInfo(string message);

};
