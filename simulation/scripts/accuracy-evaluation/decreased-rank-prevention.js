// Cooja script: count MRHOF parent rejections, TP/FP for address fd00::208:8:8:8
var TARGET_ADDR = "fd00::208:8:8:8";
var RUN_TIME_MS = 5 * 60 * 1000; // default: 5 minutes

var totalRejections = 0;
var tp = 0;
var fp = 0;
var awaitingAddr = {}; // keyed by mote ID

function normalizeAddr(s) {
  if(!s) return "";
  return s.replace(/\s+/g, "").toLowerCase();
}

function extractIPv6(s) {
  if(!s) return null;
  // lightweight IPv6-ish capture: contiguous hex + ':' groups
  var m = s.match(/([0-9a-fA-F:]{2,})/);
  if(m) return normalizeAddr(m[1]);
  return null;
}

function handleAddressCount(addr) {
  totalRejections++;
  if(addr && addr.indexOf(TARGET_ADDR) !== -1) {
    tp++;
  } else {
    fp++;
  }
}

function makeLogListener(mote) {
  try {
    var logIf = mote.getInterfaces().getLog();
    logIf.addLogListener(function(sourceMote, text) {
      // text is a single log message fragment from a mote
      if(!text) return;

      // The decisive message from rpl-mrhof.c:
      // "Prefer current parent over a neighbor with invalid hop count. Invalid neighbor's address:"
      var marker = "Prefer current parent over a neighbor with invalid hop count. Invalid neighbor's address:";
      var mid = mote.getID();

      if(text.indexOf(marker) !== -1) {
        // Same log line might already include the address; try to extract
        var maybeAddr = extractIPv6(text);
        if(maybeAddr) {
          handleAddressCount(maybeAddr);
          awaitingAddr[mid] = false;
        } else {
          // Expect the next log entry from same mote to contain the printed IPv6
          awaitingAddr[mid] = true;
        }
        return;
      }

      // If we were awaiting an address for this mote, treat this text as the printed address line
      if(awaitingAddr[mid]) {
        var addr = extractIPv6(text);
        handleAddressCount(addr);
        awaitingAddr[mid] = false;
        return;
      }

      // Defensive: sometimes the address might appear on same line without marker
      // (rare) -> check for marker-less direct address printing
      if(text.indexOf(TARGET_ADDR) !== -1) {
        // If we see the target address but didn't previously mark a rejection, don't increment totalRejections here.
        // This branch is only informational; you can decide to count it by moving logic here.
      }
    });
  } catch(e) {
    print("Failed to attach log listener to mote " + mote.getID() + ": " + e);
  }
}

// Attach listeners to all current motes and to future added motes.
for(var i = 0; i < sim.getMotes().length; i++) {
  makeLogListener(sim.getMotes()[i]);
}

// Listen for motes added later
sim.getMotes().addSimEventListener({
  moteAdded: function(mote) { makeLogListener(mote); },
  moteRemoved: function(mote) { /* ignore */ }
});

// Summary printer
function printSummary() {
  var total = totalRejections;
  var tpRate = total > 0 ? (tp / total) : 0;
  var fpRate = total > 0 ? (fp / total) : 0;
  print("=== MRHOF rejection summary ===");
  print("Total rejections: " + total);
  print("True positives (addr contains " + TARGET_ADDR + "): " + tp);
  print("False positives: " + fp);
  print("TP / total = " + tp + " / " + total + " = " + tpRate.toFixed(4));
  print("FP / total = " + fp + " / " + total + " = " + fpRate.toFixed(4));
  print("=== end summary ===");
}

// Auto-print after RUN_TIME_MS and stop simulation (optional)
setTimeout(function() {
  printSummary();
  // Optional: stop simulation automatically
  // sim.stopSimulation();
}, RUN_TIME_MS);

// Also expose functions for manual control from the script console:
// - call printSummary() to print current counts
// - call resetCounts() to zero counters
function resetCounts() {
  totalRejections = 0; tp = 0; fp = 0;
  awaitingAddr = {};
  print("Counters reset.");
}