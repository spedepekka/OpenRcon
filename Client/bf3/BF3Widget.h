/*
 * Copyright (C) 2014 The OpenRcon Project.
 *
 * This file is part of OpenRcon.
 *
 * OpenRcon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenRcon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenRcon.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BF3WIDGET_H
#define BF3WIDGET_H

#include <QTime>
#include <QTimer>

#include "ui_BF3Widget.h"
#include "BF3.h"

class BF3Widget : public BF3
{
    Q_OBJECT

public:
    BF3Widget(ServerEntry *serverEntry);
    ~BF3Widget();

    virtual Connection* getConnection() { return con; }

private:
    Ui::BF3Widget *ui;

    /* User Interface */

    // ServerInfo

    // Players
    QTimer *timerPlayerList;

    // Banlist

    // Reserved Slots

    // Spectator List

    // Console
    QCompleter *completer;

    void setAuthenticated(bool authenticated);
    void startupCommands(bool authenticated);
    void logConsole(int type, const QString &message);
    QIcon getRankIcon(int rank);

private slots:
    /* Connection */
    void onConnected();
    void onDisconnected();

    /* Events */
    void onDataSentEvent(const QString &request);
    void onDataReceivedEvent(const QString &response);

    /* Commands */

    // Misc
    void onLoginHashedCommand(bool auth);
    void onListPlayersCommand(const QList<PlayerInfo> &playerList, const PlayerSubset &playerSubset);

    // Admin
    void onAdminListPlayersCommand(const QList<PlayerInfo> &playerList, const PlayerSubset &playerSubset);

    // Banning

    // FairFight

    // Maplist

    // Player

    // Punkbuster

    // Reserved Slots

    // Spectator list

    // Squad

    // Variables

    /* User Interface */

    // ServerInfo

    // Players
    void updatePlayerList();
    void listPlayers(const QList<PlayerInfo> &playerList, const PlayerSubset &playerSubset);

    // Chat

    // Maplist

    // BanList

    // Reserved Slots

    // Spectator List

    // Options

    // Console
    void pushButton_co_co_clicked();
    void pushButton_co_pb_clicked();

};

#endif // BF3WIDGET_H
