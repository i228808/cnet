#ifndef __CNETFINAL_SDNCONTROLLER_H
#define __CNETFINAL_SDNCONTROLLER_H

#include <omnetpp.h>
#include <map>
#include <vector>
#include <string>
#include "FlowRule.h"
#include "Host.h" // For casting modules to Host*
#include "FirewallRule.h" // For FirewallRule type

using namespace omnetpp;

namespace cnetfinal {

// Forward declarations
class FlowRule;
class LocalController;
class CentralizedController;

// SDN Controller base class
class SDNController : public cSimpleModule
{
  protected:
    // Configuration parameters
    std::string controllerType;
    int flowTableSize;
    double processingDelay;
    
    // Network state information
    std::map<std::string, std::vector<FlowRule*>> flowTables;  // Switch ID -> Flow rules
    std::map<std::string, cModule*> managedSwitches;          // Switch ID -> Module pointer
    
    // Statistics
    cOutVector flowSetupTimeVector;
    cHistogram flowSetupTimeStats;
    cOutVector controllerLoadVector;
    
    // Internal methods
    virtual void processPacketIn(cMessage *msg);
    virtual void updateNetworkState();
    
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    
  public:
    SDNController();
    virtual ~SDNController();
    
    // API for switch communication
    virtual void handlePacketIn(cMessage *msg, const std::string& switchId);
    virtual void registerSwitch(const std::string& switchId, cModule* switchModule);
    virtual void installFlowRule(const std::string& switchId, FlowRule* rule);
};

// Local (campus-level) SDN controller implementation
class LocalController : public SDNController
{
  private:
    // Additional state for local controller
    std::string campusName;
    double updateInterval;
    CentralizedController* globalController;
    std::map<std::string, std::string> hostMapping;  // Host MAC -> Switch port
    
    // Additional methods
    void updateLocalTopology();
    void syncWithGlobalController();
  public:
    std::string getSwitchForHost(const std::string& hostAddr);
    
    // Constructor and Destructor declarations
    LocalController();
    virtual ~LocalController();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    
  public:
    // API for global controller
    void applyGlobalPolicy(const std::vector<FlowRule*>& rules);
    const std::string& getCampusName() const { return campusName; }
    void setCampusName(const std::string& name) { campusName = name; }
    void setGlobalController(CentralizedController* gc) { globalController = gc; }
};

// Centralized (global) SDN controller implementation
class CentralizedController : public SDNController
{
  private:
    // Corrected type for localControllers map, removed duplicate
    std::map<std::string, LocalController*> localControllers;
    std::map<std::string, std::vector<std::string>> globalTopology;  // Network topology for the centralized view
    
    // Internal methods specific to CentralizedController
    void discoverNetwork(); // Not an override if SDNController base doesn't have it as virtual
    void optimizeRoutes();
    void configureFirewall(); // For setting up firewall rules
    // The following were in private, ensure they are implemented or remove if not used
    // void handleFailure(const std::string& linkId); 
    // std::vector<std::string> calculateShortestPath(const std::string& src, const std::string& dst);
    // std::string findSwitchForHost(const std::string& hostAddr);
    
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    // finish() is inherited from SDNController if not overridden here
    
  public:
    CentralizedController(); // Constructor
    virtual ~CentralizedController(); // Destructor if needed
    
    // API for local controller communication
    void registerLocalController(const std::string& controllerId, LocalController* controller); // Changed to LocalController*
    
    // Public interface methods (were mixed in private section in file content)
    void updateLocalTopology(const std::string& controllerId, const std::map<std::string, std::vector<std::string>>& localTopo);
    void handleFlowRequest(const std::string& controllerId, const std::string& srcAddr, const std::string& dstAddr, int priority);
    void handleFailure(const std::string& linkId); // Moved to public as an example, adjust if private
    std::vector<std::string> calculateShortestPath(const std::string& src, const std::string& dst); // Moved to public
    std::string findSwitchForHost(const std::string& hostAddr); // Moved to public

    virtual void installFlowRule(const std::string& switchId, FlowRule* rule) override;
};

}  // namespace cnetfinal

#endif // __CNETFINAL_SDNCONTROLLER_H