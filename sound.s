.section .data

// use .align 4

// base info from the spec:
// base address for peripherals is 0x20000000
/* #DEFINE BASE_IO_ADDRESS 0x20000000 */
// we have 16 DMA channels (they operate independently)
// we operate DMA like this:
// 1) write address of CB into CONBLK_AD
// 2) set the ACTIVE bit
// 3) CB can point into next CB which is executed after first one was

// DMA channel #0 is located at 0x20007000
/* #DEFINE DMA_CHANNEL0_LOC 0x20007000 */
// offset between each channel location is 0x100
/* #DEFINE DMA_CHANNEL_NEXT (last) + 0x100 */

// CB data structure:
// -------------------
// .word TransferInformation
// .word SourceAddress
// .word DestinationAddress
// .word TransferLength
// .word 2DModeStride
// .word NextCB
// .word 0x00000000
// .word 0x00000000

// channel offsets:
// 0x00000000 Control and Status
// 0x00000004 CB address
// 0x00000020 debug

// control and status:
// 0x80000000 - write this to RESET
// 0x40000000 - write this to ABORT (stil tries to load next CB though)
// 0x00000100 - read this to check for ERROR
// 0x00000010 - read this to check for PAUSED
// 0x00000002 - read this to check for TRANSFER END, write to clear this status
// 0x00000001 - write here to START TRANSFER (automatically cleared after tr end)

// Control Block address:
// #0 - 256bit aligned CB address
// --- CB struct ---
// #4 - Transfer info:
//      0x00000200 - sets SRC_WIDTH (source address .align 2 (128bits))
//      0x00000100 - sets SRC_INC (source address increments by width each read)
//      0x00000020 - sets DEST_WIDTH (not set would make dest width 1 byte write)
//      0x00000010 - sets DEST_INC
//      0x00000002 - sets 2D_MODE (YLENGTH transfers of XLENGTH) otherwise linear
// #8 - Source address
// #12 - Destination address
// #16 - Transfer length - XLENGTH , YLENGTH << 16
// #20 - 2D stride - DEST_STRIDE (in BYTES) << 16, SOURCE_STRIDE
// #24 - Next conblock
// --- CB struct ---

// enable register offset 0x00000FF0

.section .text

