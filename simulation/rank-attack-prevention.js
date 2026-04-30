moteToDisable = sim.getMoteWithID(8);

// --- NGẮT KẾT NỐI ---
moteToDisable.getInterfaces().getRadio().removed();
log.log("Mote 8 has been disabled (OFF)\n");

// Chờ 40 giây (40000ms)
GENERATE_MSG(40000, "resume");
YIELD_THEN_WAIT_UNTIL(msg.equals("resume"));

// --- KÍCH HOẠT LẠI ---
moteToDisable.getInterfaces().getRadio().added();
log.log("Mote 8 is activated (ON)\n");