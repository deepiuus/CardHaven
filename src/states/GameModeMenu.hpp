/*
** EPITECH PROJECT, 2025
** TripleTriad [WSL: Debian]
** File description:
** GameModeMenu
*/

#ifndef GAMEMODEMENU_HPP_
#define GAMEMODEMENU_HPP_

#include <vector>
#include "../IGames.hpp"
#include "../utils/StateManager.hpp"
#include "../utils/MusicManager.hpp"
#include "../utils/Error.hpp"
#include "Arena.hpp"

namespace triad
{
    class GameModeMenu : public IGames {
        public:
            GameModeMenu(StateManager &stateManager);
            ~GameModeMenu() override;
            void Init() override;
            void SetKey(TKey key) override;
            int GetSelectedOption() const;
            bool IsOptionSelected() const;
            void Update() override;
            void Display() override;
            void Destroy() override;

        private:
            int width;
            int height;
            sf::Font font;
            sf::Text text;
            sf::Text titleText;
            int _selectedOption;
            bool _optionSelected;
            std::vector<std::string> _options;
            std::vector<std::string> _descriptions;
            StateManager &_stateManager;
    };
}

#endif /* !GAMEMODEMENU_HPP_ */
