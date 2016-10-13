/*----------------------------------------------------------------------------*/
/*Defines                                                                     */
/*----------------------------------------------------------------------------*/
.equ SCREEN_WIDTH,                  1024
.equ SCREEN_HEIGHT,                  768
.equ SCREEN_HEIGHT_VIRTUAL,         1536
.equ SCREEN_CLEAR_COLOUR,     0x00000000

.section .data

/*----------------------------------------------------------------------------*/
/*FrameBufferSpec structure, specifies our framebuffer needs                  */
/*----------------------------------------------------------------------------*/
.align 4
.globl FrameBufferSpec
FrameBufferSpec:
.int SCREEN_WIDTH     // Physical Width
.int SCREEN_HEIGHT  // Physical Height
.int SCREEN_WIDTH     // Virtual Width
.int SCREEN_HEIGHT_VIRTUAL // Virtual Height
.int 0                // GPU - Pitch
.int 16        // Bit Depth (8bit)
.int 0        // X
.int 0        // Y
.int 0        // GPU - Pointer
.int 0        // GPU - Size

.align 2
frameBuffer:
.word 0

.align 2
currentBufferOffset:
.word 0 // back buffer offset
.word 768 // front buffer offset

/*----------------------------------------------------------------------------*/
/*mailbox this to GPU to change the virtual offset (double buffering)         */
/*----------------------------------------------------------------------------*/
.align 4
SelectBufferSpec:
.word 0x00000030  // spec size (48 bytes)
.word 0x00000000  // request/response code
.word 0x00048009  // tag ID
.word 0x00000010  // size of value buffer
.word 0x00000000  // request/response indicator | value length
.word 0x00000000  // offset X
.word 0x00000000  // offset Y
.word 0x00000000  // response X
.word 0x00000000 //  response Y
.word 0x00000000 // end tag
.word 0x00000000 // padding
.word 0x00000000

.section .text

/*----------------------------------------------------------------------------*/
/*FrameBuffer_Init()                                                          */
/*----------------------------------------------------------------------------*/
.globl FrameBuffer_Init
FrameBuffer_Init:

// set target channel (1 is framebuffer)
fbChan .req r0
mov fbChan,#1

// load the frame buffer info structure
fbSpec .req r1
ldr fbSpec,=FrameBufferSpec

// save current return address
push {lr}

orr fbSpec, #0x40000000

push {r1}

// write
bl MailboxWrite

// read the answer
.unreq fbChan
bl MailboxRead
readStatus .req r0

pop {r1}

// result has to be 0 if the framebuffer was accepted
teq readStatus,#0
.unreq readStatus
movne r0,#0
popne {pc} // return 0 if wasnt accepted

// else return frame buffer address
mov r0,fbSpec

.unreq fbSpec
pop {pc} // return

/*----------------------------------------------------------------------------*/
/*SetFrameBuffer(r0 address)                                                  */
/*----------------------------------------------------------------------------*/
.globl SetFrameBuffer
SetFrameBuffer:
ldr r1,=frameBuffer
str r0,[r1]
mov pc,lr

/*----------------------------------------------------------------------------*/
/*ASM_ClearScreen                                                             */
/*----------------------------------------------------------------------------*/
.globl ASM_ClearScreen
ASM_ClearScreen:
stmfd sp!,{r4-r12,lr}

ldr r0,=frameBuffer // get address of the buffer
ldr r0,[r0]

