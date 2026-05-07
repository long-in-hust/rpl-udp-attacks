// Import Java classes for File IO
var FileWriter = Java.type("java.io.FileWriter");
var BufferedWriter = Java.type("java.io.BufferedWriter");

var fileName = "energy_results.csv";
var fileWriter = new FileWriter(fileName);
var bufferedWriter = new BufferedWriter(fileWriter);

// Write CSV Header
bufferedWriter.write("Time;MoteID;State;Value;TotalTicks;Permil\n");
bufferedWriter.flush();

TIMEOUT(1800000); // 30-minute timeout

while (true) {
    YIELD();

    // Check if the message contains Energest info
    if (msg.contains("INFO: Energest") && msg.contains(":")) {
        
        var parts = msg.split(":");
        
        if (parts.length >= 3) {
            var stateName = parts[1].trim(); 
            var dataPart = parts[2].trim(); 

            // Only parse lines with numerical data (containing the / separator)
            if (dataPart.indexOf("/") !== -1) {
                
                var cleanData = dataPart.replace("/", ";")
                                        .replace("(", ";")
                                        .replace(" permil)", "");

                var finalRow = time + ";" + id + ";" + stateName + ";" + cleanData;
                
                // Write to the file and the Cooja log console
                bufferedWriter.write(finalRow + "\n");
                bufferedWriter.flush(); // Ensure data is saved immediately
                log.log("Saved: " + finalRow + "\n");
            }
        }
    }

    if (msg.contains("Simulation ended")) {
        bufferedWriter.close();
        log.testOK();
    }
}