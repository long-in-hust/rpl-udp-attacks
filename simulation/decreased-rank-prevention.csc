<?xml version="1.0" encoding="UTF-8"?>
<simconf version="2023090101">
  <simulation>
    <title>rpl-decreased-rank-prevented</title>
    <speedlimit>20.0</speedlimit>
    <randomseed>321459</randomseed>
    <motedelay_us>1000000</motedelay_us>
    <radiomedium>
      org.contikios.cooja.radiomediums.UDGM
      <transmitting_range>50.0</transmitting_range>
      <interference_range>100.0</interference_range>
      <success_ratio_tx>1.0</success_ratio_tx>
      <success_ratio_rx>1.0</success_ratio_rx>
    </radiomedium>
    <events>
      <logoutput>50000</logoutput>
    </events>
    <motetype>
      org.contikios.cooja.contikimote.ContikiMoteType
      <description>Root Mote</description>
      <source>[CONFIG_DIR]/../prevent-dec-rank/udp-root.c</source>
      <commands>$(MAKE) -j$(CPUS) udp-root.cooja TARGET=cooja</commands>
      <moteinterface>org.contikios.cooja.interfaces.Position</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.Battery</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiVib</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiMoteID</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiRS232</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiBeeper</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.IPAddress</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiRadio</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiButton</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiPIR</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiClock</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiLED</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiCFS</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiEEPROM</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.Mote2MoteRelations</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.MoteAttributes</moteinterface>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="-7.827918836570781" y="21.572613762585323" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.contikimote.interfaces.ContikiMoteID
          <id>1</id>
        </interface_config>
        <interface_config>
          org.contikios.cooja.contikimote.interfaces.ContikiRS232
          <history>help~;HELP~;</history>
        </interface_config>
      </mote>
    </motetype>
    <motetype>
      org.contikios.cooja.contikimote.ContikiMoteType
      <description>Client Mote</description>
      <source>[CONFIG_DIR]/../prevent-dec-rank/udp-client.c</source>
      <commands>$(MAKE) -j$(CPUS) udp-client.cooja TARGET=cooja</commands>
      <moteinterface>org.contikios.cooja.interfaces.Position</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.Battery</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiVib</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiMoteID</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiRS232</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiBeeper</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.IPAddress</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiRadio</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiButton</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiPIR</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiClock</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiLED</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiCFS</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiEEPROM</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.Mote2MoteRelations</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.MoteAttributes</moteinterface>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="-34.40215508052279" y="40.62655849970811" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.contikimote.interfaces.ContikiMoteID
          <id>2</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="-11.117547042046995" y="117.53748680541834" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.contikimote.interfaces.ContikiMoteID
          <id>3</id>
        </interface_config>
        <interface_config>
          org.contikios.cooja.contikimote.interfaces.ContikiRS232
          <history>ping ::204:4:4:4~;ping~;</history>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="-66.33997266007862" y="106.28486690457605" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.contikimote.interfaces.ContikiMoteID
          <id>5</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="-79.90447230992943" y="81.15135763844924" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.contikimote.interfaces.ContikiMoteID
          <id>6</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="22.618661921210503" y="104.55368115655737" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.contikimote.interfaces.ContikiMoteID
          <id>7</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="-7.238932890613928" y="79.59814034752499" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.contikimote.interfaces.ContikiMoteID
          <id>9</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="-83.44235851661398" y="145.2224405066289" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.contikimote.interfaces.ContikiMoteID
          <id>10</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="-8.648023710448024" y="157.21147456747752" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.contikimote.interfaces.ContikiMoteID
          <id>11</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="6.606060866516643" y="51.602261622253884" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.contikimote.interfaces.ContikiMoteID
          <id>12</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="-43.26817738289672" y="86.0589166732774" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.contikimote.interfaces.ContikiMoteID
          <id>13</id>
        </interface_config>
      </mote>
    </motetype>
    <motetype>
      org.contikios.cooja.contikimote.ContikiMoteType
      <description>Server Mote</description>
      <source>[CONFIG_DIR]/../prevent-dec-rank/udp-server.c</source>
      <commands>$(MAKE) -j$(CPUS) udp-server.cooja TARGET=cooja</commands>
      <moteinterface>org.contikios.cooja.interfaces.Position</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.Battery</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiVib</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiMoteID</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiRS232</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiBeeper</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.IPAddress</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiRadio</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiButton</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiPIR</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiClock</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiLED</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiCFS</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiEEPROM</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.Mote2MoteRelations</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.MoteAttributes</moteinterface>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="-41.083248633863086" y="111.74247231619185" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.contikimote.interfaces.ContikiMoteID
          <id>4</id>
        </interface_config>
      </mote>
    </motetype>
    <motetype>
      org.contikios.cooja.contikimote.ContikiMoteType
      <description>Rank Decreasing attacker</description>
      <source>[CONFIG_DIR]/../rank-attack/decreased-rank-attacker-const.c</source>
      <commands>$(MAKE) -j$(CPUS) decreased-rank-attacker-const.cooja TARGET=cooja DEFINES=RPL_WITH_HOP_COUNT=1</commands>
      <moteinterface>org.contikios.cooja.interfaces.Position</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.Battery</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiVib</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiMoteID</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiRS232</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiBeeper</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.IPAddress</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiRadio</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiButton</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiPIR</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiClock</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiLED</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiCFS</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiEEPROM</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.Mote2MoteRelations</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.MoteAttributes</moteinterface>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="-50.23635474485562" y="165.81448703242452" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.contikimote.interfaces.ContikiMoteID
          <id>8</id>
        </interface_config>
      </mote>
    </motetype>
  </simulation>
  <plugin>
    org.contikios.cooja.plugins.LogListener
    <plugin_config>
      <filter>invalid</filter>
      <formatted_time />
      <coloring />
    </plugin_config>
    <bounds x="-3" y="123" height="227" width="1720" z="4" />
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.TimeLine
    <plugin_config>
      <mote>0</mote>
      <mote>1</mote>
      <mote>2</mote>
      <mote>3</mote>
      <mote>4</mote>
      <mote>5</mote>
      <mote>6</mote>
      <mote>7</mote>
      <mote>8</mote>
      <mote>9</mote>
      <mote>10</mote>
      <mote>11</mote>
      <mote>12</mote>
      <showRadioRXTX />
      <showRadioHW />
      <showLEDs />
      <zoomfactor>500.0</zoomfactor>
    </plugin_config>
    <bounds x="8" y="736" height="166" width="1720" z="6" />
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.Notes
    <plugin_config>
      <notes>Enter notes here</notes>
      <decorations>true</decorations>
    </plugin_config>
    <bounds x="872" y="0" height="160" width="848" z="7" />
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.RadioLogger
    <plugin_config>
      <split>150</split>
      <formatted_time />
      <analyzers name="6lowpan-pcap" />
    </plugin_config>
    <bounds x="875" y="398" height="402" width="844" z="8" />
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.ScriptRunner
    <plugin_config>
      <script>// Initialize counters
