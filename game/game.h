/*****************************************************************************
 * Eliot
 * Copyright (C) 1999-2012 Antoine Fraboulet & Olivier Teulière
 * Authors: Antoine Fraboulet <antoine.fraboulet @@ free.fr>
 *          Olivier Teulière <ipkiss @@ gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *****************************************************************************/

#ifndef GAME_H_
#define GAME_H_

#include <string>
#include <vector>
#include "game_params.h"
#include "logging.h"
#include "bag.h"
#include "board.h"
#include "history.h"
#include "navigation.h"
#include "command.h"

class Dictionary;
class Player;
class PlayedRack;
class Round;
class Rack;
class TurnData;

using namespace std;


/**
 * Parent class of all the Game types.
 * It offers the common attributes (Board, Bag, etc...) as well as useful
 * "helper" methods to factorize some code.
 */
class Game
{
    DEFINE_LOGGER();
public:
    virtual ~Game();

    /***************
     * Game type
     ***************/

    GameParams::GameMode getMode() const { return m_params.getMode(); }

    /***************
     * Various getters
     ***************/

    /// Get the game characteristics
    const GameParams & getParams() const { return m_params; }

    bool hasMasterGame() const { return m_masterGame != NULL; }

    /**
     * Get the dictionary associated with the game.
     * You should never create a new dictionary object while a Game
     * object still exists
     */
    const Dictionary & getDic() const { return m_params.getDic(); }

    /// Get the board
    const Board& getBoard() const { return m_board; }
    Board & accessBoard() { return m_board; }
    /// Get the bag
    const Bag& getBag() const { return m_bag; }
    Bag & accessBag() { return m_bag; }
    /**
     * The realBag is the current bag minus all the racks
     * present in the game. It represents the actual
     * letters that are left in the bag.
     * FIXME: in Duplicate mode, this method uses m_currPlayer to find the
     * rack of the player. Since not all the players played the same word,
     * it is important to set m_currPlayer accurately before!
     */
    void realBag(Bag &iBag) const;


    /// Get the history of the game */
    const History& getHistory() const { return m_history; }
    History & accessHistory() { return m_history; }

    /***************
     * Methods to access players.
     ***************/

    Player & accessPlayer(unsigned int iNum);
    const Player& getPlayer(unsigned int iNum) const;
    const Player& getCurrentPlayer() const { return getPlayer(currPlayer()); };
    unsigned int getNPlayers() const { return m_players.size(); }
    unsigned int getNHumanPlayers() const;
    unsigned int currPlayer() const { return m_currPlayer; }

    /**
     * Add a player to the game.
     * The Game object takes ownership of the given player
     */
    virtual void addPlayer(Player *iPlayer);

    /***************
     * Game handling
     ***************/

    /**
     * Start the game.
     * AI players are handled automatically, so if the game only has AI
     * players, it will play until the end.
     */
    virtual void start() = 0;

    /**
     * Indicate whether we reached the end of the game.
     * This should be checked regularly.
     * XXX: using a signal would be nice here...
     */
    virtual bool isFinished() const = 0;

    /**
     * Method used by human players to play the word iWord at coordinates
     * iCoord, and end the turn (if possible)
     * Possible return values:
     *  0: correct word, the Round can be used by the caller
     *  1: one letter of the word is invalid in the current dictionary
     *  2: invalid coordinates (unreadable or out of the board)
     *  3: word not present in the dictionary
     *  4: not enough letters in the rack to play the word
     *  5: word is part of a longer one
     *  6: word overwriting an existing letter
     *  7: invalid crosscheck
     *  8: word already present on the board (no new letter from the rack)
     *  9: isolated word (not connected to the rest)
     * 10: first word not horizontal (can only happen in duplicate mode)
     * 11: first word not covering the H8 square
     * 12: word going out of the board
     * 13: too many letters played from the rack
     */
    virtual int play(const wstring &iCoord, const wstring &iWord) = 0;

    /**
     * Shuffle the rack of the current player.
     *
     * Note that this is persistent only until the player gets a new rack.
     * The shuffling will be lost by navigating in the game history, and
     * will not be saved in savegames.
     */
    void shuffleRack();

    /**
     * Change the rack of the current player.
     * The letters must be the same as in the existing rack.
     *
     * Note that this is persistent only until the player gets a new rack.
     * The reordering will be lost by navigating in the game history, and
     * will not be saved in savegames.
     */
    void reorderRack(const PlayedRack &iNewRack);

    /// Return true if the player has played for the current turn
    // XXX: not very nice API, should be a player property...
    virtual bool hasPlayed(unsigned int player) const { return player != currPlayer(); }

    /***************
     * Setting the rack
     ***************/

    enum set_rack_mode {RACK_ALL, RACK_NEW};

    void addPoints(int iPoints) { m_points += iPoints; }

