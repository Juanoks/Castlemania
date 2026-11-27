# Castlemania

Juego de plataformas de precisión en 2D, hecho en **C++** con **SFML 3.1** como Trabajo Práctico Integrador de **Programación II** (UTN FRGP, 2026).

Recorrés un castillo de salas conectadas esquivando pinches y pilares, resolviendo puzzles con una lanza que se clava en la madera, y juntando las cuatro llaves que abren la puerta final para salir de la mazmorra.

<!-- Agregar acá una captura o un GIF del juego: ![Castlemania](docs/captura.png) -->

## Jugar sin compilar

1. Descargá el `.zip` de la sección **Releases** del repositorio.
2. Extraelo y abrí `JUGAR/Castlemania.exe`.

Si Windows avisa que falta `MSVCP140.dll` o `VCRUNTIME140.dll`, instalá el **Microsoft Visual C++ Redistributable (x64)**.

## Controles

| Tecla | Acción |
|---|---|
| A / D o ← / → | Moverse |
| W o ↑ | Saltar (en el aire, doble salto cuando se consigue) |
| Click izquierdo | Lanzar la lanza hacia el mouse |
| Click izquierdo (con la lanza afuera) | Llamarla de vuelta |
| ESC | Volver al menú |
| F11 | Pantalla completa |

En el menú: W / S o ↑ / ↓ para elegir y ENTER para confirmar. En la pantalla de récords, **B** borra los récords (con confirmación **Y** / **N**).

## Mecánicas

- **Lanza:** vuela en curva por la gravedad. Se clava en la madera y sirve de escalón; en la piedra rebota. Con otro click vuelve en línea recta hacia el jugador.
- **Doble salto:** una habilidad que se consigue durante el recorrido.
- **Botones y compuertas:** un botón queda activo mientras tenga la lanza clavada y abre o cierra las compuertas de su canal.
- **Cristales y barreras:** romper todos los cristales de un canal apaga su barrera.
- **Llaves:** cuatro (azul, amarilla, verde y roja), cada una al final de su propio camino. Juntas abren la puerta final.
- **Obstáculos:** pinches y pilares con pinches que se mueven en horizontal o en vertical.
- **Récords:** al terminar se guardan el tiempo y las muertes en un archivo, y el menú muestra los 5 mejores tiempos.

## Conceptos aplicados

- **Clases y encapsulamiento:** cada elemento del juego es una clase con atributos privados.
- **Herencia y polimorfismo:** `Obstaculo` (Pinche, PilarMovil), `Mecanismo` (Compuerta, Barrera, PuertaLlave) y `Terreno` (Bloque, Plataforma); los obstáculos y mecanismos se guardan en `vector<T*>` y se usan a través de métodos virtuales.
- **Composición:** `Partida` tiene un `Nivel` y un `Jugador`; `Nivel` tiene los vectores con todo lo que hay en la sala.
- **Memoria dinámica:** los objetos polimórficos se crean con `new` al cargar cada sala y se liberan con `delete` al cambiarla.
- **Archivos:** los récords se guardan como registros de tamaño fijo en `partidas.dat` (`fopen`, `fwrite`, `fread`, `fseek`, `ftell`).
- **Enumeradores:** `enum class` para los estados del juego, de la lanza y los tipos de texturas, sonidos y coleccionables.

## Compilar

1. Visual Studio 2022 o posterior, con "Desarrollo para el escritorio con C++".
2. SFML 3.1.0 (Visual C++ 17, 64 bits) descomprimido en cualquier carpeta.
3. Crear la variable de entorno `SFML_DIR` apuntando a esa carpeta (por ejemplo `D:\SFML-3.1.0`) y reiniciar Visual Studio. El proyecto usa `$(SFML_DIR)\include` y `$(SFML_DIR)\lib`.
4. Abrir `Juego.slnx` y compilar en **x64** (Debug para desarrollar, Release para distribuir).
5. Copiar las DLL de la carpeta `bin` de SFML junto al `.exe` (las `-d` para Debug, las otras para Release).

## Modo debug

Con `MODO_DEBUG = true` en `Constantes.h`, la partida arranca con todas las habilidades y llaves, se muestran las coordenadas del mouse, **G** muestra una grilla y **R** recarga la sala actual.

## Autores

- Juan Ignacio Crettaz — [@Juanoks](https://github.com/Juanoks)
- Mateo Viviani — [@mateiko7](https://github.com/mateiko7)

## Créditos

- Fuente: *Press Start 2P* de CodeMan38, licencia SIL Open Font License (`fuentes/OFL.txt`).
- Sonidos y música: COMPLETAR (origen y licencia).
