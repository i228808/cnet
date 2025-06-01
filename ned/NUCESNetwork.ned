package cnetfinal;

// Define the main NUCES SDN network
network NUCESNetwork
{
    parameters:
        @display("bgb=1200,800");
        
    submodules:
        configurator: NetworkConfigurator {
            parameters:
                @display("p=100,50");
        }
        
        centralController: CentralizedController {
            parameters:
                @display("p=600,50");
        }
        
        headOffice: HeadOffice {
            parameters:
                @display("p=600,200");
        }
        
        islamabadCampus: Campus {
            parameters:
                campusName = "Islamabad";
                @display("p=300,400");
        }
        
        karachiCampus: Campus {
            parameters:
                campusName = "Karachi";
                @display("p=450,600");
        }
        
        lahoreCampus: Campus {
            parameters:
                campusName = "Lahore";
                @display("p=750,600");
        }
        
        faisalabadCampus: Campus {
            parameters:
                campusName = "Faisalabad";
                @display("p=900,400");
        }
        
        peshawarCampus: Campus {
            parameters:
                campusName = "Peshawar";
                @display("p=350,200");
        }
        
        multanCampus: Campus {
            parameters:
                campusName = "Multan";
                @display("p=850,200");
        }
        
    connections:
        // Connect head office to central controller
        centralController.gate$o++ --> Eth1G --> headOffice.controllerLink$i;  // Data from Central to HeadOffice
        headOffice.controllerLink$o --> Eth1G --> centralController.gate$i++;  // Data from HeadOffice to Central
        
        // Main and backup backbone links (explicit indices)
        headOffice.backbone[0] <--> Eth1G <--> islamabadCampus.backbone[0];
        headOffice.backbone[1] <--> Eth1G <--> islamabadCampus.backbone[1];
        headOffice.backbone[2] <--> Eth1G <--> karachiCampus.backbone[0];
        headOffice.backbone[3] <--> Eth1G <--> karachiCampus.backbone[1];
        headOffice.backbone[4] <--> Eth1G <--> lahoreCampus.backbone[0];
        headOffice.backbone[5] <--> Eth1G <--> lahoreCampus.backbone[1];
        headOffice.backbone[6] <--> Eth1G <--> faisalabadCampus.backbone[0];
        headOffice.backbone[7] <--> Eth1G <--> faisalabadCampus.backbone[1];
        headOffice.backbone[8] <--> Eth1G <--> peshawarCampus.backbone[0];
        headOffice.backbone[9] <--> Eth1G <--> peshawarCampus.backbone[1];
        headOffice.backbone[10] <--> Eth1G <--> multanCampus.backbone[0];
        headOffice.backbone[11] <--> Eth1G <--> multanCampus.backbone[1];
        
        // Connect campus controllers to central controller
        centralController.gate$o++ --> Eth1G --> islamabadCampus.controllerGate$i;
        islamabadCampus.controllerGate$o --> Eth1G --> centralController.gate$i++;
        
        centralController.gate$o++ --> Eth1G --> karachiCampus.controllerGate$i;
        karachiCampus.controllerGate$o --> Eth1G --> centralController.gate$i++;
        
        centralController.gate$o++ --> Eth1G --> lahoreCampus.controllerGate$i;
        lahoreCampus.controllerGate$o --> Eth1G --> centralController.gate$i++;
        
        centralController.gate$o++ --> Eth1G --> faisalabadCampus.controllerGate$i;
        faisalabadCampus.controllerGate$o --> Eth1G --> centralController.gate$i++;
        
        centralController.gate$o++ --> Eth1G --> peshawarCampus.controllerGate$i;
        peshawarCampus.controllerGate$o --> Eth1G --> centralController.gate$i++;
        
        centralController.gate$o++ --> Eth1G --> multanCampus.controllerGate$i;
        multanCampus.controllerGate$o --> Eth1G --> centralController.gate$i++;
} 