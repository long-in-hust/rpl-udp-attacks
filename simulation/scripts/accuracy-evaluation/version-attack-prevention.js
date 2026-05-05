// Cooja script: count DIOs dropped due to invalid DAG version and classify TP/FP
// Assumptions (see below) about API and variable names.

var sim = Cooja.getSimulation();
var motes = sim.getMotes();

var stats = {
  dio_dropped_total: 0,
  tp: 0,
  fp: 0
};

function u8(v) { return v & 0xff; }
function hash_new_to_old(newv) { return u8((newv * 31) & 0xff); }

// Try to read DAG version from mote memory; returns integer or null if not available
function getMoteDagVersion(mote) {
  try {
    var mem = mote.getMemory();
    // Common symbol name in this repo: curr_instance.dag.version
    // Some Cooja versions use getIntValue, others getShort or getByte; try several.
    if (typeof mem.getIntValue === 'function') {
      var val = mem.getIntValue("curr_instance.dag.version");
      if (val !== undefined) return val & 0xff;
    }
    if (typeof mem.getByteValue === 'function') {
      var val = mem.getByteValue("curr_instance.dag.version");
      if (val !== undefined) return val & 0xff;
    }
    // fallback: try reading as symbol via getVariable
    if (typeof mem.getValue === 'function') {
      var val = mem.getValue("curr_instance.dag.version");
      if (val !== undefined) return val & 0xff;
    }
  } catch (e) { /* ignore and return null */ }
  return null;
}

// Parse raw packet bytes for an RPL DIO. Returns {version, rank, src, dst} or null.
function parseDIO(bytes) {
  // Search for ICMPv6 type 155 (RPL control). If found, DIO is ICMPv6 code 0.
  // DIO control header (RFC6719): after ICMPv6 header, DIO fields:
  //   [Instance ID:1][Version:1][Rank:2 BE] ...
  for (var i = 0; i + 4 < bytes.length; i++) {
    if ((bytes[i] & 0xff) === 155) { // ICMPv6 type 155 (RPL)
      var code = bytes[i + 1] & 0xff;
      // DIO control code is 0
      if (code !== 0) return null;
      var icmp_payload_offset = i + 4; // skip ICMPv6 header (type, code, checksum)
      // Ensure we have at least InstanceID(1) + Version(1) + Rank(2)
      if (icmp_payload_offset + 4 <= bytes.length) {
        var inst = bytes[icmp_payload_offset] & 0xff;
        var ver = bytes[icmp_payload_offset + 1] & 0xff;
        var rank = ((bytes[icmp_payload_offset + 2] & 0xff) << 8) | (bytes[icmp_payload_offset + 3] & 0xff);
        return { version: ver, rank: rank };
      }
    }
  }
  return null;
}

// Packet listener: fire when any mote receives a radio packet
function registerRadioListeners() {
  // Many Cooja builds expose getRadioMedium().addRadioPacketListener
  var med = sim.getRadioMedium();
  if (typeof med.addRadioPacketListener === 'function') {
    med.addRadioPacketListener(function(evt) {
      // evt can be: {packet, srcMote, dstMote}
      try {
        var packet = evt.packet;
        var dst = evt.dstMote;
        if (!dst) return;
        // get packet byte array
        var bytes = null;
        if (packet.getPacketData) {
          bytes = packet.getPacketData();
        } else if (packet.getByteArray) {
          bytes = packet.getByteArray();
        } else if (packet.getBytes) {
          bytes = packet.getBytes();
        }
        if (!bytes) return;
        var dio = parseDIO(bytes);
        if (!dio) return;
        // We observed a DIO arriving at dst mote with version dio.version
        handleObservedDIO(dst, dio.version);
      } catch (e) { /* ignore parse errors */ }
    });
    return true;
  }

  // Fallback: attach per-mote radio interfaces if API provides 'getInterfaces'
  for (var mi = 0; mi < motes.length; mi++) {
    (function(mote) {
      try {
        var ifaces = mote.getInterfaces();
        for (var j = 0; j < ifaces.length; j++) {
          var iface = ifaces[j];
          if (iface.getRadio && typeof iface.getRadio === 'function') {
            var radio = iface.getRadio();
            if (radio && typeof radio.addPacketListener === 'function') {
              radio.addPacketListener(function(packet) {
                try {
                  var bytes = packet.getPacketData ? packet.getPacketData() : (packet.getByteArray ? packet.getByteArray() : null);
                  if (!bytes) return;
                  var dio = parseDIO(bytes);
                  if (!dio) return;
                  handleObservedDIO(mote, dio.version);
                } catch (e) {}
              });
            }
          }
        }
      } catch (e) {}
    })(motes[mi]);
  }
  return true;
}

// Called when a DIO is observed arriving at 'mote' with incomingVersion
function handleObservedDIO(mote, incomingVersion) {
  var before = getMoteDagVersion(mote);
  // schedule a short delayed check so the mote can process the DIO if it will
  sim.scheduleEvent(function() {
    var after = getMoteDagVersion(mote);
    // If the mote's `curr_instance.dag.version` equals incomingVersion, it accepted/updated.
    // If the mote's version is unchanged and differs from incomingVersion, treat as dropped/ignored.
    if (before === null && after === null) {
      // Unable to read mote's DAG version: cannot classify — skip
      return;
    }
    if (after === incomingVersion) {
      // mote accepted/updated: not a drop
      return;
    }
    // If after !== incomingVersion, consider this a drop due to version mismatch.
    stats.dio_dropped_total++;
    var old = after;
    if (old === null) {
      // fallback: if we only had 'before', use that as old
      old = before;
    }
    if (old === null) {
      // still unknown; can't classify; skip
      return;
    }
    var hashed = hash_new_to_old(incomingVersion);
    if (hashed !== old) {
      stats.tp++;
    } else {
      stats.fp++;
    }
  }, 50); // 50 ms simulation delay to allow mote processing; adjust if needed
}

// Utility to print stats
function printStats() {
  var total = stats.dio_dropped_total;
  var tp = stats.tp;
  var fp = stats.fp;
  var tp_rate = total > 0 ? (tp / total) : 0;
  var fp_rate = total > 0 ? (fp / total) : 0;
  var s = "DIO-version-drop stats: dropped=" + total +
          ", tp=" + tp + ", fp=" + fp +
          ", tp/total=" + tp_rate.toFixed(3) +
          ", fp/total=" + fp_rate.toFixed(3);
  print(s);
}

// Register listeners and periodic printer
registerRadioListeners();
var printInterval = 10000; // ms of simulation time
sim.schedulePeriodicEvent(function() {
  printStats();
}, printInterval);

// Also print final stats when simulation stops
sim.simulationFinished = function() {
  printStats();
};