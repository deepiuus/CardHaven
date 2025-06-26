/*
** EPITECH PROJECT, 2025
** TripleTriad [WSL: Debian]
** File description:
** Adventure
*/

#include "Adventure.hpp"

namespace triad
{
    Adventure::Adventure(StateManager &stateManager) : _stateManager(stateManager),
        _levelManager(stateManager.GetLevelManager()), _map(nullptr), _rocks(), _playerPos(0, 0), _tileSize(64), _hasKey(false), _lockOpened(false), _currentSteps(0), _maxSteps(0), _gameOver(false)
    {
    }

    Adventure::~Adventure()
    {
    }

    void Adventure::Init()
    {
        MusicManager::GetInstance().Play("assets/sounds/Vitality.wav");
        initTextures();
        _levelManager.LoadLevel();
        _map = &_levelManager.GetMap();
        _rocks.clear();
        for (size_t y = 0; y < _map->size(); y++) {
            for (size_t x = 0; x < (*_map)[y].size(); x++) {
                if ((*_map)[y][x] == 'P') {
                    _playerPos = sf::Vector2i(x, y);
                }
                if ((*_map)[y][x] == 'X') {
                    _rocks.push_back(sf::Vector2i(x, y));
                }
            }
        }
        _lockOpened = false;
        _hasKey = false;
        _currentSteps = 0;
        _maxSteps = calculateMaxSteps();
        _gameOver = false;
        
        if (!_font.loadFromFile("assets/fonts/upheavtt.ttf")) {
            throw Error("Failed to load font");
        }
        _stepsText.setFont(_font);
        _stepsText.setCharacterSize(24);
        _stepsText.setFillColor(sf::Color::White);
        _stepsText.setPosition(10, 10);
        
        _gameOverText.setFont(_font);
        _gameOverText.setCharacterSize(48);
        _gameOverText.setFillColor(sf::Color::Red);
        _gameOverText.setString("Out of steps! Restarting level...");
    }

    bool Adventure::isCellFree(int x, int y) const
    {
        char cell = (*_map)[y][x];
        if (_lockOpened && cell == 'O' && !isRockAt(x, y))
            return true;
        if ((cell == ' ' || cell == 'W' || cell == 'P' || cell == 'K') && !isRockAt(x, y))
            return true;
        if (cell == 'O' && _hasKey && !isRockAt(x, y))
            return true;
        if (cell == 'X' && !isRockAt(x, y))
            return true;
        return false;
    }

    void Adventure::RockLogic(int dx, int dy, int newX, int newY)
    {
        int rockX = newX + dx;
        int rockY = newY + dy;

        if (rockY >= 0 && rockY < static_cast<int>(_map->size()) &&
            rockX >= 0 && rockX < static_cast<int>((*_map)[rockY].size()) &&
            isCellFree(rockX, rockY) && ((*_map)[rockY][rockX] != 'W')) {
            for (auto& rock : _rocks) {
                if (rock.x == newX && rock.y == newY) {
                    rock.x = rockX;
                    rock.y = rockY;
                    break;
                }
            }
            _playerPos.x = newX;
            _playerPos.y = newY;
        }
    }

    bool Adventure::isRockAt(int x, int y) const
    {
        for (const auto& rock : _rocks) {
            if (rock.x == x && rock.y == y) {
                return true;
            }
        }
        return false;
    }

    void Adventure::MapLogic(int dx, int dy)
    {
        int newX = _playerPos.x + dx;
        int newY = _playerPos.y + dy;
        bool moved = false;

        if (newY >= 0 && newY < static_cast<int>(_map->size()) &&
            newX >= 0 && newX < static_cast<int>((*_map)[newY].size())) {
            if (isRockAt(newX, newY)) {
                int oldX = _playerPos.x;
                int oldY = _playerPos.y;
                RockLogic(dx, dy, newX, newY);
                if (_playerPos.x != oldX || _playerPos.y != oldY) {
                    moved = true;
                }
            } else if (isCellFree(newX, newY)) {
                _playerPos.x = newX;
                _playerPos.y = newY;
                moved = true;
            }
            if (moved) {
                _currentSteps++;
                
                if (_currentSteps >= _maxSteps) {
                    _gameOver = true;
                    _gameOverClock.restart();
                    return;
                }
            }
            
            if ((*_map)[_playerPos.y][_playerPos.x] == 'W') {
                try {
                    _stateManager.RequestStateChange(std::make_unique<Visual>(_stateManager));
                    if (_levelManager.GetLevel() == TLevel::LEVEL5) {
                        _levelManager.ResetLevel();
                        _stateManager.RequestStateChange(std::make_unique<Menu>(_stateManager));
                        return;
                    }
                    return;
                } catch (const Error &e) {
                    _levelManager.ResetLevel();
                    _stateManager.RequestStateChange(std::make_unique<Menu>(_stateManager));
                    return;
                }
            }
            if ((*_map)[_playerPos.y][_playerPos.x] == 'K' && !_hasKey && !_lockOpened) {
                _hasKey = true;
                return;
            }
            if ((*_map)[_playerPos.y][_playerPos.x] == 'O' && _hasKey && !_lockOpened) {
                _hasKey = false;
                _lockOpened = true;
                return;
            }
        }
    }

