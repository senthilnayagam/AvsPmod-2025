# Test AviSynth Script - Unicode Test
# French: Ceci est un test avec des caractères spéciaux: café, élève, naïve
# Spanish: Esto es una prueba con caracteres especiales: año, niño
# German: Dies ist ein Test mit Sonderzeichen: über, größe
# Japanese: これはテストです
# Chinese: 这是一个测试

# Simple BlankClip
BlankClip(width=640, height=480, length=100, fps=24, color=$0000FF)

# Add subtitle
Subtitle("Unicode Test: café über 日本語", size=24, text_color=$FFFFFF)
