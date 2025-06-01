#ifndef __CNETFINAL_NETWORKCONFIGURATOR_H
#define __CNETFINAL_NETWORKCONFIGURATOR_H

#include <omnetpp.h>

using namespace omnetpp;

namespace cnetfinal {

/**
 * Simple network configurator
 */
class NetworkConfigurator : public cSimpleModule
{
  private:
    std::string configFile;
    
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    
  public:
    // Method to fetch configuration for a specific module
    virtual void configureNode(cModule *node);
    
    // Method to setup routes between hosts
    virtual void setupRoutes();
};

} // namespace cnetfinal

#endif // __CNETFINAL_NETWORKCONFIGURATOR_H 