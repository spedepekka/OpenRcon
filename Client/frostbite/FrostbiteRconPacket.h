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

#ifndef FROSTBITERCONPACKET_H
#define FROSTBITERCONPACKET_H

#include "FrostbiteRconWord.h"

#define MaxPacketSize 16384
//#define MAX_WORDS 100 // TODO: Think this right.s

class FrostbiteRconPacket : public QObject
{
    Q_OBJECT

public:
    FrostbiteRconPacket(QObject *parent = 0);
    FrostbiteRconPacket(int origin, int type, unsigned int initseq = 0, QObject *parent = 0);
    FrostbiteRconPacket(const FrostbiteRconPacket &packet, QObject *parent = 0);
    FrostbiteRconPacket &operator= (const FrostbiteRconPacket &packet);
    ~FrostbiteRconPacket();

    enum PacketOrigin {
        ServerOrigin,
        ClientOrigin
    };

    enum PacketType {
        Request,
        Response
    };

    void packWord(const FrostbiteRconWord &word);
    unsigned int getSequence() const;
    unsigned int getSequenceNum() const;
    unsigned int getSize() const;
    unsigned int getFullSize() const;
    unsigned int getWordCount() const;
    bool isResponse() const;
    bool isRequest() const;
    const FrostbiteRconWord& getWord(unsigned int index) const;
    void setSequence(int sequence);
    void setSequenceNum(int sequence);
    void clear();

private:
    unsigned int packetSequence;
    unsigned int packetSize; // Total size of packet, in bytes
    unsigned int packetWordCount; // Number of words following the packet header
    QVector<FrostbiteRconWord> packetWords; // RconWord packetWords[MAX_WORDS];

};

inline QDataStream &operator >> (QDataStream &in, FrostbiteRconPacket &packet)
{
    QDataStream::ByteOrder oldbo;
    oldbo = in.byteOrder();

    if (oldbo != QDataStream::LittleEndian) {
        in.setByteOrder(QDataStream::LittleEndian);
    }

    packet.clear();

    unsigned int seq, fsize, words;

    in >> seq;
    in >> fsize;
    in >> words;
    FrostbiteRconWord word;

    packet.setSequence(seq);

    for (unsigned int i = 0; i < words; i++) {
        in >> word;
        packet.packWord(word);
    }

    if (oldbo != QDataStream::LittleEndian) {
        in.setByteOrder(oldbo);
    }

    return in;
}

inline QDataStream &operator << (QDataStream &out, const FrostbiteRconPacket &packet)
{
    if (packet.getFullSize() <= MaxPacketSize) {
        QDataStream::ByteOrder oldbo;
        oldbo = out.byteOrder();

        if (oldbo != QDataStream::LittleEndian) {
            out.setByteOrder(QDataStream::LittleEndian);
        }

        out << packet.getSequence();
        out << packet.getFullSize();
        out << packet.getWordCount();

        for (unsigned int i = 0; i < packet.getWordCount(); i++) {
            out << packet.getWord(i);
        }

        if (oldbo != QDataStream::LittleEndian) {
            out.setByteOrder(oldbo);
        }
    }

    return out;
}

#endif // FROSTBITERCONPACKET_H
