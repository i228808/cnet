#include "SDNController.h"
#include <omnetpp.h>

namespace cnetfinal {

Define_Module(LocalController);

LocalController::LocalController() {
    globalController = nullptr;
    updateInterval = 0.0; // Default, should be set by parameter or init
}

LocalController::~LocalController() {
    // No dynamic memory owned by LocalController directly in this example for now
}

void LocalController::initialize() {
    SDNController::initialize(); // Call base initialize
    campusName = par("campusName").stdstringValue(); // Assuming NED parameter
    updateInterval = par("updateInterval").doubleValue(); // Assuming NED parameter

    // Try to find the CentralizedController
    // This assumes CentralizedController is a sibling or at a known path
    // A more robust way is for CentralizedController to register LocalControllers,
    // or for LocalControllers to find and register with CentralizedController.
    // For now, let's assume CentralizedController finds and sets this via setGlobalController.
    EV_INFO << "LocalController " << campusName << " initialized." << endl;

    // Example: schedule first topology update
    if (updateInterval > 0) {
        scheduleAt(simTime() + updateInterval, new cMessage("updateTopologyTimer"));
    }
    syncWithGlobalController(); // Initial sync attempt
}

void LocalController::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        if (strcmp(msg->getName(), "updateTopologyTimer") == 0) {
            EV_INFO << "LocalController " << campusName << ": Updating topology and syncing with global." << endl;
            updateLocalTopology();
            syncWithGlobalController();
            scheduleAt(simTime() + updateInterval, msg); // Reschedule
            return; // Important: msg is rescheduled, not deleted here if it's the timer
        }
    }
    
    // If not a self-message handled above, pass to base SDNController for packet_in etc.
    SDNController::handleMessage(msg); 
}

void LocalController::updateLocalTopology() {
    EV_INFO << "LocalController " << campusName << ": updateLocalTopology() called." << endl;
    // Placeholder: Discover local topology (switches, links, hosts in this campus)
    // Populate its own view and potentially the hostMapping.
    // For example, iterate over getParentModule()->getSubmodule("switchX") etc.
}

void LocalController::syncWithGlobalController() {
    EV_INFO << "LocalController " << campusName << ": syncWithGlobalController() called." << endl;
    if (globalController) {
        // Placeholder: Send its local topology to the globalController
        // std::map<std::string, std::vector<std::string>> currentLocalTopo; 
        // Populate currentLocalTopo from its own discovery
        // globalController->updateLocalTopology(this->campusName, currentLocalTopo);
        EV_INFO << "LocalController " << campusName << ": Found global controller, would sync." << endl;
    } else {
        EV_WARN << "LocalController " << campusName << ": Global controller not set. Cannot sync." << endl;
    }
}

std::string LocalController::getSwitchForHost(const std::string& hostAddr) {
    auto it = hostMapping.find(hostAddr);
    if (it != hostMapping.end()) {
        return it->second;
    }
    EV_WARN << "LocalController " << campusName << ": Switch not found for host " << hostAddr << endl;
    return ""; // Return empty if not found
}

void LocalController::applyGlobalPolicy(const std::vector<FlowRule*>& rules) {
    EV_INFO << "LocalController " << campusName << ": Applying " << rules.size() << " global flow rules." << endl;
    for (FlowRule* rule : rules) {
        // Assuming rule->switchId is correctly set to a switch in this campus
        // Or, the CentralizedController has already filtered/delegated appropriately.
        installFlowRule(rule->getSwitchId(), new FlowRule(*rule)); // Install a copy
    }
    // Note: The original rules vector might need to be deleted by the caller (CentralizedController)
    // if it dynamically allocated them and LocalController makes copies.
}

// Ensure SDNController.cc defines the base class methods if not pure virtual
// For example, SDNController::initialize(), SDNController::handleMessage() etc.
// SDNController::installFlowRule() is used by applyGlobalPolicy.

} // namespace cnetfinal 