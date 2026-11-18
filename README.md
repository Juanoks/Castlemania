# Punto B

Juego de plataformas de precisión en 2D, hecho en C++ con SFML 3.1 para la materia
Programación 2. El jugador recorre un mapa de salas conectadas, esquivando obstáculos y
resolviendo puzzles con una lanza, para juntar las cuatro llaves que abren la puerta final.

## Cómo compilar

1. Visual Studio 2022 o posterior, con "Desarrollo para el escritorio con C++".
2. SFML 3.1.0 (Visual C++ 17, 64 bits) descomprimido en cualquier carpeta.
3. Crear la variable de entorno `SFML_DIR` apuntando a esa carpeta
   (por ejemplo `D:\Programación\SFML-3.1.0-windows-vc17-64-bit`).
   El proyecto usa `$(SFML_DIR)\include` y `$(SFML_DIR)\lib`, así que no hace falta
   tocar las propiedades. Después de crearla, reiniciar Visual Studio.
4. Compilar en **Debug x64**.
5. Si al ejecutar falta alguna DLL de SFML, copiar las de la carpeta `bin` de SFML
   junto al `.exe` (en `x64\Debug`).

## Controles

### Menú

| Tecla | Acción |
|---|---|
| ↑ / ↓ | Seleccionar opción |
| ENTER | Confirmar opción |
| F11 | Pantalla completa |

### Juego

| Tecla | Acción |
|---|---|
| A / D | Moverse |
| W | Saltar (otra vez en el aire: doble salto, cuando se consigue) |
| Click izquierdo | Lanzar la lanza hacia el mouse |
| Click izquierdo (con la lanza afuera) | Llamar la lanza de vuelta |
| ESC | Volver al menú |

## Mecánicas

- **Lanza:** se clava en la madera y sirve como escalón (en paredes o en el piso).
  Atraviesa las plataformas y rebota en la piedra.
- **Botones y compuertas:** una lanza clavada en un botón activa las compuertas de su canal.
  Algunas compuertas son invertidas (se cierran al activar el botón) y otras son trampillas.
- **Cristales y barreras:** romper todos los cristales de un canal apaga su barrera.
- **Llaves:** hay cuatro (azul, amarilla, verde y roja). Las tres primeras abren la jaula
  del doble salto, y las cuatro abren la puerta final.
- **Obstáculos:** pinches (también hundidos o rotados) y pilares que se mueven en
  horizontal, vertical o acostados.

## Estado

- Hecho: menú, sprites, animaciones, música y sonidos, lanza, doble salto, botones y
  compuertas, cristales y barreras, puerta final, caminos de las llaves azul y amarilla.
- Pendiente: Niveles, más obstaculos/mecanicas.
