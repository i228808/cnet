package cnetfinal;

// Define our own channel types since INET import is failing
channel Eth100M extends ned.DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 100Mbps;
}

channel Eth1G extends ned.DatarateChannel
{
    parameters:
        delay = 0.1us;
        datarate = 1Gbps;
} 