var client_requests_sent = 0;
var server_requests_received = 0;
var server_responses_sent = 0;
var client_responses_received = 0;
var warmup_done_msg_logged = false;

// Time Constants (in microseconds)
var WARMUP_PERIOD = 120 * 1000000;    // Ignore the first 60 seconds
var REPORT_INTERVAL = 60 * 1000000;  // Report every 60 seconds after warmup
var next_report = WARMUP_PERIOD + REPORT_INTERVAL;

TIMEOUT(1800000); // 30-minute timeout

log.log("Simulation started. Waiting for " + (WARMUP_PERIOD / 1000000) + "s warmup...\n");

while (true) {
  YIELD();

  // ONLY start counting after the warmup period
  if (time &gt;= WARMUP_PERIOD) {
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
    if (time &gt;= next_report) {
      var upstream_pdr = (client_requests_sent &gt; 0) ? 
          (server_requests_received / client_requests_sent) * 100 : 0;
      
      var downstream_pdr = (server_responses_sent &gt; 0) ? 
          (client_responses_received / server_responses_sent) * 100 : 0;

      log.log("--- PDR Report (Post-Warmup) at " + (time / 1000000).toFixed(2) + "s ---\n");
      log.log("Upstream (Client -&gt; Server):\n");
      log.log("  Sent: " + client_requests_sent + " | Received: " + server_requests_received + "\n");
      log.log("  PDR: " + upstream_pdr.toFixed(2) + "%\n");
      
      log.log("Downstream (Server -&gt; Client):\n");
      log.log("  Sent: " + server_responses_sent + " | Received: " + client_responses_received + "\n");
      log.log("  PDR: " + downstream_pdr.toFixed(2) + "%\n");
      log.log("--------------------------------------\n");

      next_report += REPORT_INTERVAL;
    }
  }
}</script>
    </plugin_config>
    <bounds x="234" y="124" height="700" width="600" z="2" />
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.ScriptRunner
    <plugin_config>
      <script>// Import Java classes for File IO
var FileWriter = Java.type("java.io.FileWriter");
var BufferedWriter = Java.type("java.io.BufferedWriter");

var fileName = "energy_results_decrank_prevention.csv";
var fileWriter = new FileWriter(fileName);
var bufferedWriter = new BufferedWriter(fileWriter);

// Write CSV Header
bufferedWriter.write("Time;MoteID;State;Value;TotalTicks;Permil\n");
bufferedWriter.flush();

