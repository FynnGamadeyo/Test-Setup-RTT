#include <rtt/os/main.h>

#include <boost/program_options.hpp>
#include <iostream>
#include <rtt/internal/GlobalEngine.hpp>
#include <rtt/TaskContext.hpp>

#ifdef OROGEN_SERVICE_DISCOVERY_ACTIVATED
#include <service_discovery/ServiceDiscovery.hpp>
#endif // OROGEN_SERVICE_DISCOVERY_ACTIVATED

#include <rtt/typekit/RealTimeTypekit.hpp>
#include <rtt/transports/corba/TransportPlugin.hpp>
#include <rtt/transports/mqueue/TransportPlugin.hpp>

#include <rtt/types/TypekitPlugin.hpp>
#include <client/typekit/Plugin.hpp>
#include "typekit/transports/corba/TransportPlugin.hpp"
#include "typekit/transports/mqueue/TransportPlugin.hpp"
#include "typekit/transports/typelib/TransportPlugin.hpp"

#include <base/typekit/Plugin.hpp>

#include <base/transports/corba/TransportPlugin.hpp>

#include <base/transports/mqueue/TransportPlugin.hpp>

#include <base/transports/typelib/TransportPlugin.hpp>


#include <logger/typekit/Plugin.hpp>

#include <logger/transports/corba/TransportPlugin.hpp>

#include <logger/transports/mqueue/TransportPlugin.hpp>

#include <logger/transports/typelib/TransportPlugin.hpp>


#include <std/typekit/Plugin.hpp>

#include <std/transports/corba/TransportPlugin.hpp>

#include <std/transports/mqueue/TransportPlugin.hpp>

#include <std/transports/typelib/TransportPlugin.hpp>


#include <rtt/transports/corba/ApplicationServer.hpp>
#include <rtt/transports/corba/TaskContextServer.hpp>
#include <rtt/transports/corba/CorbaDispatcher.hpp>
#include <signal.h>


#include <rtt/Activity.hpp>

#include <rtt/Logger.hpp>
#include <rtt/base/ActivityInterface.hpp>


/// Bernd's library
#include <rtt/remote/ISetupGenerator.hpp>
#include <rtt/remote/INameService.hpp>
#include <rtt_corba_remote/CorbaTaskContextServer.hpp>
#include <rtt/remote/NameServiceFactory.hpp>
#include <rtt/remote/TaskContextServerFactory.hpp>
#include <rtt/remote/TaskContextServerCollection.hpp>

int i = 0;

#include <rtt/plugin/PluginLoader.hpp>


#include <dlfcn.h>

namespace orogen
{
extern RTT::TaskContext *create_client__TaskB(std::string const &instance_name);
extern RTT::TaskContext *create_logger__Logger(std::string const &instance_name);
}

namespace po = boost::program_options;

class Deinitializer
{
    friend Deinitializer &operator << (Deinitializer &, RTT::base::ActivityInterface &);

    std::vector<RTT::base::ActivityInterface *> m_activities;

#ifdef OROGEN_SERVICE_DISCOVERY_ACTIVATED
    friend Deinitializer &operator << (Deinitializer &, servicediscovery::avahi::ServiceDiscovery &);

    std::vector<servicediscovery::avahi::ServiceDiscovery *> m_service_discoveries;
#endif


public:
    ~Deinitializer() {
        for (std::vector<RTT::base::ActivityInterface *>::const_iterator it = m_activities.begin();
                it != m_activities.end(); ++it) {
            (*it)->stop();
        }

#ifdef OROGEN_SERVICE_DISCOVERY_ACTIVATED
        for (std::vector<servicediscovery::avahi::ServiceDiscovery *>::iterator sit = m_service_discoveries.begin();
                sit != m_service_discoveries.end(); ++sit) {
            (*sit)->stop();
            delete *sit;
        }
#endif
    }
};

Deinitializer &operator << (Deinitializer &deinit, RTT::base::ActivityInterface &activity)
{
    deinit.m_activities.push_back(&activity);
    return deinit;
}

