#pragma once

#include <string>
#include <iostream>
#include <C:/Users/User/Desktop/CS/SemesterE/SPL/Assingment3 New/boost_1_61_0/boost/asio.hpp>
#include <C:/Users/User/Desktop/CS/SemesterE/SPL/Assingment3 New/boost_1_61_0/boost/system/error_code.hpp>
#include <C:/Users/User/Desktop/CS/SemesterE/SPL/Assingment3 New/boost_1_61_0/boost/system/error_code.hpp>
#include <C:/Users/User/Desktop/CS/SemesterE/SPL/Assingment3 New/boost_1_61_0/boost/system/system_error.hpp>
#include "../include/boost_1_61_0/boost"
#include <boost>
#include "C:/User/Downloads/MinGW"

using boost::asio::ip::tcp;

class socket;

class io_service;

class ConnectionHandler {
private:
	const std::string host_;
	const short port_;
	boost::asio::io_service io_service_;   // Provides core I/O functionality
	tcp::socket socket_;

public:

	ConnectionHandler(std::string host, short port);

	virtual ~ConnectionHandler();

	// Connect to the remote machine
	bool connect();

	// Read a fixed number of bytes from the server - blocking.
	// Returns false in case the connection is closed before bytesToRead bytes can be read.
	bool getBytes(char bytes[], unsigned int bytesToRead);

	// Send a fixed number of bytes from the client - blocking.
	// Returns false in case the connection is closed before all the data is sent.
	bool sendBytes(const char bytes[], int bytesToWrite);

	// Read an ascii line from the server
	// Returns false in case connection closed before a newline can be read.
	bool getLine(std::string &line);

	// Send an ascii line from the server
	// Returns false in case connection closed before all the data is sent.
	bool sendLine(std::string &line);

	// Get Ascii data from the server until the delimiter character
	// Returns false in case connection closed before null can be read.
	bool getFrameAscii(std::string &frame, char delimiter);

	// Send a message to the remote host.
	// Returns false in case connection is closed before all the data is sent.
	bool sendFrameAscii(const std::string &frame, char delimiter);

	// Close down the connection properly.
	void close();

}; //class ConnectionHandler
