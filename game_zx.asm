;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; IM2 INTERRUPT SERVICE ROUTINE ;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION code_crt_common

PUBLIC _zx_isr

EXTERN _spec128, _GLOBAL_ZX_PORT_7FFD

EXTERN _ay_song_bank, _ay_song_command, _ay_song_command_param
EXTERN vtii_setup, vtii_stop, vtii_init, vtii_play

EXTERN _ay_fx_bank, _ay_fx_command, _ay_fx_command_param
EXTERN mfx_mfxptr, mfx_add, mfx_playm

_zx_isr:

   ; check for 128k model

   ld a,(_spec128)
   or a
   ret z

   ; avoid contention problems
   
   ld a,0x80
   ld i,a
   
   ; process song command

song_command:

   ld a,(_ay_song_bank)
   or $10
   
   ld bc,$7ffd
   out (c),a                   ; page in song bank
   
   ld hl,vtii_setup
   set 1,(hl)                  ; enable stack output from ay song player

   ld a,(_ay_song_command)
   
   bit 1,a
   jr z, check_song_new

song_stop:

   call vtii_stop
   jr song_exit

check_song_new:

   bit 2,a
   jr z, song_play

song_new:

   and $01                     ; keep loop bit
   or $02                      ; enable stack output from ay song player
   ld (vtii_setup),a
   
   ld hl,(_ay_song_command_param)
   
   call vtii_init
   jr song_exit

song_play:

   call vtii_play

song_exit:

   xor a
   ld (_ay_song_command),a

   ; process fx command

fx_command:

   ld a,(_ay_fx_bank)
   or $10
   
   ld bc,$7ffd
   out (c),a                   ; page in fx bank

   ld a,(_ay_fx_command)
   
   bit 1,a
   jr z, check_fx_new

fx_stop:

   ld hl,0
   ld (mfx_mfxptr+1),hl
   
   jr fx_play

check_fx_new:

   bit 2,a
   jr z, fx_play
   
fx_new:

   ld hl,(_ay_fx_command_param)
   call mfx_add
   
fx_play:

   call mfx_playm

fx_exit:

   xor a
   ld (_ay_fx_command),a
   
   ; restore original bank
   
   ld a,(_GLOBAL_ZX_PORT_7FFD)
   
   ld bc,$7ffd
   out (c),a
   
   ; restore I
   
   ld a,0xfe
   ld i,a

   ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; PRINTING UTILITIES ;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION code_user

; void zx_print_str(unsigned char ui_row, unsigned char ui_col, unsigned char *s)
; callee linkage

PUBLIC _zx_print_str

EXTERN asm_zx_cxy2saddr, asm_zx_saddr2aaddr
EXTERN _game_font, _screen_ink, _screen_paper

_zx_print_str:

IF __SDCC

   pop af
   pop hl                      ; l = row, h = col
   pop de                      ; de = s
   push af

   ld a,l
   ld l,h
   ld h,a

ENDIF

IF __SCCZ80

   pop hl
   pop de                      ; de = s
   pop bc                      ; c = col
   ex (sp),hl                  ; l = row

   ld h,l
   ld l,c

ENDIF

zx_print_str:

   ;  h = y coord
   ;  l = x coord
   ; de = char *s

   call asm_zx_cxy2saddr       ; z88dk function: character y,x to screen address
   ex de,hl

zps_sloop:

   ; de = char *screen
   ; hl = char *s

   ld a,(hl)

   or a
   ret z

   push de
   push hl

   ld l,a
   ld h,0

   ; hl = char c
   ; de = char *screen
   ; stack = char *screen, char *s

   add hl,hl
   add hl,hl
   add hl,hl
   ld bc, _game_font - 256
   add hl,bc

   ld b,8

zps_cloop:

   ld a,(hl)
   ld (de),a
   inc hl
   inc d

   djnz zps_cloop

   dec d
   ex de,hl

   call asm_zx_saddr2aaddr     ; z88dk function: screen address to attribute address

   ld a,(_screen_ink)
   ld b,a
   ld a,(_screen_paper)
   or b

   ld (hl),a

   pop hl
   pop de

   inc e
   inc hl

   jr zps_sloop


