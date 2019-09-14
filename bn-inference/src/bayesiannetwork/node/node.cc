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

#include "node.hh"

Node::Node(QString name, quint32 index, Normal prior) {
    // Set identifiers
    _name = name;
    _index = index;
    _prior = prior;
}

Node& Node::addPA(Node *parent) {
    _pa.append(parent);
    return (*this);
}

Node& Node::addCH(Node *child) {
    _ch.append(child);
    return (*this);
}
