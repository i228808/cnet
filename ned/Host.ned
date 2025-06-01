package cnetfinal;

import inet.linklayer.ethernet.EthernetInterface;

// Define a host module with Ethernet capability
module Host
{
    parameters:
        @display("i=device/pc");
        @class(cnetfinal::Host);
        string macAddress = default("auto");
        string networkAddress @mutable = default("");
        bool isFirewallModule = default(false);
        @numInitStages(2);
        
    gates:
        inout gate;
        
    submodules:
        eth: EthernetInterface {
            parameters:
                mac.address = parent.macAddress; // Use the parent's MAC address
                @display("p=100,100");
        }
        
    connections:
        eth.phys <--> gate;
}