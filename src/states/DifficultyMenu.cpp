/*
** EPITECH PROJECT, 2025
** TripleTriad [WSL: Debian]
** File description:
** DifficultyMenu
*/

#include "DifficultyMenu.hpp"

namespace triad
{
    DifficultyMenu::DifficultyMenu(StateManager &stateManager)
        : _stateManager(stateManager), width(800), height(600), _selectedOption(0),
        _optionSelected(false)
    {
        _options = {"Easy", "Normal", "Hard", "Back"};
        _descriptions = {
            "Player deck is the ennemy's last deck",
            "Random decks for both players",
            "Keep the starter deck for the entire game"
        };
    }

    DifficultyMenu::~DifficultyMenu()
    {
    }

    void DifficultyMenu::Init()
    {
        if (!font.loadFromFile("assets/fonts/upheavtt.ttf")) {
            throw Error("Failed to load font");
        }
    }

    void DifficultyMenu::SetKey(TKey key)
    {
        switch (key) {
            case TKey::UP:
                _selectedOption = (_selectedOption - 1 + _options.size()) % _options.size();
                break;
            case TKey::DOWN:
                _selectedOption = (_selectedOption + 1) % _options.size();
                break;
            case TKey::ESCAPE:
                _selectedOption = 3;
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

    int DifficultyMenu::GetSelectedOption() const
    {
        return _selectedOption;
    }

    bool DifficultyMenu::IsOptionSelected() const
    {
        return _optionSelected;
    }

    void DifficultyMenu::Update()
    {
        if (!_stateManager.GetWindow().isOpen()) {
            _stateManager.RequestStateChange(nullptr);
            return;
        }
        if (_optionSelected) {
            switch (_selectedOption) {
                case 0:
                    _stateManager.GetLevelManager().ResetLevel();
                    _stateManager.GetLevelManager().SetDifficulty(TDifficulty::EASY);
                    _stateManager.RequestStateChange(std::make_unique<Adventure>(_stateManager));
                    break;
                case 1:
                    _stateManager.GetLevelManager().ResetLevel();
                    _stateManager.GetLevelManager().SetDifficulty(TDifficulty::NORMAL);
                    _stateManager.RequestStateChange(std::make_unique<Adventure>(_stateManager));
                    break;
                case 2:
                    _stateManager.GetLevelManager().ResetLevel();
                    _stateManager.GetLevelManager().SetDifficulty(TDifficulty::HARD);
                    _stateManager.RequestStateChange(std::make_unique<Adventure>(_stateManager));
                    break;
                case 3:
                    _stateManager.RequestStateChange(std::make_unique<Menu>(_stateManager));
                    break;
            }
            _optionSelected = false;
        }
    }

    void DifficultyMenu::Display()
    {
        const float spacing = 60.0f;
        const float descSpacing = 40.0f;
        sf::Vector2i mousePosition = sf::Mouse::getPosition(_stateManager.GetWindow());

        _stateManager.GetWindow().clear(sf::Color::Black);
        titleText.setFont(font);
        titleText.setString("Select Difficulty");
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
            if (optionBounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)))
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

    void DifficultyMenu::Destroy()
    {
        font = sf::Font();
        text = sf::Text();
        titleText = sf::Text();
    }
}
