# Eliot
# Copyright (C) 1999-2012  Antoine Fraboulet & Olivier Teulière
# Authors: Antoine Fraboulet <antoine.fraboulet@free.fr>
#          Olivier Teulière <ipkiss @@ gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

noinst_LIBRARIES = libgame.a

AM_CPPFLAGS = -I$(top_srcdir)/dic -I../intl -I$(top_srcdir)/intl @LIBCONFIG_CFLAGS@ @ARABICA_CFLAGS@ @EXPAT_CFLAGS@ @LOG4CXX_CFLAGS@

libgame_a_SOURCES= \
    game_exception.cpp game_exception.h \
    command.h command.cpp \
    coord.cpp coord.h \
    cross.cpp cross.h \
    rack.cpp rack.h \
    pldrack.cpp pldrack.h \
    round.cpp round.h \
    move.cpp move.h \
    results.cpp results.h \
    bag.cpp bag.h \
    turn_data.cpp turn_data.h \
    history.cpp history.h \
    hints.cpp hints.h \
    player.cpp player.h \
    cmd/topping_move_cmd.cpp cmd/topping_move_cmd.h \
    cmd/player_event_cmd.cpp cmd/player_event_cmd.h \
    cmd/player_move_cmd.cpp cmd/player_move_cmd.h \
    cmd/player_rack_cmd.cpp cmd/player_rack_cmd.h \
    ai_player.h \
    ai_percent.cpp ai_percent.h \
    game_params.h \
    board_layout.cpp board_layout.h \
    board.cpp board.h \
    board_cross.cpp \
    matrix.h \
    board_search.cpp board_search.h \
    settings.cpp settings.h \
    navigation.cpp navigation.h \
    game.cpp game.h \
    cmd/game_move_cmd.h cmd/game_move_cmd.cpp \
    cmd/game_rack_cmd.h cmd/game_rack_cmd.cpp \
    cmd/master_move_cmd.h cmd/master_move_cmd.cpp \
    turn.cpp turn.h \
    move_selector.cpp move_selector.h \
    duplicate.cpp duplicate.h \
    arbitration.cpp arbitration.h \
    freegame.cpp freegame.h \
    training.cpp training.h \
    topping.cpp topping.h \
    public_game.cpp public_game.h \
    game_factory.cpp game_factory.h \
    xml_writer.cpp xml_writer.h \
    xml_reader.cpp xml_reader.h