#ifdef OROGEN_SERVICE_DISCOVERY_ACTIVATED
Deinitializer &operator << (Deinitializer &deinit, servicediscovery::avahi::ServiceDiscovery &service_discovery)
{
    deinit.m_service_discoveries.push_back(&service_discovery);
    return deinit;
}
#endif

int sigint_com[2];
void sigint_quit_orb(int)
{
    uint8_t dummy = 0;
    unsigned int sent = 0;
    while (sent < sizeof(uint8_t)) {
        int ret = write(sigint_com[1], &dummy, sizeof(uint8_t));
        if (ret < 0) {
            std::cerr << "Failed to signal quit to orb" << std::endl;
            break;
        }
        sent += ret;
    }
}

int ORO_main(int argc, char *argv[])
{
    po::options_description desc("Options");

    desc.add_options()
    ("help", "show all available options supported by this deployment")
    ("prefix", po::value<std::string>(), "Sets a prefix for all TaskContext names")
#ifdef OROGEN_SERVICE_DISCOVERY_ACTIVATED
    ("sd-domain", po::value<std::string>(), "set service discovery domain")
#endif // OROGEN_SERVICE_DISOCVERY_ACTIVATED
    ("with-ros", po::value<bool>()->default_value(false), "also publish the task as ROS node, default is false")
    ("rename", po::value< std::vector<std::string> >(), "rename a task of the deployment: --rename oldname:newname");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }




    RTT::types::TypekitRepository::Import(new RTT::types::RealTimeTypekitPlugin);

    RTT::types::TypekitRepository::Import(new RTT::corba::CorbaLibPlugin);


    RTT::types::TypekitRepository::Import(new RTT::mqueue::MQLibPlugin);



    RTT::types::TypekitRepository::Import(new orogen_typekits::clientTypekitPlugin);

    RTT::types::TypekitRepository::Import(new orogen_typekits::clientCorbaTransportPlugin);

    RTT::types::TypekitRepository::Import(new orogen_typekits::clientMQueueTransportPlugin);

    RTT::types::TypekitRepository::Import(new orogen_typekits::clientTypelibTransportPlugin);




    RTT::types::TypekitRepository::Import(new orogen_typekits::baseTypekitPlugin);

    RTT::types::TypekitRepository::Import(new orogen_typekits::baseCorbaTransportPlugin);

    RTT::types::TypekitRepository::Import(new orogen_typekits::baseMQueueTransportPlugin);

    RTT::types::TypekitRepository::Import(new orogen_typekits::baseTypelibTransportPlugin);



    RTT::types::TypekitRepository::Import(new orogen_typekits::loggerTypekitPlugin);

    RTT::types::TypekitRepository::Import(new orogen_typekits::loggerCorbaTransportPlugin);

    RTT::types::TypekitRepository::Import(new orogen_typekits::loggerMQueueTransportPlugin);

    RTT::types::TypekitRepository::Import(new orogen_typekits::loggerTypelibTransportPlugin);



    RTT::types::TypekitRepository::Import(new orogen_typekits::stdTypekitPlugin);

    RTT::types::TypekitRepository::Import(new orogen_typekits::stdCorbaTransportPlugin);

    RTT::types::TypekitRepository::Import(new orogen_typekits::stdMQueueTransportPlugin);

    RTT::types::TypekitRepository::Import(new orogen_typekits::stdTypelibTransportPlugin);


    ///// ApplicationServer ///


    std::string prefix = "";

    if (vm.count("prefix"))
        prefix = vm["prefix"].as<std::string>();

    bool with_ros = false;

    if (vm.count("with-ros"))
        with_ros = vm["with-ros"].as<bool>();

    std::string task_name;

    std::map<std::string, std::string> rename_map;

    if (vm.count("rename")) {

        const std::vector< std::string> &ren_vec = vm["rename"].as<std::vector <std::string> >();

        for (unsigned int i = 0; i < ren_vec.size(); i++) {

            const std::string &ren_str = ren_vec.at(i);

            unsigned int colon_pos = ren_str.find(':');
            if (colon_pos == std::string::npos) continue;

            rename_map.insert(std::pair<std::string, std::string>(
                                  ren_str.substr(0, colon_pos), ren_str.substr(colon_pos + 1)));
        }
    }



