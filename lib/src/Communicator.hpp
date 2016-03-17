#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP 1

#include<lib/Message.hpp>


class Communicator
{
public:
  void send(const std::string& msg);
  void print(const Message& msg);
};


#endif
