/*
 * Copyright (C) 2010 The OpenRcon Project.
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

#ifndef FROSTBITECONNECTION_H
#define FROSTBITECONNECTION_H

#include "Connection.h"

#include "FrostbiteRconPacket.h"

#include "PlayerSubset.h"

#define MIN_PACKET_SIZE 12

class FrostbiteConnection : public Connection
{
    Q_OBJECT

public:
    FrostbiteConnection(QObject *parent = 0);
    ~FrostbiteConnection();

    void hostConnect(const QString &host, int port);
    void sendCommand(const QString &command);

protected:
    virtual void parse(const QString &request, const FrostbiteRconPacket &packet, const FrostbiteRconPacket &lastSentPacket) = 0;

    bool toBool(const QString &value);
    QString toString(bool value);

private:
    int packetReadState;
    char lastHeader[MIN_PACKET_SIZE];
    QVector<FrostbiteRconPacket> packetSendQueue;
    unsigned int nextPacketSequence;

    void clear();
    void sendPacket(const FrostbiteRconPacket &packet, bool response = false);
    void handlePacket(const FrostbiteRconPacket &packet);

    enum PacketReading {
        PacketReadingHeader,
        PacketReadingData
    };

private slots:
    void readyRead();

};

#endif // FROSTBITECONNECTION_H
