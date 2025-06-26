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
#include "../utils/LevelManager.hpp"
#include "Menu.hpp"
#include "Adventure.hpp"
#include <utility>
#include <random>

namespace triad
{
    enum class TGameMode {
        PLAYER_VS_PLAYER,
        PLAYER_VS_AI
    };

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
            void SetScore();
            void SetDifficulty(TDifficulty difficulty);
            void SetGameMode(TGameMode gameMode);

        private:
            void SetupDecksBasedOnDifficulty();
            void AITurn();
            int EvaluateMove(int cardIndex, int boardX, int boardY);
            bool CanCaptureCards(const Card* card, int boardX, int boardY);
            std::pair<int, sf::Vector2i> FindBestAIMove();
            void SetupBoard(float cellSize, float cellGap, float gridStartX, float gridStartY);
            void SetupCards(int cardSpacing, int cardY);
            void DraggingCard();
            void OccupyCell();
            void PlaceCard(int x, int y);
            void CaptureCard(int x, int y);
            void ResetCard();
            bool isCardOnBoard(const Card* card) const;
            bool isPlayerCardOnBoard(int playerIndex, int cardIndex) const;
            int GetHoveredId(const std::vector<sf::Sprite>& cards, const std::vector<const Card*>& deck, float baseX, float baseY, int cardCount, const sf::Vector2i& mousePos) const;
            void SetHoveredStyle(sf::Sprite& card, bool isHovered, float baseX, float baseY, sf::Color normalColor, sf::Color hoverColor) const;
            void SetHoveredCards(std::vector<sf::Sprite>& cards, const std::vector<const Card*>& deck, float baseX, sf::Color normalColor, sf::Color hoverColor, int hoveredIdx) const;
            void SetDraggedCards(std::vector<sf::Sprite> &cards, std::vector<const Card *> &deck, int playerIndex, const sf::Vector2i &mousePos);
            bool SetCapturedCard(int x, int y, int nx, int ny, int myValue, int theirValue, const char* dirName);
            void DrawCards(sf::RenderWindow& window, const std::vector<sf::Sprite>& cards, const std::vector<const Card*>& deck, int hoveredIdx, bool drawHoveredLast) const;
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
            int _currentTurn;
            sf::Font _font;
            sf::Text _turnText;
            sf::Text _endGameText;
            sf::Clock _endGameClock;
            bool _endGame;
            bool _fromMenu;
            int _playerCount;
            int _ennemyCount;
            int _occupiedCount;
            int _cardSpacing;
            int _cardY;
            TDifficulty _difficulty;
            TGameMode _gameMode;
            sf::Clock _aiTimer;
            bool _aiThinking;
            bool _player1CardsPlayed[5];
            bool _player2CardsPlayed[5];
    };
}

#endif /* !ARENA_HPP_ */
