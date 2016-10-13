.section .text

/*----------------------------------------------------------------------------*/
/*GetSystemTime()                                                             */
/*----------------------------------------------------------------------------*/
.globl GetSystemTime
GetSystemTime:

// load in the base address for the system timer
address .req r2
ldr address,=0x20003000

// load in the current 8byte tick value
lowerBytes .req r0
higherBytes .req r1
ldrd lowerBytes,higherBytes,[address,#4]

.unreq higherBytes
.unreq lowerBytes
.unreq address

// return
mov pc,lr
