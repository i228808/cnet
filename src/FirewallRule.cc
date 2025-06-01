#include "FirewallRule.h"
#include <vector>
#include <sstream> // For std::stringstream
#include <iostream> // For std::stoul, std::cerr (debugging)
#include <stdexcept> // For std::invalid_argument, std::out_of_range

namespace cnetfinal {

// Helper to convert IP string to unsigned int (simplified, assumes IPv4)
// Does not handle validation extensively.
unsigned int ipToUint(const std::string& ipStr) {
    unsigned int ipUint = 0;
    std::stringstream ss(ipStr);
    std::string segment;
    int shift = 24;
    while (std::getline(ss, segment, '.')) {
        try {
            unsigned long val = std::stoul(segment);
            if (val > 255) return 0; // Invalid segment
            ipUint |= (val << shift);
            shift -= 8;
        } catch (const std::invalid_argument& ia) {
            // std::cerr << "Invalid argument for stoul: " << segment << std::endl;
            return 0; // Invalid segment
        } catch (const std::out_of_range& oor) {
            // std::cerr << "Out of range for stoul: " << segment << std::endl;
            return 0; // Invalid segment
        }
    }
    if (shift != -8) return 0; // Not enough segments
    return ipUint;
}

// Helper to check if an IP address matches a CIDR prefix.
// e.g., ip="192.168.1.10", prefix="192.168.1.0/24"
bool ipMatchesPrefix(const std::string& ip, const std::string& prefix) {
    if (prefix == "ANY") return true;
    if (ip == "ANY") return false; // Packet IP shouldn't be ANY unless specific signaling

    size_t slashPos = prefix.find('/');
    std::string prefixIpStr;
    int maskBits;

    if (slashPos == std::string::npos) { // No CIDR mask, direct IP match
        prefixIpStr = prefix;
        maskBits = 32; 
    } else {
        prefixIpStr = prefix.substr(0, slashPos);
        try {
            maskBits = std::stoi(prefix.substr(slashPos + 1));
        } catch (const std::invalid_argument& ia) {
            // std::cerr << "Invalid mask bits in prefix: " << prefix << std::endl;
            return false;
        } catch (const std::out_of_range& oor) {
            // std::cerr << "Mask bits out of range in prefix: " << prefix << std::endl;
            return false;
        }
        if (maskBits < 0 || maskBits > 32) return false; // Invalid mask
    }

    unsigned int ipAddrUint = ipToUint(ip);
    unsigned int prefixIpUint = ipToUint(prefixIpStr);

    if (ipAddrUint == 0 || prefixIpUint == 0) { // Invalid IP string conversion
        // Fallback to string comparison for non-CIDR or if conversion failed
        if (slashPos == std::string::npos) return ip == prefix;
        return false; // Cannot reliably compare CIDR if IP conversion failed
    }
    
    unsigned int netMask = (maskBits == 0) ? 0 : (0xFFFFFFFF << (32 - maskBits));
    
    return (ipAddrUint & netMask) == (prefixIpUint & netMask);
}

FirewallRule::FirewallRule() : ruleId(0), srcIpPrefix("ANY"), dstIpPrefix("ANY"), srcPort(-1), dstPort(-1), protocol("ANY"), action(ALLOW), priority(0) {}

FirewallRule::FirewallRule(int id, const std::string& sip, const std::string& dip, 
                           int sport, int dport, const std::string& proto, 
                           FirewallAction act, int prio)
    : ruleId(id), srcIpPrefix(sip), dstIpPrefix(dip), srcPort(sport), dstPort(dport), protocol(proto), action(act), priority(prio) {}

bool FirewallRule::matches(const std::string& pktSrcIp, const std::string& pktDstIp, 
                          int pktSrcPort, int pktDstPort, const std::string& pktProto) const {
    // Placeholder: always returns false. Replace with real matching logic as needed.
    return false;
}

std::string FirewallRule::toString() const {
    std::ostringstream oss;
    oss << "RuleID: " << ruleId << ", SrcIP: " << srcIpPrefix << ", DstIP: " << dstIpPrefix
        << ", SrcPort: " << srcPort << ", DstPort: " << dstPort
        << ", Proto: " << protocol << ", Action: " << (action == ALLOW ? "ALLOW" : "DENY")
        << ", Priority: " << priority;
    return oss.str();
}

} // namespace cnetfinal 