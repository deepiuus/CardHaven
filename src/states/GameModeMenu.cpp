/*
** EPITECH PROJECT, 2025
** TripleTriad [WSL: Debian]
** File description:
** GameModeMenu
*/

#include "GameModeMenu.hpp"
#include "Menu.hpp"

namespace triad
{
    GameModeMenu::GameModeMenu(StateManager &stateManager)
        : _stateManager(stateManager), width(800), height(600), _selectedOption(0),
        _optionSelected(false)
    {
        _options = {"Player vs AI", "Player vs Player", "Back"};
        _descriptions = {
            "Play against a bot",
            "Play against each other",
        };
    }

    GameModeMenu::~GameModeMenu()
    {
    }

    void GameModeMenu::Init()
    {
        if (!font.loadFromFile("assets/fonts/upheavtt.ttf")) {
            throw Error("Failed to load font");
        }
    }

    void GameModeMenu::SetKey(TKey key)
    {
        switch (key) {
            case TKey::UP:
                _selectedOption = (_selectedOption - 1 + _options.size()) % _options.size();
                break;
            case TKey::DOWN:
                _selectedOption = (_selectedOption + 1) % _options.size();
                break;
            case TKey::ESCAPE:
                _selectedOption = 2;
                _optionSelected = true;
                break;
            case TKey::ENTER:
            case TKey::LCLICK:
            case TKey::RCLICK:
                _optionSelected = true;
                break;
            default:
                break;
        }
    }

    int GameModeMenu::GetSelectedOption() const
    {
        return _selectedOption;
    }

    bool GameModeMenu::IsOptionSelected() const
    {
        return _optionSelected;
    }

    void GameModeMenu::Update()
    {
        if (!_stateManager.GetWindow().isOpen()) {
            _stateManager.RequestStateChange(nullptr);
            return;
        }
        if (_optionSelected) {
            switch (_selectedOption) {
                case 0: {
                    auto arena = std::make_unique<Arena>(_stateManager);
                    arena->SetFromMenu(true);
                    arena->SetGameMode(TGameMode::PLAYER_VS_AI);
                    _stateManager.RequestStateChange(std::move(arena));
                    break;
                }
                case 1: {
                    auto arena = std::make_unique<Arena>(_stateManager);
                    arena->SetFromMenu(true);
                    arena->SetGameMode(TGameMode::PLAYER_VS_PLAYER);
                    _stateManager.RequestStateChange(std::move(arena));
                    break;
                }
                case 2:
                    _stateManager.RequestStateChange(std::make_unique<Menu>(_stateManager));
                    break;
            }
            _optionSelected = false;
        }
    }

    void GameModeMenu::Display()
    {
        const float spacing = 60.0f;
        const float descSpacing = 40.0f;
        sf::Vector2i mousePosition = sf::Mouse::getPosition(_stateManager.GetWindow());

        _stateManager.GetWindow().clear(sf::Color::Black);
        
        titleText.setFont(font);
        titleText.setString("Select Game Mode");
        titleText.setCharacterSize(48);
        titleText.setFillColor(sf::Color::Yellow);
        titleText.setPosition(width / 2 - titleText.getGlobalBounds().width / 2, 100);
        _stateManager.GetWindow().draw(titleText);

        for (size_t i = 0; i < _options.size(); ++i) {
            float optionY = height / 2 - (_options.size() * spacing) / 2 + i * spacing;
            
            text.setFont(font);
            text.setString(_options[i]);
            text.setCharacterSize(40);
            sf::FloatRect optionBounds(
                width / 2 - text.getGlobalBounds().width / 2,
                optionY,
                text.getGlobalBounds().width,
                text.getCharacterSize()
            );
            
            if (optionBounds.contains(static_cast<float>(mousePosition.x),
                                    static_cast<float>(mousePosition.y)))
                _selectedOption = static_cast<int>(i);
            
            text.setFillColor(i == _selectedOption ? sf::Color::Red : sf::Color::White);
            text.setPosition(width / 2 - text.getGlobalBounds().width / 2, optionY);
            _stateManager.GetWindow().draw(text);
            
            if (i < _descriptions.size()) {
                sf::Text descText;
                descText.setFont(font);
                descText.setString(_descriptions[i]);
                descText.setCharacterSize(20);
                descText.setFillColor(i == _selectedOption ? sf::Color::Cyan : sf::Color(150, 150, 150));
                descText.setPosition(width / 2 - descText.getGlobalBounds().width / 2, optionY + descSpacing);
                _stateManager.GetWindow().draw(descText);
            }
        }
        _stateManager.GetWindow().display();
    }

    void GameModeMenu::Destroy()
    {
        font = sf::Font();
        text = sf::Text();
        titleText = sf::Text();
    }
}
