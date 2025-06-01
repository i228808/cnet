package cnetfinal;

import cnetfinal.SDNController;

// Local SDN Controller for a campus
module LocalController extends SDNController
{
    parameters:
        @display("i=pc/laptop"); // Or a more appropriate controller icon
        string campusName = default(""); // Name of the campus this controller manages
        double updateInterval @unit(s) = default(5s); // Interval to update/sync topology (in seconds)
        // controllerType should be inherited or overridden if needed, e.g. "local"
        controllerType = "local"; 
    gates:
        // Inherits gates from SDNController (e.g., gate[] which is inout gate[])
        // No need to redefine "inout gate[]" here as it's inherited.
        // The line below is removed:
        // inout gate[]; 
} 