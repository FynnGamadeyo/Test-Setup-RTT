/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "TaskB.hpp"
#include <rtt_corba_remote/CorbaTaskContextServer.hpp>
#include <rtt_corba_remote/CorbaNameService.hpp>
#include <rtt/plugin/PluginLoader.hpp>
#include <dlfcn.h>
using namespace client;



TaskB::TaskB(std::string const &name)
    : TaskBBase(name)
{
  
//     boost::shared_ptr<RTT::plugin::PluginLoader>  ploader = RTT::plugin::PluginLoader::Instance();
//     std::string const path = "/home/nam/Arbeit/Roc/rtt_corba_remote/build/src/";
//     if (ploader->loadPlugin("librtt_corba_remote", path)) {
//         std::cout << "load plugin success\n";
// 
//         void *handle;
//         handle = ploader->getHandler("rtt_corba_remote");
//         typedef void(*setupGenerator_sig)();
// 
//         setupGenerator_sig setupGenerator = (setupGenerator_sig) dlsym(handle, "SetupGenerator");
//         const char *dlsym_error = dlerror();
//         if (dlsym_error) {
//             std::cout  << "this plugin is not a NameService\n";
//         } else {
//             setupGenerator();
//         }
//     } else {
//         std::cout << "fail to load plugin\n";
//     }

}

TaskB::TaskB(std::string const &name, RTT::ExecutionEngine *engine)
    : TaskBBase(name, engine)
{

}

TaskB::~TaskB()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See TaskB.hpp for more detailed
// documentation about them.

bool TaskB::configureHook()
{
    if (! TaskBBase::configureHook())
        return false;





    return true;
}
bool TaskB::startHook()
{
    if (! TaskBBase::startHook())
        return false;
    return true;
}
void TaskB::updateHook()
{
    TaskBBase::updateHook();


    std::string msg;
    _messages.read(msg);

    std::cout << msg.c_str() << '\n';

}
void TaskB::errorHook()
{
    TaskBBase::errorHook();
}
void TaskB::stopHook()
{
    TaskBBase::stopHook();
}
void TaskB::cleanupHook()
{
    TaskBBase::cleanupHook();
}
