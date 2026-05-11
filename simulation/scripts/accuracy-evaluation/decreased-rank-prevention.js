// Cooja script: count MRHOF parent rejections, TP/FP for address fd00::208:8:8:8
var TARGET_ADDR = "fd00::208:8:8:8";
var END_TIME = 5 * 60 * 1000000; // 5 minutes in microseconds (Cooja time)

var totalRejections = 0;
var tp = 0;
var fp = 0;
var lastMoteId = -1;
var lastMarkerTime = 0;

TIMEOUT(1800000); // 30-minute timeout

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

log.log("MRHOF rejection detection script started. Target: " + TARGET_ADDR + "\n");

while (true) {
  YIELD();

  // The marker message from rpl-mrhof.c:
  // "Prefer current parent over a neighbor with invalid hop count. Invalid neighbor's address:"
  var marker = "Prefer current parent over a neighbor with invalid hop count. Invalid neighbor's address:";

  if (msg.contains(marker)) {
    // Try to extract the address that follows the marker in the same log message.
    var maybeAddr = extractAddressAfterMarker(msg, marker);
    if (maybeAddr) {
      totalRejections++;
      if (maybeAddr.indexOf(TARGET_ADDR) !== -1) {
        tp++;
        log.log("[MRHOF] TP: " + maybeAddr + "\n");
      } else {
        fp++;
        log.log("[MRHOF] FP: " + maybeAddr + "\n");
      }
    } else {
      // Mark that we should expect the address on the next message from this mote.
      lastMoteId = msg.getMoteID();
      lastMarkerTime = time;
    }
  } else if (lastMoteId >= 0 && msg.getMoteID() === lastMoteId && (time - lastMarkerTime) < 100000) {
    // This might be the address line following the marker.
    var addr = extractIPv6(msg);
    if (addr) {
      totalRejections++;
      if (addr.indexOf(TARGET_ADDR) !== -1) {
        tp++;
        log.log("[MRHOF] TP: " + addr + "\n");
      } else {
        fp++;
        log.log("[MRHOF] FP: " + addr + "\n");
      }
      lastMoteId = -1;
    }
  }

  // Check if we've reached the end time
  if (time >= END_TIME) {
    var total = totalRejections;
    var tpRate = total > 0 ? (tp / total) : 0;
    var fpRate = total > 0 ? (fp / total) : 0;

    log.log("=== MRHOF rejection summary ===\n");
    log.log("Total rejections: " + total + "\n");
    log.log("True positives (addr contains " + TARGET_ADDR + "): " + tp + "\n");
    log.log("False positives: " + fp + "\n");
    log.log("TP / total = " + tp + " / " + total + " = " + tpRate.toFixed(4) + "\n");
    log.log("FP / total = " + fp + " / " + total + " = " + fpRate.toFixed(4) + "\n");
    log.log("=== end summary ===\n");
    log.testOK();
    break;
  }
}