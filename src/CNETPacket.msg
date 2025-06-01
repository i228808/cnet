// CNETPacket.msg

namespace cnetfinal;

// Basic packet structure for CNETFINAL simulations
// We'll use string for IPs for simplicity with current FirewallRule, 
// but a dedicated IPAddress class would be better for IPUtils.
packet CNETPacket {
    string sourceIP;
    string destinationIP;
    int sourcePort = -1;       // -1 if not applicable
    int destinationPort = -1;  // -1 if not applicable
    string protocol;         // e.g., "TCP", "UDP", "ICMP", "DHCP_DISCOVER", "DNS_QUERY"
    // Potentially add a payload field if needed, e.g., string payloadData;
}

// Specific message types for services, can extend CNETPacket or be separate
// For now, we can use the protocol field in CNETPacket to distinguish.

// Example for DHCP (can be refined later)
// packet DHCPDiscover extends CNETPacket {
//    protocol = "DHCP_DISCOVER";
//    // DHCP specific fields like MAC address, transaction ID etc.
// }

// packet DHCPOffer extends CNETPacket {
//    protocol = "DHCP_OFFER";
//    // Offered IP, lease time, server IP etc.
// }

// We will also need to define messages for actual application data later. 