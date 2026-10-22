#include "Jugador.h"
#include <SFML/window.hpp>

Jugador::Jugador(sf::Vector2f posicion) {
    _forma.setSize(sf::Vector2f(50.f, 80.f)); 
    // Vector2f Es una clase simple que representa un par de números: una coordenada x y una coordenada y.
    // Vector2f = vector 2D de floats (10.f = float | 10.0 = double) Vector2f espera 2 floats.  
    _forma.setFillColor(sf::Color::Red);

    _velocidadX = 300.f;
    _gravedad = 980.f;
    _fuerzaSalto = -500.f;
    
    reiniciar(posicion);
}

void Jugador::dibujar(sf::RenderWindow& ventana){
	ventana.draw(_forma);	
}

void Jugador::reiniciar(sf::Vector2f posicion) {
    // SFML ubica la esquina superior izquierda del rectángulo en esa coordenada, no el centro del rectángulo.
    _forma.setPosition(posicion);
    _velocidadY = 0.f;
    _desplazamientoX = 0.f;
    _enElSuelo = false;
}

void Jugador::mover(float dt) { //“¿qué teclas apretó el jugador?” y reaccionar a eso.
    sf::Vector2f movimiento(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
        movimiento.x -= _velocidadX * dt;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        movimiento.x += _velocidadX * dt;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){  
        if (_enElSuelo) {
            _velocidadY = _fuerzaSalto;
            _enElSuelo = false;
        }
    }
    _forma.move(movimiento); //Para mover la _forma, las clases sf::RectangleShape (y en general las formas de SFML)...
                             //tienen un método move(Vector2f) que le suma un desplazamiento a la posición actual.
    _desplazamientoX = movimiento.x;
}

//“¿qué le hace el mundo al jugador este frame?”: la gravedad lo tira hacia abajo, se aplica el movimiento vertical y después las plataformas lo frenan (ver aterrizarEn).
// 
//dt en SFML se refiere comúnmente al Delta Time (Tiempo Delta).Es el tiempo que transcurre entre el fotograma (frame) anterior 
//y el fotograma actual en un videojuego.
void Jugador::actualizar(float dt) {
    _alturaPiesAnterior = getHitbox().position.y+ getHitbox().size.y;
    _enElSuelo = false;
    _velocidadY += _gravedad * dt;
    
    _forma.move(sf::Vector2f(0.f,_velocidadY*dt));
}

void Jugador::aterrizarEn(sf::FloatRect plataformaHitbox) {
    if (!getHitbox().findIntersection(plataformaHitbox)) { // ¿este rectángulo NO superpone con el de la plataforma?
        return;  // NO se superponen
    } 
    if (_velocidadY < 0) { // si el jugador va subiendo, la plataforma no lo frena: la atraviesa desde abajo
        return;
    }
    if (_alturaPiesAnterior > plataformaHitbox.position.y + 0.5f) { // si los pies del jugador estaban más abajo que la parte de 
        return;                                                     // arriba de la plataforma, no aterriza.
    }
    // Si llegó hasta acá, ningún chequeo lo descartó: aterrizó.
    // Se lo apoya con los pies justo sobre la plataforma y se frena la caída.
    _forma.setPosition(sf::Vector2f(_forma.getPosition().x, plataformaHitbox.position.y - getHitbox().size.y));
    // y = parte de arriba de la plataforma menos el alto del jugador
    _velocidadY = 0.f;
    _enElSuelo = true;
}

// Choque de costado contra un bloque sólido.
void Jugador::chocarHorizontal(sf::FloatRect bloqueHitBox) {
    // ¿No lo toca? Nada que hacer.
    if (!getHitbox().findIntersection(bloqueHitBox)) {
        return;
    }
    // ¿Está parado encima del bloque? Entonces no es un choque de costado
    // (sin esto, al caminar sobre un bloque lo empujaría hacia un lado).
    if (getHitbox().size.y + _forma.getPosition().y  <= bloqueHitBox.position.y + 0.5f) {
        return;
    }
    // si jugador viene desde la izquierda hacia un bloque a su derecha
    if (_desplazamientoX > 0) {
        // lo dejamos pegado al lado izquierdo del bloque.
        _forma.setPosition(sf::Vector2f(bloqueHitBox.position.x - getHitbox().size.x , _forma.getPosition().y));
    }            // borde de la izquierda del bloque(su posicion) - ancho el jugador | La altura del jugador permanece igual.
                 // esto establece la posicion del jugador hacia atras (su ancho) evitando que atraviese el bloque
                 
    // si jugador viene desde la derecha hacia un bloque a su izquierda
    if (_desplazamientoX < 0) {
        _forma.setPosition(sf::Vector2f(bloqueHitBox.position.x + bloqueHitBox.size.x , _forma.getPosition().y));
                 // aca necesitamos hacer: posicion del bloque en eje x mas su ancho dado que la posicion x siempre empieza
                 // en el borde izquierdo
    }
}

void Jugador::chocarVertical(sf::FloatRect bloqueHitBox) {
    // ¿No lo toca? Nada que hacer.
    if (!getHitbox().findIntersection(bloqueHitBox)) {
        return;
    }
    // jugador choca el bloque por arriba
    if (_velocidadY > 0) {
        _forma.setPosition(sf::Vector2f(_forma.getPosition().x , bloqueHitBox.position.y - getHitbox().size.y));
        _velocidadY = 0.f; // reinicia velocidad de caida
        _enElSuelo = true;
    }
    // jugador choca el bloque por abajo
    else if (_velocidadY < 0) {
        _forma.setPosition(sf::Vector2f(_forma.getPosition().x, bloqueHitBox.position.y + bloqueHitBox.size.y));
        _velocidadY = 0.f;
    }
}

/*
sf::FloatRect es “un rectángulo guardado como números”. Es una clase simple de SFML, parecida a Vector2f:
así como Vector2f guarda un par de floats (x, y), un FloatRect guarda dos Vector2f:

position: la esquina superior izquierda del rectángulo (x, y).
size: su ancho y alto (x = ancho, y = alto).
No dibuja nada, es solo información. Es como una “caja invisible” que envuelve a la forma. Eso es lo que se llama hitbox.
*/
sf::FloatRect Jugador::getHitbox() const {
    return _forma.getGlobalBounds(); 
} ///_forma.getGlobalBounds() ya devuelve esa caja armada, ajustada a donde está la forma en este momento.
    
