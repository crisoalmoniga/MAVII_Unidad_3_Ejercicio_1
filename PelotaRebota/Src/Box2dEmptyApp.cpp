#include "Game.h"

using namespace sf;

class Pelota
{
private:
    // Sprite y su imagen para representación gráfica
    Sprite* _sprite;
    Texture* _image;
    RenderWindow* wnd;
    // Body para representación física
    b2Body* _body;

public:
    // Constructor de la clase Pelota
    Pelota(b2World* _world, RenderWindow* _wnd)
    {
        // Guardamos un puntero a la ventana para dibujar luego
        wnd = _wnd;
        // Cargamos la textura de la pelota desde un archivo
        _image = new Texture();
        _image->loadFromFile("Pelota.png");
        // Creamos el sprite usando la textura cargada
        _sprite = new Sprite(*_image);

        // Definimos las propiedades del cuerpo físico de la pelota
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(100.0f, 0.0f); // Posición inicial del cuerpo físico
        _body = _world->CreateBody(&bodyDef);

        // Creamos la forma de colisión para la pelota (en este caso, un cuadrado)
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(20.0f, 20.0f); // Tamaño de la pelota

        // Definimos las propiedades del fixture de la pelota
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 10.0f; // Densidad del cuerpo físico
        fixtureDef.friction = 0.3f; // Fricción del cuerpo físico
        fixtureDef.restitution = 1.0f; // Restitución (elasticidad) del cuerpo físico
        _body->CreateFixture(&fixtureDef); // Creamos el fixture y lo asociamos al cuerpo físico
    }

    // Método que actualiza la posición del sprite según la posición del cuerpo físico
    void ActualizarPosiciones()
    {
        b2Vec2 pos = _body->GetPosition();
        _sprite->setPosition(pos.x, pos.y);
    }

    // Método que dibuja el sprite en la ventana
    void Dibujar()
    {
        wnd->draw(*_sprite);
    }
    // Otros métodos relacionados con el personaje
};

// Función principal del programa
int main(int argc, char* argv[])
{
    // Creamos una instancia del juego
    Game* Juego;
    // Inicializamos el juego con una ventana de 800x600 píxeles con el título "Avatar"
    Juego = new Game(800, 600, "Avatar - MAVII");
    // Iniciamos el bucle principal del juego
    Juego->Loop();

    return 0;
}
