#include <iostream>
#include "engine.h"
#include "threadManager.h"
int main()
{
    Engine *engine = Engine::getInstance() ;
    engine->Init(engine);

    ThreadManager *ctxTM = ThreadManager::getInstance();
    ctxTM->startThread(engine->Run,engine);

#if 1
    pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    while(1)
    {
        pthread_mutex_lock(&mut);
        pthread_cond_wait(&cond, &mut);
    }
    pthread_mutex_unlock(&mut);
#endif
    return 0;
}

