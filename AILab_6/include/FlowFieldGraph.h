#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
#include <vector>
#include <list>

struct Tile
{
	int m_cost;
	float integratedCost;
	sf::Vector2i m_neighbour;
};

class FlowFieldGraph : public sf::Drawable
{
	sf::Vector2i m_goal;
	sf::Vector2i m_start;

	void setNeighbourCosts(sf::Vector2i t_current, sf::Vector2i t_goal);
	sf::Vector2i bestNeighbour(sf::Vector2i);
	void createFlowField();

	std::vector<std::vector<Tile>> m_tiles;
	std::queue<sf::Vector2i> m_tileCostQueue;

	sf::Font m_font;
	sf::Text m_text;
	sf::RenderTexture m_renderTexture;
	sf::Sprite m_renderTextureSprite;

public:
	FlowFieldGraph(int t_row, int t_col);

	void generate();
	std::list<sf::Vector2i> const* getPath(sf::Vector2i t_start);

	void setGoal(sf::Vector2i, bool);
	void setBlocked(sf::Vector2i, bool);
	void clearTile(sf::Vector2i, bool);

	Tile getTile(sf::Vector2i);

	std::vector<std::vector<Tile>>& getTiles();

	void init();
	void draw(sf::RenderTarget& t_target, sf::RenderStates t_states) const override;
	void drawPath(std::list<sf::Vector2i>const* t_path = nullptr);
};

