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

#ifndef BF4LEVELDICTIONARY_H
#define BF4LEVELDICTIONARY_H

#include "LevelDictionary.h"
#include "BF4GameModeEntry.h"

class BF4LevelDictionary : public LevelDictionary
{
    Q_OBJECT

public:
    BF4LevelDictionary(QObject *parent = 0);
    ~BF4LevelDictionary();

};

#endif // BF4LEVELDICTIONARY_H
