package cnetfinal;

// A simple network configurator
simple NetworkConfigurator
{
    parameters:
        @display("i=block/cogwheel");
        @class(cnetfinal::NetworkConfigurator);
        string configFile = default("ipconfig.xml");
} 