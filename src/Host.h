#ifndef __CNETFINAL_HOST_H
#define __CNETFINAL_HOST_H

#include <omnetpp.h>
#include <vector>
#include "FirewallRule.h"

using namespace omnetpp;

namespace cnetfinal {

/**
 * Simple host implementation
 */
class Host : public cSimpleModule
{
  private:
    std::string macAddress;     // For L2 communication
    std::string networkAddress; // For L3 communication (IP)
    bool isFirewall;
    std::vector<FirewallRule> firewallRules;
    cMessage *sendTimer; // Timer for sending periodic packets

    // Internal method to check rules (implementation in Host.cc)
    FirewallAction checkFirewallRules(cMessage *msg);

  protected:
    virtual int numInitStages() const override { return 2; }
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

  public:
    Host();
    virtual ~Host(); // Declare destructor
    // Method for SDN controller to install firewall rules
    void installFirewallRule(const FirewallRule& rule);
    bool isDesignatedFirewall() const { return isFirewall; }
};

} // namespace cnetfinal

#endif // __CNETFINAL_HOST_H 