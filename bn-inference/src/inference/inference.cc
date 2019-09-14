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

#include "inference.hh"

#include <bayesiannetwork/bayesiannetwork.hh>
#include <utils/utils.hh>
#include <math.h>
#include <iostream>

Inference::Inference(BayesianNetwork *bn) {
    // Copy reference to network
    _bn = bn;

    // Init tree
    _numOfVariable = _bn->nodeList.size();

    // Resize auxiliar vectors
    _hasEvidence.resize(_numOfVariable);
    _piValue.resize(_numOfVariable);
    _lambdaValue.resize(_numOfVariable);
    _expectation.resize(_numOfVariable);

    // Resize auxiliar matrix lines
    _piMessage.resize(_numOfVariable);
    _lambdaMessage.resize(_numOfVariable);

    // Resize auxiliar matrix itens per lines
    for(int i=0; i<_numOfVariable; i++) {
        _piMessage[i].resize(_numOfVariable);
        _lambdaMessage[i].resize(_numOfVariable);
    }

    // Initialize values
    for(int i=0; i<_numOfVariable; i++) {
        // Initialize vectors
        _hasEvidence[i] = false;     // has no evidence
        _lambdaValue[i] = Normal();  // lambda val: mean 0 and var inf

        for(int j=0; j<_numOfVariable; j++) {
            // Initialize matrix
            _piMessage[i][j] = Normal();     // pi mess:     mean 0 and var inf
            _lambdaMessage[i][j] = Normal(); // lambda mess: mean 0 and var inf
        }
    }

    // For the root nodes initialize expectation and pi values
    for(int i=0; i<_numOfVariable; i++) {
        // If list of parent is empty, node is root node
        if(_bn->nodeList[i]->getParents().isEmpty()) {
            _piValue[i] = _bn->nodeList[i]->getPrior();
            _expectation[i] = _bn->nodeList[i]->getPrior();
        } else {
            _piValue[i] = Normal(); // lambda val: mean 0 and var inf
            _expectation[i] = Normal();
        }
    }

    // Root nodes send pi message
    for(int i=0; i<_numOfVariable; i++) {
        // If list of parent is empty, node is root node
        if(_bn->nodeList[i]->getParents().isEmpty()) {
            for(int j=0; j<_bn->nodeList[i]->getChildren().size(); j++) {
                // Send pi message
                std::cout << "Send Pi Message from #" << _bn->nodeList[i]->getIndex() << " to #" << _bn->nodeList[i]->getChildren()[j]->getIndex() << "\n";
                sendPiMessage(_bn->nodeList[i], _bn->nodeList[i]->getChildren()[j]);
            }
        }
    }

}

void Inference::sendLambdaMessage(Node *child, Node *parent) { // from child to parent
    // Get parent and child index
    int indexPA = parent->getIndex();
    int indexCH = child->getIndex();

    // Calculate lambda message [Variance and Mean]
    double variance = _lambdaValue[indexCH-1].variance() + child->getPrior().variance();
    double mean = _lambdaValue[indexCH-1].mean();

    for(int i=0; i<child->getParents().size(); i++){
        // Get other child index
        int parentIT = child->getParents()[i]->getIndex()-1;

        // If got index is the same as indexPA, skip
        if(parentIT == indexPA-1) { continue; }

        // Calculate lambda message contribution
        variance += pow(_bn->matrixB[indexCH-1][parentIT],2)*_piMessage[indexCH-1][parentIT].variance();
        mean -= _bn->matrixB[indexCH-1][parentIT]*_piMessage[indexCH-1][parentIT].mean();
    }

    // Get final value for lambda message
    variance = variance/pow(_bn->matrixB[indexCH-1][indexPA-1],2);
    mean = mean/_bn->matrixB[indexCH-1][indexPA-1];
    _lambdaMessage[indexCH-1][indexPA-1] = Normal(mean, variance);

    // Calculate lambda value for parent node
    double lambdaVariance = 0;
    double lambdaMean = 0;

    for(int i=0; i<parent->getChildren().size(); i++){
        // Get other child index
        int childIT = parent->getChildren()[i]->getIndex()-1;

        // Calculate lambda message contribution
        double chVariance = _lambdaMessage[childIT][indexPA-1].variance();
        if(chVariance!=qInf()) {
            lambdaVariance += (chVariance==0)? qInf() : 1/chVariance;
            lambdaMean += (chVariance==0)? qInf() : _lambdaMessage[i][indexPA-1].mean()/chVariance;
        } // Else sum zero
    }

    // Set lambda value
    lambdaVariance = (lambdaVariance==qInf())? 0 : (lambdaVariance==0)? qInf() : 1/lambdaVariance;
    lambdaMean = lambdaVariance*lambdaMean;
    _lambdaValue[indexPA-1] = Normal(lambdaMean, lambdaVariance);

    // Calculate expectation
    _expectation[indexPA-1] = Utils::calculateExpectation(_piValue[indexPA-1], _lambdaValue[indexPA-1]);

    // Send lambda message to parents
    for(int i=0; i<parent->getParents().size(); i++){
        // Send lambda message message
        sendLambdaMessage(parent, parent->getParents()[i]);
    }

    // Send lambda message to other children
    for(int i=0; i<parent->getChildren().size(); i++){
        // Get other parent index
        int childIT = parent->getChildren()[i]->getIndex()-1;

        // If got index is the same as indexPA, skip
        if(childIT == indexCH-1) { continue; }

        // Send lambda message message
        sendPiMessage(parent, parent->getChildren()[i]);
    }

}

