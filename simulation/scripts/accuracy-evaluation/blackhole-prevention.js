// blacklist-measurement.js
// Counts blacklists, true positives (parent == fd00::208:8:8:8), false positives, and reports rates.

var BLACKLIST_COUNT = 0;
var TP_COUNT = 0;
var FP_COUNT = 0;

var TARGET_ADDR = "fd00::208:8:8:8";

var WARMUP_PERIOD = 60 * 1000000;    // ignore first 60s
var REPORT_INTERVAL = 60 * 1000000;  // report every 60s after warmup
var STOP_TIME = 11 * 60 * 1000000;   // stop after 11 minutes
var next_report = WARMUP_PERIOD + REPORT_INTERVAL;

log.log("Blacklist measurement script started. Warmup: 60s. Target: " + TARGET_ADDR + "\n");

while (true) {
  YIELD();

  // Only measure after warmup
  if (time >= WARMUP_PERIOD) {

    // Detect blacklist confirmation log lines that include the address and "is a blackhole"
    if (msg.contains("is a blackhole")) {
      // Try to extract the IPv6 address immediately before "is a blackhole"
      var m = msg.match(/([0-9a-f:]+)\s+is\s+a\s+blackhole/i);
      if (m && m[1]) {
        var addr = m[1].toLowerCase();
        BLACKLIST_COUNT++;
        if (addr === TARGET_ADDR) {
          TP_COUNT++;
          log.log("[BLACKLIST] True Positive: " + addr + " (total: " + BLACKLIST_COUNT + ")\n");
        } else {
          FP_COUNT++;
          log.log("[BLACKLIST] False Positive: " + addr + " (total: " + BLACKLIST_COUNT + ")\n");
        }
      } else {
        // Fallback: count as blacklist but unknown addr
        BLACKLIST_COUNT++;
        FP_COUNT++;
        log.log("[BLACKLIST] Unknown addr (counted as FP). Total: " + BLACKLIST_COUNT + "\n");
      }
    }

    // Periodic reporting
    if (time >= next_report) {
      var tp_rate = (BLACKLIST_COUNT > 0) ? (TP_COUNT / BLACKLIST_COUNT) : 0;
      var fp_rate = (BLACKLIST_COUNT > 0) ? (FP_COUNT / BLACKLIST_COUNT) : 0;

      log.log("--- Blacklist Report at " + (time / 1000000).toFixed(0) + "s ---\n");
      log.log("  Blacklists: " + BLACKLIST_COUNT + "\n");
      log.log("  True Positives: " + TP_COUNT + "\n");
      log.log("  False Positives: " + FP_COUNT + "\n");
      log.log("  TP/Total: " + tp_rate.toFixed(3) + " | FP/Total: " + fp_rate.toFixed(3) + "\n");
      log.log("-------------------------------------------\n");

      next_report += REPORT_INTERVAL;
    }

    // Final stop and report
    if (time >= STOP_TIME) {
      var tp_rate = (BLACKLIST_COUNT > 0) ? (TP_COUNT / BLACKLIST_COUNT) : 0;
      var fp_rate = (BLACKLIST_COUNT > 0) ? (FP_COUNT / BLACKLIST_COUNT) : 0;

      log.log("=== FINAL BLACKLIST METRICS ===\n");
      log.log("Blacklists: " + BLACKLIST_COUNT + "\n");
      log.log("True Positives: " + TP_COUNT + "\n");
      log.log("False Positives: " + FP_COUNT + "\n");
      log.log("TP/Total: " + tp_rate.toFixed(3) + "\n");
      log.log("FP/Total: " + fp_rate.toFixed(3) + "\n");
      log.testOK();
      break;
    }
  }
}