#include "SDNController.h"
#include <omnetpp.h>
#include "Host.h"         // For Host type and dynamic_cast
#include "FirewallRule.h" // For FirewallRule type

namespace cnetfinal {

Define_Module(CentralizedController);

// Constructor
CentralizedController::CentralizedController() {
    // Initialization if any, e.g., setting default values for members.
}

// Destructor
CentralizedController::~CentralizedController() {
    // Clean up dynamically allocated resources, e.g., FlowRule objects if not handled elsewhere
    // or LocalController pointers if CentralizedController owns them.
    for (auto& pair : localControllers) {
        // If CentralizedController is responsible for deleting LocalController objects:
        // delete pair.second; // This depends on ownership model!
    }
    localControllers.clear();
}

void CentralizedController::initialize()
{
    SDNController::initialize(); // Call base class initialize
    EV_INFO << "CentralizedController initialized: " << getFullPath() << endl;
    discoverNetwork();
    optimizeRoutes(); // This might be too early if network isn't fully up.

    // Attempt to find and configure the firewall
    configureFirewall();
}

void CentralizedController::configureFirewall()
{
    // Assuming the firewall is named "firewallServer" and is in "headOffice"
    // Path: NUCESNetwork.headOffice.firewallServer
    cModule *mod = getSystemModule()->getSubmodule("headOffice");
    if (!mod) {
        EV_ERROR << "CentralizedController: Could not find 'headOffice' module." << endl;
        return;
    }
    cModule *fwMod = mod->getSubmodule("firewallServer");
    if (!fwMod) {
        EV_ERROR << "CentralizedController: Could not find 'firewallServer' in 'headOffice'." << endl;
        return;
    }

    Host *firewallHost = dynamic_cast<Host*>(fwMod);
    // Always configure firewallServer instance regardless of Host::isFirewall flag
    if (firewallHost) {
        EV_INFO << "CentralizedController: Found firewall at " << firewallHost->getFullPath() << endl;

        // Example Rule 1: Deny all TCP traffic from 10.0.1.x to 192.168.1.10 on port 80
        FirewallRule rule1(1, "10.0.1.0/24", "192.168.1.10/32", -1, 80, "TCP", DENY, 100);
        firewallHost->installFirewallRule(rule1);

        // Example Rule 2: Allow all ICMP traffic (ping)
        FirewallRule rule2(2, "ANY", "ANY", -1, -1, "ICMP", ALLOW, 50);
        firewallHost->installFirewallRule(rule2);
        
        // Example Rule 3: Allow specific host 10.0.1.5 to access 192.168.1.10 on port 80 (overrides rule1 due to priority)
        FirewallRule rule3(3, "10.0.1.5/32", "192.168.1.10/32", -1, 80, "TCP", ALLOW, 110);
        firewallHost->installFirewallRule(rule3);

        // Example Rule 4: Default Allow for DNS traffic to dnsServer (assuming its IP is 10.1.0.2)
        // Needs correct IP for dnsServer
        // FirewallRule rule4(4, "ANY", "10.1.0.2/32", -1, 53, "UDP", ALLOW, 40);
        // firewallHost->installFirewallRule(rule4);

    } else {
        EV_ERROR << "CentralizedController: Module " << fwMod->getFullPath() << " is not a Host type. Cannot configure as firewall." << endl;
    }
}

// ... (rest of CentralizedController.cc, including overridden installFlowRule)

void CentralizedController::handleMessage(cMessage *msg)
{
    SDNController::handleMessage(msg); // Call base class handler
}

// Override installFlowRule as previously discussed
void CentralizedController::installFlowRule(const std::string& switchId, FlowRule *rule)
{
    EV_INFO << "CentralizedController received installFlowRule for " << switchId << ": " << rule->toString() << endl;

    // Try to find the campus based on switchId (e.g., "islamabadCampus_coreSwitch1")
    size_t underscorePos = switchId.find('_');
    if (underscorePos != std::string::npos) {
        std::string campusName = switchId.substr(0, underscorePos);
        auto it = localControllers.find(campusName);
        if (it != localControllers.end()) {
            LocalController* lc = it->second;
            EV_INFO << "Delegating installFlowRule for " << switchId << " to LocalController " << lc->getFullPath() << endl;
            // We made installFlowRule public in SDNController, so this should be fine.
            lc->installFlowRule(switchId, rule); // LC will own the rule if it installs it
            return;
        }
        else {
            EV_WARN << "No LocalController found for campus '" << campusName << "' to handle switch " << switchId << endl;
        }
    }
    else {
        EV_WARN << "Could not determine campus from switchId '" << switchId << "'. Applying to self (if managed)." << endl;
    }

    // Fallback to base class behavior (manage by CentralizedController itself or log error)
    SDNController::installFlowRule(switchId, rule); // Base class will handle or delete rule
}

void CentralizedController::discoverNetwork() {
    EV_INFO << "CentralizedController: discoverNetwork() called." << endl;
    // Placeholder: Logic to discover the global network topology.
    // This might involve iterating through modules, or receiving info from LocalControllers.
    // For now, it might populate globalTopology based on NED structure if simple enough
    // or wait for local controllers to register and send their topologies.

    // After discovering, it might be appropriate to optimize routes
    optimizeRoutes();
}

void CentralizedController::optimizeRoutes() {
    EV_INFO << "CentralizedController: optimizeRoutes() called." << endl;
    // Placeholder: Logic to calculate and optimize routes globally.
    // This would then translate to FlowRules pushed to switches via LocalControllers or directly.
    // Example: Iterate all known hosts, calculate paths between them, install rules.
}

void CentralizedController::handleFailure(const std::string& linkId) {
    EV_INFO << "CentralizedController: handleFailure() for link " << linkId << endl;
    // Placeholder: Logic to handle a link failure, re-calculate routes, etc.
}

std::vector<std::string> CentralizedController::calculateShortestPath(const std::string& src, const std::string& dst) {
    EV_INFO << "CentralizedController: calculateShortestPath() from " << src << " to " << dst << endl;
    // Placeholder: Implement Dijkstra's or other pathfinding algorithm on globalTopology.
    return {};
}

std::string CentralizedController::findSwitchForHost(const std::string& hostAddr) {
    EV_INFO << "CentralizedController: findSwitchForHost() for host " << hostAddr << endl;
    // Placeholder: Logic to find which switch a host is connected to based on global topology or local controller info.
    return "";
}

// Updated signature to match declaration
void CentralizedController::registerLocalController(const std::string& controllerId, LocalController* controller) {
    EV_INFO << "CentralizedController: registerLocalController() for ID " << controllerId << " at " << controller->getFullPath() << endl;
    localControllers[controllerId] = controller;
    // Optionally, request its topology immediately
    // controller->sendTopologyRequest(); // If such a mechanism exists
}

void CentralizedController::updateLocalTopology(const std::string& controllerId, const std::map<std::string, std::vector<std::string>>& localTopo) {
    EV_INFO << "CentralizedController: updateLocalTopology() from " << controllerId << endl;
    // Placeholder: Merge localTopo into globalTopology and potentially re-optimize routes.
    for(const auto& pair : localTopo) {
        globalTopology[pair.first] = pair.second; // Simple merge/overwrite
    }
    optimizeRoutes(); // Re-optimize after topology update
}

void CentralizedController::handleFlowRequest(const std::string& controllerId, const std::string& srcAddr, const std::string& dstAddr, int priority) {
    EV_INFO << "CentralizedController: handleFlowRequest() from " << controllerId 
            << " for " << srcAddr << " -> " << dstAddr << " prio " << priority << endl;
    // Placeholder: Centralized decision making for a flow.
    // May involve calculateShortestPath and then installFlowRule via the appropriate LocalController or directly.
}

} // namespace cnetfinal
