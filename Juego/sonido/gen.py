"""Paquete de efectos de sonido retro para "Punto B" (sintetizados, sin samples)."""
import os
import wave
import numpy as np
from scipy import signal

SR = 44100
OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "sonidos")
os.makedirs(OUT, exist_ok=True)
rng = np.random.default_rng(1930)

NOTAS = {"C": -9, "C#": -8, "D": -7, "D#": -6, "E": -5, "F": -4, "F#": -3,
         "G": -2, "G#": -1, "A": 0, "A#": 1, "B": 2}


def n(nombre):
    """'A4' -> 440.0"""
    nota, octava = nombre[:-1], int(nombre[-1])
    return 440.0 * 2 ** ((NOTAS[nota] + 12 * (octava - 4)) / 12)


def t_(dur):
    return np.arange(int(SR * dur)) / SR


def barrido(f0, f1, dur, curva=1.0):
    """Frecuencia que va de f0 a f1 (exponencial, suena natural)."""
    x = np.linspace(0, 1, int(SR * dur)) ** curva
    return f0 * (f1 / f0) ** x


def osc(freq, onda="cuadrada", duty=0.5):
    """Oscilador estilo chip. freq puede ser un array (barridos)."""
    fase = np.cumsum(np.asarray(freq, dtype=float)) / SR
    p = fase % 1.0
    if onda == "cuadrada":
        return np.where(p < duty, 1.0, -1.0)
    if onda == "triangular":
        return 4 * np.abs(p - 0.5) - 1
    if onda == "sierra":
        return 2 * p - 1
    return np.sin(2 * np.pi * fase)


