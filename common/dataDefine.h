/*

 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.

 *

 * Author:     wangcl <wangchunlin@uniontech.com>

 *

 * Maintainer: wangcl <wangchunlin@uniontech.com>

 *

 * This program is free software: you can redistribute it and/or modify

 * it under the terms of the GNU General Public License as published by

 * the Free Software Foundation, either version 3 of the License, or

 * any later version.

 *

 * This program is distributed in the hope that it will be useful,

 * but WITHOUT ANY WARRANTY; without even the implied warranty of

 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the

 * GNU General Public License for more details.

 *

 * You should have received a copy of the GNU General Public License

 * along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */
#ifndef DATADEFINE_H
#define DATADEFINE_H

#include <QString>

enum ActionType { Unknow = 0, Action, Separator};
struct ActionData{
    ActionType type;
    QString name;
    QString icon;
    QString tips;
    QString commd;
};

static const QString MENUGROUP("Action");
static const QString MENUTYPE("Type");
static const QString MENUNAME("Name");
static const QString MENUICON("Icon");
static const QString MENUTIPS("Tips");
static const QString MENUCOMMD("Commd");
static const QString MENUTYPEACTION("Action");
static const QString MENUTYPESEPARATOR("Separator");

#endif // DATADEFINE_H
