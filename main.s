.section .text

// main function
.globl main
main:

// Start L1 Cache
//mrc p15,0,r0,c1,c0,0
//orr r0, #0x4
//orr r0, #0x800
//orr r0, #0x1000
//mcr p15,0,r0,c1,c0,0

// Enable Vector Floating Point Calculations
mrc p15, #0, r0, c1, c0, #2
orr r0, r0, #0xF00000
mcr p15, #0, r0, c1, c0, #2
mov r0, #0x40000000
fmxr fpexc, r0

bl game_loop

exit_loop$:
  bl DebugLED_ON
  b exit_loop$
