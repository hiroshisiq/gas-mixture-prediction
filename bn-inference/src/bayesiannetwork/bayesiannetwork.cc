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

#include "bayesiannetwork.hh"
#include <iostream>

// Important: this function only works if the nodes on the json are sorted by index
BayesianNetwork::BayesianNetwork(QString jsonFileName) {
    // Verbose
    std::cout << "[BAYESIAN NETWORK] Try to read JSON file" << std::endl;

    // Load file
    QFile file(jsonFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
       std::cout << file.errorString().toStdString() << "\n";
       exit(EXIT_FAILURE);
    } else {
        std::cout << "[BAYESIAN NETWORK] File \""+ jsonFileName.toStdString() +"\" loaded successfully\n";
    }

    // Convert text into JSON object
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject jsonModel = doc.object();
    file.close();

    // Load parameters
    loadNodes(jsonModel);
    loadMatrixB(jsonModel);

    std::cout << "[BAYESIAN NETWORK] Model created successfully" << std::endl;
}

void BayesianNetwork::loadNodes(QJsonObject jsonModel) {
    // Get nodes configuration
    QJsonValue value = jsonModel.value("nodes");
    QJsonArray nodeArray = value.toArray();

    // Create nodes and append to list
    foreach (const QJsonValue & node, nodeArray) {
        // Get node id from json
        QString name = node.toObject().value("name").toString();
        quint32 index = node.toObject().value("index").toInt();

        // Get node distribution
        double mean = node.toObject().value("prior").toObject().value("mean").toDouble();
        double variance = node.toObject().value("prior").toObject().value("var").toDouble();

//        // Debug print
//        std::cout << "Loaded Node: " << name.toStdString() << " / " << index << " / " << mean << " / " << variance << "\n";

        // Create and append node read from json file
        nodeList.append(new Node(name, index, Normal(mean, variance)));
    }

//    // Sort created list by index (to prevent error when assign parent/child relationship)
//    std::sort(nodeList.begin(), nodeList.end(), Node::compareIndex);

    // Create Parent/Child link (note that this will only work if the nodes in the json file is sorted by index)
    foreach (const QJsonValue & node, nodeArray) {
        // Check if node has parent
        if(node.toObject().contains("parent")==false)
            continue;

        // Get it node index
        quint32 index = node.toObject().value("index").toInt();

        // Get node parents index from json
        QJsonArray parentList = node.toObject().value("parent").toArray();
        foreach (const QJsonValue& parentIndex, parentList) {
            nodeList.at(index-1)->addPA(nodeList.at(parentIndex.toInt()-1));
            nodeList.at(parentIndex.toInt()-1)->addCH(nodeList.at(index-1));
        }
    }
}

void BayesianNetwork::loadMatrixB(QJsonObject jsonModel) {
    // Redimension matrixB to system size
    resizeMatrixB();

    // Get nodes configuration
    QJsonValue value = jsonModel.value("b");
    QJsonArray bMatrix = value.toArray();

    // Line/Column counter
    int i=0, j=0;

    // Read b value matrix
    foreach (const QJsonValue& line, bMatrix) {
        QJsonArray column = line.toArray();
        foreach(const QJsonValue& item, column) {
            matrixB[i][j] = item.toDouble();
            j++;
        }
        i++; j=0;
    }
}

void BayesianNetwork::resizeMatrixB() {
    // Get node list size
    int targetSize = nodeList.size();

    // Resize number of lines
    matrixB.resize(targetSize);

    // Rezise each line
    for(int i=0; i<targetSize; i++) {
        matrixB[i].resize(targetSize);
    }
}
