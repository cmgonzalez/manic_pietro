;; AY RELATED FOR BANK_3

; OWN COPY OF VTII PLAYER

SECTION BANK_3_VTII

INCLUDE "src/VTII10bG-mfx.asm"

; OWN COPY OF MFXPLAYER

SECTION BANK_3_MFX

INCLUDE "src/mfx.asm"

; SONGS & EFFECTS IN THIS BANK

SECTION BANK_3_AUDIO

PUBLIC _ay_song_03_tdm_cs9
_ay_song_03_tdm_cs9:
   BINARY "songs/tdm_cs9.pt3"

PUBLIC _ay_song_03_sunflower
_ay_song_03_sunflower:
   BINARY "songs/sunflower.pt3"

PUBLIC _ay_song_03_regis
_ay_song_03_regis:
  BINARY "songs/regis.pt3"
