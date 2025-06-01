package cnetfinal;

// Define a lab network module
module LabNetwork
{
    parameters:
        int numSystems = default(40);
        @display("bgb=400,300");
        
    gates:
        inout uplink;
        inout controllerGate;  // New gate for controller connection
        
    submodules:
        labSwitch: OpenFlowSwitch {
            parameters:
                @display("p=200,100");
        }
        system[numSystems]: Host {
            parameters:
                @display("i=device/pc;p=100,200,m,5,80");
        }
    connections:
        // Connect systems to the lab switch
        for i=0..numSystems-1 {
            system[i].gate <--> Eth100M <--> labSwitch.dataPort++;
        }
        
        // Connect lab switch to uplink
        labSwitch.dataPort++ <--> uplink;
        
        // Connect lab switch to controller - use explicit suffixes
        labSwitch.controllerIn <-- controllerGate$i;
        labSwitch.controllerOut --> controllerGate$o;
} 