#include "Tile.h"

Tile::Tile(int t_cost, sf::Font& t_font, bool t_isPassable) :
	m_font{ t_font },
	m_isPassable{ t_isPassable }
{
	setCost(t_cost);
}

Tile::Tile(int t_cost, sf::Vector2f t_goal, sf::Vector2f t_position, sf::Font& t_font, bool t_isPassable) :
	m_font{ t_font },
	m_isPassable{ t_isPassable },
	m_pos{ t_position },
	m_goalPos{ t_goal }
{
	setCost(t_cost);
}

void Tile::setID(int t_id)
{
	m_id = t_id;
}

int Tile::getID()
{
	return m_id;
}

void Tile::setCost(int t_cost)
{
	m_cost = t_cost;
}

int Tile::getCost()
{
	return m_cost;
}

void Tile::setMarked(bool t_marked)
{
	m_marked = t_marked;
}

bool Tile::getMarked()
{
	return m_marked;
}

bool Tile::isGoal()
{
	return m_isGoal;
}

bool Tile::isStart()
{
	return m_isStart;
}

bool Tile::isPassable()
{
	return m_isPassable;
}

void Tile::setGoalNode(bool t_isGoal)
{
	m_isGoal = t_isGoal;
	if (m_isGoal)	m_shape.setFillColor(sf::Color::Red);
	else if (!m_isStart) m_shape.setFillColor(sf::Color::Blue);
}

void Tile::setStartNode(bool t_isStart)
{
	m_isStart = t_isStart;
	if (m_isStart)	m_shape.setFillColor(sf::Color::Green);
	else if (!m_isGoal) m_shape.setFillColor(sf::Color::Blue);
}


void Tile::initText()
{
	m_display.setFont(m_font);
	m_display.setFillColor(sf::Color::White);
	m_display.setCharacterSize(6);
	m_display.setPosition(m_shape.getPosition());
	m_display.setString(std::to_string(m_cost));
}

void Tile::initSprite()
{
}

void Tile::draw(sf::RenderTarget& t_target, sf::RenderStates t_states) const
{
}

