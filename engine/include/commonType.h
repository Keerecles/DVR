#ifndef _COMMON_H_
#define _COMMON_H_
#include <pthread.h>


#define MSG_KEY_BIND "bind"
#define MSG_KEY_DESTINATION "destination"
#define MSG_KEY_ORIGIN "origin"
#define MSG_KEY_RECEIVER "receiver"
#define MSG_KEY_OPID "functionid"

#define SOCKET_PATH "/tmp/socket_path"
#define CONNECT_CLIENT_MAX 5

#define SOCKET_MSG_LENGTH 1024


typedef int t_int;
typedef unsigned int t_uint;


enum ERROR_CODE {
    E_OPERATION_ERROR_NONE,
    E_OPERATION_ERROR_PARA,
    E_OPERATION_ERROR_NOT_INIT,
    E_OPERATION_ERROR_THREAD_CREATE,
    E_OPERATION_ERROR_THREAD_NUM_MAX,
    E_OPERATION_ERROR_FUNC_CALL,
    E_OPERATION_ERROR_SOCKET_CONNECTION,
    E_OPERATION_ERROR_WRITE_SOCKET,
    E_OPERATION_ERROR_UNKNOW,
};
enum THREAD_NUM {
    E_THREAD_NUM_1,
    E_THREAD_NUM_2,
    E_THREAD_NUM_3,
    E_THREAD_NUM_4,
    E_THREAD_NUM_MAX,
};
class ThreadData
{
public:
    t_int thread_num;
    pthread_t id[E_THREAD_NUM_MAX];
    void *ctx;
};

#endif
