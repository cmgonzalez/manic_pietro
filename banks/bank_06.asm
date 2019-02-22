; MAP DATA & TILES

SECTION BANK_6_DATA

;; AY RELATED FOR

; OWN COPY OF VTII PLAYER

SECTION BANK_6_VTII

INCLUDE "src/VTII10bG-mfx.asm"

; OWN COPY OF MFXPLAYER

SECTION BANK_6_MFX

INCLUDE "src/mfx.asm"

; SONGS & EFFECTS IN THIS BANK

SECTION BANK_6_AUDIO
PUBLIC _ay_song_time_atack
_ay_song_time_atack:
BINARY "songs/time_atack.pt3"

PUBLIC _ay_song_mistery
_ay_song_mistery:
BINARY "songs/mistery.pt3"

PUBLIC _ay_song_medievo
_ay_song_medievo:
BINARY "songs/medievo.pt3"
