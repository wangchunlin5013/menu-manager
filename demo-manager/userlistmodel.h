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
#ifndef USERLISTMODEL_H
#define USERLISTMODEL_H

#include <QAbstractListModel>

class UserListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        hasSeparator = Qt::UserRole + 100,
        Text,
        Tips
    };
    struct ModelData {
        bool hasSeparator = false;
        QString text;
    };

    explicit UserListModel();
    ~UserListModel();

    void initData();

    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

public:
    void addMenu(const QString &text);

private:
    QList<ModelData> datas;
};

#endif // USERLISTMODEL_H
