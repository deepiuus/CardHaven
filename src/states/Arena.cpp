/*
** EPITECH PROJECT, 2025
** TripleTriad [WSL: Debian]
** File description:
** Arena
*/

#include "Arena.hpp"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

namespace triad
{
    Arena::Arena(StateManager &stateManager)
        : _stateManager(stateManager), width(800), height(600), _endGame(false), _fromMenu(false),
          _playerCount(0), _ennemyCount(0), _occupiedCount(0), _cardSpacing(30), _cardY(60)
    {
    }

    Arena::~Arena()
    {
    }

    void Arena::SetupCards(int cardSpacing, int cardY)
    {
        sf::Sprite sprite1;
        sf::Sprite sprite2;

        _player1Deck = {
            &CardManager::GetInstance().GetCard(1),
            &CardManager::GetInstance().GetCard(2),
            &CardManager::GetInstance().GetCard(3),
            &CardManager::GetInstance().GetCard(4),
            &CardManager::GetInstance().GetCard(5),
        };
        _player2Deck = {
            &CardManager::GetInstance().GetCard(6),
            &CardManager::GetInstance().GetCard(7),
            &CardManager::GetInstance().GetCard(8),
            &CardManager::GetInstance().GetCard(9),
            &CardManager::GetInstance().GetCard(10),
        };
        _player1Cards.clear();
        _player2Cards.clear();
        for (int i = 0; i < 5; i++) {
            sprite1.setTexture(_player1Deck[i]->GetTexture());
            sprite1.setPosition(30, cardY + i * (60 + cardSpacing));
            sprite1.setColor(sf::Color(100, 100, 255));
            _player1Cards.push_back(sprite1);
            sprite2.setTexture(_player2Deck[i]->GetTexture());
            sprite2.setPosition(
                width - 30 - sprite2.getTexture()->getSize().x,
                cardY + i * (60 + cardSpacing));
            sprite2.setColor(sf::Color(255, 100, 100));
            _player2Cards.push_back(sprite2);
        }
    }

    void Arena::SetupBoard(float cellSize, float cellGap, float gridStartX, float gridStartY)
    {
        if (!_texture.loadFromFile("assets/sprites/Board.png")) {
            throw Error("Failed to load texture");
        }
        for (int y = 0; y < 3; y++)
            for (int x = 0; x < 3; x++) {
                _boardGrid[y][x] = sf::FloatRect(
                    gridStartX + x * (cellSize + cellGap),
                    gridStartY + y * (cellSize + cellGap),
                    cellSize,
                    cellSize
                );
                _boardOccupancy[y][x] = {-1, nullptr};
                _boardSprites[y][x] = nullptr;
            }
    }

   void Arena::Init()
    {
        float cellSize = 120;
        float cellGap = 5;
        float gridWidth = 3 * cellSize + 2 * cellGap;
        float gridHeight = 3 * cellSize + 2 * cellGap;
        float gridStartX = width / 2 - gridWidth / 2;
        float gridStartY = height / 2 - gridHeight / 2;

        MusicManager::GetInstance().Play("assets/sounds/Epitomize.wav");
        SetupBoard(cellSize, cellGap, gridStartX, gridStartY);
        SetupCards(_cardSpacing, _cardY);
        _currentTurn = 1;
        if (!_font.loadFromFile("assets/fonts/upheavtt.ttf")) {
            throw Error("Failed to load font");
        }
        _turnText.setFont(_font);
        _turnText.setCharacterSize(36);
        _turnText.setFillColor(sf::Color::White);
        _turnText.setPosition(width / 2 - 130, 10);
        _turnText.setString(_currentTurn == 1 ? "Player 1 turn" : "Player 2 turn");
        _endGameText.setFont(_font);
        _endGameText.setCharacterSize(48);
        _endGameText.setFillColor(sf::Color::Yellow);
        _endGameText.setString("");
        _endGameText.setPosition(width / 2 - 120, height / 2 - 120);
    }

