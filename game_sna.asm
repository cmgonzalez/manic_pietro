;; For SNA Snapshot Only

; embed screenshot

SECTION BANK_5

BINARY "game_scr.bin"

; indicate 128k spectrum

SECTION code_crt_init

EXTERN _spec128

ld a,$ff
ld (_spec128),a
