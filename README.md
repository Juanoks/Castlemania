# Castlemania

Juego de plataformas de precisión en 2D, hecho en C++ con SFML 3.1 para la materia
Programación 2. El jugador recorre un mapa de salas conectadas, esquivando obstáculos y
resolviendo puzzles con una lanza, para juntar las cuatro llaves que abren la puerta final
y salir de la mazmorra.

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
| W / S o ↑ / ↓ | Seleccionar opción |
| ENTER | Confirmar opción |
| ESC | Volver al menú principal |
| F11 | Pantalla completa |

### Juego

| Tecla | Acción |
|---|---|
| A / D o ← / → | Moverse |
| W o ↑ | Saltar (otra vez en el aire: doble salto, cuando se consigue) |
| Click izquierdo | Lanzar la lanza hacia el mouse |
| Click izquierdo (con la lanza afuera) | Llamar la lanza de vuelta |
| ESC | Volver al menú |

## Mecánicas

- **Lanza:** se clava en la madera y sirve como escalón (en paredes o en el piso).
 Al llamarla vuelve en línea recta atravesando todo. 
- **Doble salto:** 
- **Botones y compuertas:** 
- **Llaves:** hay cuatro (azul, amarilla, verde y roja), cada una al final de su propio
  camino.
- **Obstáculos:** pinches y pilares moviles que se mueven en horizontal, vertical.
- **Records:** al terminar se guardan el tiempo y las muertes, y el menú muestra los
  mejores 5 tiempos.

## Herramientas para armar salas

Con `MODO_DEBUG = true` en `Constantes.h`, la partida arranca con todas las habilidades
y llaves, se muestran las coordenadas del mouse, **G** muestra una grilla y **R** recarga
la sala actual.
