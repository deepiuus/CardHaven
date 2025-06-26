/*
** EPITECH PROJECT, 2025
** TripleTriad [WSL: Debian]
** File description:
** DifficultyMenu
*/

#ifndef DIFFICULTYMENU_HPP_
#define DIFFICULTYMENU_HPP_

#include <vector>
#include "../IGames.hpp"
#include "../utils/StateManager.hpp"
#include "../utils/MusicManager.hpp"
#include "../utils/Error.hpp"
#include "../utils/LevelManager.hpp"
#include "Adventure.hpp"
#include "Menu.hpp"

namespace triad
{
    class DifficultyMenu : public IGames {
        public:
            DifficultyMenu(StateManager &stateManager);
            ~DifficultyMenu() override;
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

#endif /* !DIFFICULTYMENU_HPP_ */
