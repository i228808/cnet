package cnetfinal;

simple OpenFlowSwitch
{
    parameters:
        @display("i=device/switch");
        @class(cnetfinal::OpenFlowSwitch);
        string switchId = default("");  // Auto-assigned if empty
        int flowTableCapacity = default(100);  // Maximum number of flow entries
        bool isCore = default(false);  // Whether this is a core switch
        // VLAN support
        int vlanId = default(-1);      // VLAN ID for this switch (-1 = no VLAN)
        bool enableVlan = default(false); // Enable VLAN support
        @signal[packetProcessingTime](type="simtime_t");
        @signal[tableHitRatio](type="double");
        @statistic[packetProcessingTime](title="Packet processing time"; record=vector,stats; unit=s);
        @statistic[tableHitRatio](title="Flow table hit ratio"; record=vector);
        
    gates:
        inout dataPort[];  // Data plane ports
        input controllerIn;  // Control plane input from SDN controller
        output controllerOut;  // Control plane output to SDN controller
}