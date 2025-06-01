package cnetfinal;

// Define a simple host module
simple Host
{
    parameters:
        @display("i=device/pc");
        @class(cnetfinal::Host);
        string macAddress = default("");
        string networkAddress @mutable = default("");
        bool isFirewallModule = default(false);
        @numInitStages(2);
        
    gates:
        inout gate;
} 