    void Adventure::getDirection(int &dx, int &dy, TKey key)
    {
        switch (key) {
            case TKey::UP:
                dy = -1;
                break;
            case TKey::DOWN:
                dy = 1;
                break;
            case TKey::LEFT:
                dx = -1;
                break;
            case TKey::RIGHT:
                dx = 1;
                break;
            case TKey::ESCAPE:
                _levelManager.ResetLevel();
                _stateManager.RequestStateChange(std::make_unique<Menu>(_stateManager));
                break;
            default:
                break;
        }
    }

    void Adventure::SetKey(TKey key)
    {
        if (_gameOver) {
            return;
        }
        
        int dx = 0;
        int dy = 0;

        switch (key) {
            case TKey::R:
                _levelManager.ResetLevel();
                Init();
                return;
            case TKey::N1:
                _levelManager.SetLevel(TLevel::LEVEL1);
                Init();
                return;
            case TKey::N2:
                _levelManager.SetLevel(TLevel::LEVEL2);
                Init();
                return;
            case TKey::N3:
                _levelManager.SetLevel(TLevel::LEVEL3);
                Init();
                return;
            case TKey::N4:
                _levelManager.SetLevel(TLevel::LEVEL4);
                Init();
                return;
            case TKey::N5:
                _levelManager.SetLevel(TLevel::LEVEL5);
                Init();
                return;
            default:
                break;
        }

        getDirection(dx, dy, key);
        if (_map) {
            MapLogic(dx, dy);
        }
    }

    void Adventure::Update()
    {
        if (_gameOver) {
            if (_gameOverClock.getElapsedTime().asSeconds() >= 2.0f) {
                _gameOver = false;
                Init();
            }
        }
    }

