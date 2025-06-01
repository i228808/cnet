package cnetfinal;

import cnetfinal.SDNController;

// Centralized (global) SDN controller
simple CentralizedController extends SDNController
{
    parameters:
        @display("i=block/control,green");
        @class(cnetfinal::CentralizedController);
        controllerType = "centralized";  // Always centralized
    gates:
        // Inherits gates from SDNController (e.g., gate[])
        // Define specific gates if CentralizedController needs unique connections
        // not covered by the base SDNController's gates.
        // For now, assume base gates are sufficient or direct module access is used for LC communication.
} 