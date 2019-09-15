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

#ifndef INFERENCE_HH
#define INFERENCE_HH

#include <QtCore>
#include <utils/normal.hh>

class BayesianNetwork;
class Node;

class Inference {
public:
    Inference(BayesianNetwork* bn);

    // Belief Propagation for GBN
    void updateTree(QVector<QString> nodes, QVector<double> evidence);
    void sendPiMessage(Node* parent, Node* child);      // Sender, Receiver
    void sendLambdaMessage(Node* child, Node* parent);  // Sender, Receiver

    // Reset
    void resetEvidences();

    // Get expectations
    QHash<QString, Normal> getExpectation(QVector<QString> namelist);

private:
    // Network
    BayesianNetwork* _bn;
    int _numOfVariable;

    // Belief propagation auxiliar distributions
    QVector<bool> _hasEvidence;
    QVector<Normal> _piValue;
    QVector<Normal> _lambdaValue;
    QVector<Normal> _expectation;
    QVector<QVector<Normal> > _piMessage;
    QVector<QVector<Normal> > _lambdaMessage;
};

#endif // INFERENCE_HH
