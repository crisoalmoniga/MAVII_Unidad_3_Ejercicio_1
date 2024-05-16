#include "Avatar.h"

Avatar::Avatar(b2Body* pCuerpo, sf::Sprite* pSprite) : m_pCuerpo(pCuerpo)
{
    m_posAux = m_pCuerpo->GetPosition();
    m_pSprite = pSprite;

    b2AABB aabb;
    aabb.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);
    aabb.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);
    for (b2Fixture* f = m_pCuerpo->GetFixtureList(); f; f = f->GetNext())
        aabb.Combine(f->GetAABB(0));
    float sizeX = aabb.GetExtents().x * 2.0f / m_pSprite->getTexture()->getSize().x;
    float sizeY = aabb.GetExtents().y * 2.0f / m_pSprite->getTexture()->getSize().y;
    m_pSprite->setScale(sf::Vector2f(sizeX, sizeY));
    m_pSprite->setOrigin(sf::Vector2f(m_pSprite->getTexture()->getSize().x / 2.0f, m_pSprite->getTexture()->getSize().y / 2.0f));
    m_pSprite->setPosition(m_posAux.x, m_posAux.y);
    m_pCuerpo->GetUserData().pointer = (uintptr_t)this;
}

Avatar::~Avatar()
{
    for (b2Fixture* f = m_pCuerpo->GetFixtureList(); f; f = f->GetNext())
        m_pCuerpo->DestroyFixture(f);
    if (m_pSprite != NULL)
        delete m_pSprite;
    m_pCuerpo->GetWorld()->DestroyBody(m_pCuerpo);
    m_pCuerpo = NULL;
}

Avatar::operator b2Body* ()
{
    return m_pCuerpo;
}

void Avatar::Actualizar()
{
    m_posAux = m_pCuerpo->GetPosition();
    m_angAux = m_pCuerpo->GetAngle();
    m_pSprite->setPosition(m_posAux.x, m_posAux.y);
    m_pSprite->setRotation(m_angAux * 180 / 3.14f);
}

void Avatar::Dibujar(sf::RenderWindow& RW)
{
    RW.draw(*m_pSprite);
}

b2Vec2 Avatar::GetPosition()
{
    return m_pCuerpo->GetPosition();
}

b2Body& Avatar::GetCuerpo()
{
    return *m_pCuerpo;
}

sf::Sprite& Avatar::GetSprite()
{
    return *m_pSprite;
}

void Avatar::SetPosition(b2Vec2 position)
{
    m_pCuerpo->SetTransform(position, m_pCuerpo->GetAngle());
}

void Avatar::AplicarFuerza(b2Vec2& fuerza, b2Vec2& origen)
{
    m_pCuerpo->ApplyForce(fuerza, origen, true);
}

void Avatar::AplicarImpulso(b2Vec2& impulso, b2Vec2& origen)
{
    m_pCuerpo->ApplyLinearImpulse(impulso, origen, true);
}

void Avatar::ActualizarConColisiones(float deltaTime, sf::RenderWindow& window)
{
    // La lógica de colisión debe manejarse en Box2D
    Actualizar();
}