    void Adventure::setTiles(sf::Vector2f offset)
    {
        for (size_t y = 0; y < _map->size(); y++) {
            for (size_t x = 0; x < (*_map)[y].size(); x++) {
                _floorSprite.setScale(4.0f, 4.0f);
                _floorSprite.setPosition(offset.x + x * _tileSize, offset.y + y * _tileSize);
                _stateManager.GetWindow().draw(_floorSprite);
            }
        }
        for (size_t y = 0; y < _map->size(); y++) {
            for (size_t x = 0; x < (*_map)[y].size(); x++) {
                if (x == _playerPos.x && y == _playerPos.y) {
                    _chadSprite.setScale(1.0f, 1.0f);
                    _chadSprite.setPosition(offset.x + x * _tileSize, offset.y + y * _tileSize);
                    _stateManager.GetWindow().draw(_chadSprite);
                }
                switch ((*_map)[y][x]) {
                    case '#':
                        _wallSprite.setScale(4.0f, 4.0f);
                        _wallSprite.setPosition(offset.x + x * _tileSize, offset.y + y * _tileSize);
                        _stateManager.GetWindow().draw(_wallSprite);
                        break;
                    case 'W':
                        _waifuSprite.setScale(1.0f, 1.0f);
                        _waifuSprite.setPosition(offset.x + x * _tileSize, offset.y + y * _tileSize);
                        _stateManager.GetWindow().draw(_waifuSprite);
                        break;
                    case 'K':
                        if (!_hasKey && !_lockOpened) {
                            _keySprite.setScale(1.0f, 1.0f);
                            _keySprite.setPosition(offset.x + x * _tileSize, offset.y + y * _tileSize);
                            _stateManager.GetWindow().draw(_keySprite);
                        }
                        break;
                    case 'O':
                        if (!_lockOpened) {
                            _lockSprite.setScale(4.0f, 4.0f);
                            _lockSprite.setPosition(offset.x + x * _tileSize, offset.y + y * _tileSize);
                            _stateManager.GetWindow().draw(_lockSprite);
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }

    void Adventure::initTextures()
    {
        _floorTexture.loadFromFile("assets/sprites/floor.png");
        _wallTexture.loadFromFile("assets/sprites/wall.png");
        _rockTexture.loadFromFile("assets/sprites/rock.png");
        _waifuTexture.loadFromFile("assets/sprites/WaifuChibi.png");
        _chadTexture.loadFromFile("assets/sprites/ChadChibi.png");
        _keyTexture.loadFromFile("assets/sprites/Key.png");
        _lockTexture.loadFromFile("assets/sprites/Lock.png");
        _floorSprite.setTexture(_floorTexture);
        _wallSprite.setTexture(_wallTexture);
        _rockSprite.setTexture(_rockTexture);
        _waifuSprite.setTexture(_waifuTexture);
        _chadSprite.setTexture(_chadTexture);
        _keySprite.setTexture(_keyTexture);
        _lockSprite.setTexture(_lockTexture);
    }

    
    int Adventure::calculateMaxSteps() const
    {
        TDifficulty difficulty = _levelManager.GetDifficulty();
        TLevel level = _levelManager.GetLevel();
        
        int baseSteps = 0;
        
        switch (level) {
            case TLevel::LEVEL1: baseSteps = 25; break;
            case TLevel::LEVEL2: baseSteps = 25; break;
            case TLevel::LEVEL3: baseSteps = 35; break;
            case TLevel::LEVEL4: baseSteps = 25; break;
            case TLevel::LEVEL5: baseSteps = 30; break;
        }
        
        switch (difficulty) {
            case TDifficulty::EASY:
                return baseSteps + 10;
            case TDifficulty::NORMAL:
                return baseSteps + 5;
            case TDifficulty::HARD:
                return baseSteps - 5;
        }
        
        return baseSteps;
    }

    void Adventure::Display()
    {
        if (!_stateManager.GetWindow().isOpen() || !_map) {
            return;
        }
        sf::Vector2f offset(
            (_stateManager.GetWindow().getSize().x - _map->at(0).size() * _tileSize) / 2,
            (_stateManager.GetWindow().getSize().y - _map->size() * _tileSize) / 2
        );
        _stateManager.GetWindow().clear(sf::Color::Black);
        setTiles(offset);
        for (const auto& rock : _rocks) {
            _rockSprite.setScale(4.0f, 4.0f);
            _rockSprite.setPosition(offset.x + rock.x * _tileSize, offset.y + rock.y * _tileSize);
            _stateManager.GetWindow().draw(_rockSprite);
        }
        _stepsText.setString("Steps: " + std::to_string(_currentSteps) + " / " + std::to_string(_maxSteps));
        if (_currentSteps > _maxSteps * 0.8f) {
            _stepsText.setFillColor(sf::Color::Red);
        } else {
            _stepsText.setFillColor(sf::Color::White);
        }
        _stateManager.GetWindow().draw(_stepsText);
        if (_gameOver) {
            _gameOverText.setPosition(
                _stateManager.GetWindow().getSize().x / 2 - _gameOverText.getGlobalBounds().width / 2,
                _stateManager.GetWindow().getSize().y / 2 - _gameOverText.getGlobalBounds().height / 2
            );
            _stateManager.GetWindow().draw(_gameOverText);
        }
    }

    void Adventure::Destroy()
    {
        _floorSprite.setTexture(sf::Texture());
        _wallSprite.setTexture(sf::Texture());
        _rockSprite.setTexture(sf::Texture());
        _waifuSprite.setTexture(sf::Texture());
        _chadSprite.setTexture(sf::Texture());
        _keySprite.setTexture(sf::Texture());
        _lockSprite.setTexture(sf::Texture());
        _floorTexture = sf::Texture();
        _wallTexture = sf::Texture();
        _rockTexture = sf::Texture();
        _waifuTexture = sf::Texture();
        _chadTexture = sf::Texture();
        _floorSprite = sf::Sprite();
        _wallSprite = sf::Sprite();
        _rockSprite = sf::Sprite();
        _waifuSprite = sf::Sprite();
        _chadSprite = sf::Sprite();
        _keySprite = sf::Sprite();
        _lockSprite = sf::Sprite();
    }
}
