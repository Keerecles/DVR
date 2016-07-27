#ifndef __BASE_H__
#define __BASE_H__
#include <iostream>
#include <string>
#include <list>
#include <pthread.h>
class Base {
public:
    Base(std::string className);                 // default constructor
    ~Base();              // destructor
    std::string getClassName();  //get class name
    bool msgListEmpty();
    void * getMsgFromListFront();
    void addMsgToListBack(void * msg);
    virtual void handleMsg(void * msg, void *data);

    pthread_mutex_t * getMutexPtr();
    pthread_cond_t * getCondPtr();
    
private:
    std::string mClassName; //class name
    std::list<void *> mMsgList;  //msg list

    pthread_mutex_t mutex ;
    pthread_cond_t cond ;
};
#endif // __BASE_H__
