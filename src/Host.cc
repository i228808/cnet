#include <omnetpp.h>
#include "Host.h"
#include <algorithm> // For std::sort
#include "CNETPacket_m.h" // Include the generated packet header

using namespace omnetpp;

namespace cnetfinal {

Define_Module(Host);

Host::Host() {
    isFirewall = false; // Default constructor initialization
    sendTimer = nullptr; // Initialize timer pointer
}

// Add destructor to cancel and delete timer
Host::~Host() {
    cancelAndDelete(sendTimer);
}

void Host::installFirewallRule(const FirewallRule& rule) {
    if (isFirewall) {
        // Prevent duplicate rule IDs if necessary, or just add
        firewallRules.push_back(rule);
        // Sort rules by priority (higher number = higher priority)
        std::sort(firewallRules.begin(), firewallRules.end(), 
                  [](const FirewallRule& a, const FirewallRule& b) {
            return a.priority > b.priority;
        });
        EV_INFO << "Installed firewall rule on " << getFullPath() << ": " << rule.toString() << endl;
    } else {
        EV_WARN << "Attempted to install firewall rule on non-firewall host: " << getFullPath() << endl;
    }
}

// Basic placeholder for extracting packet info. Needs actual implementation.
void extractPacketInfo(cMessage *msg, std::string& srcIp, std::string& dstIp, int& srcPort, int& dstPort, std::string& protocol) {
    CNETPacket *packet = dynamic_cast<CNETPacket*>(msg);
    if (packet) {
        srcIp = packet->getSourceIP();
        dstIp = packet->getDestinationIP();
        srcPort = packet->getSourcePort();
        dstPort = packet->getDestinationPort();
        protocol = packet->getProtocol();
        EV_DETAIL << "Extracted packet info: " << srcIp << ":" << srcPort 
                  << " -> " << dstIp << ":" << dstPort << " Proto: " << protocol;
    } else {
        // Not a CNETPacket or msg is null. Use defaults or log error.
        EV_WARN << "extractPacketInfo: Message '" << (msg ? msg->getName() : "null") << "' is not a CNETPacket. Using default/empty values for firewall check.";
        srcIp = "0.0.0.0"; // Default to non-matching or handle error appropriately
        dstIp = "0.0.0.0";
        srcPort = -1;
        dstPort = -1;
        protocol = "UNKNOWN";
    }
}

FirewallAction Host::checkFirewallRules(cMessage *msg) {
    std::string pktSrcIp, pktDstIp, pktProtocol;
    int pktSrcPort, pktDstPort;

extractPacketInfo(msg, pktSrcIp, pktDstIp, pktSrcPort, pktDstPort, pktProtocol);

    for (const auto& rule : firewallRules) {
        if (rule.matches(pktSrcIp, pktDstIp, pktSrcPort, pktDstPort, pktProtocol)) {
            EV_INFO << "Firewall " << getFullPath() << ": Packet matches rule ID " << rule.ruleId 
                    << " (" << rule.toString() << "). Action: " 
                    << (rule.action == ALLOW ? "ALLOW" : "DENY") << endl;
            return rule.action;
        }
    }
    EV_INFO << "Firewall " << getFullPath() << ": Packet did not match any rule. Defaulting to DENY." << endl;
    return DENY; // Default policy: DENY if no rule explicitly allows
}

void Host::initialize(int stage)
{
    EV_INFO << "Host " << getFullPath() << " initialize() called at stage " << stage << endl;
    if (stage == 0) {
        macAddress = par("macAddress").stdstringValue();
        isFirewall = par("isFirewallModule").boolValue();
        sendTimer = nullptr;
        // Do NOT read networkAddress or schedule traffic yet
    } else if (stage == 1) {
        networkAddress = par("networkAddress").stdstringValue();
        if (networkAddress.empty()) {
            EV_WARN << "Host " << getFullPath() << " WARNING: networkAddress is still empty in stage 1!" << endl;
        }
        if (isFirewall) {
            EV_INFO << "Host " << getFullPath() << " initialized as a FIREWALL." << endl;
            // Install default test rules if none present
            if (firewallRules.empty()) {
                FirewallRule r1(1, "10.0.0.1/32", "10.0.0.2/32", -1, -1, "TCP", DENY, 100);
                installFirewallRule(r1);
                FirewallRule r2(2, "ANY", "ANY", -1, -1, "ANY", ALLOW, 0);
                installFirewallRule(r2);
            }
        } else {
            EV_INFO << "Host initialized: " << getFullPath()
                    << " MAC: " << (macAddress.empty() ? "[none]" : macAddress)
                    << " IP: " << (networkAddress.empty() ? "[none]" : networkAddress) << endl;
            // If not a firewall, and has an IP, schedule to send a test packet
            if (!networkAddress.empty() && getName() != std::string("dnsServer") && getName() != std::string("dhcpServer")) {
                sendTimer = new cMessage("sendTestData");
                scheduleAt(simTime() + exponential(1.0), sendTimer);
            }
        }
        // Application server traffic generation
        if (getName() == std::string("videoServer") || getName() == std::string("iotServer") || getName() == std::string("dnsServer")) {
            sendTimer = new cMessage("appTraffic");
            scheduleAt(simTime() + exponential(2.0), sendTimer);
        }
    }
}

void Host::handleMessage(cMessage *msg)
{
    if (isFirewall) {
        EV_INFO << "Firewall " << getFullPath() << " received a packet: " << msg->getName() << " Class: " << msg->getClassName() << endl;
        CNETPacket *cnetPkt = dynamic_cast<CNETPacket*>(msg);
        if (!cnetPkt && !msg->isSelfMessage()) { // If it's not our packet type and not a self-msg, log and delete
             EV_WARN << "Firewall " << getFullPath() << " received non-CNETPacket: " << msg->getName() << ". Deleting." << endl;
             delete msg;
             return;
        }

        FirewallAction action = checkFirewallRules(msg); // msg here could be CNETPacket or self-message
        if (action == DENY) {
            EV_INFO << "Firewall " << getFullPath() << ": DENYING and deleting packet " << msg->getName() << endl;
            delete msg;
        } else { // ALLOW
            EV_INFO << "Firewall " << getFullPath() << ": ALLOWING and forwarding packet " << msg->getName() << endl;
            send(msg, "gate$o"); 
        }
        return; // Firewall processing complete
    }

    // --- Regular Host (Non-Firewall) Logic --- 
    if (msg == sendTimer) { // Timer to send a packet
        CNETPacket *pkt = new CNETPacket();
        pkt->setSourceIP(networkAddress.c_str());
        // Send to a generic test destination or a known server
        // This needs to be configured or made more intelligent
        if (networkAddress == "10.0.1.10") { // Example: host 10.0.1.10 pings 10.0.2.10
            pkt->setDestinationIP("10.0.2.10");
        } else {
            pkt->setDestinationIP("192.168.1.10"); // Default test destination
        }
        pkt->setSourcePort(12345); // Example ephemeral port
        pkt->setDestinationPort(80);   // Example: HTTP port
        pkt->setProtocol("TCP");
        pkt->setByteLength(20); // Minimal packet size

        EV_INFO << getFullPath() << ": Sending test packet: " << pkt->getSourceIP() << ":" << pkt->getSourcePort()
                << " -> " << pkt->getDestinationIP() << ":" << pkt->getDestinationPort() << " Proto: " << pkt->getProtocol() << endl;
        send(pkt, "gate$o");

        // Schedule next send
        scheduleAt(simTime() + exponential(5.0), sendTimer); // Send every 5s (example)
        return;
    }

    // Application server traffic generation
    if (msg == sendTimer && (getName() == std::string("videoServer") || getName() == std::string("iotServer") || getName() == std::string("dnsServer"))) {
        CNETPacket *pkt = new CNETPacket();
        pkt->setSourceIP(networkAddress.c_str());
        pkt->setSourcePort(10000 + intuniform(0,999));
        pkt->setDestinationIP("255.255.255.255"); // Broadcast for now
        pkt->setDestinationPort(20000 + intuniform(0,999));
        if (getName() == std::string("videoServer")) pkt->setProtocol("VIDEO");
        else if (getName() == std::string("iotServer")) pkt->setProtocol("IOT");
        else pkt->setProtocol("DNS_RESPONSE");
        pkt->setByteLength(1000); // Simulate large video/IoT/DNS packet
        EV_INFO << getFullPath() << ": Sending application traffic: " << pkt->getProtocol() << endl;
        send(pkt, "gate$o");
        scheduleAt(simTime() + exponential(10.0), sendTimer); // Repeat every ~10s
        return;
    }

    CNETPacket *receivedPkt = dynamic_cast<CNETPacket*>(msg);
    if (receivedPkt) {
        EV_INFO << getFullPath() << ": Received CNETPacket from " << receivedPkt->getSourceIP() 
                << " to " << receivedPkt->getDestinationIP() << " Proto: " << receivedPkt->getProtocol() << endl;
        // Simple echo for ICMP, or log for others
        if (std::string(receivedPkt->getProtocol()) == "ICMP") {
            if (receivedPkt->getDestinationIP() == networkAddress) { // It's for me
                EV_INFO << getFullPath() << ": Echoing ICMP packet from " << receivedPkt->getSourceIP() << endl;
                CNETPacket *replyPkt = new CNETPacket("ICMPEchoReply");
                replyPkt->setSourceIP(networkAddress.c_str());
                replyPkt->setDestinationIP(receivedPkt->getSourceIP());
                replyPkt->setProtocol("ICMP_REPLY"); // Or just ICMP, and inspect payload type
                replyPkt->setSourcePort(receivedPkt->getDestinationPort());
                replyPkt->setDestinationPort(receivedPkt->getSourcePort());
                replyPkt->setByteLength(20);
                send(replyPkt, "gate$o");
            }
        }
        delete receivedPkt; // Consume the packet
    } else if (!msg->isSelfMessage()){
        EV_WARN << getFullPath() << ": Received non-CNETPacket message: " << msg->getName() << ". Deleting." << endl;
        delete msg; // Not a self message, not a CNETPacket we understand.
    }
    // Self messages not handled above are just deleted if they are not sendTimer
     else if (msg->isSelfMessage() && msg != sendTimer) {
        EV_INFO << getFullPath() << ": Deleting unhandled self-message " << msg->getName() << endl;
        delete msg;
    }
}

void Host::finish()
{
    if (isFirewall) {
        EV_INFO << "Firewall " << getFullPath() << " finishing. Rules applied: " << firewallRules.size() << endl;
        // Record firewall-specific stats if any
    }
    // General Host stats
    recordScalar("isFirewall", isFirewall ? 1 : 0);
    // Cancel timer in finish to be safe, though destructor handles it
    if (sendTimer && sendTimer->isScheduled()) {
        cancelEvent(sendTimer);
    }
}

} // namespace cnetfinal 