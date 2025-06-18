/*
** EPITECH PROJECT, 2025
** TripleTriad [WSL: Debian]
** File description:
** Adventure
*/

#ifndef ADVENTURE_HPP_
#define ADVENTURE_HPP_

#include "../IGames.hpp"
#include "../utils/StateManager.hpp"
#include "../utils/LevelManager.hpp"
#include "Menu.hpp"
#include "Visual.hpp"
#include "Arena.hpp"

namespace triad
{
    class Adventure : public IGames {
        public:
            Adventure(StateManager &stateManager);
            ~Adventure() override;
            void Init() override;
            void SetKey(TKey key) override;
            void Update() override;
            void Display() override;
            void Destroy() override;

        private:
            void getDirection(int &dx, int &dy, TKey key);
            void MapLogic(int dx, int dy);
            bool isRockAt(int x, int y) const;
            void RockLogic(int dx, int dy, int newX, int newY);
            bool isCellFree(int x, int y) const;
            void initTextures();
            void setTiles(sf::Vector2f offset);
            StateManager &_stateManager;
            LevelManager _levelManager;
            const std::vector<std::vector<char>> *_map;
            std::vector<sf::Vector2i> _rocks;
            sf::Vector2i _playerPos;
            int _tileSize;
            sf::Texture _floorTexture;
            sf::Texture _wallTexture;
            sf::Texture _rockTexture;
            sf::Texture _waifuTexture;
            sf::Texture _chadTexture;
            sf::Sprite _floorSprite;
            sf::Sprite _wallSprite;
            sf::Sprite _rockSprite;
            sf::Sprite _waifuSprite;
            sf::Sprite _chadSprite;
            sf::Texture _keyTexture;
            sf::Sprite _keySprite;
            sf::Texture _lockTexture;
            sf::Sprite _lockSprite;
            bool _hasKey;
    };
}

#endif /* !SETTINGS_HPP_ */
