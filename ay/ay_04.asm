;; AY RELATED FOR BANK_4

; OWN COPY OF VTII PLAYER

SECTION BANK_4_VTII

INCLUDE "src/VTII10bG-mfx.asm"

; OWN COPY OF MFXPLAYER

SECTION BANK_4_MFX

INCLUDE "src/mfx.asm"

; SONGS & EFFECTS IN THIS BANK

SECTION BANK_4_AUDIO

PUBLIC _ay_song_04_ingame
_ay_song_04_ingame:
  BINARY "songs/Davos_MOZART.pt3"

PUBLIC _ay_song_04_ingame0
_ay_song_04_ingame0:
  BINARY "songs/pietro_cavern.pt3"

PUBLIC _ay_song_04_menu
_ay_song_04_menu:
    BINARY "songs/pietro_blue.pt3"
