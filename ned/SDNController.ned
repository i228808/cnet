package cnetfinal;

// SDN Controller - base module
simple SDNController
{
    parameters:
        @display("i=block/control");
        @class(cnetfinal::SDNController);
        string controllerType = default("local");  // "centralized" or "local"
        int flowTableSize = default(1000);
        double processingDelay @unit(s) = default(0.5ms);
        @signal[flowSetupTime](type="simtime_t");
        @signal[controllerLoad](type="double");
        @statistic[flowSetupTime](title="Flow setup time"; record=vector,stats; unit=s);
        @statistic[controllerLoad](title="Controller load"; record=vector);
        
    gates:
        inout gate[];  // Connections to switches or other controllers
}

// CentralizedController is now defined in CentralizedController.ned
// The following definition has been removed:
// // Centralized (global) SDN controller
// simple CentralizedController extends SDNController
// {
//     parameters:
//         @display("i=block/control,green");
//         @class(cnetfinal::CentralizedController);
//         controllerType = "centralized";  // Always centralized
// }

// LocalController is now defined in LocalController.ned
// The following definition has been removed:
// // Local (campus-level) SDN controller
// simple LocalController extends SDNController
// {
//     parameters:
//         @display("i=block/control,blue");
//         @class(cnetfinal::LocalController);
//         controllerType = "local";  // Always local
//         double updateInterval @unit(s) = default(5s);  // Interval for syncing with global controller
// } 