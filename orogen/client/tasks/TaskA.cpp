/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */



#include "TaskA.hpp"
#include <rtt_corba_remote/CorbaTaskContextServer.hpp>
#include <rtt_corba_remote/CorbaTaskContextServer.hpp>
#include <rtt_corba_remote/CorbaNameService.hpp>
#include <rtt/plugin/PluginLoader.hpp>
#include <dlfcn.h>
using namespace client;


TaskA::TaskA(std::string const &name)
    : TaskABase(name)
{
    
//     boost::shared_ptr<RTT::plugin::PluginLoader>  ploader = RTT::plugin::PluginLoader::Instance();
//     
//     // Pfad zum Ordner "Plugin"
//     std::string const path = "/home/nam/Arbeit/Roc/rtt_corba_remote/build/src/";
//     
//     
//     // versucht alle Plugins aus dem Ordner "Plugin" zu laden
//     if (ploader->loadPlugin("librtt_corba_remote", path)) {
//         std::cout << "load plugin success\n";
// 
// 	void *handle;
//         handle = ploader->getHandler("rtt_corba_remote");
// 	if(handle==0){
// 	    std::cout << "No plugin with name " << "rtt_corba_remote \n" ;
// 	    return;
// 	}
// 	
// 	typedef void(*setupGenerator_sig)();
// 
//         setupGenerator_sig setupGenerator = (setupGenerator_sig) dlsym(handle, "SetupGenerator");
//         const char *dlsym_error = dlerror();
// 	
// 	// Prüft ob eine Methode innerhalb eines Objekts existiert 
//         if (dlsym_error) {
//             std::cout  << "this plugin is not a NameService\n";
//         } else {
//             setupGenerator();
//         }
//     } else {
//         std::cout << "fail to load plugin\n";
//    }

}

TaskA::TaskA(std::string const &name, RTT::ExecutionEngine *engine)
    : TaskABase(name, engine)
{

}

TaskA::~TaskA()
{

}

bool TaskA::configureHook()
{
    if (! TaskABase::configureHook())
        return false;



    return true;
}
bool TaskA::startHook()
{
    if (! TaskABase::startHook())
        return false;
    return true;
}
void TaskA::updateHook()
{
    TaskABase::updateHook();
    _messages.write("This is a test!");

}
void TaskA::errorHook()
{
    TaskABase::errorHook();
}
void TaskA::stopHook()
{
    TaskABase::stopHook();
}
void TaskA::cleanupHook()
{
    TaskABase::cleanupHook();
}

