// version-drop-measurement.js
// Count DIOs dropped due to invalid DAG version and classify TP/FP.
// TP = source == fd00::208:8:8:8

var DROP_MSG = "Dropping DIO due to invalid DAG version from";
var TARGET_ADDR = "fd00::208:8:8:8";

var DROPPED = 0;
var TP = 0;
var FP = 0;

var WARMUP_PERIOD = 60 * 1000000;    // ignore first 60s
var REPORT_INTERVAL = 60 * 1000000;  // report every 60s after warmup
var STOP_TIME = 11 * 60 * 1000000;   // stop after 11 minutes
var next_report = WARMUP_PERIOD + REPORT_INTERVAL;

log.log("Version-drop measurement script started. Warmup: 60s. Target: " + TARGET_ADDR + "\n");

while (true) {
  YIELD();

  if (time < WARMUP_PERIOD) {
    continue;
  }

  if (msg.contains(DROP_MSG)) {
    // Try to extract IPv6 address from the same log line
    var m = msg.match(/Dropping DIO due to invalid DAG version from\s*([0-9a-f:]+)/i);
    var addr = null;
    if (m && m[1]) {
      addr = m[1].toLowerCase();
    } else {
      // fallback: try to find any IPv6-like token in the message
      var m2 = msg.match(/([0-9a-f:]{3,})/i);
      if (m2 && m2[1]) {
        addr = m2[1].toLowerCase();
      }
    }

    DROPPED++;
    if (addr === TARGET_ADDR) {
      TP++;
      log.log("[VERSION-DROP] True Positive: " + addr + " (total dropped: " + DROPPED + ")\n");
    } else if (addr) {
      FP++;
      log.log("[VERSION-DROP] False Positive: " + addr + " (total dropped: " + DROPPED + ")\n");
    } else {
      // Unknown addr -> count as FP (conservative)
      FP++;
      log.log("[VERSION-DROP] Unknown addr (counted as FP). Total dropped: " + DROPPED + "\n");
    }
  }

  // Periodic reporting
  if (time >= next_report) {
    var tp_rate = (DROPPED > 0) ? (TP / DROPPED) : 0;
    var fp_rate = (DROPPED > 0) ? (FP / DROPPED) : 0;

    log.log("--- Version-Drop Report at " + (time / 1000000).toFixed(0) + "s ---\n");
    log.log("  DIOs dropped (version mismatch): " + DROPPED + "\n");
    log.log("  True Positives: " + TP + "\n");
    log.log("  False Positives: " + FP + "\n");
    log.log("  TP/Total: " + tp_rate.toFixed(3) + " | FP/Total: " + fp_rate.toFixed(3) + "\n");
    log.log("-------------------------------------------\n");

    next_report += REPORT_INTERVAL;
  }

  // Final stop and report
  if (time >= STOP_TIME) {
    var tp_rate = (DROPPED > 0) ? (TP / DROPPED) : 0;
    var fp_rate = (DROPPED > 0) ? (FP / DROPPED) : 0;

    log.log("=== FINAL VERSION-DROP METRICS ===\n");
    log.log("DIOs dropped (version mismatch): " + DROPPED + "\n");
    log.log("True Positives: " + TP + "\n");
    log.log("False Positives: " + FP + "\n");
    log.log("TP/Total: " + tp_rate.toFixed(3) + "\n");
    log.log("FP/Total: " + fp_rate.toFixed(3) + "\n");
    log.testOK();
    break;
  }
}