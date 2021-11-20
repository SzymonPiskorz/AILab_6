#include "FlowFieldGraph.h"

FlowFieldGraph::FlowFieldGraph(int t_row, int t_col)
{
	m_tiles.resize(t_col);
	for (auto& row : m_tiles)
		row.resize(t_col);

	init();
}

void FlowFieldGraph::setNeighbourCosts(sf::Vector2i t_current, sf::Vector2i t_goal)
{
	for (int dir = 0; dir < 9; dir++)
	{
		if (dir == 4) continue;

		int n_row = t_current.x + (dir / 3) - 1;
		int n_col = t_current.y + (dir % 3) - 1;

		if (n_row >= 0 && n_col >= 0 && n_row < m_tiles.size() && n_col < m_tiles.at(n_row).size())
		{
			if (m_tiles.at(n_row).at(n_col).m_cost != 0 || (t_goal.x == n_row && t_goal.y == n_col))
				continue;

			int cost = m_tiles.at(t_current.x).at(t_current.y).m_cost + 1;
			m_tiles.at(n_row).at(n_col).m_cost = cost;

			float distX = static_cast<float>(t_goal.x) - n_row;
			float distY = static_cast<float>(t_goal.y) - n_col;
			float distance = sqrt(distX * distX + distY * distY);

			m_tiles.at(n_row).at(n_col).integratedCost = static_cast<float>(cost) * 100 + distance;

			m_tileCostQueue.push(sf::Vector2i{ n_row, n_col });
		}
	}
}

sf::Vector2i FlowFieldGraph::bestNeighbour(sf::Vector2i t_tile)
{
	int bestDir = -1;
	float bestDirVal = 0.0f;

	for (int dir = 0; dir < 9; dir++)
	{
		if (dir == 4) continue;

		int n_row = t_tile.x + (dir / 3) - 1;
		int n_col = t_tile.y + (dir % 3) - 1;

		if (n_row >= 0 && n_col >= 0 && n_row < m_tiles.size() && n_col < m_tiles.at(n_row).size())
		{
			if (bestDir == -1 || m_tiles.at(n_row).at(n_col).integratedCost < bestDirVal)
			{
				bestDir = dir;
				bestDirVal = m_tiles.at(n_row).at(n_col).integratedCost;
			}
		}
	}

	return sf::Vector2i(t_tile.x + (bestDir / 3) - 1, t_tile.y + (bestDir % 3) - 1);
}

void FlowFieldGraph::createFlowField()
{
	for (int row = 0; row < m_tiles.size(); row++)
	{
		for (int col = 0; col < m_tiles.size(); col++)
		{
			if (m_tiles.at(row).at(col).m_cost == std::numeric_limits<int>::max())
			{
				m_tiles.at(row).at(col).m_neighbour = sf::Vector2i(row, col);
			}
			else
			{
				m_tiles.at(row).at(col).m_neighbour = bestNeighbour(sf::Vector2i(row, col));
			}
		}
	}
}

void FlowFieldGraph::generate()
{
	for (auto& row : m_tiles)
		for (auto& tile : row)
			if (tile.m_cost != std::numeric_limits<int>::max())
				tile.m_cost = 0;

	m_tiles.at(m_goal.x).at(m_goal.y).m_cost = 0;
	m_tiles.at(m_goal.x).at(m_goal.y).integratedCost = 0.0f;
	m_tiles.at(m_goal.x).at(m_goal.y).m_neighbour = m_goal;

	m_tileCostQueue.push(m_goal);

	while (!m_tileCostQueue.empty())
	{
		sf::Vector2i tile = m_tileCostQueue.front();
		m_tileCostQueue.pop();

		setNeighbourCosts(tile, m_goal);
	}

	createFlowField();
}

std::list<sf::Vector2i> const* FlowFieldGraph::getPath(sf::Vector2i t_start)
{
	m_start = t_start;
	if (t_start.x >= m_tiles.size() && t_start.y >= m_tiles.at(0).size())
	{
		return nullptr;
	}

	if (getTile(t_start).m_cost == std::numeric_limits<int>::max() && t_start == m_goal)
	{
		return nullptr;
	}

	std::list<sf::Vector2i>* path = new std::list<sf::Vector2i>();
	path->push_back(getTile(t_start).m_neighbour);

	while (path->back() != m_goal)
	{
		path->push_back(getTile(path->back()).m_neighbour);
	}

	return path;
	
}

void FlowFieldGraph::setGoal(sf::Vector2i t_pos, bool t_gen)
{
	if (t_pos.x < m_tiles.size() && t_pos.y < m_tiles.at(0).size())
	{
		m_goal = t_pos;

		if (t_gen)
			generate();
	}
}

