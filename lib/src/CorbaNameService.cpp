#include "CorbaNameService.hpp"
#include <rtt/remote/TypedNameServiceGenerator.hpp>
#include <rtt/remote/NameServiceFactory.hpp>
#include <rtt/remote/INameServiceGenerator.hpp>


/**
 * @brief Constructor of the Corba Name Service.
 */
CorbaNameService::CorbaNameService()
{

}

/**
 * @brief Destructor of the Corba Name Service.
 */
CorbaNameService::~CorbaNameService()
{

}

/**
 * @brief This method sets up the name service generator for the  * Corba implementation, 
 * i.e. it registers this type at the generator.
 * 
 * @return void
 */
void CorbaNameService::SetupGenerator()
{
  // Create a shared pointer to a typed name service generator for the corba implementation
  RTT::Communication::INameServiceGenerator::shared_ptr pCorbaNameServiceGenerator( new RTT::Communication::TypedNameServiceGenerator<CorbaNameService>("CORBA_NAME_SERVICE") );
  
  // Register the pointer to this typed generator at the name service factory
  RTT::Communication::NameServiceFactory::GetInstance()->RegisterNameServiceGenerator("CORBA_NAME_SERVICE", pCorbaNameServiceGenerator);
}

/**
 * @brief Gets the URI for the current task context.
 * 
 * @return std::string
 */
std::string CorbaNameService::getURI(std::string Name)
{
  std::string URI = "";
  
  return URI;
}

/**
 * @brief Registers the taskcontext at the nameservice
 * 
 * @param taskcontext The task context
 * @return bool
 */
bool CorbaNameService::RegisterTaskContextServer(std::string Name, RTT::Communication::TaskContextServerCollection& tcsCollection)
{
  return true;
}
