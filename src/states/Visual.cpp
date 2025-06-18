/*
** EPITECH PROJECT, 2025
** TripleTriad [WSL: Debian]
** File description:
** Visual
*/

#include "Visual.hpp"

namespace triad
{
    Visual::Visual(StateManager &stateManager)
        : _width(800), _height(600), _currentFace(0), _stateManager(stateManager),
          _levelManager(stateManager.GetLevelManager())
    {
    }

    Visual::~Visual()
    {
    }

    void Visual::Init()
    {
        MusicManager::GetInstance().Play("assets/sounds/Titanium.wav");
        initTextures();
        if (!_font.loadFromFile("assets/fonts/upheavtt.ttf")) {
            throw Error("Failed to load font");
        }
        _levelManager.StartDialogue();
        _text.setFont(_font);
        _text.setCharacterSize(24);
        _text.setFillColor(sf::Color::White);
    }

    void Visual::SetKey(TKey key)
    {
        switch (key) {
            case TKey::ESCAPE:
                _levelManager.ResetLevel();
                _stateManager.RequestStateChange(std::make_unique<Menu>(_stateManager));
                return;
            case TKey::ENTER:
            case TKey::SPACE:
            case TKey::LCLICK:
                if (!_levelManager.IsDialogueFinished()) {
                    if (_levelManager.GetDialogueIndex() == 0)
                        _currentFace = 4;
                    else if (_levelManager.GetDialogueIndex() == 1)
                        _currentFace = 2;
                    _levelManager.NextDialogue();
                } else {
                    _stateManager.RequestStateChange(std::make_unique<Arena>(_stateManager));
                    return;
                }
                break;
            default:
                break;
        }
    }

    void Visual::Update()
    {
    }

    void Visual::initTextures()
    {
        _orokaTexture.loadFromFile("assets/sprites/Oroka/Body/OrokaBase.png");
        _orokaFaceTexture.loadFromFile("assets/sprites/Oroka/Face/OrokaFace.png");
        _orokaPoutTextures.clear();
        for (int i = 1; i < 5; ++i) {
            _orokaPoutTextures.emplace_back();
            _orokaPoutTextures.back().loadFromFile("assets/sprites/Oroka/Face/OrokaPout" + std::to_string(i) + ".png");
        }
        _orokaSprite.setTexture(_orokaTexture);
        _orokaFaceSprite.setTexture(_orokaFaceTexture);
    }

    void Visual::Display()
    {
        if (!_stateManager.GetWindow().isOpen()) {
            return;
        }
        _stateManager.GetWindow().clear(sf::Color::Black);
        _orokaSprite.setScale(3.0f, 3.0f);
        _orokaSprite.setPosition(
            _width / 2 - _orokaTexture.getSize().x * 3 / 2,
            _height / 2 - _orokaTexture.getSize().y / 2
        );
        _stateManager.GetWindow().draw(_orokaSprite);
        sf::Sprite headSprite;
        if (_currentFace == 0) {
            headSprite = _orokaFaceSprite;
        } else if (_currentFace > 0 && static_cast<size_t>(_currentFace) <= _orokaPoutTextures.size()) {
            headSprite.setTexture(_orokaPoutTextures[_currentFace - 1]);
        } else {
            headSprite = _orokaFaceSprite;
        }
        headSprite.setScale(3.0f, 3.0f);
        headSprite.setPosition(_orokaSprite.getPosition().x + 45, _orokaSprite.getPosition().y + 30);
        _stateManager.GetWindow().draw(headSprite);
        if (!_levelManager.IsDialogueFinished()) {
            _text.setString(_levelManager.GetDialogue());
            _text.setPosition(10, 10);
            _stateManager.GetWindow().draw(_text);
        }
    }

    void Visual::Destroy()
    {
        _text = sf::Text();
        _font = sf::Font();
    }
}
