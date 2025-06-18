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
        _levelManager(stateManager.GetLevelManager()), _map(nullptr), _rocks(), _playerPos(0, 0), _tileSize(64), _hasKey(false)
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
    }

    bool Adventure::isCellFree(int x, int y) const
    {
        char cell = (*_map)[y][x];

        if ((cell == ' ' || cell == 'W' || cell == 'P') && !isRockAt(x, y))
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

        if (newY >= 0 && newY < static_cast<int>(_map->size()) &&
            newX >= 0 && newX < static_cast<int>((*_map)[newY].size())) {
            if (isRockAt(newX, newY)) {
                RockLogic(dx, dy, newX, newY);
            } else if (isCellFree(newX, newY)) {
                _playerPos.x = newX;
                _playerPos.y = newY;
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
            if ((*_map)[_playerPos.y][_playerPos.x] == 'K') {
                _hasKey = true;
                _keyTexture = sf::Texture();
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
        int dx = 0;
        int dy = 0;

        getDirection(dx, dy, key);
        if (_map) {
            MapLogic(dx, dy);
        }
    }

    void Adventure::Update()
    {
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
                        _keySprite.setScale(1.0f, 1.0f);
                        _keySprite.setPosition(offset.x + x * _tileSize, offset.y + y * _tileSize);
                        _stateManager.GetWindow().draw(_keySprite);
                        break;
                    case 'O':
                        _lockSprite.setScale(4.0f, 4.0f);
                        _lockSprite.setPosition(offset.x + x * _tileSize, offset.y + y * _tileSize);
                        _stateManager.GetWindow().draw(_lockSprite);
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

    void Adventure::Display()
    {
        if (!_stateManager.GetWindow().isOpen() || !_map) {
            return;
        }
        sf::Vector2f offset(
            (_stateManager.GetWindow().getSize().x - _map->at(0).size() * _tileSize) / 2,
            (_stateManager.GetWindow().getSize().y - _map->size() * _tileSize) / 2
        );
        _stateManager.GetWindow().clear(sf::Color::Yellow);
        setTiles(offset);
        for (const auto& rock : _rocks) {
            _rockSprite.setScale(4.0f, 4.0f);
            _rockSprite.setPosition(offset.x + rock.x * _tileSize, offset.y + rock.y * _tileSize);
            _stateManager.GetWindow().draw(_rockSprite);
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
