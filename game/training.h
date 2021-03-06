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

#ifndef TRAINING_H_
#define TRAINING_H_

#include <string>

#include "game.h"
#include "results.h"
#include "logging.h"

class Player;

using std::string;
using std::wstring;


/**
 * This class handles the logic specific to a training game.
 * As its name indicates, it is not a game in the literal meaning of the word,
 * in particular because the rack can be set at will.
 *
 * Note: No player should be added to this game, a human player is added
 * automatically (in the start() method)
 */
class Training: public Game
{
    DEFINE_LOGGER();
    friend class GameFactory;
public:

    /*************************
     * Game handling
     *************************/
    virtual void start();

    virtual bool isFinished() const;

    /// See description of Game::play()
    virtual int play(const wstring &iCoord, const wstring &iWord);

    void search();
    const Results& getResults() const { return m_results; }
    int playResult(unsigned int iResultIndex);

    /**
     * Complete (or reset) the rack randomly.
     * @exception EndGameException if it is impossible to complete the rack
     * for some reason...
     */
    void setRackRandom(bool iCheck, set_rack_mode iRackMode);

    /**
     * Set the rack with the given letters
     * @exception EndGameException if the game is over
     * @exception GameException if any other error occurs
     */
    void setRackManual(bool iCheck, const wstring &iLetters);

    /**
     * Override the default behaviour of addPlayer(), because in training
     * mode we only want a human player
     */
    virtual void addPlayer(Player *iPlayer);

private:
    /// Private constructor and destructor to force using the GameFactory class
    Training(const GameParams &iParams, const Game *iMasterGame);

    /// Record a player move
    void recordPlayerMove(const Move &iMove, Player &ioPlayer);

    void endTurn();

    /// Search results, with all the possible rounds up to a predefined limit
    LimitResults m_results;
};

#endif /* _TRAINING_H_ */

