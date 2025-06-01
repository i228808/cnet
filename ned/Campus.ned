package cnetfinal;
import cnetfinal.OpenFlowSwitch;
import cnetfinal.LocalController;
import cnetfinal.Host;
import cnetfinal.LabNetwork;

// Define a campus network
module Campus
{
    parameters:
        int numLabs = default(10);
        int systemsPerLab = default(40);
        string campusName;
        @display("bgb=600,500");
        
    gates:
        inout backbone[2];
        inout controllerGate;  // Gate to connect to central controller
        
    submodules:
        localController: LocalController {
            parameters:
                @display("p=300,50");
        }
        coreSwitch1: OpenFlowSwitch {
            parameters:
                isCore = true;
                @display("p=200,150");
        }
        coreSwitch2: OpenFlowSwitch {
            parameters:
                isCore = true;
                @display("p=300,150");
        }
        coreSwitch3: OpenFlowSwitch {
            parameters:
                isCore = true;
                @display("p=400,150");
        }
        lab[numLabs]: LabNetwork {
            parameters:
                numSystems = parent.systemsPerLab;  // Explicit reference to parent parameter
                @display("p=100,350,m,4,180");
        }
        edgeServer: Host {
            parameters:
                @display("i=device/server;p=500,350");
        }
        // Application traffic modules (placeholders)
        videoServer: Host {
            parameters:
                @display("i=device/server;p=400,380;t=Video Server");
        }
        iotServer: Host {
            parameters:
                @display("i=device/server;p=520,380;t=IoT Server");
        }
    connections:
        // Connect local controller to core switches
        localController.gate$o++ --> Eth1G --> coreSwitch1.controllerIn;
        localController.gate$i++ <-- Eth1G <-- coreSwitch1.controllerOut;
        localController.gate$o++ --> Eth1G --> coreSwitch2.controllerIn;
        localController.gate$i++ <-- Eth1G <-- coreSwitch2.controllerOut;
        localController.gate$o++ --> Eth1G --> coreSwitch3.controllerIn;
        localController.gate$i++ <-- Eth1G <-- coreSwitch3.controllerOut;
        
        // Connect core switches in a mesh for redundancy
        coreSwitch1.dataPort++ <--> Eth1G <--> coreSwitch2.dataPort++;
        coreSwitch2.dataPort++ <--> Eth1G <--> coreSwitch3.dataPort++;
        coreSwitch3.dataPort++ <--> Eth1G <--> coreSwitch1.dataPort++;
        
        // Connect labs to core switches (distributed for load balancing)
        for i=0..numLabs-1 {
            coreSwitch1.dataPort++ <--> Eth1G <--> lab[i].uplink if i % 3 == 0;
            coreSwitch2.dataPort++ <--> Eth1G <--> lab[i].uplink if i % 3 == 1;
            coreSwitch3.dataPort++ <--> Eth1G <--> lab[i].uplink if i % 3 == 2;
            
            // Connect each lab's controller gate to the local controller
            localController.gate$o++ --> lab[i].controllerGate$i;
            lab[i].controllerGate$o --> Eth1G --> localController.gate$i++;
        }
        
        // Connect edge server
        coreSwitch1.dataPort++ <--> Eth1G <--> edgeServer.gate;
        
        // Connect to backbone (main and backup)
        coreSwitch1.dataPort++ <--> backbone[0];
        coreSwitch1.dataPort++ <--> backbone[1];
        
        // Connect controller gate to local controller
        controllerGate <--> localController.gate++;
        
        // Connect application servers to core switches
        coreSwitch2.dataPort++ <--> Eth1G <--> videoServer.gate;
        coreSwitch3.dataPort++ <--> Eth1G <--> iotServer.gate;
} 