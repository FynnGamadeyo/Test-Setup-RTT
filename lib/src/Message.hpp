#ifndef MESSAGE_HPP
#define MESSAGE_HPP 1

#include <string>


struct Message{
	std::string text;
	Message():text(""){};
	Message(const std::string& msg):text(msg){};		
};

#endif