    void Arena::ResetCard()
    {
        if (_draggedCard.x == 0)
            _player1Cards[_draggedCard.y].setPosition(30,
                _cardY + _draggedCard.y * (60 + _cardSpacing));
        else if (_draggedCard.x == 1)
            _player2Cards[_draggedCard.y].setPosition(
                width - 30 - _player2Cards[_draggedCard.y].getTexture()->getSize().x,
                _cardY + _draggedCard.y * (60 + _cardSpacing));
    }

    
    bool Arena::SetCapturedCard(int x, int y, int nx, int ny, int myValue, int theirValue, const char* dirName)
    {
        if (nx < 0 || nx > 2 || ny < 0 || ny > 2)
            return false;
        int currentPlayer = _boardOccupancy[y][x].owner;
        int adjPlayer = _boardOccupancy[ny][nx].owner;
        const Card *currentCard = _boardOccupancy[y][x].card;
        const Card *adjCard = _boardOccupancy[ny][nx].card;
        if (adjPlayer != -1 && adjPlayer != currentPlayer) {
            if (myValue > theirValue) {
                _boardOccupancy[ny][nx].owner = currentPlayer;
                if (_boardSprites[ny][nx])
                    _boardSprites[ny][nx]->setColor(currentPlayer == 0 ? sf::Color(100, 100, 255) : sf::Color(255, 100, 100));
                printf("%s captured with %s the card %s from the %s!\n",
                    currentPlayer == 0 ? "Player" : "Ennemy",
                    currentCard->GetName().c_str(),
                    adjCard->GetName().c_str(),
                    dirName);
                return true;
            }
        }
        return false;
    }

