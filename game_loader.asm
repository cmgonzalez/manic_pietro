;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; MINI LOADER ;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION MCLOAD

PUBLIC mini_loader

EXTERN __LOADER_head, __LOADER_CODE_tail

mini_loader:

   ;; Small loader stored in screen to allow overwrite of basic area

   ; Enable 48k ROM

   di

   ld bc,$1ffd

   ld a,$04
   out (c),a                   ; +2A/+3 : disk motor off, 48k rom selected, normal paging

   ld b,$7f

   ld a,$10
   out (c),a                   ; 48k rom selected, bank 00 at top of ram

   ; Load main loader over basic area

   ld ix,__LOADER_head
   ld de,__LOADER_CODE_tail - __LOADER_head

   ld a,$ff
   scf

   call 0x0556
   di

   jp c, __LOADER_head
   rst 0                       ; if tape loading error

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; MAIN LOADER (UNCOMPRESSED) ;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

IFNDEF PCOMPRESS

   ;; Machine Code Loader (No Compression)

   SECTION LOADER

      jp game_loader

   SECTION LOADER_CODE

   PUBLIC game_loader

   EXTERN __CODE_head, __CODE_END_tail, __NIRVANAP_head
   EXTERN __BANK_1_head, __BANK_1_tail
   EXTERN __BANK_3_head, __BANK_3_MISC_tail
   EXTERN __BANK_4_head, __BANK_4_MISC_tail
   EXTERN __BANK_6_head, __BANK_6_MISC_tail
   EXTERN _spec128

   game_loader:

      ; 1. SCREEN$

      ld ix,16384
      ld de,6912

      ld a,$ff
      scf

      call 0x0556
      di

      jp nc, 0                    ; if tape loading error

      ; 2. Nirvana+

      ld ix,__NIRVANAP_head
      ld de,9055

      ld a,$ff
      scf

      call 0x0556
      di

      jp nc, 0                    ; if tape loading error

      ; 3. Manic Pietro

      ld sp,__CODE_head                             ; move stack underneath program
      ld iy,128                                     ; move IY into ROM in case an im1 interrupt occurs

      ld ix,__CODE_head                             ; start of binary
      ld de,__CODE_END_tail - __CODE_head           ; length of binary

      ld a,$ff
      scf

      call 0x0556
      di

      jp nc, 0                    ; if tape loading error

      ; 4. AY Sound for 128k Spectrum

      ; check for 128k spectrum

      ld bc,$7ffd
      ld de,$1011

      ld hl,0xc000
      ld a,(hl)                   ; a = byte at 0xc000

      out (c),e                   ; enable BANK_1

      cpl
      ld (hl),a                   ; write complemented byte to 0xc000
      cpl

      out (c),d                   ; restore BANK_0

      cp (hl)                     ; did the byte change?
      ld (hl),a                   ; restore byte at 0xc000

      jp nz, __CODE_head          ; if the byte changed this is a 48k machine

      ; load extra banks for 128k machines
      
      ld hl,__BANK_1_tail - __BANK_1_head
      
      call load_bank
      jp nc, __CODE_head          ; if tape loading error forget about sound effects
      
      inc e
      inc e                       ; BANK_3
      ld hl,__BANK_3_MISC_tail - __BANK_3_head

      call load_bank
      jp nc, __CODE_head          ; if tape loading error forget about sound effects


      inc e                       ; BANK_4
      ld hl,__BANK_4_MISC_tail - __BANK_4_head

      call load_bank
      jp nc, __CODE_head          ; if tape loading error forget about sound effects

      inc e
      inc e                       ; BANK_6
      ld hl,__BANK_6_MISC_tail - __BANK_6_head

      call load_bank
      jp nc, __CODE_head          ; if tape loading error forget about sound effects

      ld a,$ff
      ld (_spec128),a             ; indicate 128k spectrum

      jp __CODE_head

   load_bank:

      out (c),e                   ; enable BANK

      push bc
      push de

      ex de,hl
      ld ix,0xc000

      ld a,$ff
      scf

      call 0x0556
      di

      pop de
      pop bc

      out (c),d                   ; restore BANK
      ret

ENDIF

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; MAIN LOADER (COMPRESSED) ;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

