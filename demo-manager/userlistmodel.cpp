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
#include "userlistmodel.h"
#include "../common/dataDefine.h"
#include "menumanager.h"

UserListModel::UserListModel()
{
    initData();
}

UserListModel::~UserListModel()
{

}

void UserListModel::initData()
{
    datas.clear();
    const QList<ActionData> &userMenus = MenuManager::instance()->getUserMenus();
    for(int i=0; i<userMenus.count(); i++) {
        const ActionData &menu = userMenus.at(i);
        if (menu.type == ActionType::Action) {
            ModelData data;
            data.text = menu.name;
            datas.append(data);
        } else if (menu.type == ActionType::Separator && 0 != i) {
            // 保证先有菜单项，再有分隔符
            datas[datas.count()-1].hasSeparator = true;
        }
    }
    if (!datas.isEmpty()) {
        // 保证最后一个菜单的后面不存在无效的分隔符
        datas[datas.count()-1].hasSeparator = false;
    }
}

QModelIndex UserListModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int UserListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return datas.count();
}

QVariant UserListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= datas.count() || index.column() >= 1)
        return QVariant();
    switch (role) {
    case hasSeparator:
        return datas.at(index.row()).hasSeparator;
    case Text:
    case Qt::DisplayRole:
        return datas.at(index.row()).text;
    default:
        return QVariant();
    }
}

void UserListModel::addMenu(const QString &text)
{
    ModelData menu;
    menu.text = text;
    beginInsertRows(QModelIndex(), datas.count(), datas.count() + 1);
    datas.append(menu);
    endInsertRows();
}

