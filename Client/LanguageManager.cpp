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

 * You should have received a copy of the GNU General Public License
 * along with OpenRcon.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "LanguageManager.h"

LanguageManager::LanguageManager(QObject *parent) : QObject(parent)
{
    languageList.append(LanguageEntry(tr("English"), "en_US", ":/flags/united_kingdom.png"));
    languageList.append(LanguageEntry(tr("Norwegian"), "nb_NO", ":/flags/norway.png"));
}

LanguageManager::~LanguageManager()
{

}

LanguageEntry LanguageManager::getLanguage(int index)
{
    return languageList.at(index);
}

LanguageEntry LanguageManager::getLanguage(const QString &code)
{
    foreach (LanguageEntry language, languageList) {
        if (language.code == code) {
            return language;
        }
    }

    return LanguageEntry();
}

QList<LanguageEntry> LanguageManager::getLanguages()
{
    return languageList;
}
