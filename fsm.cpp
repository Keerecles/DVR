FSM *FSM::mInstance = NULL;

//--------------------------------------------------------------------------
/**
 *  @brief  default constructor
 */
//--------------------------------------------------------------------------
FSM::FSM()
		: mClassName("FSM")
{
	pluginInit();
}

//--------------------------------------------------------------------------
/**
 *  @brief  default destructor
 */
//--------------------------------------------------------------------------
FSM::~FSM(){

}

std::string FSM::getClassName()
{
    return mClassName;
}

FSM *FSM::getInstance()
{
    if (NULL == mInstance) {
        mInstance = new FSM();
    }
    return mInstance;
}

void FSM::pluginInit(void *data){

  	g_mutex_init (&mMutex);
    ctxEngine = static_cast<Engine*>(data);
    p_queue = g_async_queue_new ();

}

void * FSM::pluginRun(void *data){
	
	/* Start the Gst main thread*/
	startNewThread(gstMainThread);

}
    
void pluginPause(){

}
    
void FSM::pluginResume(){

}

void FSM::pluginStop(){

}

void FSM::startNewThread(void *thread){
	Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = E_ENGINE_FUNCTION_START_NEW_THREAD;
    ctxMsg->dataListVoid.push_back(FSM::getInstance());
    ctxMsg->funcList.push_back(thread);
    sendMessage(FSM::getInstance(), ctxMsg);
}

