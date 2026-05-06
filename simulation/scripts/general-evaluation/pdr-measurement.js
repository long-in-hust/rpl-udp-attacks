// Initialize counters
var client_requests_sent = 0;
var server_requests_received = 0;
var server_responses_sent = 0;
var client_responses_received = 0;
var warmup_done_msg_logged = false;

// Time Constants (in microseconds)
var WARMUP_PERIOD = 60 * 1000000;    // Ignore the first 60 seconds
var END_TIME = 16 * 60 * 1000000;     // Total simulation time of 16 minutes
var REPORT_INTERVAL = 60 * 1000000;  // Report every 60 seconds after warmup
var next_report = WARMUP_PERIOD + REPORT_INTERVAL;

log.log("Simulation started. Waiting for " + (WARMUP_PERIOD / 1000000) + "s warmup...\n");

while (true) {
  YIELD();

  // ONLY start counting after the warmup period
  if (time >= WARMUP_PERIOD) {
    if (!warmup_done_msg_logged) {
      log.log("Warmup complete. Starting PDR measurement at " + (time / 1000000).toFixed(2) + "s...\n");
      warmup_done_msg_logged = true;
    }

    // 1. Count Client Requests Sent
    if (msg.contains("Sending request")) {
      client_requests_sent++;
    }

    // 2. Count Server Requests Received
    if (msg.contains("Received request")) {
      server_requests_received++;
    }

    // 3. Count Server Responses Sent
    if (msg.contains("Sending response")) {
      server_responses_sent++;
    }

    // 4. Count Client Responses Received
    if (msg.contains("Received response")) {
      client_responses_received++;
    }

    // Periodic Reporting
    if (time >= next_report) {
      var upstream_pdr = (client_requests_sent > 0) ? 
          (server_requests_received / client_requests_sent) * 100 : 0;
      
      var downstream_pdr = (server_responses_sent > 0) ? 
          (client_responses_received / server_responses_sent) * 100 : 0;

      log.log("--- PDR Report (Post-Warmup) at " + (time / 1000000).toFixed(2) + "s ---\n");
      log.log("Upstream (Client -> Server):\n");
      log.log("  Sent: " + client_requests_sent + " | Received: " + server_requests_received + "\n");
      log.log("  PDR: " + upstream_pdr.toFixed(2) + "%\n");
      
      log.log("Downstream (Server -> Client):\n");
      log.log("  Sent: " + server_responses_sent + " | Received: " + client_responses_received + "\n");
      log.log("  PDR: " + downstream_pdr.toFixed(2) + "%\n");
      log.log("--------------------------------------\n");

      next_report += REPORT_INTERVAL;
    }
    if (time > END_TIME) {
        log.log("--- FINAL 11-MINUTE MARK REACHED ---\n");
        // This is the clean way to stop the simulation in Cooja's Scripting API
        log.log("Stopping simulation...\n");
        log.testOK(); // This calls the simulation stop mechanism and closes the script
        break; // End simulation after the final report
    }
  }
}