.include "hdr.asm"

.section ".rodata1" superfree

patternsbg1:
.incbin "pvsneslibbg1.pic"
patternsbg1_end:

mapbg1:
.incbin "pvsneslibbg1.map"
mapbg1_end:

palettebg1:
.incbin "pvsneslibbg1.pal"
palettebg1_end:

patternsbg2:
.incbin "pvsneslibbg2.pic"
patternsbg2_end:

mapbg2:
.incbin "pvsneslibbg2.map"
mapbg2_end:

palettebg2:
.incbin "pvsneslibbg2.pal"
palettebg2_end:

.ends



.section ".rosprite" superfree

gfxpsrite:
.incbin "sprites.pic"
gfxpsrite_end:

palsprite:
.incbin "sprites.pal"
palsprite_end:

.ends



.SECTION ".dmas16_text" SUPERFREE

.EQU REG_TM                $212C
.EQU REG_TS                $212D
.EQU REG_W12SEL            $2123
.EQU REG_W34SEL            $2124
.EQU REG_WOBJSEL           $2125
.EQU REG_WH0               $2126
.EQU REG_WH1               $2127
.EQU REG_WBGLOG            $212A
.EQU REG_WOBJLOG           $212B
.EQU REG_TMW               $212E

.EQU REG_HDMAEN             $420C

; TODO Separate routine to setup windows
; TODO routine to switch hdma table

; void setModeHdmaWindow1And2SingleHdma(u8 bgrnd, u8 bgrndmask,u8* hdmatable)
; FIXME Hard-coded BG (0x3) / MASK (0xc3) ?!
; 8 9 10-13
setModeHdmaWindow1And2SingleHdma:
    php
    phb
    phx

    sep #$20
    lda #0
    pha
    plb

    lda 8,s                                                   ; got all the flags to active windows on BG1..4
    ora #$10                                                  ; also add obj in window effect
    sta REG_TMW                                               ; active or not window

    lda 8,s
    and #$0C                                                  ; if effect on BG3 or BG4, not same register
    bne +
    lda 9,s                                                   ; got all the flags to mask effect (inside, outside on BG1..2)
    sta REG_W12SEL
    bra ++
+:
    lda 9,s                                                   ; got all the flags to mask effect (inside, outside on BG3..4)
    sta REG_W34SEL

++: lda 9,s                                                   ; todo : find a way to manage easily objects -> currently, it works only for BG1
    ;FIXME sta REG_WOBJSEL


    ; FIXME Single Mode 4 HDMA

    lda #$04                                                  ; 4 registers, write once
    sta $4320
    lda #$26                                                  ; 2126  Window 1 Left Position (X1) to 2129 Window 2 Right Position (X2)
    sta $4321                                                 ; destination
    lda 12,s                                                  ; bank address of table
    sta $4324

    rep #$20
    lda 10,s                                                  ; low address of table
    sta $4322

    sep #$20
    lda #$04                                                  ; channel 2 transfers all 4 window registers
    sta.l   REG_HDMAEN

    plx
    plb
    plp
    rtl



.ENDS