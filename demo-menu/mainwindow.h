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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class MainWindowPrivate;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~MainWindow();

protected:
    MainWindow(MainWindowPrivate &dd, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    Q_DECLARE_PRIVATE(MainWindow)
};

#endif // MAINWINDOW_H
