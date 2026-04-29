/* Disable specific mote (e.g., node 2) at start */
var nodeToDisable = 8;
var delayUntilEnable = 30000; // 30 seconds in milliseconds

TIMEOUT(100000); // Total simulation timeout

// Find mote and disable radio
mote = sim.getMoteWithID(nodeToDisable);
if (mote != null) {
    mote.getRadio().setOnline(false);
    log.log("Node " + nodeToDisable + " disabled.\n");
}

// Schedule reactivation
GENERATE_MSG(delayUntilEnable, "enable-node");

// Act on scheduled message
YIELD_THEN_WAIT_UNTIL(msg.equals("enable-node"));
if (mote != null) {
    mote.getRadio().setOnline(true);
    log.log("Node " + nodeToDisable + " enabled.\n");
}