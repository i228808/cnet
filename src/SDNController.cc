#include "SDNController.h"
#include "FlowRule.h"
#include "Host.h"
#include "OpenFlowSwitch.h"
#include <algorithm>

namespace cnetfinal {

// Base SDN Controller implementation
Define_Module(SDNController);

SDNController::SDNController()
{
}

SDNController::~SDNController()
{
    // Clean up flow rules
    for (auto& entry : flowTables) {
        for (auto& rule : entry.second) {
            delete rule;
        }
    }
}

void SDNController::initialize()
{
    // Get configuration parameters
    controllerType = par("controllerType").stdstringValue();
    flowTableSize = par("flowTableSize");
    processingDelay = par("processingDelay");
    
    // Initialize statistics
    flowSetupTimeVector.setName("flowSetupTime");
    flowSetupTimeStats.setName("flowSetupTimeDistribution");
    controllerLoadVector.setName("controllerLoad");
    
    // Schedule initial topology discovery
    scheduleAt(simTime() + 5.0, new cMessage("discoverTopology"));
    
    EV_INFO << getFullPath() << " initialized with type: " << controllerType << ", flow table size: " << flowTableSize << endl;
}

void SDNController::handleMessage(cMessage *msg)
{
    if (strcmp(msg->getName(), "discoverTopology") == 0) {
        updateNetworkState();
        scheduleAt(simTime() + 60.0, msg);  // Schedule next topology update
    }
    else if (msg->getKind() == 1) {  // PacketIn message
        processPacketIn(msg);
        delete msg;
    }
    else {
        delete msg;
    }
}

void SDNController::updateNetworkState()
{
    EV_INFO << getFullPath() << " updating network state..." << endl;
    // Base implementation - will be overridden by subclasses
}

void SDNController::processPacketIn(cMessage *msg)
{
    // Base implementation - will be overridden by subclasses
    EV_INFO << getFullPath() << ": Processing packet in: " << msg->getName() << endl;
}

void SDNController::installFlowRule(const std::string& switchId, FlowRule* rule)
{
    EV_INFO << getFullPath() << ": Installing flow rule on switch " << switchId << ": " << rule->toString() << endl;
    
    if (managedSwitches.find(switchId) == managedSwitches.end()) {
        EV_ERROR << getFullPath() << ": Error: Cannot install flow rule, switch " << switchId << " not found" << endl;
        delete rule;
        return;
    }
    
    // Add rule to flow table
    flowTables[switchId].push_back(rule);
    
    // Send FlowMod message to switch
    cMessage *flowMod = new cMessage("flowMod");
    flowMod->setKind(2); // FlowMod message
    
    // Send to the target switch
    // In a real implementation, we would attach the rule to the message
    sendDirect(flowMod, managedSwitches[switchId], "controllerIn");
}

void SDNController::handlePacketIn(cMessage *msg, const std::string& switchId)
{
    // Base implementation - just process the packet
    processPacketIn(msg);
}

void SDNController::registerSwitch(const std::string& switchId, cModule* switchModule)
{
    EV_INFO << getFullPath() << ": Registering switch " << switchId << " (" << switchModule->getFullPath() << ")" << endl;
    managedSwitches[switchId] = switchModule;
}

void SDNController::finish()
{
    // Record final statistics
    recordScalar("avgFlowSetupTime", flowSetupTimeStats.getMean());
    recordScalar("maxFlowSetupTime", flowSetupTimeStats.getMax());
    recordScalar("totalFlowRequests", flowSetupTimeStats.getCount());
}

} // namespace cnetfinal