var WARMUP_PERIOD = 120 * 1000000; // 120-second warmup to let the network stabilize

TIMEOUT(1800000); // 30-minute timeout

while (true) {
    YIELD();

    if (time &gt;= WARMUP_PERIOD) {

      // Check if the message contains Energest info
      if (msg.contains("INFO: Energest") &amp;&amp; msg.contains(":")) {
          
          var parts = msg.split(":");
          
          if (parts.length &gt;= 3) {
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
}</script>
    </plugin_config>
    <bounds x="285" y="333" height="700" width="600" z="3" />
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.ScriptRunner
    <plugin_config>
      <script>// Initialize counters
var dio_count = 0;
var dis_count = 0;
var dao_count = 0;
var data_sent = 0;

// Time Constants (in microseconds)
var WARMUP_PERIOD = 120 * 1000000;    
var REPORT_INTERVAL = 60 * 1000000;  
var next_report = WARMUP_PERIOD + REPORT_INTERVAL;

TIMEOUT(1800000); // 30-minute timeout

log.log("Monitoring Overhead &amp; Data. Warmup: 60s. End time: 1800s.\n");

while (true) {
  YIELD();

  // ONLY measure after the 60s warmup
  if (time &gt;= WARMUP_PERIOD) {
    
    // 1. Detect Control Messages (Using your specific log strings)
    // We count 'sending' to measure the overhead generated BY the nodes
    if (msg.contains("sending") &amp;&amp; msg.contains("-DIO")) {
      dio_count++;
    } else if (msg.contains("sending a DIS")) {
      dis_count++;
    } else if (msg.contains("sending a DAO")) {
      dao_count++;
    }

    // 2. Detect Data Traffic (To calculate Overhead Ratio)
    if (msg.contains("Sending request")) {
      data_sent++;
    }

    // 3. Periodic Reporting (Every 60 seconds)
    if (time &gt;= next_report) {
      var total_control = dio_count + dis_count + dao_count;
      var overhead_ratio = (data_sent &gt; 0) ? (total_control / data_sent) : total_control;

      log.log("--- Report at " + (time / 1000000).toFixed(0) + "s ---\n");
      log.log("Control Messages (Sent):\n");
      log.log("  DIO: " + dio_count + " | DIS: " + dis_count + " | DAO: " + dao_count + "\n");
      log.log("  Total Overhead: " + total_control + " packets\n");
      log.log("  Data Sent: " + data_sent + " packets\n");
      
      // If data_sent is 0, we report total control as the absolute overhead
      if (data_sent &gt; 0) {
        log.log("  Overhead Ratio: " + overhead_ratio.toFixed(2) + " control/data\n");
      }
      log.log("--------------------------------------\n");

      next_report += REPORT_INTERVAL;
    }
  }
}</script>
    </plugin_config>
    <bounds x="1203" y="100" height="700" width="600" z="1" />
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.ScriptRunner
    <plugin_config>
      <script>// Cooja script: count MRHOF parent rejections, TP/FP for address fd00::208:8:8:8
var TARGET_ADDR = "fe80::208:8:8:8";
var END_TIME = 29 * 60 * 1000000 + 59 * 1000000; // 5 minutes in microseconds (Cooja time)

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
  if(token &amp;&amp; token.indexOf(":") !== -1 &amp;&amp; /^[0-9a-f:]+$/i.test(token)) {
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

  // Only process logs from motes 10 and 11
  var mid = id;
  if (mid !== 10 &amp;&amp; mid !== 11) {
    continue;
  }

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
      lastMoteId = mid;
      lastMarkerTime = time;
    }
  } else if (lastMoteId &gt;= 0 &amp;&amp; mid === lastMoteId &amp;&amp; (time - lastMarkerTime) &lt; 100000) {
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
  if (time &gt;= END_TIME) {
    var total = totalRejections;
    var tpRate = total &gt; 0 ? (tp / total) : 0;
    var fpRate = total &gt; 0 ? (fp / total) : 0;

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
}</script>
      <active>true</active>
    </plugin_config>
    <bounds x="1051" y="200" height="700" width="600" />
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.Visualizer
    <plugin_config>
      <moterelations>true</moterelations>
      <skin>org.contikios.cooja.plugins.skins.IDVisualizerSkin</skin>
      <skin>org.contikios.cooja.plugins.skins.GridVisualizerSkin</skin>
      <skin>org.contikios.cooja.plugins.skins.TrafficVisualizerSkin</skin>
      <skin>org.contikios.cooja.plugins.skins.UDGMVisualizerSkin</skin>
      <viewport>3.1129530672288555 0.0 0.0 3.1129530672288555 465.568464023571 -17.613625089474198</viewport>
    </plugin_config>
    <bounds x="1" y="1" height="681" width="745" z="5" />
  </plugin>
</simconf>
