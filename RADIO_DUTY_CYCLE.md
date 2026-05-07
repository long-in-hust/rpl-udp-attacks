# Radio Duty Cycle for Cooja Platform

## Overview
The Cooja platform now supports a configurable radio duty cycle that makes energy simulations more realistic. Instead of the radio being on the entire simulation, it now:
1. Starts **OFF**
2. Periodically turns **ON** for a listening window
3. Turns **OFF** to sleep
4. Repeats the cycle

This mimics real wireless devices that use low-power listening or duty cycling to save energy.

## Default Behavior
- **Listening window (ON)**: 100 ms (CLOCK_SECOND / 10)
- **Sleep window (OFF)**: 900 ms (9 * CLOCK_SECOND / 10)
- **Duty cycle**: 10% (100ms on, 900ms off)

## Customization

To change the duty cycle parameters, add these macros to your `project-conf.h`:

```c
/* Disable duty cycle entirely (radio always on - old behavior) */
#define COOJA_RADIO_DUTY_CYCLE_ENABLED 0

/* Change listening window to 50ms */
#define COOJA_RADIO_ON_TIME (CLOCK_SECOND / 20)

/* Change sleep window to 450ms (for 10% duty cycle with 50ms window) */
#define COOJA_RADIO_OFF_TIME (CLOCK_SECOND / 2)

/* Example: 50% duty cycle (500ms on, 500ms off) */
#define COOJA_RADIO_ON_TIME  (CLOCK_SECOND / 2)
#define COOJA_RADIO_OFF_TIME (CLOCK_SECOND / 2)
```

## Energy Impact

- **Total simulation time**: unchanged (CPU + LPM + DEEP_LPM)
- **LISTEN time**: now ~10% of total (matching ON time)
- **CPU/LPM balance**: more realistic, less radio overhead
- **TX**: still can happen instantly (radio turns on temporarily if needed)

## Energest Data Interpretation

In your CSV energy logs:
- **Radio Rx (LISTEN)** will now show non-zero only during the ON windows
- **Radio Tx** can still occur anytime (radio_send() turns radio on temporarily)
- **Radio total** = Radio Tx + Radio Rx (as before)
- **CPU percentage** may appear higher because nodes spend less time in LISTEN, more in LPM

## Example Configuration

For a realistic duty cycle (like real low-power devices), add to `project-conf.h`:

```c
/* 10% duty cycle: 100ms listen, 900ms sleep */
#define COOJA_RADIO_DUTY_CYCLE_ENABLED 1
#define COOJA_RADIO_ON_TIME  (CLOCK_SECOND / 10)   /* 100ms */
#define COOJA_RADIO_OFF_TIME (CLOCK_SECOND * 9 / 10) /* 900ms */
```

## Disabling Duty Cycle

If you want the old behavior (radio always on) for comparison:

```c
#define COOJA_RADIO_DUTY_CYCLE_ENABLED 0
```

Then manually turn the radio on in your code or use `NETSTACK_RADIO.on()` at startup.

## Comparison with Real Devices

- **CC2420 radio** (Tmote Sky): ~20mA TX, ~20mA RX, ~1μA sleep
  - With 10% duty cycle: ~80% energy savings vs always-on
- **Cooja simulation**: Now reflects this pattern in LISTEN time
- **Note**: Cooja doesn't model actual current draw, only time spent in each state
