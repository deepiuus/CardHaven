/*
** EPITECH PROJECT, 2025
** TripleTriad [WSL: Debian]
** File description:
** Arena
*/

#ifndef ARENA_HPP_
#define ARENA_HPP_

#include "../IGames.hpp"
#include "../utils/StateManager.hpp"
#include "../utils/CardManager.hpp"
#include "Menu.hpp"
#include "Adventure.hpp"

namespace triad
{
    class Arena : public IGames {
        public:
            Arena(StateManager &stateManager);
            ~Arena() override;
            void Init() override;
            void SetKey(TKey key) override;
            void Update() override;
            void Display() override;
            void Destroy() override;
            void SetFromMenu(bool fromMenu);

        private:
            void SetupBoard(float cellSize, float cellGap, float gridStartX, float gridStartY);
            void SetupCards(int cardSpacing, int cardY);
            void DraggingCard();
            void OccupyCell();
            void PlaceCard(int x, int y);
            void CaptureCard(int x, int y);
            void ResetCard();
            int width;
            int height;
            sf::Sprite _sprite;
            sf::Texture _texture;
            sf::Sprite _cardSprite;
            StateManager &_stateManager;
            std::vector<sf::Sprite> _player1Cards;
            std::vector<sf::Sprite> _player2Cards;
            std::vector<const Card*> _player1Deck;
            std::vector<const Card*> _player2Deck;
            sf::Vector2i _draggedCard{-1, -1};
            bool _dragging = false;
            sf::Vector2f _dragOffset;
            sf::FloatRect _boardGrid[3][3];
            struct CellInfo {
                int owner;
                const Card *card;
            };
            CellInfo _boardOccupancy[3][3];
            sf::Sprite* _boardSprites[3][3];
            bool _fromMenu;
    };
}

#endif /* !ARENA_HPP_ */