void Inference::sendPiMessage(Node *parent, Node *child) { // from parent to child
    // Get parent and child index
    int indexPA = parent->getIndex();
    int indexCH = child->getIndex();

    // Calculate pi message [Variance and Mean]
    double variance = (_piValue[indexPA-1].variance()==qInf())? 0 : (_piValue[indexPA-1].variance()==0)? qInf() : 1/_piValue[indexPA-1].variance();
    double mean = (_piValue[indexPA-1].variance()==qInf())? 0 : (_piValue[indexPA-1].variance()==0)? qInf() : _piValue[indexPA-1].mean()/_piValue[indexPA-1].variance();

    for(int i=0; i<parent->getChildren().size(); i++){
        // Get other child index
        int childIT = parent->getChildren()[i]->getIndex()-1;

        // If got index is the same as indexCH, skip
        if(childIT == indexCH-1) { continue; }

        // Calculate lambda message contribution
        double chVariance = _lambdaMessage[childIT][indexPA-1].variance();
        if(chVariance!=qInf()) {
            variance += (chVariance==0)? qInf() : 1/chVariance;
            mean += (chVariance==0)? qInf() : _lambdaMessage[childIT][indexPA-1].mean()/chVariance;
        } // Else sum zero
    }

    // Get final value for pi message
    variance = (variance==qInf())? 0 : (variance==0)? qInf() : 1/variance;
    mean = (variance==qInf())? 0 : (variance==0)? qInf() : mean/variance;
    _piMessage[indexCH-1][indexPA-1] = Normal(mean, variance);

    // Compute child node pi value and expectation if it hasn't evidence
    if(_hasEvidence[indexCH-1]==false) {
        // Calculate pi value for child node
        double piVariance = child->getPrior().variance();
        double piMean = 0;

        for(int i=0; i<child->getParents().size(); i++){
            // Get other child index
            int parentIT = child->getParents()[i]->getIndex()-1;

            piVariance += pow(_bn->matrixB[indexCH-1][parentIT],2)*_piMessage[indexCH-1][parentIT].variance();
            piMean += _bn->matrixB[indexCH-1][parentIT]*_piMessage[indexCH-1][parentIT].mean();
        }

        // Set pi value
        _piValue[indexCH-1] = Normal(piMean, piVariance);

        // Calculate expectation
        _expectation[indexCH-1] = Utils::calculateExpectation(_piValue[indexCH-1], _lambdaValue[indexCH-1]);

        // Send pi message to children of child node
        for(int j=0; j<child->getChildren().size(); j++) {
            // Send pi message
            sendPiMessage(child, child->getChildren()[j]);
        }
    }

    // Send lambda message to other parents [Do not send if X is to initialized]
    if(_hasEvidence[indexCH-1]==true) {
        for(int i=0; i<child->getParents().size(); i++){
            // Get other parent index
            int parentIT = child->getParents()[i]->getIndex()-1;

            // If got index is the same as indexPA, skip
            if(parentIT == indexPA-1) { continue; }

            // Send lambda message message
            sendLambdaMessage(child, child->getParents()[i]);
        }
    }

//    // Debug print
//    std::cout << "PiMessage from #" << indexPA << " to #" << indexCH << " (" << mean << ", " << variance << ")\n";
}

void Inference::updateTree(QVector<QString> name, QVector<double> evidence) {

}
