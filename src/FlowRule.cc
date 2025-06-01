#include "FlowRule.h"
#include <sstream>

namespace cnetfinal {

std::string FlowRule::toString() const {
    std::ostringstream oss;
    oss << "FlowRule[id=" << ruleId << ", switch=" << switchId << ", priority=" << priority;
    
    if (!srcMac.empty()) oss << ", srcMac=" << srcMac;
    if (!dstMac.empty()) oss << ", dstMac=" << dstMac;
    if (!srcIp.empty()) oss << ", srcIp=" << srcIp;
    if (!dstIp.empty()) oss << ", dstIp=" << dstIp;
    if (srcPort > 0) oss << ", srcPort=" << srcPort;
    if (dstPort > 0) oss << ", dstPort=" << dstPort;
    if (protocol > 0) oss << ", protocol=" << protocol;
    
    oss << ", action=";
    switch (action) {
        case FORWARD: oss << "FORWARD"; break;
        case DROP: oss << "DROP"; break;
        case MODIFY: oss << "MODIFY"; break;
        case MIRROR: oss << "MIRROR"; break;
        default: oss << "UNKNOWN"; break;
    }
    
    if (action == FORWARD || action == MIRROR)
        oss << ", outPort=" << outputPort;
        
    if (timeout > 0)
        oss << ", timeout=" << timeout;
        
    oss << "]";
    return oss.str();
}

} // namespace cnetfinal 