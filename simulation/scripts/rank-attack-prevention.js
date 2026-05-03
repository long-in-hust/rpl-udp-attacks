moteToDisable = sim.getMoteWithID(8);

// --- NGẮT KẾT NỐI ---
moteToDisable.getInterfaces().getRadio().removed();
log.log("Mote 8 has been disabled (OFF)\n");

// Chờ 60 giây (60000ms)
GENERATE_MSG(60000, "resume");
YIELD_THEN_WAIT_UNTIL(msg.equals("resume"));

// --- KÍCH HOẠT LẠI ---
moteToDisable.getInterfaces().getRadio().added();
log.log("Mote 8 is activated (ON)\n");