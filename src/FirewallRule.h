#ifndef __CNETFINAL_FIREWALLRULE_H
#define __CNETFINAL_FIREWALLRULE_H

#include <string>
#include <omnetpp.h> // For EV

namespace cnetfinal {

enum FirewallAction {
    ALLOW,
    DENY
};

// Basic Firewall Rule structure
// For simplicity, we'll match on string representations of IP addresses and ports.
// In a more detailed simulation, these would be proper IPAddress and Port objects.
class FirewallRule {
public:
    int ruleId;
    std::string srcIpPrefix; // e.g., "192.168.1.0/24" or "ANY"
    std::string dstIpPrefix; // e.g., "10.0.0.5/32" or "ANY"
    int srcPort;             // Port number, or -1 for ANY
    int dstPort;             // Port number, or -1 for ANY
    std::string protocol;    // e.g., "TCP", "UDP", "ICMP", or "ANY"
    FirewallAction action;
    int priority;            // Higher number means higher priority

    FirewallRule();
    FirewallRule(int id, const std::string& sip, const std::string& dip, 
                 int sport, int dport, const std::string& proto, 
                 FirewallAction act, int prio);

    bool matches(const std::string& pktSrcIp, const std::string& pktDstIp, 
                 int pktSrcPort, int pktDstPort, const std::string& pktProto) const;

    std::string toString() const;
};

} // namespace cnetfinal

#endif // __CNETFINAL_FIREWALLRULE_H 