; void zx_print_int(unsigned char ui_row, unsigned char ui_col, unsigned int val)
; callee linkage

PUBLIC _zx_print_int

EXTERN l_small_utoa, _tbuffer

_zx_print_int:

IF __SDCC

   pop af
   pop de                      ; e = row, d = col
   pop hl                      ; hl = val
   push af

   ld a,e
   ld e,d
   ld d,a

ENDIF

IF __SCCZ80

   pop af
   pop hl                      ; hl = val
   pop de                      ; e = col
   pop bc                      ; c = row
   push af

   ld d,c

ENDIF

zx_print_int:

   ;  e = col
   ;  d = row
   ; hl = val

   push de
   ld de,_tbuffer

   ; hl = unsigned int val
   ; de = char *buffer
   ; stack = row/col

   scf                         ; request leading zeroes
   call l_small_utoa           ; z88dk function: unsigned int to ascii buffer

   ; ld a,'0'

   ; ld (de),a                   ; add trailing zero to multiply scores by 10
   ; inc de

   xor a
   ld (de),a                   ; zero terminate

   pop hl
   ld de,_tbuffer

   ; de = char *buffer
   ;  h = row
   ;  l = col

   jp zx_print_str


; void zx_print_chr(unsigned char ui_row, unsigned char ui_col, unsigned char val)
; callee linkage

PUBLIC _zx_print_chr

EXTERN l_small_utoa, _tbuffer

_zx_print_chr:

IF __SDCC

   pop hl
   dec sp
   pop de                      ; d = row
   ex (sp),hl                  ; l = col, h = val

   ld e,l
   ld l,h
   ld h,0

ENDIF

IF __SCCZ80

   pop af
   pop hl                      ; hl = val
   pop de                      ; e = col
   pop bc                      ; c = row
   push af

   ld d,c

ENDIF

zx_print_chr:

   ;  e = col
   ;  d = row
   ; hl = val

   push de
   ld de,_tbuffer

   ; hl = unsigned int val
   ; de = char *buffer
   ; stack = row/col

   scf                         ; request leading zeroes
   call l_small_utoa           ; z88dk function: unsigned int to ascii buffer

   xor a
   ld (de),a                   ; zero terminate

   pop hl
   ld de,_tbuffer+2            ; print last three digits only

   ; de = char *buffer
   ;  h = row
   ;  l = col

   jp zx_print_str


; void zx_print_bonus_time(unsigned char ui_row, unsigned char ui_col, unsigned int time)
; callee linkage

PUBLIC _zx_print_bonus_time

EXTERN l_small_utoa, _tbuffer

_zx_print_bonus_time:

IF __SDCC

   pop af
   pop de                      ; e = row, d = col
   pop hl                      ; hl = time
   push af

   ld a,e
   ld e,d
   ld d,a

ENDIF

IF __SCCZ80

   pop af
   pop hl                      ; hl = time
   pop de                      ; e = col
   pop bc                      ; c = row
   push af

   ld d,c

ENDIF

zx_print_bonus_time:

   ;  e = col
   ;  d = row
   ; hl = time in hundredths of second

   push de
   ld de,_tbuffer

   ; hl = unsigned int time
   ; de = char *buffer
   ; stack = row/col

   scf                         ; request leading zeroes
   call l_small_utoa           ; z88dk function: unsigned int to ascii buffer

   ; insert decimal point before last two digits

   ld hl,_tbuffer+3
   ld e,(hl)
   ld (hl),'.'
   inc hl
   ld d,(hl)
   ld (hl),e
   inc hl
   ld (hl),d
   inc hl
   ld (hl),0                   ; zero terminate

   pop hl
   ld de,_tbuffer

   ; de = char *buffer
   ;  h = row
   ;  l = col

   jp zx_print_str