def ruido(dur, tasa=None):
    """Ruido blanco; con 'tasa' se retiene cada muestra (ruido tipo NES)."""
    m = int(SR * dur)
    if tasa is None:
        return rng.uniform(-1, 1, m)
    paso = max(1, int(SR / tasa))
    base = rng.uniform(-1, 1, m // paso + 2)
    return np.repeat(base, paso)[:m]


def decae(dur, vel=5.0, ataque=0.002):
    t = t_(dur)
    e = np.exp(-vel * t / dur)
    a = int(SR * ataque)
    if a > 0:
        e[:a] *= np.linspace(0, 1, a)
    return e


def filtro(x, corte, tipo="low", orden=2):
    sos = signal.butter(orden, corte, btype=tipo, fs=SR, output="sos")
    return signal.sosfilt(sos, x)


def mezclar(*capas):
    """Suma capas (señal, inicio_en_segundos)."""
    largo = max(int(SR * ini) + len(s) for s, ini in capas)
    out = np.zeros(largo)
    for s, ini in capas:
        i = int(SR * ini)
        out[i:i + len(s)] += s
    return out


def nota(freq, dur, onda="cuadrada", duty=0.5, vel=4.0, vibrato=0.0):
    f = np.full(int(SR * dur), float(freq))
    if vibrato:
        f = f * (1 + vibrato * np.sin(2 * np.pi * 7 * t_(dur)))
    return osc(f, onda, duty) * decae(dur, vel)


def melodia(notas, onda="cuadrada", duty=0.5, vel=4.0, vol=1.0):
    """notas: lista de (nombre o None, duracion)."""
    partes = []
    for nom, d in notas:
        partes.append(np.zeros(int(SR * d)) if nom is None
                      else nota(n(nom), d, onda, duty, vel) * vol)
    return np.concatenate(partes)


def eco(x, retardo=0.12, veces=3, caida=0.45):
    capas = [(x * caida ** k, retardo * k) for k in range(veces + 1)]
    return mezclar(*capas)


def guardar(nombre, x, pico=0.8, cola=0.004):
    x = np.asarray(x, dtype=float)
    x = x - np.mean(x)
    f = int(SR * cola)                      # fundidos para evitar "clics"
    x[:f // 2] *= np.linspace(0, 1, f // 2)
    x[-f:] *= np.linspace(1, 0, f)
    x = x / np.max(np.abs(x)) * pico
    pcm = (x * 32767).astype("<i2")
    with wave.open(os.path.join(OUT, nombre + ".wav"), "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(SR)
        w.writeframes(pcm.tobytes())
    return x


S = {}  # nombre -> (señal, pico)

# ---------------- Jugador ----------------
S["salto"] = (osc(barrido(290, 640, 0.16, 0.8), "cuadrada", 0.5) * decae(0.16, 3.0), 0.55)

d = 0.15
S["doble_salto"] = (osc(barrido(520, 1150, d, 0.7), "cuadrada", 0.25) * decae(d, 2.5)
                    * (1 + 0.25 * np.sin(2 * np.pi * 38 * t_(d))), 0.55)

S["aterrizar"] = (mezclar(
    (filtro(ruido(0.07), 900) * decae(0.07, 7), 0),
    (osc(barrido(130, 55, 0.09), "seno") * decae(0.09, 5) * 0.9, 0)), 0.45)

d = 0.62
caida = osc(barrido(700, 48, d, 0.7), "cuadrada", 0.5) * decae(d, 2.2)
caida *= np.where(osc(np.full(len(caida), 26.0), "cuadrada") > 0, 1.0, 0.35)  # temblor
S["muerte"] = (mezclar(
    (ruido(0.22, 9000) * decae(0.22, 5) * 0.9, 0),
    (caida, 0.02),
    (filtro(ruido(0.5, 2500), 1400) * decae(0.5, 3) * 0.5, 0.1)), 0.85)

S["respawn"] = (melodia([("C5", 0.06), ("E5", 0.06), ("G5", 0.06), ("C6", 0.16)],
                        "triangular", vel=2.5), 0.6)

# ---------------- Lanza ----------------
d = 0.2
soplo = ruido(d)
soplo = signal.sosfilt(signal.butter(2, [900, 4200], btype="band", fs=SR, output="sos"), soplo)
env = np.sin(np.pi * np.linspace(0, 1, len(soplo)) ** 0.6) ** 1.5
S["lanza_lanzar"] = (mezclar(
    (soplo * env, 0),
    (osc(barrido(1100, 380, 0.12), "triangular") * decae(0.12, 4) * 0.35, 0)), 0.6)

d = 0.34
tt = t_(d)
golpe = filtro(ruido(0.03), 2500) * decae(0.03, 6)
cuerpo = osc(barrido(210, 95, 0.14), "seno") * decae(0.14, 5)
madera = np.sin(2 * np.pi * 340 * tt) * np.exp(-28 * tt) * 0.6
vibra = osc(np.full(len(tt), 150.0), "triangular") * np.exp(-11 * tt) \
    * (0.5 + 0.5 * np.sin(2 * np.pi * 31 * tt)) * 0.45       # la lanza queda temblando
S["lanza_clavar"] = (mezclar((golpe, 0), (cuerpo, 0), (madera, 0), (vibra, 0.02)), 0.75)

d = 0.13
tt = t_(d)
clink = (np.sin(2 * np.pi * 1870 * tt) + 0.6 * np.sin(2 * np.pi * 2790 * tt)
         + 0.4 * np.sin(2 * np.pi * 4120 * tt)) * np.exp(-34 * tt)
S["lanza_rebote"] = (mezclar((clink, 0), (filtro(ruido(0.015), 3000, "high") * 0.6, 0)), 0.55)

d = 0.24
sube = osc(barrido(260, 980, d, 1.4), "triangular")
env = np.linspace(0, 1, len(sube)) ** 1.6
soplo = filtro(ruido(d), 1800, "high") * env * 0.35
S["lanza_llamar"] = (sube * env + soplo, 0.55)

S["lanza_atrapar"] = (mezclar(
    (filtro(ruido(0.02), 2000, "high") * decae(0.02, 4) * 0.7, 0),
    (osc(barrido(760, 480, 0.07), "cuadrada", 0.25) * decae(0.07, 4), 0)), 0.5)

# ---------------- Objetos y mundo ----------------
S["llave"] = (melodia([("B5", 0.07), ("E6", 0.3)], "cuadrada", 0.5, vel=3.5), 0.6)

arp = melodia([("C5", 0.07), ("E5", 0.07), ("G5", 0.07), ("C6", 0.07), ("E6", 0.07),
               ("G6", 0.07), ("C7", 0.4)], "cuadrada", 0.25, vel=2.2)
bajo = melodia([("C3", 0.42), ("C4", 0.4)], "triangular", vel=1.5, vol=0.6)
S["habilidad"] = (eco(mezclar((arp, 0), (bajo, 0)), 0.11, 2, 0.35), 0.75)

d = 0.75
tt = t_(d)
roce = filtro(ruido(d, 1800), 500) * np.minimum(1, tt / 0.05) * np.minimum(1, (d - tt) / 0.1)
motor = osc(barrido(55, 90, d), "cuadrada", 0.5) * 0.35 * np.minimum(1, (d - tt) / 0.1)
tope = osc(barrido(120, 50, 0.12), "seno") * decae(0.12, 5) * 1.3
campana = melodia([("G5", 0.09), ("C6", 0.09), ("E6", 0.32)], "triangular", vel=2.5, vol=0.8)
S["puerta_abrir"] = (mezclar((roce, 0), (motor, 0), (tope, d - 0.03), (campana, d + 0.08)), 0.8)

S["puerta_cerrada"] = (melodia([("A2", 0.1), (None, 0.04), ("A2", 0.16)],
                               "cuadrada", 0.5, vel=1.5), 0.55)

clic = filtro(ruido(0.012), 2500, "high") * 0.8
S["boton_activar"] = (mezclar((clic, 0), (melodia([("E5", 0.06), ("B5", 0.14)],
                                                  "cuadrada", 0.5, 3.5), 0.01)), 0.6)
S["boton_desactivar"] = (mezclar((clic, 0), (melodia([("B5", 0.06), ("E5", 0.14)],
                                                     "cuadrada", 0.5, 3.5), 0.01)), 0.6)

S["guardado"] = (eco(melodia([("E5", 0.09), ("G#5", 0.09), ("B5", 0.09), ("E6", 0.28)],
                             "triangular", vel=2.5), 0.14, 3, 0.4), 0.7)

S["cambio_sala"] = (mezclar(
    (filtro(ruido(0.2), 1500) * np.sin(np.pi * np.linspace(0, 1, int(SR * 0.2))) * 0.6, 0),
    (osc(barrido(330, 500, 0.18), "triangular") * decae(0.18, 2.5) * 0.7, 0.02)), 0.4)

S["pilar_golpe"] = (mezclar(
    (osc(barrido(100, 36, 0.28), "seno") * decae(0.28, 4), 0),
    (filtro(ruido(0.16, 3000), 700) * decae(0.16, 5) * 0.9, 0)), 0.75)

# ---------------- Menú y final ----------------
S["menu_mover"] = (nota(n("A5"), 0.045, "cuadrada", 0.25, 3.0), 0.4)
S["menu_seleccionar"] = (melodia([("A5", 0.05), ("E6", 0.16)], "cuadrada", 0.25, 3.0), 0.55)
S["menu_volver"] = (melodia([("E6", 0.05), ("A5", 0.14)], "cuadrada", 0.25, 3.0), 0.5)

mel = melodia([("G5", 0.12), ("G5", 0.12), ("G5", 0.12), ("C6", 0.36), ("A#5", 0.24),
               ("C6", 0.12), ("D6", 0.12), ("E6", 0.12), ("G6", 0.7)],
              "cuadrada", 0.5, vel=2.0)
arm = melodia([("E5", 0.12), ("E5", 0.12), ("E5", 0.12), ("G5", 0.36), ("F5", 0.24),
               ("G5", 0.12), ("A#5", 0.12), ("C6", 0.12), ("E6", 0.7)],
              "cuadrada", 0.25, vel=2.0, vol=0.55)
baj = melodia([("C3", 0.36), ("C4", 0.36), ("A#3", 0.24), ("G3", 0.36), ("C3", 0.7)],
              "triangular", vel=1.2, vol=0.8)
S["victoria"] = (mezclar((mel, 0), (arm, 0), (baj, 0)), 0.8)

brillo = melodia([("E6", 0.05), ("G6", 0.05), ("B6", 0.05), ("E7", 0.05), ("B6", 0.05),
                  ("E7", 0.05), ("G7", 0.3)], "cuadrada", 0.25, vel=2.5)
S["nuevo_record"] = (eco(brillo, 0.1, 3, 0.4), 0.65)

# ---------------- Guardar todo + demo ----------------
finales = {}
for nombre, (senal, pico) in S.items():
    finales[nombre] = guardar(nombre, senal, pico)

silencio = np.zeros(int(SR * 0.45))
demo = np.concatenate([np.concatenate([x, silencio]) for x in finales.values()])
pcm = (demo * 32767).astype("<i2")
with wave.open(os.path.join(os.path.dirname(OUT), "demo_todos_los_sonidos.wav"), "wb") as w:
    w.setnchannels(1)
    w.setsampwidth(2)
    w.setframerate(SR)
    w.writeframes(pcm.tobytes())

print(f"{'archivo':24} {'dur(s)':>7} {'pico':>6} {'rms':>6}")
for nombre, x in finales.items():
    assert np.isfinite(x).all() and np.max(np.abs(x)) <= 1.0
    print(f"{nombre + '.wav':24} {len(x) / SR:7.2f} {np.max(np.abs(x)):6.2f} "
          f"{np.sqrt(np.mean(x ** 2)):6.3f}")
print("total:", len(finales), "sonidos; demo:", round(len(demo) / SR, 1), "s")