void FlowFieldGraph::setBlocked(sf::Vector2i t_pos, bool t_gen)
{
	if (t_pos.x < m_tiles.size() && t_pos.y < m_tiles.at(0).size())
	{
		m_tiles.at(t_pos.x).at(t_pos.y).m_cost = std::numeric_limits<int>::max();
		m_tiles.at(t_pos.x).at(t_pos.y).integratedCost = std::numeric_limits<float>::max();

		if (t_gen)
			generate();
	}
}

void FlowFieldGraph::clearTile(sf::Vector2i t_pos, bool t_gen)
{
	if (t_pos.x < m_tiles.size() && t_pos.y < m_tiles.at(0).size())
	{
		m_tiles.at(t_pos.x).at(t_pos.y).m_cost = 0;

		if (t_gen)
			generate();
	}
}

Tile FlowFieldGraph::getTile(sf::Vector2i t_pos)
{
	return m_tiles.at(t_pos.x).at(t_pos.y);
}

std::vector<std::vector<Tile>>& FlowFieldGraph::getTiles()
{
	return m_tiles;
}

void FlowFieldGraph::init()
{
	if (!m_font.loadFromFile("ASSETS//FONTS//ariblk.ttf"))
	{
		std::cout << "Error loading font" << std::endl;
	}

	m_text.setFont(m_font);
	m_text.setCharacterSize(12u);

	sf::Vector2i cellSize{ 24, 24 };

	m_renderTexture.create(50 * cellSize.x, 50 * cellSize.y);

	m_renderTextureSprite.setTexture(m_renderTexture.getTexture());
}

void FlowFieldGraph::draw(sf::RenderTarget& t_target, sf::RenderStates t_states) const
{
	t_target.draw(m_renderTextureSprite, t_states);
}

void FlowFieldGraph::drawPath(std::list<sf::Vector2i>const* t_path)
{
	m_renderTexture.clear();

	auto& tiles = getTiles();

	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f{ 24, 24 });

	sf::VertexArray lines{ sf::Lines };

	for (int x = 0; x < tiles.size(); ++x)
	{
		for (int y = 0; y < tiles.at(x).size(); ++y)
		{
			// Continues to the next cell if a wall.
			if (tiles.at(x).at(y).m_cost == std::numeric_limits<int>::max()) continue;

			// Takes some cell info for ease of access.
			int cost = tiles.at(x).at(y).m_cost;
			sf::Vector2f position{ static_cast<float>(x) * 24.0f, static_cast<float>(y) * 24.0f};

			// Sets up the text for this cell.
			m_text.setPosition(position + sf::Vector2f{ 24, 24 }/ 2.0f);
			m_text.setString(std::to_string(cost));
			sf::FloatRect rect = m_text.getGlobalBounds();
			m_text.setOrigin(rect.width / 2.0f, rect.height / 2.0f);

			// Sets up the rect for this cell.
			rectangle.setPosition(position);
			uint8_t value = std::max(50 - static_cast<int>(cost), 0) * 5u;
			rectangle.setFillColor(sf::Color{ 0u, 0u, value });

			// Draws the text and rect.
			m_renderTexture.draw(rectangle);
			m_renderTexture.draw(m_text);

			// Adds the flow lines.
			sf::Vector2f neighbour = static_cast<sf::Vector2f>(tiles.at(x).at(y).m_neighbour);
			sf::Vector2f neighbourPosition = sf::Vector2f(static_cast<float>(neighbour.x) * 24 ,static_cast<float>(neighbour.y) * 24);

			// Adds the start of the line as the tile position.
			lines.append({ position + sf::Vector2f{ 24, 24 } / 2.0f, sf::Color::Blue });

			// Adds the end of the line as the best neighbours position.
			lines.append({ neighbourPosition + sf::Vector2f{ 24, 24 } / 2.0f, sf::Color::White });
		}
	}

	if (t_path)
	{
		for (auto it = t_path->begin(); it != t_path->end(); ++it)
		{
			sf::Vector2f position{ static_cast<float>(it->x) * 24.0f,static_cast<float>(it->y) * 24.0f};
			rectangle.setPosition(position);
			rectangle.setFillColor(sf::Color{ 0, 255, 0 });
			m_renderTexture.draw(rectangle);
		}
	}

	m_renderTexture.draw(lines);
	m_renderTexture.display();
	sf::FloatRect bounds = m_renderTextureSprite.getGlobalBounds();
}
