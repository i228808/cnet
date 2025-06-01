package cnetfinal;
import cnetfinal.OpenFlowSwitch;
import cnetfinal.Host;

// Define Head Office module
module HeadOffice
{
    parameters:
        @display("bgb=700,400");
        
    gates:
        inout controllerLink;
        inout backbone[12];
        
    submodules:
        coreSwitch1: OpenFlowSwitch {
            parameters:
                isCore = true;
                @display("p=200,150");
        }
        coreSwitch2: OpenFlowSwitch {
            parameters:
                isCore = true;
                @display("p=350,150");
        }
        coreSwitch3: OpenFlowSwitch {
            parameters:
                isCore = true;
                @display("p=500,150");
        }
        
        dnsServer: Host {
            parameters:
                @display("i=device/server;p=200,300;t=DNS Server");
        }
        
        dhcpServer: Host {
            parameters:
                @display("i=device/server;p=350,300;t=DHCP Server");
        }
        
        firewallServer: Host {
            parameters:
                @display("i=device/server;p=500,300;t=Firewall");
                isFirewallModule = true;
        }
        
        edgeServer1: Host {
            parameters:
                @display("i=device/server;p=250,350;t=Edge Server 1");
        }
        
        edgeServer2: Host {
            parameters:
                @display("i=device/server;p=450,350;t=Edge Server 2");
        }
        
        // Application traffic modules (placeholders)
        videoServer: Host {
            parameters:
                @display("i=device/server;p=300,380;t=Video Server");
        }
        iotServer: Host {
            parameters:
                @display("i=device/server;p=400,380;t=IoT Server");
        }
        
        // Add internal switch to connect all controllers to the external controller link
        controllerSwitch: OpenFlowSwitch {
            parameters:
                isCore = true;
                @display("p=350,50;i=device/switch2");
        }
        
    connections:
        // Connect core switches to controller switch
        coreSwitch1.controllerOut --> Eth1G --> controllerSwitch.dataPort$i++;
        controllerSwitch.dataPort$o++ --> Eth1G --> coreSwitch1.controllerIn;
        
        coreSwitch2.controllerOut --> Eth1G --> controllerSwitch.dataPort$i++;
        controllerSwitch.dataPort$o++ --> Eth1G --> coreSwitch2.controllerIn;
        
        coreSwitch3.controllerOut --> Eth1G --> controllerSwitch.dataPort$i++;
        controllerSwitch.dataPort$o++ --> Eth1G --> coreSwitch3.controllerIn;
        
        // Connect controller switch to external controller link (Linter's implied preference)
        controllerSwitch.controllerIn <-- controllerLink$i;
        controllerSwitch.controllerOut --> controllerLink$o;
        
        // Connect core switches in a mesh
        coreSwitch1.dataPort++ <--> Eth1G <--> coreSwitch2.dataPort++;
        coreSwitch2.dataPort++ <--> Eth1G <--> coreSwitch3.dataPort++;
        coreSwitch3.dataPort++ <--> Eth1G <--> coreSwitch1.dataPort++;
        
        // Connect servers to core switches
        coreSwitch1.dataPort++ <--> Eth1G <--> dnsServer.gate;
        coreSwitch2.dataPort++ <--> Eth1G <--> dhcpServer.gate;
        coreSwitch3.dataPort++ <--> Eth1G <--> firewallServer.gate;
        
        // Connect edge servers
        coreSwitch1.dataPort++ <--> Eth1G <--> edgeServer1.gate;
        coreSwitch2.dataPort++ <--> Eth1G <--> edgeServer2.gate;
        
        // Connect application servers to core switches
        coreSwitch1.dataPort++ <--> Eth1G <--> videoServer.gate;
        coreSwitch2.dataPort++ <--> Eth1G <--> iotServer.gate;
        
        // Connect backbone links to core switches (distributed)
        for i=0..sizeof(backbone)-1 {
            coreSwitch1.dataPort++ <--> backbone[i] if i % 3 == 0;
            coreSwitch2.dataPort++ <--> backbone[i] if i % 3 == 1;
            coreSwitch3.dataPort++ <--> backbone[i] if i % 3 == 2;
        }
} 