

.section .text

.globl DebugLED_ON
DebugLED_ON:

stmfd sp!,{r0-r2, lr}

ldr r0,=0x20200004
ldr r1,=0x20200028

mov r2,#1
lsl r2,#18
str r2,[r0]

mov r2,#1
lsl r2,#16

str r2,[r1]

ldmfd sp!,{r0-r2, pc}

.globl DebugLED_OFF
DebugLED_OFF:

stmfd sp!,{r0-r1, lr}

ldr r0,=0x2020001C

mov r1,#1
lsl r1,#16

str r1,[r0]

ldmfd sp!,{r0-r1, pc}

.globl DebugLED_ERROR
DebugLED_ERROR:

ldr r0,=0x20200004
ldr r2,=0x20200028
ldr r4,=0x2020001C

mov r1,#1
lsl r1,#18
str r1,[r0]

mov r1,#1
lsl r1,#16

loop:

str r1,[r2]

mov r3,#0xF0000
wait1:
sub r3,r3,#1
cmp r3,#0
bne wait1

str r1,[r4]

mov r3,#0xF0000
wait2:
sub r3,r3,#1
cmp r3,#0
bne wait2

b loop
