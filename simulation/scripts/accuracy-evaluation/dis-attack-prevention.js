var ATTACKER = "fd00::208:8:8:8";
var tp = 0;
var fp = 0;
var totalDropped = 0;
var totalDisSeen = 0;
var attackerDisSeen = 0;
var benignDisSeen = 0;

while (true) {
  YIELD();

  var isDisLine = msg.contains("Dropping DIS packet.") ||
                  msg.contains("Accepting DIS packet.");
  if (!isDisLine) {
    continue;
  }

  var isAttacker = msg.contains(ATTACKER);
  var isDrop = msg.contains("Dropping DIS packet.");

  totalDisSeen++;
  if (isAttacker) {
    attackerDisSeen++;
  } else {
    benignDisSeen++;
  }

  if (isDrop) {
    totalDropped++;
    if (isAttacker) {
      tp++;
    } else {
      fp++;
    }
  }

  if (time >= END_TIME) {
    var dropBasedTpRate = totalDropped > 0 ? (100.0 * tp / totalDropped) : 0;
    var dropBasedFpRate = totalDropped > 0 ? (100.0 * fp / totalDropped) : 0;
    var obsBasedTpRate = totalDisSeen > 0 ? (100.0 * tp / totalDisSeen) : 0;
    var obsBasedFpRate = totalDisSeen > 0 ? (100.0 * fp / totalDisSeen) : 0;

    log.log("DIS summary\n");
    log.log("  total DIS seen: " + totalDisSeen + "\n");
    log.log("  attacker DIS seen: " + attackerDisSeen + "\n");
    log.log("  benign DIS seen: " + benignDisSeen + "\n");
    log.log("  dropped DIS total: " + totalDropped + "\n");
    log.log("  true positives: " + tp + "\n");
    log.log("  false positives: " + fp + "\n");
    log.log("  TP/total-dropped: " + dropBasedTpRate.toFixed(2) + "%\n");
    log.log("  FP/total-dropped: " + dropBasedFpRate.toFixed(2) + "%\n");
    log.log("  TP/total-seen: " + obsBasedTpRate.toFixed(2) + "%\n");
    log.log("  FP/total-seen: " + obsBasedFpRate.toFixed(2) + "%\n");
    log.testOK();
    break;
  }
}