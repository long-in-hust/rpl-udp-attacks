/*
 * Script to extract Energest data from all nodes (12-15) 
 * and save to a CSV file.
 */

// Initialize the log file
log.setFile("energy_results.csv");
log.write("Time;MoteID;State;Value;TotalTicks;Permil\n");

TIMEOUT(960000); // Set timeout (e.g., 60 minutes in ms)

while (true) {
    // Wait for a message from any mote
    YIELD();

    // Check if the message contains Energest info
    if (msg.contains("INFO: Energest")) {
        
        // Log the raw data with mote ID and timestamp
        // msg format: [INFO: Energest  ] State : Ticks / Total (Permil)
        
        var parts = msg.split(":");
        if (parts.length >= 3) {
            var stateInfo = parts[2].trim(); // e.g., "CPU  60005112/ 60005112 (1000 permil)"
            
            // Clean up the string for CSV
            var cleanRow = time + ";" + id + ";" + stateInfo.replace(/\//g, ";").replace(/\(/g, ";").replace(/ permil\)/g, "");
            
            log.write(cleanRow + "\n");
        }
    }

    // Optional: Stop script if a specific condition is met
    if (msg.contains("Simulation ended")) {
        log.testOK(); 
    }
}