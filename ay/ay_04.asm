;; AY RELATED FOR BANK_4

; OWN COPY OF VTII PLAYER

SECTION BANK_4_VTII

INCLUDE "src/VTII10bG-mfx.asm"

; OWN COPY OF MFXPLAYER

SECTION BANK_4_MFX

INCLUDE "src/mfx.asm"

; SONGS & EFFECTS IN THIS BANK

SECTION BANK_4_AUDIO

PUBLIC _ay_song_04_hope
_ay_song_04_hope:
  BINARY "songs/hope.pt3"

PUBLIC _ay_song_04_ciel
_ay_song_04_ciel:
  BINARY "songs/ciel_2.pt3"

PUBLIC _ay_song_04_luna
_ay_song_04_luna:
  BINARY "songs/luna.pt3"

PUBLIC _ay_song_04_doom
_ay_song_04_doom:
  BINARY "songs/doom.pt3"
