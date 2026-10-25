# Punto B

Juego de plataformas de precisión en 2D, hecho en C++ con SFML 3.1
para la materia Programación 2. El jugador debe llegar del punto A
al punto B de cada nivel esquivando obstáculos.

## Cómo compilar

1. Visual Studio 2022 o posterior, con "Desarrollo para el escritorio con C++".
2. SFML 3.1.0 (Visual C++ 17, 64 bits) descomprimido en
   `D:\Programación\SFML-3.1.0-windows-vc17-64-bit\`.
   Si está en otra ruta, cambiarla en Propiedades del proyecto:
   C/C++ → Directorios de inclusión adicionales, y
   Vinculador → Directorios de bibliotecas adicionales.
3. Compilar en **Debug x64**.
4. Si al ejecutar falta alguna DLL de SFML, copiar las de la carpeta
   `bin` de SFML junto al `.exe` (en `x64\Debug`).

## Controles

| Tecla | Acción |
|---|---|
| A / D | Moverse |
| W | Saltar |
| F11 | Pantalla completa |
| ESC | Salir |

## Estado

- Hecho: movimiento con salto y gravedad, pinches, meta, 3 niveles, plataformas.
- Pendiente: caída al vacío, menú, archivos `.dat`, sprites, sonido, etc.
