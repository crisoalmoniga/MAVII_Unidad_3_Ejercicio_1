#include "Game.h"
#include "Box2DHelper.h"

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
    this->ancho = ancho;
    this->alto = alto;
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    SetZoom(); // Configuración del zoom de la cámara
    InitPhysics(); // Inicialización del mundo físico
}

// Método principal que maneja el bucle del juego
void Game::Loop()
{
    sf::Clock clock;
    while (wnd->isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        wnd->clear(clearColor); // Limpia la ventana con un color especificado
        DoEvents(); // Procesa los eventos del sistema
        UpdatePhysics(); // Actualiza la simulación física
        controlBodyAvatar->Actualizar(); // Actualiza la posición de la pelota
        DrawGame(); // Dibuja el juego en la ventana
        wnd->display(); // Muestra la ventana renderizada
    }
}

// Actualiza la simulación física
void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8); // Avanza la simulación física
    phyWorld->ClearForces(); // Limpia las fuerzas aplicadas a los cuerpos
    phyWorld->DebugDraw(); // Dibuja el mundo físico (para depuración)
}

// Dibuja los elementos del juego en la ventana
void Game::DrawGame()
{
    // Dibujamos el suelo
    sf::RectangleShape groundShape(sf::Vector2f(500, 5));
    groundShape.setFillColor(sf::Color::Red);
    groundShape.setPosition(0, 95);
    wnd->draw(groundShape);

    controlBodyAvatar->Dibujar(*wnd); // Dibuja el avatar en la ventana
}

// Procesa los eventos del sistema
void Game::DoEvents()
{
    Event evt;
    while (wnd->pollEvent(evt))
    {
        switch (evt.type)
        {
        case Event::Closed:
            wnd->close(); // Cierra la ventana
            break;
        }
    }

    // Mueve el cuerpo controlado por el teclado
    controlBody->SetAwake(true); // Activa el cuerpo para que responda a fuerzas y colisiones
    if (Keyboard::isKeyPressed(Keyboard::Left))
        controlBody->SetLinearVelocity(b2Vec2(-50.0f, 0.0f));
    if (Keyboard::isKeyPressed(Keyboard::Right))
        controlBody->SetLinearVelocity(b2Vec2(50.0f, 0.0f));
    if (Keyboard::isKeyPressed(Keyboard::Down))
        controlBody->SetLinearVelocity(b2Vec2(0.0f, 50.0f));
    if (Keyboard::isKeyPressed(Keyboard::Up))
        controlBody->SetLinearVelocity(b2Vec2(0.0f, -50.0f));
}

// Configura el área visible en la ventana de renderizado
void Game::SetZoom()
{
    View camara;
    camara.setSize(100.0f, 100.0f); // Tamaño del área visible
    camara.setCenter(50.0f, 50.0f); // Centra la vista en estas coordenadas
    wnd->setView(camara); // Asigna la vista a la ventana
}

// Inicializa el mundo físico y los elementos estáticos del juego
void Game::InitPhysics()
{
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX); // Configura el renderizado para que muestre todo
    phyWorld->SetDebugDraw(debugRender);

    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

    b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

    b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(50.0f, 50.0f);
    controlBody = phyWorld->CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = 5.0f;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f;
    fixtureDef.restitution = 0.9f; // Ajusta esto para controlar el rebote

    controlBody->CreateFixture(&fixtureDef);

    texturaPelota.loadFromFile("Pelota.png");

    controlBodyAvatar = new Avatar(controlBody, new sf::Sprite(texturaPelota));
}