#include <omnetpp.h>
#include "NetworkConfigurator.h"
#include "SDNController.h"
#include "OpenFlowSwitch.h"
#include "Host.h"
#include "FlowRule.h"

using namespace omnetpp;
using namespace cnetfinal;

// This file explicitly ensures all module classes are registered
namespace cnetfinal {
    class ModuleRegistrator {
    public:
        ModuleRegistrator() {
            // The actual registration is done in the module's .cc files with Define_Module
            // This is just to ensure the linker includes those symbols
        }
    };
    
    // Static instance to ensure initialization
    static ModuleRegistrator registrator;
} 