<?xml version="1.0" encoding="UTF-8"?>
<simconf version="2023090101">
  <simulation>
    <title>rpl-decreased-rank-attack</title>
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
      <source>[CONFIG_DIR]/../normal-nodes/udp-root.c</source>
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
      <source>[CONFIG_DIR]/../normal-nodes/udp-client.c</source>
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
          <pos x="21.66612973808449" y="114.45234596178979" />
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
          <pos x="-71.77260247484031" y="140.50357150033068" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.contikimote.interfaces.ContikiMoteID
          <id>10</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="-17.36690936172823" y="152.30581026943787" />
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
      <source>[CONFIG_DIR]/../normal-nodes/udp-server.c</source>
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
          <pos x="-25.76851080617147" y="99.87355049973083" />
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
      <commands>$(MAKE) -j$(CPUS) decreased-rank-attacker-const.cooja TARGET=cooja DEFINES=RPL_WITH_HOP_COUNT=0</commands>
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
          <pos x="-48.32201251639416" y="161.22006568411703" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.contikimote.interfaces.ContikiMoteID
          <id>8</id>
        </interface_config>
      </mote>
    </motetype>
  </simulation>
  <plugin>
    org.contikios.cooja.plugins.Visualizer
    <plugin_config>
      <moterelations>true</moterelations>
      <skin>org.contikios.cooja.plugins.skins.IDVisualizerSkin</skin>
      <skin>org.contikios.cooja.plugins.skins.GridVisualizerSkin</skin>
      <skin>org.contikios.cooja.plugins.skins.TrafficVisualizerSkin</skin>
      <skin>org.contikios.cooja.plugins.skins.UDGMVisualizerSkin</skin>
      <viewport>2.6118631902188545 0.0 0.0 2.6118631902188545 616.8185076493809 -20.14698853057784</viewport>
    </plugin_config>
    <bounds x="1" y="1" height="800" width="873" z="3" />
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.LogListener
    <plugin_config>
      <filter>ID:8</filter>
      <formatted_time />
      <coloring />
    </plugin_config>
    <bounds x="1" y="160" height="240" width="1720" z="10" />
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
    <bounds x="0" y="795" height="166" width="1720" z="5" />
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.Notes
    <plugin_config>
      <notes>Enter notes here</notes>
      <decorations>true</decorations>
    </plugin_config>
    <bounds x="872" y="0" height="160" width="848" z="9" />
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.LogListener
    <plugin_config>
      <filter>INFO: Energest</filter>
      <formatted_time />
      <coloring />
    </plugin_config>
    <bounds x="1" y="160" height="240" width="1720" z="6" />
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
    <bounds x="0" y="795" height="166" width="1720" z="7" />
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.Notes
    <plugin_config>
      <notes>Enter notes here</notes>
      <decorations>true</decorations>
    </plugin_config>
    <bounds x="872" y="0" height="160" width="848" z="8" />
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.RadioLogger
    <plugin_config>
      <split>150</split>
      <formatted_time />
      <analyzers name="6lowpan" />
    </plugin_config>
    <bounds x="875" y="398" height="402" width="844" z="4" />
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
var WARMUP_PERIOD = 60 * 1000000;    // Ignore the first 60 seconds
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
      <active>true</active>
    </plugin_config>
    <bounds x="978" y="4" height="700" width="600" z="1" />
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.ScriptRunner
    <plugin_config>
      <script>// Import Java classes for File IO
var FileWriter = Java.type("java.io.FileWriter");
var BufferedWriter = Java.type("java.io.BufferedWriter");

var fileName = "energy_results_decrank_attack.csv";
var fileWriter = new FileWriter(fileName);
var bufferedWriter = new BufferedWriter(fileWriter);

// Write CSV Header
bufferedWriter.write("Time;MoteID;State;Value;TotalTicks;Permil\n");
bufferedWriter.flush();

TIMEOUT(1800000); // 30-minute timeout

while (true) {
    YIELD();

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
}</script>
      <active>true</active>
    </plugin_config>
    <bounds x="1072" y="188" height="700" width="600" />
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
var WARMUP_PERIOD = 60 * 1000000;    
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
      <active>true</active>
    </plugin_config>
    <bounds x="781" y="254" height="700" width="600" z="2" />
  </plugin>
</simconf>