// Initialize some global threads so that we can properly setup their threading
// parameters
    RTT::internal::GlobalEngine::Instance(ORO_SCHED_OTHER, RTT::os::LowestPriority);

//First Create all Tasks to be able to set some (slave-) activities later on in the second loop
    task_name = "orogen_default_client__TaskB";
    if (rename_map.count(task_name))
        task_name = rename_map[task_name];
    else
        task_name = prefix + task_name;

    // Implem Begin
    std::auto_ptr<RTT::TaskContext> task_orogen_default_client__TaskB(
        orogen::create_client__TaskB(task_name));

    ///////////////////////
    ////////// Instance of TaskContextServerFactory create a taskcontext server object
    ////////// based upon the desired implementation option.

    void *handle=0;
    boost::shared_ptr<RTT::plugin::PluginLoader>  ploader = RTT::plugin::PluginLoader::Instance();
    std::string const path = "/home/nam/Arbeit/Roc/rtt_corba_remote/build/src/";
    typedef void(*setupGenerator_sig)();
    setupGenerator_sig setupGenerator;
    
    if (ploader->loadPlugin("librtt_corba_remote", path)) {
        std::cout << "load plugin success\n";

       
        handle = ploader->getHandler("rtt_corba_remote");
       

        setupGenerator = (setupGenerator_sig) dlsym(handle, "SetupGenerator");
        const char *dlsym_error = dlerror();
        if (dlsym_error) {
            std::cout  << "this plugin is not a NameService\n";
        } else {
            setupGenerator();
        }
    } else {
        std::cout << "fail to load plugin\n";
    }    
    
    
    /////////////////////////////////////////////////////////////////////////////////////


    RTT::Communication::ITaskContextServer::shared_ptr tcs;
    ploader = RTT::plugin::PluginLoader::Instance();

    typedef RTT::Communication::ITaskContextServer::shared_ptr(*createTaskServer_sig)(RTT::TaskContext *, int, char **);
    createTaskServer_sig createTaskServer;
    if (handle != 0) {
        createTaskServer = (createTaskServer_sig) dlsym(handle, "CreateService");
        const char *dlsym_error = dlerror();
        if (dlsym_error) {
            std::cerr << "this plugin is not a TaskContextServer\n";
          
        } else {
            std::cout << "Success: execute createTaskServer()\n";
            tcs = createTaskServer(task_orogen_default_client__TaskB.get(),  argc, argv);
        }
    } else {
      
    }



    ///// implem END

    task_name = "orogen_default_client__TaskB_Logger";
    if (rename_map.count(task_name))
        task_name = rename_map[task_name];
    else
        task_name = prefix + task_name;

    std::auto_ptr<RTT::TaskContext> task_orogen_default_client__TaskB_Logger(
        orogen::create_logger__Logger(task_name));

    RTT::Communication::ITaskContextServer::shared_ptr loggerTCS =
        RTT::Communication::TaskContextServerFactory::GetInstance()->CreateTaskContextServer("CORBA_TCS", task_orogen_default_client__TaskB_Logger.get(),  argc, argv);

    createTaskServer(task_orogen_default_client__TaskB_Logger.get(),  argc, argv);
    // Loggers
    /*
    RTT::corba::TaskContextServer::Create( task_orogen_default_client__TaskB_Logger.get() );
    RTT::corba::CorbaDispatcher::Instance( task_orogen_default_client__TaskB_Logger->ports(), ORO_SCHED_OTHER, RTT::os::LowestPriority );
    */



//Create all Activities afterwards to be sure all tasks are created. The Activitied are also handeld by the deployment because
//the order needs to be known since slav activities are useable
//



    //B WICHTIG !!!
    RTT::Activity *activity_orogen_default_client__TaskB = new RTT::Activity(
        ORO_SCHED_OTHER,
        RTT::os::LowestPriority,
        0,
        task_orogen_default_client__TaskB->engine(),
        "orogen_default_client__TaskB");


    {
        RTT::os::Thread *thread = dynamic_cast<RTT::os::Thread *>(activity_orogen_default_client__TaskB);
        if (thread)
            thread->setStopTimeout(10);
    }

    task_orogen_default_client__TaskB->setActivity(activity_orogen_default_client__TaskB);
    //B Wichtig


    RTT::Activity *activity_orogen_default_client__TaskB_Logger = new RTT::Activity(
        ORO_SCHED_OTHER,
        RTT::os::LowestPriority,
        0,
        task_orogen_default_client__TaskB_Logger->engine(),
        "orogen_default_client__TaskB_Logger");


    {
        RTT::os::Thread *thread = dynamic_cast<RTT::os::Thread *>(activity_orogen_default_client__TaskB_Logger);
        if (thread)
            thread->setStopTimeout(10);
    }

    task_orogen_default_client__TaskB_Logger->setActivity(activity_orogen_default_client__TaskB_Logger);



    Deinitializer deinit;

