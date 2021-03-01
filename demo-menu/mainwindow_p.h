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
#ifndef MAINWINDOW_P_H
#define MAINWINDOW_P_H

#include "mainwindow.h"

#include <private/qwidget_p.h>

enum ActionType { Unknow = 0, Action, Separator};
struct ActionData{
    ActionType type;
    QString name;
    QString icon;
    QString tips;
    QString commd;
};

class MainWindowPrivate : public QWidgetPrivate
{
public:
    explicit MainWindowPrivate();
    ~MainWindowPrivate();

    void init();
    void parseFile();

    QString configPath;
    bool hasCustom = false;
    QList<ActionData> actiondatas;

private:
    Q_DECLARE_PUBLIC(MainWindow)
};

#endif // MAINWINDOW_P_H
