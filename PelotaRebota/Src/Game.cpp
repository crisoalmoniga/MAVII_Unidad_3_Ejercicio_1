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
    phyWorld->DebugDraw();
}

// Dibuja los elementos del juego en la ventana
void Game::DrawGame()
{
    // Suelo
    sf::RectangleShape groundShape(sf::Vector2f(500, 5));
    groundShape.setFillColor(sf::Color::Red);
    groundShape.setPosition(0, 95);
    wnd->draw(groundShape);

    // Techo
    sf::RectangleShape roofShape(sf::Vector2f(500, 5));
    roofShape.setFillColor(sf::Color::Blue);
    roofShape.setPosition(0, 0);
    wnd->draw(roofShape);

    // Pared izquierda
    sf::RectangleShape leftWallShape(sf::Vector2f(5, alto));
    leftWallShape.setFillColor(sf::Color::Green);
    leftWallShape.setPosition(0, 0);
    wnd->draw(leftWallShape);

    // Pared derecha
    sf::RectangleShape rightWallShape(sf::Vector2f(5, alto));
    rightWallShape.setFillColor(sf::Color::Yellow);
    rightWallShape.setPosition(ancho - 5, 0);
    wnd->draw(rightWallShape);

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
    camara.setSize(100.0f, 100.0f);
    camara.setCenter(50.0f, 50.0f);
    wnd->setView(camara);
}

// Inicializa las físicas
void Game::InitPhysics()
{
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f)); // Gravedad del mundo físico

    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX); // Configura el renderizado para que muestre todo
    phyWorld->SetDebugDraw(debugRender);

    // Crea el suelo
    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

    // Crea el techo
    b2Body* roofBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    roofBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

    // Crea la pared izquierda
    b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

    // Crea la pared derecha
    b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

    // Crea el cuerpo dinámico de la pelota
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(50.0f, 50.0f); // Posición inicial
    controlBody = phyWorld->CreateBody(&bodyDef);

    // Crea la forma de la pelota
    b2CircleShape circleShape;
    circleShape.m_radius = 5.0f; // Radio de la pelota

    // Propiedades de la pelota
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f; // Densidad de la pelota
    fixtureDef.friction = 0.5f; // Fricción de la pelota
    fixtureDef.restitution = 0.9f; // Coeficiente de restitución

    controlBody->CreateFixture(&fixtureDef); // Crea el fixture con las propiedades definidas

    texturaPelota.loadFromFile("Pelota.png"); // Carga de la textura de la pelota

    // Crea el avatar de la pelota con la textura
    controlBodyAvatar = new Avatar(controlBody, new sf::Sprite(texturaPelota));
}