ldr r1,[r0,#16]
ldr r2,=currentBufferOffset
ldr r2,[r2]

mul r3,r2,r1

ldr r0,[r0,#32]
add r0,r0,r3

ldr r2,=SCREEN_HEIGHT
ldr r3,=SCREEN_CLEAR_COLOUR
ldr r6,=SCREEN_WIDTH

mov r4,#0
mov r5,#0
loop_y$:
  cmp r4, r2
  bge loop_y_end$

  mov r5,#0
  loop_x$:
  cmp r5, r6
  bge loop_x_end$

  mul r7,r1,r4
  add r7,r7,r5,lsl #1

  str r3,[r0,r7]

  add r5,r5,#1
  b loop_x$
  loop_x_end$:

  add r4,r4,#1
  b loop_y$
loop_y_end$:

// return
ldmfd sp!,{r4-r12, pc}

/*----------------------------------------------------------------------------*/
/*ASM_DrawImage(r0 image, r1 size, r2 posX, r3 posY)                          */
/*----------------------------------------------------------------------------*/
.globl ASM_DrawImage
ASM_DrawImage:

imagePointer .req r0
imageSize .req r1
posX .req r2
posY .req r3

stmfd sp!,{r4-r11,lr}

sizeX .req r4
sizeY .req r5

ldr sizeX,[imageSize]
ldr sizeY,[imageSize,#4]

sub posX, posX, sizeX, lsr #1
sub posY, posY, sizeY, lsr #1

.unreq imageSize

ldr r1,=frameBuffer // get address of the buffer
ldr r1,[r1]
ldr r11,[r1,#16]    // get pitch
ldr r1,[r1,#32]

ldr r8,=currentBufferOffset
ldr r8,[r8]

mul r8,r8,r11
add r1,r1,r8

cur_x .req r6
cur_y .req r7

mov cur_x,#0
mov cur_y,#0

loop_y:
  cmp cur_y,sizeY
  bge loop_y_end

  mov cur_x,#0
    loop_x:
    cmp cur_x,sizeX
    bge loop_x_end

    mla r8,cur_y,sizeX,cur_x
    lsl r8,#1
    pld [imagePointer, #256]
    ldr r10,[imagePointer,r8]

    add r8,cur_x,posX
    add r9,cur_y,posY

    mul r9,r9,r11
    add r9,r9,r8,lsl #1

    str r10,[r1,r9]

    add cur_x,cur_x,#2
    b loop_x

  loop_x_end:

  add cur_y,cur_y,#1
  b loop_y
loop_y_end:

.unreq cur_x
.unreq cur_y

.unreq posY
.unreq posX
.unreq imagePointer
.unreq sizeX
.unreq sizeY

// return
ldmfd sp!,{r4-r11, pc}

/*----------------------------------------------------------------------------*/
/*ASM_DrawImageTr(r0 image, r1 size, r2 posX, r3 posY)                        */
/*----------------------------------------------------------------------------*/
.globl ASM_DrawImageTr
ASM_DrawImageTr:

imagePointer .req r0
imageSize .req r1
posX .req r2
posY .req r3

stmfd sp!,{r4-r12,lr}

sizeX .req r4
sizeY .req r5
sizeR .req r6
curF .req r7

ldr sizeR,[imageSize]
ldr sizeY,[imageSize,#4]
ldr sizeX,[imageSize,#8]
ldr curF,[imageSize,#12]

sub posX, posX, sizeX, lsr #1
sub posY, posY, sizeY, lsr #1

.unreq imageSize

ldr r1,=frameBuffer // get address of the buffer
ldr r1,[r1]
ldr r12,[r1,#16]    // get pitch
ldr r1,[r1,#32]

ldr r8,=currentBufferOffset
ldr r8,[r8]

mul r8,r8,r12
add r1,r1,r8

cur_x .req r8
cur_y .req r9
start_x .req r10

mul start_x, sizeX, curF

mov cur_x,#0
mov cur_y,#0

loop_yTR:
  cmp cur_y,sizeY
  bge loop_y_endTR

  mov cur_x,#0
    loop_xTR:
    cmp cur_x,sizeX
    bge loop_x_endTR

    stmfd sp!,{r4-r6,r10}
    mul r11,cur_y,sizeR
    add r11,r11,cur_x
    add r11,r11,start_x
    lsl r11,#1
    pld [imagePointer, #32]
    ldrh r10,[imagePointer,r11]

    ldr r4,=0x1F
    cmp r4,r10
    beq skip_pixel$

    mov r4,cur_x
    add r4,r4,posX
    mov r5,cur_y
    add r5,r5,posY

    mul r6,r5,r12
    add r6,r6,r4,lsl #1

    strh r10,[r1,r6]

skip_pixel$:
    ldmfd sp!,{r4-r6,r10}
    add cur_x,cur_x,#1
    b loop_xTR

  loop_x_endTR:

  add cur_y,cur_y,#1
  b loop_yTR
loop_y_endTR:

.unreq cur_x
.unreq cur_y
.unreq start_x
.unreq curF
.unreq sizeR

.unreq posY
.unreq posX
.unreq imagePointer
.unreq sizeX


// return
ldmfd sp!,{r4-r12, pc}

/*----------------------------------------------------------------------------*/
/*ASM_SwapBuffers(void)                                                       */
/*----------------------------------------------------------------------------*/
.globl ASM_SwapBuffers
ASM_SwapBuffers:

stmfd sp!,{lr}

// swap logically
ldr r0,=currentBufferOffset
ldr r0,[r0]
cmp r0,#0
beq change_to_768$
  mov r1,#0  // we filled backbuffer at offset 768
  mov r2,#768 // so we want to render it
  b changed$
change_to_768$:
  mov r1,#768 // we filled backbuffer at offset 0
  mov r2,#0   // so we want to render it
changed$:

ldr r0,=currentBufferOffset
str r1,[r0]
str r2,[r0,#4]

ldr r0,=SelectBufferSpec
str r2,[r0,#24] // set offset
mov r1,#0
str r1,[r0,#4]  // clear response bits
str r1,[r0,#16]

ldr r1,=SelectBufferSpec
mov r0,#8
bl MailboxWrite
bl MailboxRead

// return
ldmfd sp!,{pc}
