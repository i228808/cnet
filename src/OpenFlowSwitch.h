#ifndef __CNETFINAL_OPENFLOWSWITCH_H
#define __CNETFINAL_OPENFLOWSWITCH_H

#include <omnetpp.h>
#include <map>
#include <vector>
#include <string>
#include "FlowRule.h"

using namespace omnetpp;

namespace cnetfinal {

class OpenFlowSwitch : public cSimpleModule
{
  private:
    // Configuration
    std::string switchId;
    cModule* controller;
    bool isCore;
    
    // Flow table
    std::vector<FlowRule*> flowTable;
    int flowTableCapacity;
    
    // Port mapping
    std::map<int, std::string> portToMacMap;
    std::map<std::string, int> macToPortMap;
    
    // Statistics
    cOutVector packetProcessingTimeVector;
    cHistogram packetProcessingTimeStats;
    cOutVector tableHitRatioVector;
    int tableMisses;
    int tableHits;
    
    // VLAN support
    int vlanId;
    bool enableVlan;
    
    // Internal methods
    bool processPacket(cMessage *msg);
    void sendToController(cMessage *msg, const std::string& srcMac, 
                         const std::string& dstMac, int inPort);
    int findMatchingRule(const std::string& srcMac, const std::string& dstMac);
    void learnMacAddress(const std::string& mac, int port);
    void removeExpiredRules();
    
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    
  public:
    OpenFlowSwitch();
    virtual ~OpenFlowSwitch();
    
    // Methods called by controllers
    void installFlowRule(FlowRule* rule);
    void modifyFlowRule(int ruleId, FlowRule* newRule);
    void removeFlowRule(int ruleId);
    void clearFlowTable();
};

} // namespace cnetfinal

#endif // __CNETFINAL_OPENFLOWSWITCH_H 