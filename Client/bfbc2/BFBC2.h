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

#ifndef BFBC2_H
#define BFBC2_H

#include <QTimer>
#include <QCryptographicHash>

#include "FrostbiteGame.h"
#include "BFBC2Connection.h"
#include "BFBC2LevelDictionary.h"

class BFBC2 : public FrostbiteGame
{
    Q_OBJECT

public:
    BFBC2(ServerEntry *serverEntry);
    ~BFBC2();

private:
    bool authenticated;

private slots: 
    void onConnected();
    void onLoginHashedCommand(const QByteArray &salt);
    void onLoginHashedCommand();

    void slotCommandMapListListRounds(QStringList ml);

protected:
    BFBC2Connection *con;
    BFBC2LevelDictionary *levelDictionary;

    QTimer *commandRefreshTimer;

    QString currentMod;
    QString currentGamemode;
    QStringList gamemodes;
    int nextLevelIndex;
    QStringList mapListU;

    bool isAuthenticated();

    void sendSayMessage(const QString &msg, const QString &group);
    void sendYellMessage(const QString &message, int duration, const QString &group);
    void killPlayer(const QString &player);
    void kickPlayer(const QString &player, const QString &reason);
    void banPlayer(const QString &type, const QString &id, const QString &timeout, const QString &reason);
    void unbanPlayer(const QString &type, const QString &id);
    void movePlayer(const QString &player, const QString &teamId, const QString &squadId, const QString &fk);

    QString getMapName(const QString &engineName, const QString &gamemode);
    QPixmap getMapImage(const QString &engineName, const QString &gamemode);
};

#endif // BFBC2_H
