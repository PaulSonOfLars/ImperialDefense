// apparently RPi uses so called mailbox to communicate with the GPU
// so we send our frame data and it waits and renders those

// when writing or reading such "mail", we transfer 32bits such as:
// 0 - 3 bits are the mailbox channel (1 is the framebuffer)
// 4 - 31 bits are the data we send

// functions explained:
// when we use function in ARM assembly, r0 up to r3 are the inputs, r0 output
// we make sure that r4 - r12 are be preserved!
// we dont care about r0- r3 being preserved
//
// there is return address register lr
//
// we call function using bl <label>
//   bl <label> sets lr to the return address (next line)
//      and then branches to the function
//
// so when calling function from function, you want to preserve lr as well
// and to return, we just do "mov pc,lr" which sets the PC to return line

.section .text

/*----------------------------------------------------------------------------*/
/*void MailboxWrite(r0 channel, r1 value)                                     */
/*----------------------------------------------------------------------------*/
.globl MailboxWrite
MailboxWrite:

stmfd sp!,{r2-r3,lr}

// set the aliases
channel .req  r0
data    .req  r1
address .req  r2

// load the base mailbox address
ldr address,=0x2000B880

// this loops until the mailbox is ready to receive data
status_loop$:
status .req r3

// get the current status which is offset 0x18 from the mailbox address
ldr status,[address,#0x18]
// check if the 31th bit is set (which means box is full)
tst status,#0x80000000

.unreq status
bne status_loop$

// combine and write the mail
output .req r3
mov output, channel
add output, data
str output,[address,#0x20]

// unset the aliases
.unreq  output
.unreq  address
.unreq  data
.unreq  channel

// return
ldmfd sp!,{r2-r3,pc}

/*----------------------------------------------------------------------------*/
/*r0 = MailboxRead(r0 channel)                                                */
/*----------------------------------------------------------------------------*/
.globl MailboxRead
MailboxRead:

stmfd sp!,{r1-r3,lr}

// set the aliases
channel .req r0
address .req r1

// load the base mailbox address
ldr address,=0x2000B880

correct_mailbox$:
status_loop2$:
status .req r2

// get the status of the mailbox
ldr status,[address,#0x18]
// check if 30th bit is set (which means MAIL_EMPTY)
tst status,#0x40000000

.unreq status
bne status_loop2$

// read in the data available
data .req r2
ldr data,[address,#0]

// check if the data is for the channel we ask for
received_channel .req r3
and received_channel,data,#0xF // get first 4bits
teq received_channel,channel // compare channels
.unreq received_channel
bne correct_mailbox$ // if not, try to get new data

.unreq address
.unreq channel

and r0,data,#0xfffffff0 // move last 28bits to result
.unreq data

// return
ldmfd sp!,{r1-r3,pc}
