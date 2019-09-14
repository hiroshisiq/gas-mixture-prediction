/*
 *  This file is part of bn-inference.
 *  Copyright (C) 2019, Hiroshi Siqueira.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef NODE_HH
#define NODE_HH

#include <utils/normal.hh>
#include <QtCore>

class Node {
public:
    Node(QString name, quint32 index, Normal prior = Normal());

    // Add parent or children
    Node& addPA(Node* parent);
    Node& addCH(Node* child);

    // Getter
    QString getName() { return _name; }
    quint32 getIndex() { return _index; }
    Normal getPrior() { return _prior; }
    QList<Node*> getParents() { return _pa; }
    QList<Node*> getChildren() { return _ch; }

    // Operator overload
    static bool compareIndex(Node* n1, Node* n2) { return (n1->getIndex() < n2->getIndex()); }

private:
    // Identification
    QString _name;
    quint32 _index;

    // Node distributions:
    Normal _prior;

    // List of children (CH) and parents (PA)
    QList<Node*> _ch;
    QList<Node*> _pa;
};

#endif // NODE_HH
