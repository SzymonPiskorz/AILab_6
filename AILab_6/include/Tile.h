#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Tile : public sf::Drawable
{
private:
	sf::Vector2f m_pos;
	sf::Vector2f m_goalPos;
	int m_cost;
	sf::Text m_display;
	sf::Font& m_font;
	sf::RectangleShape m_shape;
	int m_id;
	bool m_isPassable;
	bool m_isGoal;
	bool m_isStart;
	bool m_marked;

	void initText();
	void initSprite();
	virtual void draw(sf::RenderTarget& t_target, sf::RenderStates t_states) const;

public:
	Tile(int t_cost, sf::Font& t_font, bool t_isPassable);
	Tile(int t_cost, sf::Vector2f t_goal, sf::Vector2f t_position, sf::Font& t_font, bool t_isPassable);

	void setID(int t_id);
	int getID();

	void setCost(int t_cost);
	int getCost();

	void setMarked(bool t_marked);
	bool getMarked();

	bool isGoal();
	bool isStart();
	bool isPassable();

	void setGoalNode(bool);
	void setStartNode(bool);
};

