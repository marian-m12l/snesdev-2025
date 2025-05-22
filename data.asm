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
; TODO Single HDMA to transfer L/R position at once ?
; TODO Single HDMA to transfer L/R positions at once FOR BOTH WINDOWS ?

; void setModeHdmaWindow2(u8 bgrnd, u8 bgrndmask,u8* hdmatableL,u8* hdmatableR)
; 8 9 10-13 14-17
; FIXME 8 10 12-15 16-19 ???
setModeHdmaWindow2:
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

; FIXME only BG1/2 ?

    lda 8,s
    and #$0C                                                  ; if effect on BG3 or BG4, not same register
    bne +
    lda 10,s                                                   ; got all the flags to mask effect (inside, outside on BG1..2)
    sta REG_W12SEL
    bra ++
+:
    lda 10,s                                                   ; got all the flags to mask effect (inside, outside on BG3..4)
    sta REG_W34SEL

++: lda 10,s                                                   ; todo : find a way to manage easily objects -> currently, it works only for BG1
    ;sta REG_WOBJSEL

; FIXME Use a single 2-registers HDMA for L/R window position ?

; HDMA 2/3

    stz $4320                                                 ; 1 register, write once
    lda #$28                                                  ; 2128  Window 2 Left Position (X1)
    sta $4321                                                 ; destination
    lda 14,s                                                  ; bank address of left  table
    sta $4324

    stz $4330                                                 ; 1 register, write once
    lda #$29                                                  ; 2129 Window 2 Right Position (X2)
    sta $4331
    lda 18,s                                                  ; bank address of right table
    sta $4334

    rep #$20
    lda 12,s                                                  ; low address of left table
    sta $4322                                                 ; low address of right table
    lda 16,s
    sta $4332

    sep #$20
    lda #$0c                                                  ; channel 2 & 3       00001100
    sta.l   REG_HDMAEN

    plx
    plb
    plp
    rtl




; void setModeHdmaWindow(u8 bgrnd, u8 bgrndmask,u8* hdmatableL,u8* hdmatableR)
; 8 9 10-13 14-17
; FIXME 8 10 12-15 16-19 ???
setModeHdmaWindow2bis:
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

    stz $4340                                                 ; 1 register, write once
    lda #$28                                                  ; 2128  Window 2 Left Position (X1)
    sta $4341                                                 ; destination
    lda 12,s                                                  ; bank address of left  table
    sta $4344

    stz $4350                                                 ; 1 register, write once
    lda #$29                                                  ; 2129 Window 2 Right Position (X2)
    sta $4351
    lda 16,s                                                  ; bank address of right table
    sta $4354

    rep #$20
    lda 10,s                                                  ; low address of left table
    sta $4342                                                 ; low address of right table
    lda 14,s
    sta $4352

    sep #$20
    lda #$30                                                  ; channel 4 & 5       00110000
    sta.l   REG_HDMAEN

    plx
    plb
    plp
    rtl

; void setModeHdmaWindow(u8 bgrnd, u8 bgrndmask,u8* hdmatableL,u8* hdmatableR)
; 8 9 10-13 14-17
; FIXME 8 10 12-15 16-19 ???
setModeHdmaWindow2ter:
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

    stz $4320                                                 ; 1 register, write once
    lda #$28                                                  ; 2128  Window 2 Left Position (X1)
    sta $4321                                                 ; destination
    lda 12,s                                                  ; bank address of left  table
    sta $4324

    stz $4330                                                 ; 1 register, write once
    lda #$29                                                  ; 2129 Window 2 Right Position (X2)
    sta $4331
    lda 16,s                                                  ; bank address of right table
    sta $4334

    rep #$20
    lda 10,s                                                  ; low address of left table
    sta $4322                                                 ; low address of right table
    lda 14,s
    sta $4332

    sep #$20
    lda #$3c                                                  ; channel 2 & 3       00001100 FIXME + 4 & 5 !!!
    sta.l   REG_HDMAEN

    plx
    plb
    plp
    rtl



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