    const Navigation & getNavigation() const { return m_navigation; }
    Navigation & accessNavigation() { return m_navigation; }

    /**
     * This function checks whether it is legal to play the given word at the
     * given coordinates. If so, the function fills a Move object, also given
     * as a parameter.
     * Possible return values: same as the play() method
     * If checkRack is false, the return value 4 is impossible to get
     * (no check is done on the rack letters).
     */
    int checkPlayedWord(const wstring &iCoord,
                        const wstring &iWord,
                        Move &oMove,
                        bool checkRack = true,
                        bool checkWordAndJunction = true) const;

private:
    /// Game characteristics
    GameParams m_params;

    /// Master game (can be NULL)
    const Game *m_masterGame;

    /**
     * History of the game.
     */
    History m_history;

    Navigation m_navigation;

    int m_points;


    /// Change the player who is supposed to play
    void setCurrentPlayer(unsigned int iPlayerId) { m_currPlayer = iPlayerId; }

    /// Command used to keep track of the current player changes
    class CurrentPlayerCmd: public Command
    {
        public:
            CurrentPlayerCmd(Game &ioGame,
                             unsigned int iPlayerId);

            virtual wstring toString() const;

        protected:
            virtual void doExecute();
            virtual void doUndo();

        private:
            Game &m_game;
            unsigned int m_newPlayerId;
            unsigned int m_oldPlayerId;
    };

// TODO: check what should be private and what should be protected
protected:
    /// All the players, indexed by their ID
    vector<Player*> m_players;
    /// ID of the "current" player
    unsigned int m_currPlayer;

    /// Board
    Board m_board;

    /// Bag
    Bag m_bag;

    /**
     * Protected constructor.
     * The iMasterGame parameter is optional (i.e. it can be NULL).
     * This game takes ownership of the master game, if one is provided.
     */
    Game(const GameParams &iParams, const Game *iMasterGame);

    /*********************************************************
     * Helper functions
     *********************************************************/

    /**
     * Check if it is possible to draw a new rack from the bag,
     * after we put back the contents of iPld.
     * @param iCheck if true, the number of consonants and vowels will
     * be checked.
     * @param oReason if the pointer is valid, and the method returns false,
     *      oReason will point to the error code. Possible values:
     *          1: The bag is empty
     *          2: Not enough vowels or consonants to complete the rack
     * @return true if a rack can be drawn, false otherwise
     */
    bool canDrawRack(const PlayedRack &iPld, bool iCheck, int *oReason = NULL) const;

    /**
     * Complete the given rack randomly.
     *
     * Completing a rack randomly is more complex than it seems, because we
     * must take into account several constraints:
     *  - if iCheck is true, we must ensure that the rack contains a minimum
     *    number of vowels and consonants (2 of each in the 15 first moves of
     *    the game, 1 of each after)
     *  - the game is over if the (real) bag contains only vowels or only
     *    consonants, and in particular if it contains only one letter
     *  - some letters (in particular the joker) can count both as a vowel and
     *    as a consonant (but not at the same time)
     *  - in a joker game, the joker must be present in the rack unless there
     *    is no joker left in the bag. In addition, we must prevent that both
     *    jokers are present in the rack at the same time
     *  - if completing a rack doesn't meet the requirements on the vowels and
     *    consonants, we must reject the rack completely (but only once,
     *    otherwise we have no guarantee that the rejects will stop eventually).
     *    This also means we have to check whether completing the rack with the
     *    requirements is possible...
     */
    PlayedRack helperSetRackRandom(const PlayedRack &iPld,
                                   bool iCheck, set_rack_mode mode) const;

    /**
     * Return a rack for the given letters, after performing some checks.
     * The '+' and '-' signs are accepted in the letters but ignored.
     */
    PlayedRack helperSetRackManual(bool iCheck, const wstring &iLetters) const;

    /**
     * Helper method to retrieve the rack for the current turn
     * in the master game. Should only be called when hasMasterGame()
     * returns true.
     */
    PlayedRack getRackFromMasterGame() const;

    /**
     * Helper method to retrieve the move played for the current turn
     * in the master game. Should only be called when hasMasterGame()
     * returns true.
     */
    Move getMoveFromMasterGame() const;

    /**
     * Helper function to set the game rack and the players rack at the same time.
     * Shouldn't be used in free game mode.
     */
    void setGameAndPlayersRack(const PlayedRack &iRack, bool iWithNoMove);

    void nextPlayer();

    /**
     * Check if the players rack can be obtained from the bag.
     * Since letters are removed from the bag only when the
     * round is played we need to check that ALL the racks
     * are in the bag simultaneously.
     *
     * FIXME: since we do not check for all racks it works
     * for training and duplicate but it won't work for
     * freegames.
     */
    bool rackInBag(const Rack &iRack, const Bag &iBag) const;

};

#endif /* _GAME_H_ */