IFDEF PCOMPRESS

   ;; Machine Code Loader (Compressed)

   SECTION LOADER

      jp game_loader

   ;; l_ret is a library function used as an easy exit with built-in pop.
   ;; asm_dzx7_standard jumps into it so we explicitly place it here in
   ;; the loader so that it is available during the entire load process.

      pop hl

   l_ret:

      ret

   SECTION LOADER_CODE

   PUBLIC game_loader
   PUBLIC l_ret

   EXTERN asm_dzx7_standard
   EXTERN __CODE_head, __CODE_END_tail, __NIRVANAP_head, _spec128
   EXTERN LEN_SCREEN, LEN_NIRVANAP, LEN_GAME, LEN_BANK_1, LEN_BANK_3, LEN_BANK_4, LEN_BANK_6

   game_loader:

      ; 1. SCREEN$

      ld ix,32768                 ; load compressed screen$ to convenient spot
      ld de,LEN_SCREEN

      ld a,$ff
      scf

      call 0x0556
      di

      jp nc, 0                    ; if tape loading error

      ld hl,32768
      ld de,16384

      call asm_dzx7_standard      ; decompress to 16384

      ; 2. Nirvana+

      ld ix,32768                 ; load compressed nirvana+ to convenient spot
      ld de,LEN_NIRVANAP

      ld a,$ff
      scf

      call 0x0556
      di

      jp nc, 0                    ; if tape loading error

      ld hl,32768
      ld de,__NIRVANAP_head

      call asm_dzx7_standard      ; decompress to 56323

      ; 3. Manic Pietro

      ld sp,__CODE_head                             ; move stack underneath program
      ld iy,128                                     ; move IY into ROM in case an im1 interrupt occurs

      ld ix,__CODE_END_tail + 6 - LEN_GAME        ; load binary with fixed six byte delta (see zx7 docs)
      ld de,LEN_GAME                              ; length of binary

      ld a,$ff
      scf

      call 0x0556
      di

      jp nc, 0                    ; if tape loading error

      ld hl,__CODE_END_tail + 6 - LEN_GAME
      ld de,__CODE_head

      call asm_dzx7_standard      ; perform overlapped decompression

      ; 4. AY Sound for 128k Spectrum

      ; check for 128k spectrum

      ld bc,$7ffd
      ld de,$1011

      ld hl,0xc000
      ld a,(hl)                     ; a = byte at 0xc000

      out (c),e                     ; enable BANK_1

      cpl
      ld (hl),a                     ; write complemented byte to 0xc000
      cpl

      out (c),d                     ; restore BANK_0

      cp (hl)                       ; did the byte change?
      ld (hl),a                     ; restore byte at 0xc000

      jp nz, __CODE_head            ; if the byte changed this is a 48k machine

      ; load extra banks for 128k machines

      ld ix,0x10000 - LEN_BANK_1    ; load at top of bank to enable overlapped decompression
      ld hl,LEN_BANK_1
      
      call load_bank
      jr nc, set_mode               ; if tape loading error stay in 48k mode
      
      inc e
      inc e                         ; BANK_3
      
      ld ix,0x10000 - LEN_BANK_3    ; load at top of bank to enable overlapped decompression
      ld hl,LEN_BANK_3

      call load_bank
      jr nc, set_mode               ; if tape loading error stay in 48k mode

      inc e                         ; BANK_4

      ld ix,0x10000 - LEN_BANK_4    ; load at top of bank to enable overlapped decompression
      ld hl,LEN_BANK_4

      call load_bank
      jr nc, set_mode               ; if tape loading error stay in 48k mode

      inc e
      inc e                         ; BANK_6

      ld ix,0x10000 - LEN_BANK_6    ; load at top of bank to enable overlapped decompression
      ld hl,LEN_BANK_6

      call load_bank
      jr nc, set_mode               ; if tape loading error stay in 48k mode

      ld a,$ff

   set_mode:

      ld (_spec128),a               ; indicate if 128k spectrum
      jp __CODE_head

   load_bank:

      out (c),e                     ; enable BANK

      push bc
      push de

      push ix
      ex de,hl

      ld a,$ff
      scf

      call 0x0556
      di

      pop hl

      ld a,0
      jr nc, lb_exit                ; if tape loading error

      ld de,0xc000
      call asm_dzx7_standard        ; decompress to 0xc000

      scf

   lb_exit:

      pop de
      pop bc

      out (c),d                     ; restore BANK
      ret

ENDIF

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
