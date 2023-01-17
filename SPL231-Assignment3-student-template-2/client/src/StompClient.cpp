//
// Created by User on 16/01/2023.
//
#include <vector>
#include "../include/ConnectionHandler.h"
#include <thread>
#include "../include/StompProtocol.h"
using namespace std;
string buildConnectFrame(vector<string>& input);
int main(int argc, char *argv[]) {
    bool isValid = true;
    string host;
    int port;
    string username;
    string password;
    //first, get the login details from command login {host:port} {username} {password}
    vector<string> keyboardInput;
    string currWord;
    while (cin >> currWord)
        keyboardInput.push_back(currWord);
    if(keyboardInput[0]!="login")
        cout<<"Error: Initial command invalid"<<endl;
    else{
        int index = Game::findChar(keyboardInput[1],':');
        host = keyboardInput[1].substr(0,index);
        port = stoi(keyboardInput[1].substr(index+1,keyboardInput[1].length()));
        username = keyboardInput[2];
        password = keyboardInput[3];

    }
    ConnectionHandler connectionHandler(host,port);
    if(!connectionHandler.connect()){
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
    //Create StompProtocol
    StompProtocol stompProtocol(&connectionHandler, username);
    //Create Connect frame
    string frameToSend = StompProtocol::buildConnectFrame(keyboardInput);
    //Send frame to server
    connectionHandler.sendLine(frameToSend);
    //get answer back
    string answer;
    //make sure the frame's command is CONNECTED
    connectionHandler.getLine(answer);
    vector<string> answerToVector = StompProtocol::stringToVector(answer);
    if(answerToVector[0] != "CONNECTED")
        cout<< "Got the wrong frame, Cannot proceed!" <<endl;
    else{ //frameCommand == "CONNECTED"
        stompProtocol.setConnected(true);
    while(true){
        //create the thread that will read from the keyboard
        std::thread t1([](StompProtocol stompProtocol) {
            while (stompProtocol.isConnected()) {
                std::string line = stompProtocol.readFromKeyboard(); // Get the line from the keyboard
                stompProtocol.processFromKeyboard(line); // Process the line from the keyboard
            }
        }, stompProtocol);

        //Use main Thread to read messages from the server
        while(stompProtocol.isConnected()){
            stompProtocol.processFromServer();
        }
        t1.join();

    }
    }
}