#ifdef OROGEN_SERVICE_DISCOVERY_ACTIVATED
    if (vm.count("sd-domain")) {
        servicediscovery::avahi::ServiceConfiguration sd_conf_orogen_default_client__TaskB(task_orogen_default_client__TaskB->getName(), vm["sd-domain"].as<std::string>());
        sd_conf_orogen_default_client__TaskB.setDescription("IOR", RTT::corba::TaskContextServer::getIOR(task_orogen_default_client__TaskB.get()));
        sd_conf_orogen_default_client__TaskB.setDescription("TASK_MODEL", "client::TaskB");
        servicediscovery::avahi::ServiceDiscovery *sd_orogen_default_client__TaskB = new servicediscovery::avahi::ServiceDiscovery();
        deinit << *sd_orogen_default_client__TaskB;
        sd_orogen_default_client__TaskB->start(sd_conf_orogen_default_client__TaskB);

        servicediscovery::avahi::ServiceConfiguration sd_conf_orogen_default_client__TaskB_Logger(task_orogen_default_client__TaskB_Logger->getName(), vm["sd-domain"].as<std::string>());
        sd_conf_orogen_default_client__TaskB_Logger.setDescription("IOR", RTT::corba::TaskContextServer::getIOR(task_orogen_default_client__TaskB_Logger.get()));
        sd_conf_orogen_default_client__TaskB_Logger.setDescription("TASK_MODEL", "logger::Logger");
        servicediscovery::avahi::ServiceDiscovery *sd_orogen_default_client__TaskB_Logger = new servicediscovery::avahi::ServiceDiscovery();
        deinit << *sd_orogen_default_client__TaskB_Logger;
        sd_orogen_default_client__TaskB_Logger->start(sd_conf_orogen_default_client__TaskB_Logger);
    }
#endif // OROGEN_SERVICE_DISCOVERY_ACTIVATED




    // Start some activities
    deinit << *activity_orogen_default_client__TaskB;


    deinit << *activity_orogen_default_client__TaskB_Logger;



    if (with_ros) {
        throw std::runtime_error("Requesting to start as ROS node, but the support for 'ros' transport is not available. Recompile with 'ros' transport option!");
    }

    /** Setup shutdown procedure on SIGINT. We use a pipe-based channel to do
        so, as we can't shutdown the ORB from the signal handler */
    if (pipe(sigint_com) == -1) {
        std::cerr << "failed to setup SIGINT handler: " << strerror(errno) << std::endl;
        return 1;
    }

    struct sigaction sigint_handler;
    sigint_handler.sa_handler = &sigint_quit_orb;
    sigemptyset(&sigint_handler.sa_mask);
    sigint_handler.sa_flags     = 0;
    sigint_handler.sa_restorer  = 0;
    if (-1 == sigaction(SIGINT, &sigint_handler, 0)) {
        std::cerr << "failed to install SIGINT handler" << std::endl;
        return 1;
    }
    sigset_t unblock_sigint;
    sigemptyset(&unblock_sigint);
    sigaddset(&unblock_sigint, SIGINT);
    if (-1 == sigprocmask(SIG_UNBLOCK, &unblock_sigint, NULL)) {
        std::cerr << "failed to install SIGINT handler" << std::endl;
        return 1;
    }

    tcs->Start();
  
    while (true) {
        uint8_t dummy;
        int read_count = read(sigint_com[0], &dummy, 1);
        if (read_count == 1)
            break;
    }

    tcs->Stop();


    return 0;
}

