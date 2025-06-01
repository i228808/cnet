#include <omnetpp.h>
#include "NetworkConfigurator.h"

namespace cnetfinal {

Define_Module(NetworkConfigurator);

void NetworkConfigurator::initialize()
{
    // Load configuration from file
    configFile = par("configFile").stdstringValue();
    EV << "NetworkConfigurator initializing with config file: " << configFile << endl;
    
    // Assign IP addresses to all hosts in the network
    cModule *system = getSystemModule();
    int campusIdx = 1;
    for (cModule::SubmoduleIterator campus(system); !campus.end(); ++campus) {
        cModule *campusMod = *campus;
        if (strstr(campusMod->getName(), "Campus") || strstr(campusMod->getName(), "campus")) {
            // For each campus, find labs
            int labIdx = 1;
            for (cModule::SubmoduleIterator lab(campusMod); !lab.end(); ++lab) {
                cModule *labMod = *lab;
                if (strstr(labMod->getName(), "LabNetwork") || strstr(labMod->getName(), "lab")) {
                    // For each lab, find hosts
                    int hostIdx = 0;
                    for (cModule::SubmoduleIterator host(labMod); !host.end(); ++host) {
                        cModule *hostMod = *host;
                        if (strstr(hostMod->getName(), "system")) {
                            char ip[32];
                            sprintf(ip, "10.%d.%d.%d", campusIdx, labIdx, hostIdx+1);
                            hostMod->par("networkAddress").setStringValue(ip);
                            EV << "Assigned IP " << ip << " to " << hostMod->getFullPath() << endl;
                            hostIdx++;
                        }
                    }
                    labIdx++;
                }
            }
            campusIdx++;
        }
    }
    // Assign IPs to head office servers
    cModule *headOffice = system->getSubmodule("headOffice");
    if (headOffice) {
        int hoHostIdx = 1;
        for (cModule::SubmoduleIterator host(headOffice); !host.end(); ++host) {
            cModule *hostMod = *host;
            // Only assign to Host modules (not switches)
            if (strstr(hostMod->getNedTypeName(), "Host")) {
                char ip[32];
                sprintf(ip, "10.0.0.%d", hoHostIdx++);
                hostMod->par("networkAddress").setStringValue(ip);
                EV << "Assigned IP " << ip << " to " << hostMod->getFullPath() << endl;
            }
        }
    }
    EV << "Network configuration complete" << endl;
}

void NetworkConfigurator::handleMessage(cMessage *msg)
{
    // This module doesn't process messages
    EV << "NetworkConfigurator received unexpected message: " << msg->getName() << endl;
    delete msg;
}

void NetworkConfigurator::configureNode(cModule *node)
{
    if (!node) return;
    
    EV << "Configuring node: " << node->getFullName() << endl;
    
    // In a full implementation, this would assign IP addresses, etc.
    // For this simplified version, we just log that we're configuring the node
}

void NetworkConfigurator::setupRoutes()
{
    EV << "Setting up routes in the network" << endl;
    
    // In a full implementation, this would configure routing tables
    // For this simplified version, we just log that we're setting up routes
}

} // namespace cnetfinal 