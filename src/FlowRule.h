#ifndef __CNETFINAL_FLOWRULE_H
#define __CNETFINAL_FLOWRULE_H

#include <string>
#include <vector>
#include <omnetpp.h>

namespace cnetfinal {

// Flow action types
enum FlowAction {
    FORWARD,
    DROP,
    MODIFY,
    MIRROR
};

// Flow rule class
class FlowRule {
private:
    int ruleId;
    std::string switchId;
    int priority;
    std::string srcMac;
    std::string dstMac;
    std::string srcIp;
    std::string dstIp;
    int srcPort;
    int dstPort;
    int protocol;
    FlowAction action;
    int outputPort;
    double timeout;
    omnetpp::simtime_t installTime;
    
public:
    FlowRule() : 
        ruleId(0), priority(0), srcPort(0), dstPort(0), 
        protocol(0), action(FORWARD), outputPort(0), timeout(0) {}
    
    FlowRule(int id, const std::string& swId, int prio) :
        ruleId(id), switchId(swId), priority(prio), srcPort(0), dstPort(0),
        protocol(0), action(FORWARD), outputPort(0), timeout(0) {}
        
    ~FlowRule() {}
    
    // Getters
    int getRuleId() const { return ruleId; }
    const std::string& getSwitchId() const { return switchId; }
    int getPriority() const { return priority; }
    const std::string& getSrcMac() const { return srcMac; }
    const std::string& getDstMac() const { return dstMac; }
    const std::string& getSrcIp() const { return srcIp; }
    const std::string& getDstIp() const { return dstIp; }
    int getSrcPort() const { return srcPort; }
    int getDstPort() const { return dstPort; }
    int getProtocol() const { return protocol; }
    FlowAction getAction() const { return action; }
    int getOutputPort() const { return outputPort; }
    double getTimeout() const { return timeout; }
    omnetpp::simtime_t getInstallTime() const { return installTime; }
    
    // Setters
    void setRuleId(int id) { ruleId = id; }
    void setSwitchId(const std::string& id) { switchId = id; }
    void setPriority(int prio) { priority = prio; }
    void setSrcMac(const std::string& mac) { srcMac = mac; }
    void setDstMac(const std::string& mac) { dstMac = mac; }
    void setSrcIp(const std::string& ip) { srcIp = ip; }
    void setDstIp(const std::string& ip) { dstIp = ip; }
    void setSrcPort(int port) { srcPort = port; }
    void setDstPort(int port) { dstPort = port; }
    void setProtocol(int proto) { protocol = proto; }
    void setAction(FlowAction act) { action = act; }
    void setOutputPort(int port) { outputPort = port; }
    void setTimeout(double time) { timeout = time; }
    void setInstallTime(omnetpp::simtime_t time) { installTime = time; }
    
    // Match functions
    bool matches(const std::string& srcMac, const std::string& dstMac) const {
        return (this->srcMac.empty() || this->srcMac == srcMac) && 
               (this->dstMac.empty() || this->dstMac == dstMac);
    }
    
    bool matchesIP(const std::string& srcIp, const std::string& dstIp) const {
        return (this->srcIp.empty() || this->srcIp == srcIp) && 
               (this->dstIp.empty() || this->dstIp == dstIp);
    }
    
    bool matchesPort(int srcPort, int dstPort) const {
        return (this->srcPort == 0 || this->srcPort == srcPort) && 
               (this->dstPort == 0 || this->dstPort == dstPort);
    }
    
    bool matchesProtocol(int proto) const {
        return (this->protocol == 0 || this->protocol == proto);
    }
    
    // Check if the rule is expired
    bool isExpired(omnetpp::simtime_t currentTime) const {
        return timeout > 0 && (currentTime - installTime).dbl() > timeout;
    }
    
    // Utility functions
    std::string toString() const;
};

} // namespace cnetfinal

#endif // __CNETFINAL_FLOWRULE_H 