    void Arena::CaptureCard(int x, int y)
    {
        const Card *currentCard = _boardOccupancy[y][x].card;
        _playerCount = 0;
        _ennemyCount = 0;
        _occupiedCount = 0;
        if (y > 0)
            SetCapturedCard(x, y, x, y-1, currentCard->GetTop(), _boardOccupancy[y-1][x].card ? _boardOccupancy[y-1][x].card->GetBottom() : -1, "top");
        if (y < 2)
            SetCapturedCard(x, y, x, y+1, currentCard->GetBottom(), _boardOccupancy[y+1][x].card ? _boardOccupancy[y+1][x].card->GetTop() : -1, "bottom");
        if (x > 0)
            SetCapturedCard(x, y, x-1, y, currentCard->GetLeft(), _boardOccupancy[y][x-1].card ? _boardOccupancy[y][x-1].card->GetRight() : -1, "left");
        if (x < 2)
            SetCapturedCard(x, y, x+1, y, currentCard->GetRight(), _boardOccupancy[y][x+1].card ? _boardOccupancy[y][x+1].card->GetLeft() : -1, "right");
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (_boardOccupancy[i][j].owner == 0)
                    _playerCount++;
                else if (_boardOccupancy[i][j].owner == 1)
                    _ennemyCount++;
                if (_boardOccupancy[i][j].owner != -1)
                    _occupiedCount++;
            }
        }
        printf("Player has %d cards, Ennemy has %d cards\n", _playerCount, _ennemyCount);
        if (_occupiedCount == 9) {
            if (_playerCount > _ennemyCount) {
                printf("Player wins\n");
                _endGameText.setString("Victory !");
                _endGameText.setFillColor(sf::Color(100, 255, 100));
            } else if (_ennemyCount > _playerCount) {
                printf("Ennemy wins\n");
                _endGameText.setString("Defeat...");
                _endGameText.setFillColor(sf::Color(255, 100, 100));
            } else {
                printf("It's a draw\n");
                _endGameText.setString("Draw !");
                _endGameText.setFillColor(sf::Color(200, 200, 200));
            }
            _endGameText.setPosition(width / 2 - _endGameText.getLocalBounds().width / 2, height / 2 - 50);
            _endGame = true;
            _endGameClock.restart();
            return;
        }
    }

    void Arena::PlaceCard(int x, int y)
    {
        sf::Vector2f cellCenter = {
            _boardGrid[y][x].left + _boardGrid[y][x].width / 2.f,
            _boardGrid[y][x].top + _boardGrid[y][x].height / 2.f
        };

        if (_draggedCard.x == 0) {
            _player1Cards[_draggedCard.y].setPosition(
                cellCenter.x - _player1Cards[_draggedCard.y].getTexture()->getSize().x / 2.f,
                cellCenter.y - _player1Cards[_draggedCard.y].getTexture()->getSize().y / 2.f
            );
            _boardSprites[y][x] = &_player1Cards[_draggedCard.y];
            _boardOccupancy[y][x] = {0, _player1Deck[_draggedCard.y]};
        } else {
            _player2Cards[_draggedCard.y].setPosition(
                cellCenter.x - _player2Cards[_draggedCard.y].getTexture()->getSize().x / 2.f,
                cellCenter.y - _player2Cards[_draggedCard.y].getTexture()->getSize().y / 2.f
            );
            _boardSprites[y][x] = &_player2Cards[_draggedCard.y];
            _boardOccupancy[y][x] = {1, _player2Deck[_draggedCard.y]};
        }
        _currentTurn = 1 - _currentTurn;
        _turnText.setString(_currentTurn == 1 ? "Player 1 turn" : "Player 2 turn");
    }

    void Arena::OccupyCell()
    {
        int foundX = -1;
        int foundY = -1;

        if (_draggedCard.x < 0 || _draggedCard.y < 0 ||
            _draggedCard.x > 1 || _draggedCard.y >= 5)
            return;
        sf::FloatRect bounds = (_draggedCard.x == 0)
            ? _player1Cards[_draggedCard.y].getGlobalBounds()
            : _player2Cards[_draggedCard.y].getGlobalBounds();

        if (!_dragging)
            return;
        for (int y = 0; y < 3; y++)
            for (int x = 0; x < 3; x++)
                if (_boardGrid[y][x].intersects(bounds) && _boardOccupancy[y][x].owner == -1) {
                    foundX = x;
                    foundY = y;
                }
        if (foundX != -1 && foundY != -1) {
            PlaceCard(foundX, foundY);
            CaptureCard(foundX, foundY);
        } else {
            ResetCard();
        }
        _dragging = false;
        _draggedCard = {-1, -1};
    }

    bool Arena::isCardOnBoard(const Card* card) const
    {
        for (int y = 0; y < 3; ++y)
            for (int x = 0; x < 3; ++x)
                if (_boardOccupancy[y][x].card == card)
                    return true;
        return false;
    }

    void Arena::SetDraggedCards(std::vector<sf::Sprite> &cards, std::vector<const Card *> &deck, int playerIndex, const sf::Vector2i &mousePos)
    {
        for (size_t i = 0; i < cards.size(); i++) {
            if (isCardOnBoard(deck[i]))
                continue;
            if (cards[i].getGlobalBounds().contains(
                static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                _dragging = true;
                _draggedCard = {playerIndex, static_cast<int>(i)};
                _dragOffset = cards[i].getPosition() - sf::Vector2f(mousePos);
                break;
            }
        }
    }

    void Arena::DraggingCard()
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(_stateManager.GetWindow());
        if (_currentTurn == 1) {
            SetDraggedCards(_player1Cards, _player1Deck, 0, mousePos);
        } else {
            SetDraggedCards(_player2Cards, _player2Deck, 1, mousePos);
        }
    }

    void Arena::SetKey(TKey key)
    {
        if (key == TKey::LCLICK) {
            DraggingCard();
        }
        if (key == TKey::ESCAPE) {
            _stateManager.RequestStateChange(std::make_unique<Menu>(_stateManager));
        }
        if (key == TKey::NONE) {
            OccupyCell();
        }
    }

    int Arena::GetHoveredId(const std::vector<sf::Sprite>& cards, const std::vector<const Card*>& deck, float baseX, float baseY, int cardCount, const sf::Vector2i& mousePos) const
    {
        for (int i = cardCount - 1; i >= 0; --i) {
            if (isCardOnBoard(deck[i]))
                continue;
            float x = baseX;
            float y = baseY + i * (60 + _cardSpacing);
            sf::FloatRect bounds(x, y, cards[i].getTexture()->getSize().x, cards[i].getTexture()->getSize().y);
            if (bounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                return i;
        }
        return -1;
    }

    void Arena::SetHoveredStyle(sf::Sprite& card, bool hovered, float baseX, float baseY, sf::Color normalColor, sf::Color hoverColor) const
    {
        if (hovered) {
            card.setScale(1.2f, 1.2f);
            card.setColor(hoverColor);
            card.setPosition(baseX - card.getTexture()->getSize().x * 0.1f,
                             baseY - card.getTexture()->getSize().y * 0.1f);
        } else {
            card.setScale(1.f, 1.f);
            card.setColor(normalColor);
            card.setPosition(baseX, baseY);
        }
    }

    void Arena::SetHoveredCards(std::vector<sf::Sprite>& cards, const std::vector<const Card*>& deck, float baseX, sf::Color normalColor, sf::Color hoverColor, int hoveredIdx) const
    {
        for (size_t i = 0; i < cards.size(); ++i) {
            if (isCardOnBoard(deck[i]))
                continue;
            float y = _cardY + i * (60 + _cardSpacing);
            SetHoveredStyle(cards[i], static_cast<int>(i) == hoveredIdx, baseX, y, normalColor, hoverColor);
        }
    }

    void Arena::DrawCards(sf::RenderWindow& window, const std::vector<sf::Sprite>& cards, const std::vector<const Card*>& deck, int hoveredIdx, bool drawHoveredLast) const
    {
        for (size_t i = 0; i < cards.size(); ++i) {
            if (isCardOnBoard(deck[i]))
                continue;
            if (drawHoveredLast && static_cast<int>(i) == hoveredIdx)
                continue;
            window.draw(cards[i]);
        }
        if (drawHoveredLast && hoveredIdx != -1)
            window.draw(cards[hoveredIdx]);
    }

    void Arena::Update()
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(_stateManager.GetWindow());
        int hoveredId = -1;
        if (_currentTurn == 1) {
            hoveredId = GetHoveredId(_player1Cards, _player1Deck, 30, _cardY, _player1Cards.size(), mousePos);
            SetHoveredCards(_player1Cards, _player1Deck, 30, sf::Color(100, 100, 255), sf::Color(180, 180, 255), hoveredId);
            SetHoveredCards(_player2Cards, _player2Deck, width - 30 - (_player2Cards.empty() ? 0 : _player2Cards[0].getTexture()->getSize().x), sf::Color(255, 100, 100), sf::Color(255, 180, 180), -1);
        } else {
            hoveredId = GetHoveredId(_player2Cards, _player2Deck, width - 30 - (_player2Cards.empty() ? 0 : _player2Cards[0].getTexture()->getSize().x), _cardY, _player2Cards.size(), mousePos);
            SetHoveredCards(_player2Cards, _player2Deck, width - 30 - (_player2Cards.empty() ? 0 : _player2Cards[0].getTexture()->getSize().x), sf::Color(255, 100, 100), sf::Color(255, 180, 180), hoveredId);
            SetHoveredCards(_player1Cards, _player1Deck, 30, sf::Color(100, 100, 255), sf::Color(180, 180, 255), -1);
        }
        if (_dragging) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(_stateManager.GetWindow());
            if (_draggedCard.x == 0)
                _player1Cards[_draggedCard.y].setPosition(sf::Vector2f(mousePos) + _dragOffset);
            else if (_draggedCard.x == 1)
                _player2Cards[_draggedCard.y].setPosition(sf::Vector2f(mousePos) + _dragOffset);
        }
        if (_endGame) {
            if (_endGameClock.getElapsedTime().asSeconds() >= 1.5f) {
                _endGame = false;
                if (_fromMenu) {
                    auto arena = std::make_unique<Arena>(_stateManager);
                    arena->SetFromMenu(true);
                    _stateManager.RequestStateChange(std::move(arena));
                } else if (_playerCount > _ennemyCount) {
                    _stateManager.GetLevelManager().NextLevel();
                    _stateManager.RequestStateChange(std::make_unique<Adventure>(_stateManager));
                } else if (_ennemyCount > _playerCount) {
                    _stateManager.GetLevelManager().ResetLevel();
                    _stateManager.RequestStateChange(std::make_unique<Adventure>(_stateManager));
                } else {
                    _stateManager.RequestStateChange(std::make_unique<Menu>(_stateManager));
                }
                return;
            }
        }
    }

    void Arena::SetScore()
    {
        float total = _playerCount + _ennemyCount;
        float barWidth = width * 0.8f;
        float barHeight = 15.f;
        float barX = width * 0.1f;
        float barY = 560.f;
        float p1Width = (total > 0 ? barWidth * (_playerCount / total) : 0);
        float p2Width = (total > 0 ? barWidth * (_ennemyCount / total) : 0);
        sf::RectangleShape p1Bar(sf::Vector2f(p1Width, barHeight));
        p1Bar.setPosition(barX, barY);
        p1Bar.setFillColor(sf::Color(100, 100, 255));
        sf::RectangleShape p2Bar(sf::Vector2f(p2Width, barHeight));
        p2Bar.setPosition(barX + p1Width, barY);
        p2Bar.setFillColor(sf::Color(255, 100, 100));
        _stateManager.GetWindow().draw(p1Bar);
        _stateManager.GetWindow().draw(p2Bar);
        sf::Text scoreText;
        scoreText.setFont(_font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setString(std::to_string(_playerCount) + " - " + std::to_string(_ennemyCount));
        sf::FloatRect textRect = scoreText.getLocalBounds();
        scoreText.setPosition(barX + barWidth / 2 - textRect.width / 2, barY - textRect.height + 2);
        _stateManager.GetWindow().draw(scoreText);
    }

    void Arena::Display()
    {
        sf::RenderWindow &window = _stateManager.GetWindow();
        int hoveredId = -1;
        window.clear(sf::Color::Blue);
        _sprite.setTexture(_texture);
        _sprite.setPosition(width / 2 - _texture.getSize().x / 2, height / 2 - _texture.getSize().y / 2);
        _sprite.setScale(1.0f, 1.0f);
        window.draw(_sprite);
        window.draw(_turnText);
        SetScore();
        for (int y = 0; y < 3; ++y) {
            for (int x = 0; x < 3; ++x) {
                if (_boardSprites[y][x])
                    window.draw(*_boardSprites[y][x]);
            }
        }
        sf::Vector2i mousePos = sf::Mouse::getPosition(_stateManager.GetWindow());
        if (_currentTurn == 1) {
            hoveredId = GetHoveredId(_player1Cards, _player1Deck, 30, _cardY, _player1Cards.size(), mousePos);
            DrawCards(window, _player1Cards, _player1Deck, hoveredId, true);
            DrawCards(window, _player2Cards, _player2Deck, -1, false);
        } else {
            hoveredId = GetHoveredId(_player2Cards, _player2Deck, width - 30 - (_player2Cards.empty() ? 0 : _player2Cards[0].getTexture()->getSize().x), _cardY, _player2Cards.size(), mousePos);
            DrawCards(window, _player1Cards, _player1Deck, -1, false);
            DrawCards(window, _player2Cards, _player2Deck, hoveredId, true);
        }
        if (_endGame && !_endGameText.getString().isEmpty())
            window.draw(_endGameText);
    }

    void Arena::Destroy()
    {
    }

    void Arena::SetFromMenu(bool fromMenu)
    {
        _fromMenu = fromMenu;
    }
}
