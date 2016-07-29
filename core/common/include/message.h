#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include "commonType.h"
typedef int t_int;
typedef unsigned int t_uint;
class Message {
public:
    // default constructor
    Message()
        : mClassName("Message"){}

    ~Message(){}
    
    void dump()
    {
    }
    
    std::string origin;
    std::string destination;
    t_int       opertation_id;
    std::string      receiver;
    std::string mClassName;
    std::vector<void *> dataListVoid;
    std::vector<unsigned long> dataListUINT;
    std::vector<std::string> dataListString;

    typedef void *(*func)(void *);
    std::vector<func> funcList;
    
#if 0
    void *data; //destination obj
    void *data_extra_1;
    unsigned long threadID;
#endif
};

#endif // __MESSAGE_H__
