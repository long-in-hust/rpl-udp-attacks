// version-drop-measurement.js
// Count DIOs dropped due to invalid DAG version and classify TP/FP.
// TP = source == fe80::208:8:8:8

var DROP_MSG = "Blacklisting the source IP address of the malformed DIO: ";
var TARGET_ADDR = "fe80::208:8:8:8";

var DROPPED = 0;
var TP = 0;
var FP = 0;

var WARMUP_PERIOD = 120 * 1000000;    // ignore first 60s
var REPORT_INTERVAL = 60 * 1000000;  // report every 60s after warmup
var STOP_TIME = 29 * 60 * 1000000 + 59.5 * 1000000; 
var next_report = WARMUP_PERIOD + REPORT_INTERVAL;

TIMEOUT(1800000); // 30-minute timeout

log.log("Version-drop measurement script started. Warmup: 60s. Target: " + TARGET_ADDR + "\n");

function normalizeAddr(s) {
  if(!s) return "";
  return s.replace(/\s+/g, "").toLowerCase();
}

function extractIPv6(s) {
  if(!s) return null;
  var text = normalizeAddr(s);

  // Prefer a token that contains ':' and only IPv6 characters.
  var token = text.split(/[^0-9a-fA-F:]+/)[0];
  if(token && token.indexOf(":") !== -1 && /^[0-9a-f:]+$/i.test(token)) {
    return token;
  }

  // Fallback: find a compact IPv6-looking fragment with at least one colon.
  var m = text.match(/([0-9a-fA-F:]*:[0-9a-fA-F:]+)/);
  if(m) return normalizeAddr(m[1]);
  return null;
}

function extractAddressAfterMarker(text, marker) {
  if(!text || text.indexOf(marker) === -1) return null;
  var tail = text.substring(text.indexOf(marker) + marker.length);
  return extractIPv6(tail);
}

while (true) {
  YIELD();

  if (time < WARMUP_PERIOD) {
    continue;
  }

  if (msg.contains(DROP_MSG)) {
    // Try to extract IPv6 address from the same log line
    addr = extractAddressAfterMarker(msg, DROP_MSG);
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