/*
** EPITECH PROJECT, 2025
** TripleTriad [WSL: Debian]
** File description:
** StateManager
*/

#include "StateManager.hpp"

namespace triad
{
    StateManager::StateManager()
    {
        _currentState = nullptr;
        _window.create(sf::VideoMode(800, 600), "Triple Triad");
    }

    bool StateManager::IsWindowOpen() const
    {
        return _window.isOpen();
    }

    void StateManager::HandleInput()
    {
        sf::Event event;

        while (_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                _window.close();
            }
            if (_currentState) {
                TKey key = TKey::NONE;
                if (event.type == sf::Event::KeyPressed) {
                    switch (event.key.code) {
                        case sf::Keyboard::Up:
                            key = TKey::UP;
                            break;
                        case sf::Keyboard::Down:
                            key = TKey::DOWN;
                            break;
                        case sf::Keyboard::Left:
                            key = TKey::LEFT;
                            break;
                        case sf::Keyboard::Right:
                            key = TKey::RIGHT;
                            break;
                        case sf::Keyboard::Enter:
                            key = TKey::ENTER;
                            break;
                        case sf::Keyboard::Space:
                            key = TKey::SPACE;
                            break;
                        case sf::Keyboard::Escape:
                            key = TKey::ESCAPE;
                            break;
                        case sf::Keyboard::R:
                            key = TKey::R;
                            break;
                        case sf::Keyboard::Num1:
                        case sf::Keyboard::Numpad1:
                            key = TKey::N1;
                            break;
                        case sf::Keyboard::Num2:
                        case sf::Keyboard::Numpad2:
                            key = TKey::N2;
                            break;
                        case sf::Keyboard::Num3:
                        case sf::Keyboard::Numpad3:
                            key = TKey::N3;
                            break;
                        case sf::Keyboard::Num4:
                        case sf::Keyboard::Numpad4:
                            key = TKey::N4;
                            break;
                        case sf::Keyboard::Num5:
                        case sf::Keyboard::Numpad5:
                            key = TKey::N5;
                            break;
                        default:
                            break;
                    }
                } else if (event.type == sf::Event::MouseButtonPressed) {
                    switch (event.mouseButton.button) {
                        case sf::Mouse::Left:
                            key = TKey::LCLICK;
                            break;
                        case sf::Mouse::Right:
                            key = TKey::RCLICK;
                            break;
                        default:
                            break;
                    }
                } else if (event.type == sf::Event::MouseButtonReleased) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (_currentState) {
                            _currentState->SetKey(TKey::NONE);
                        }
                    }
                }
                if (key != TKey::NONE) {
                    _currentState->SetKey(key);
                }
            }
        }
    }

    IGames *StateManager::GetCurrentState() const
    {
        return _currentState.get();
    }

    sf::RenderWindow& StateManager::GetWindow()
    {
        return _window;
    }

    void StateManager::SetState(std::unique_ptr<IGames> state)
    {
        if (_currentState) {
            _currentState->Destroy();
        }
        _currentState = std::move(state);
        _currentState->Init();
    }

    void StateManager::RequestStateChange(std::unique_ptr<IGames> nextState)
    {
        _nextState = std::move(nextState);
    }

    void StateManager::ProcessStateChange()
    {
        if (_nextState) {
            SetState(std::move(_nextState));
        }
    }

    void StateManager::Update()
    {
        if (_currentState) {
            _currentState->Update();
        }
    }

    void StateManager::Display()
    {
        if (!_window.isOpen())
            return;
        if (_currentState) {
            _currentState->Display();
        }
        _window.display();
    }

    LevelManager &StateManager::GetLevelManager()
    {
        return _levelManager;
    }
}
