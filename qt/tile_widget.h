/*****************************************************************************
 * Eliot
 * Copyright (C) 2010 Olivier Teulière
 * Authors: Olivier Teulière <ipkiss @@ gmail.com>
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

#ifndef TILE_WIDGET_H_
#define TILE_WIDGET_H_

#include <QtGui/QWidget>
#include "tile.h"


/**
 * Simplified tile, only used to draw the coordinates of the board
 */
class BasicTileWidget: public QWidget
{
public:
    BasicTileWidget(QWidget *parent = 0, QString text = "");

    int getSquareSize() const;

    virtual int heightForWidth(int w) const;

protected:
    /// Define a default size
    virtual QSize sizeHint() const;
    /// Paint the square
    virtual void paintEvent(QPaintEvent *iEvent);

private:
    QString m_text;
};


/**
 * Widget used to display a square on the board, with or without letter.
 */
class TileWidget: public BasicTileWidget
{
    Q_OBJECT;

public:
    enum Multiplier
    {
        NONE,
        LETTER_DOUBLE,
        LETTER_TRIPLE,
        WORD_DOUBLE,
        WORD_TRIPLE
    };

    enum State
    {
        NORMAL,
        PREVIEW,
        PLAYED,
        IN_RACK
    };

    explicit TileWidget(QWidget *parent = 0, Multiplier multiplier = NONE,
                        int row = 0, int col = 0);

public slots:
    virtual void tileChanged(const Tile &iTile, bool isJoker, State state);
    virtual void arrowChanged(bool showArrow, bool horizontalArrow);

signals:
    void mousePressed(int row, int col, QMouseEvent *iEvent);

protected:
    /// Paint the square
    virtual void paintEvent(QPaintEvent *iEvent);
    /// Catch mouse events to send a signal
    virtual void mousePressEvent(QMouseEvent *iEvent);

private:
    /// Encapsulated tile
    Tile m_tile;

    /// Word or letter multipler
    Multiplier m_multiplier;

    /// Position of the tile, only used to fire the mousePressed() signal
    // XXX: another way would be to send *this in the mousePressed() signal
    int m_row;
    int m_col;

    /// Whether the tile is a joker
    bool m_isJoker;

    /// State of the tile
    State m_state;

    /// Whether we should show the arrow
    bool m_showArrow;

    /// Whether the arrow is horizontal
    bool m_horizontalArrow;

    /// Define a few background colours
    //@{
    static const QColor EmptyColour;
    static const QColor L2Colour;
    static const QColor L3Colour;
    static const QColor W2Colour;
    static const QColor W3Colour;
    static const QColor TileColour;
    static const QColor PreviewColour;
    static const QColor PlayedColour;
    static const QColor NormalColour;
    static const QColor JokerColour;
    static const QColor ArrowColour;
    //@}
};

class TileWidgetDecorator : public TileWidget
{
    Q_OBJECT;

public:
    TileWidgetDecorator(QWidget *parent, TileWidget &wrapped);

public slots:
    virtual void tileChanged(const Tile &iTile, bool isJoker, State state);
    virtual void arrowChanged(bool showArrow, bool horizontalArrow);

signals:
    void mousePressed(int row, int col, QMouseEvent *iEvent);

protected:
    /// Paint the square
    virtual void paintEvent(QPaintEvent *iEvent);

private:
    TileWidget & m_wrapped;
};

#endif
