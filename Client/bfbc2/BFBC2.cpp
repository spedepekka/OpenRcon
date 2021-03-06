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

#include "BFBC2.h"

BFBC2::BFBC2(ServerEntry *serverEntry) : FrostbiteGame(serverEntry), authenticated(false)
{
    con = new BFBC2Connection();
    con->hostConnect(serverEntry->host, serverEntry->port);

    levelDictionary = new BFBC2LevelDictionary(this);

    versionMap.insert(571287, "R21");
    versionMap.insert(581637, "R22");
    versionMap.insert(584642, "R23");
    versionMap.insert(593485, "R24");
    versionMap.insert(602833, "R25");
    versionMap.insert(609063, "R26");
    versionMap.insert(617877, "R27");
    versionMap.insert(621775, "R28");
    versionMap.insert(638140, "R30");
    versionMap.insert(720174, "R32");
    versionMap.insert(851434, "R34");

    // Connection
    connect(con, SIGNAL(onConnected()), this, SLOT(onConnected()));

    // Commands
    connect(con, SIGNAL(onLoginHashedCommand(const QByteArray&)), this, SLOT(onLoginHashedCommand(const QByteArray&)));
    connect(con, SIGNAL(onLoginHashedCommand()), this, SLOT(onLoginHashedCommand()));
}

BFBC2::~BFBC2()
{
    delete con;
    delete levelDictionary;
}

void BFBC2::onConnected()
{
    if (!isAuthenticated()) {
        con->sendCommand("login.hashed");
    }
}

void BFBC2::onLoginHashedCommand()
{
    authenticated = true;
}

void BFBC2::onLoginHashedCommand(const QByteArray &salt)
{
    QString password = serverEntry->password;

    if (!isAuthenticated()) {
        if (!password.isEmpty()) {
            QCryptographicHash hash(QCryptographicHash::Md5);
            hash.addData(salt);
            hash.addData(password.toLatin1().constData());

            con->sendCommand(QString("\"login.hashed\" \"%1\"").arg(hash.result().toHex().toUpper().constData()));
        }
    }
}

bool BFBC2::isAuthenticated()
{
    return authenticated;
}

void BFBC2::slotCommandMapListListRounds(QStringList ml)
{
    mapListU = ml;
}

void BFBC2::sendSayMessage(const QString &msg, const QString &group)
{
    if (!group.isEmpty()) {
        con->sendCommand(QString("\"admin.say\" \"%1\" \"%2\"").arg(msg).arg(group));
    } else {
        con->sendCommand(QString("\"admin.say\" \"%1\" \"all\"").arg(msg));
    }
}

void BFBC2::sendYellMessage(const QString &message, int duration, const QString &group)
{
    if (!group.isEmpty()) {
        con->sendCommand(QString("\"admin.yell\" \"%1\" \"%2\" \"%3\"").arg(message).arg(duration).arg(group));
    } else {
        con->sendCommand(QString("\"admin.yell\" \"%1\" \"%2\" \"all\"").arg(message).arg(duration));
    }
}

void BFBC2::killPlayer(const QString &player)
{
    con->sendCommand(QString("\"admin.killPlayer\" \"%1\"").arg(player));
}

void BFBC2::kickPlayer(const QString &player, const QString &reason)
{
    con->sendCommand(QString("\"admin.kickPlayer\" \"%1\" \"%2\"").arg(player).arg(reason));
}

void BFBC2::banPlayer(const QString &type, const QString &id, const QString &timeout, const QString &reason)
{
    // banList.add <id-type, id, timeout, reason> Add player/IP/GUID to ban list for a certain amount of time
    con->sendCommand(QString("\"banList.add\" \"%1\" \"%2\" \"%3\" \"%4\"").arg(type, id, timeout, reason));
}

void BFBC2::unbanPlayer(const QString &type, const QString &id)
{
    con->sendCommand(QString("\"banList.remove\" \"%1\" \"%2\"").arg(type, id));
}

void BFBC2::movePlayer(const QString &player, const QString &teamId, const QString &squadId, const QString &fk)
{
    if (!player.isEmpty()) {
        con->sendCommand(QString("\"admin.movePlayer\" \"%1\" \"%2\" \"%3\" \"%4\"").arg(player, teamId, squadId, fk));
    }
}

QString BFBC2::getMapName(const QString &engineName, const QString &gamemode)
{
    int gamemodeIndex = gamemodes.indexOf(QRegExp(gamemode, Qt::CaseInsensitive));

    if (gamemodeIndex != -1) {
        LevelEntry level = levelDictionary->getLevel(engineName); // TODO: Mind the gamemode?

        return level.name;
    }

    return QString();
}

QPixmap BFBC2::getMapImage(const QString &engineName, const QString &gamemode)
{
    int gamemodeIndex = gamemodes.indexOf(QRegExp(gamemode, Qt::CaseInsensitive));

    if (gamemodeIndex != -1) {
        LevelEntry level = levelDictionary->getLevel(engineName); // TODO: Mind the gamemode?

        return level.image;
    }

    return QPixmap();
}
