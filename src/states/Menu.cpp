/*
** EPITECH PROJECT, 2025
** TripleTriad [WSL: Debian]
** File description:
** Menu
*/

#include "Menu.hpp"
#include "Adventure.hpp"
#include "Arena.hpp"
#include "Settings.hpp"

namespace triad
{
    Menu::Menu(StateManager &stateManager)
        : _stateManager(stateManager), width(800), height(600), _selectedOption(0),
        _optionSelected(false), _hoveredOption(-1)
    {
        _options = {"Adventure", "Arena", "Settings", "Quit"};
    }

    Menu::~Menu()
    {
    }

    void Menu::Init()
    {
        MusicManager::GetInstance().Play("assets/sounds/Alchemy.wav");
        if (!font.loadFromFile("assets/fonts/upheavtt.ttf")) {
            throw Error("Failed to load font");
        }
    }

    void Menu::SetKey(TKey key)
    {
        switch (key) {
            case TKey::UP:
                _selectedOption = (_selectedOption - 1 + _options.size()) % _options.size();
                break;
            case TKey::DOWN:
                _selectedOption = (_selectedOption + 1) % _options.size();
                break;
            case TKey::ESCAPE:
                _stateManager.GetWindow().close();
                _stateManager.RequestStateChange(nullptr);
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

    int Menu::GetSelectedOption() const
    {
        return _selectedOption;
    }

    bool Menu::IsOptionSelected() const
    {
        return _optionSelected;
    }

    void Menu::Update()
    {
        if (!_stateManager.GetWindow().isOpen()) {
            _stateManager.RequestStateChange(nullptr);
            return;
        }
        if (_optionSelected) {
            switch (_selectedOption) {
                case 0:
                    _stateManager.GetLevelManager().ResetLevel();
                    _stateManager.RequestStateChange(std::make_unique<Adventure>(_stateManager));
                    break;
                case 1: {
                    auto arena = std::make_unique<Arena>(_stateManager);
                    arena->SetFromMenu(true);
                    _stateManager.RequestStateChange(std::move(arena));
                    break;
                }
                case 2:
                    _stateManager.RequestStateChange(std::make_unique<Settings>(_stateManager));
                    break;
                case 3:
                    _stateManager.GetWindow().close();
                    _stateManager.RequestStateChange(nullptr);
                    break;
            }
            _optionSelected = false;
        }
    }

    void Menu::Display()
    {
        const float spacing = 50.0f;
        sf::Vector2i mousePosition = sf::Mouse::getPosition(_stateManager.GetWindow());

        _stateManager.GetWindow().clear(sf::Color::Black);
        for (size_t i = 0; i < _options.size(); ++i) {
            text.setFont(font);
            text.setString(_options[i]);
            text.setCharacterSize(40);
            sf::FloatRect optionBounds(
                width / 2 - text.getGlobalBounds().width / 2,
                height / 2 - (_options.size() * spacing) / 2 + i * spacing,
                text.getGlobalBounds().width,
                text.getCharacterSize()
            );
            if (optionBounds.contains(static_cast<float>(mousePosition.x),
                                    static_cast<float>(mousePosition.y)))
                _selectedOption = static_cast<int>(i);
            text.setFillColor(i == _selectedOption ? sf::Color::Red : sf::Color::White);
            text.setPosition(width / 2 - text.getGlobalBounds().width / 2,
                            height / 2 - (_options.size() * spacing) / 2 + i * spacing);
            _stateManager.GetWindow().draw(text);
        }
        _stateManager.GetWindow().display();
    }

    void Menu::Destroy()
    {
        font = sf::Font();
        text = sf::Text();
    }
}
