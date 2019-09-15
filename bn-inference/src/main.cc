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

#include <QCoreApplication>
#include <bayesiannetwork/bayesiannetwork.hh>
#include <inference/inference.hh>

#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Bayesian Network
    BayesianNetwork bn("../../bn-inference/data/gas-bn-v1.json");

    // Init inference module
    Inference bpInference(&bn);
    QVector<QString> nodes({"Sensor1", "Sensor2", "Sensor3", "Sensor4", "Sensor5", "Sensor6", "Sensor7", "Sensor8", "Sensor9", "Sensor10", "Sensor11", "Sensor12", "Sensor13", "Sensor14", "Sensor15", "Sensor16"});

    // Update network with evidence
    // CO2 (253.33), Ethylene (6.67)
    QVector<double> evidence({2330.91,3856.25,3488.77,3990.66,2125.76,2553.89,4134.21,4354.47,614.88,760.26,3598.64,3086.82,1106.49,1175.79,3798.68,3481.08});
    bpInference.updateTree(nodes, evidence);

    // Get expectation of some nodes
    QVector<QString> expNodes({"CO2", "Ethylene"});
    QVector<Normal> exp = bpInference.getExpectation(expNodes);
    for(Normal n : exp) {
        std::cout << "Expectation: (" << n.mean() << ", " << n.variance() << ")\n";
    }

    return app.exec();
}
