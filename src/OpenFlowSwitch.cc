#include "OpenFlowSwitch.h"
#include "FlowRule.h"
#include "SDNController.h"

namespace cnetfinal {

Define_Module(OpenFlowSwitch);

OpenFlowSwitch::OpenFlowSwitch()
{
    tableMisses = 0;
    tableHits = 0;
    controller = nullptr;
}

OpenFlowSwitch::~OpenFlowSwitch()
{
    // Clean up flow rules
    for (auto& rule : flowTable) {
        delete rule;
    }
    flowTable.clear();
}

void OpenFlowSwitch::initialize()
{
    // Get configuration
    switchId = par("switchId").stdstringValue();
    if (switchId.empty()) {
        switchId = getFullPath();
    }
    flowTableCapacity = par("flowTableCapacity");
    isCore = par("isCore");
    // VLAN support
    vlanId = par("vlanId");
    enableVlan = par("enableVlan");
    EV_INFO << getFullPath() << ": VLAN support: " << (enableVlan ? "ENABLED" : "DISABLED") << ", vlanId=" << vlanId << endl;
    
    // Find controller connection
    cGate* controllerGateOut = gate("controllerOut");
    if (controllerGateOut && controllerGateOut->isConnected()) {
        cGate* peerGate = controllerGateOut->getPathEndGate();
        if (peerGate) {
            controller = peerGate->getOwnerModule();
            
            // Register with controller
            if (controller) {
                SDNController* sdnController = dynamic_cast<SDNController*>(controller);
                if (sdnController) {
                    sdnController->registerSwitch(switchId, this);
                } else {
                    // Check if the connected module is another OpenFlowSwitch (acting as an aggregator)
                    OpenFlowSwitch* intermediarySwitch = dynamic_cast<OpenFlowSwitch*>(controller);
                    if (intermediarySwitch) {
                        EV_INFO << "Switch " << switchId << " is connected to intermediary switch " 
                                << controller->getFullPath() << " on its control plane." << endl;
                    } else {
                        EV_WARN << "Controller module for switch " << switchId 
                                << " (" << controller->getFullPath() << ") is not an SDNController nor an OpenFlowSwitch aggregator." << endl;
                    }
                }
            }
        }
    } else {
        // Fallback or alternative: try to get controller via controllerIn if controllerOut is not connected or suitable
        cGate* controllerGateIn = gate("controllerIn");
        if (controllerGateIn && controllerGateIn->isConnected()) {
            cGate* peerGate = controllerGateIn->getPathEndGate();
            if (peerGate) {
                controller = peerGate->getOwnerModule();
                
                // Register with controller
                if (controller) {
                    SDNController* sdnController = dynamic_cast<SDNController*>(controller);
                    if (sdnController) {
                        sdnController->registerSwitch(switchId, this);
                    } else {
                        // Check if the connected module is another OpenFlowSwitch (acting as an aggregator)
                        OpenFlowSwitch* intermediarySwitch = dynamic_cast<OpenFlowSwitch*>(controller);
                        if (intermediarySwitch) {
                            EV_INFO << "Switch " << switchId << " (via In) is connected to intermediary switch " 
                                    << controller->getFullPath() << " on its control plane." << endl;
                        } else {
                            EV_WARN << "Controller module (via In) for switch " << switchId 
                                    << " (" << controller->getFullPath() << ") is not an SDNController nor an OpenFlowSwitch aggregator." << endl;
                        }
                    }
                }
            }
        } else {
            EV_WARN << "Switch " << switchId << " has no controller connected to controllerOut or controllerIn." << endl;
        }
    }
    
    // Initialize statistics
    packetProcessingTimeVector.setName("packetProcessingTime");
    packetProcessingTimeStats.setName("packetProcessingTimeDistribution");
    tableHitRatioVector.setName("tableHitRatio");
    
    // Schedule periodic cleanup of expired rules
    scheduleAt(simTime() + 10.0, new cMessage("cleanupRules"));
    
    EV << "OpenFlow Switch " << switchId << " initialized with flow table capacity " 
       << flowTableCapacity << endl;
}

void OpenFlowSwitch::handleMessage(cMessage *msg)
{
    if (strcmp(msg->getName(), "cleanupRules") == 0) {
        removeExpiredRules();
        scheduleAt(simTime() + 10.0, msg);  // Reschedule cleanup
        return;
    }
    else if (msg->getKind() == 2) {  // FlowMod message from controller
        // Handle flow modification
        // In a real implementation, we would extract the flow rule from the message
        // For now, just log it
        EV << "Received flow modification from controller" << endl;
        delete msg;
        return;
    }
    
    // Start processing timer
    simtime_t startTime = simTime();
    
    // Process packet
    bool hit = processPacket(msg);
    
    // Update statistics
    simtime_t processingTime = simTime() - startTime;
    packetProcessingTimeVector.record(processingTime);
    packetProcessingTimeStats.collect(processingTime);
    
    if (hit) {
        tableHits++;
    } else {
        tableMisses++;
    }
    
    // Update hit ratio statistics
    double hitRatio = (double)tableHits / (tableHits + tableMisses);
    tableHitRatioVector.record(hitRatio);
    
    // Packets are deleted in processPacket() or sent to other modules
}

bool OpenFlowSwitch::processPacket(cMessage *msg)
{
    // Extract packet information
    // In a real implementation, we would extract headers from the packet
    // For this simulation, we'll just generate some example values
    std::string srcMac = "00:00:00:00:00:01"; // Example - would extract from packet
    std::string dstMac = "00:00:00:00:00:02"; // Example - would extract from packet
    int inPort = msg->getArrivalGate()->getIndex();
    
    // Learn source MAC address
    learnMacAddress(srcMac, inPort);
    
    // Find matching flow rule
    int ruleIndex = findMatchingRule(srcMac, dstMac);
    
    if (ruleIndex >= 0) {
        // Found matching rule - process according to actions
        FlowRule* rule = flowTable[ruleIndex];
        
        switch (rule->getAction()) {
            case FORWARD:
                // Forward packet to output port
                if (rule->getOutputPort() < gateSize("dataPort")) {
                    send(msg, "dataPort", rule->getOutputPort());
                } else {
                    EV << "Error: Output port " << rule->getOutputPort() 
                       << " is out of range" << endl;
                    delete msg;
                }
                break;
                
            case DROP:
                // Drop packet
                EV << "Dropping packet based on flow rule" << endl;
                delete msg;
                break;
                
            case MODIFY:
                // Modify packet headers and forward
                // In a real implementation, we would modify the packet headers
                EV << "Modifying packet headers and forwarding" << endl;
                if (rule->getOutputPort() < gateSize("dataPort")) {
                    send(msg, "dataPort", rule->getOutputPort());
                } else {
                    delete msg;
                }
                break;
                
            case MIRROR:
                // Mirror packet to another port and forward
                // In a real implementation, we would create a copy of the packet
                EV << "Mirroring packet to monitoring port" << endl;
                if (rule->getOutputPort() < gateSize("dataPort")) {
                    send(msg, "dataPort", rule->getOutputPort());
                } else {
                    delete msg;
                }
                break;
                
            default:
                EV << "Unknown action in flow rule" << endl;
                delete msg;
                break;
        }
        
        return true;
    } else {
        // No matching rule - try MAC learning table
        if (macToPortMap.find(dstMac) != macToPortMap.end()) {
            // Known destination - forward directly
            int outPort = macToPortMap[dstMac];
            EV << "Forwarding packet to port " << outPort << " based on MAC learning table" << endl;
            send(msg, "dataPort", outPort);
            
            // Create a flow rule for future packets
            FlowRule* rule = new FlowRule();
            rule->setRuleId(flowTable.size() + 1);
            rule->setSwitchId(switchId);
            rule->setPriority(1);
            rule->setSrcMac(srcMac);
            rule->setDstMac(dstMac);
            rule->setAction(FORWARD);
            rule->setOutputPort(outPort);
            rule->setTimeout(30.0);  // Expire after 30 seconds
            rule->setInstallTime(simTime());
            
            installFlowRule(rule);
            
            return true;
        } else {
            // Unknown destination - send to controller
            sendToController(msg, srcMac, dstMac, inPort);
            return false;
        }
    }
}

void OpenFlowSwitch::learnMacAddress(const std::string& mac, int port)
{
    // Update MAC learning tables
    portToMacMap[port] = mac;
    macToPortMap[mac] = port;
    
    EV << "Learned MAC " << mac << " on port " << port << endl;
}

void OpenFlowSwitch::sendToController(cMessage *msg, const std::string& srcMac, 
                                    const std::string& dstMac, int inPort)
{
    if (!controller) {
        EV << "No controller connected, dropping packet" << endl;
        delete msg;
        return;
    }
    
    // Create packet-in message
    cMessage *packetIn = new cMessage("packetIn");
    packetIn->setKind(1);  // PacketIn message
    
    // Add packet information
    // In a real implementation, we would attach the packet data
    
    EV << "Sending PacketIn to controller for unknown MAC " << dstMac << endl;
    
    // Send to controller
    send(packetIn, "controllerOut");
    
    // Delete the original packet - controller will tell us what to do
    delete msg;
}

int OpenFlowSwitch::findMatchingRule(const std::string& srcMac, const std::string& dstMac)
{
    // Search for matching rule with the highest priority
    int highestPriority = -1;
    int matchIndex = -1;
    
    for (size_t i = 0; i < flowTable.size(); i++) {
        if (flowTable[i]->matches(srcMac, dstMac)) {
            if (flowTable[i]->getPriority() > highestPriority) {
                highestPriority = flowTable[i]->getPriority();
                matchIndex = i;
            }
        }
    }
    
    return matchIndex;
}

void OpenFlowSwitch::removeExpiredRules()
{
    simtime_t currentTime = simTime();
    
    // Find and remove expired rules
    for (auto it = flowTable.begin(); it != flowTable.end(); ) {
        if ((*it)->isExpired(currentTime)) {
            EV << "Removing expired flow rule: " << (*it)->toString() << endl;
            delete *it;
            it = flowTable.erase(it);
        } else {
            ++it;
        }
    }
}

void OpenFlowSwitch::installFlowRule(FlowRule* rule)
{
    // Check if we're at capacity
    if (flowTable.size() >= (size_t)flowTableCapacity) {
        // Remove the oldest rule
        if (!flowTable.empty()) {
            EV << "Flow table full, removing oldest rule" << endl;
            delete flowTable.front();
            flowTable.erase(flowTable.begin());
        }
    }
    
    // Set the installation time
    rule->setInstallTime(simTime());
    
    // Add to flow table
    flowTable.push_back(rule);
    
    EV << "Installed flow rule: " << rule->toString() << endl;
}

void OpenFlowSwitch::modifyFlowRule(int ruleId, FlowRule* newRule)
{
    // Find the rule with the given ID
    for (size_t i = 0; i < flowTable.size(); i++) {
        if (flowTable[i]->getRuleId() == ruleId) {
            // Replace the rule
            EV << "Modifying flow rule " << ruleId << endl;
            delete flowTable[i];
            flowTable[i] = newRule;
            return;
        }
    }
    
    EV << "Error: Cannot modify flow rule " << ruleId << ", not found" << endl;
    delete newRule;  // Clean up if rule not found
}

void OpenFlowSwitch::removeFlowRule(int ruleId)
{
    // Find the rule with the given ID
    for (auto it = flowTable.begin(); it != flowTable.end(); ++it) {
        if ((*it)->getRuleId() == ruleId) {
            // Remove the rule
            EV << "Removing flow rule " << ruleId << endl;
            delete *it;
            flowTable.erase(it);
            return;
        }
    }
    
    EV << "Error: Cannot remove flow rule " << ruleId << ", not found" << endl;
}

void OpenFlowSwitch::clearFlowTable()
{
    EV << "Clearing flow table" << endl;
    
    // Delete all rules
    for (auto& rule : flowTable) {
        delete rule;
    }
    
    // Clear the table
    flowTable.clear();
}

void OpenFlowSwitch::finish()
{
    // Record final statistics
    recordScalar("tableMisses", tableMisses);
    recordScalar("tableHits", tableHits);
    recordScalar("avgProcessingTime", packetProcessingTimeStats.getMean());
    recordScalar("flowTableSize", flowTable.size());
    
    if (tableMisses + tableHits > 0) {
        recordScalar("finalHitRatio", (double)tableHits / (tableHits + tableMisses));
    } else {
        recordScalar("finalHitRatio", 0.0);
    }
}

} // namespace cnetfinal 