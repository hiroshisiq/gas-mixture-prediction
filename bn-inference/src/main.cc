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

/// EX 44
    BayesianNetwork bn("../../bn-inference/data/ex-44.json");

    // Init inference module
    Inference bpInference(&bn);

    // Set evidence
    QVector<QString> nodesEvidence({"Y"});
    QVector<double> evidence({300});
    bpInference.updateTree(nodesEvidence, evidence);

    // Get expectation
    QVector<QString> nodesExpectation({"X"});
    QVector<Normal> exp = bpInference.getExpectation(nodesExpectation);
    for(Normal n : exp) { std::cout << "Expectation: (" << n.mean() << ", " << n.variance() << ")\n"; }

/// EX 45
//    BayesianNetwork bn("../../bn-inference/data/ex-45.json");

//    // Init inference module
//    Inference bpInference(&bn);

//    // Set evidence
//    QVector<QString> nodesEvidence({"D"});
//    QVector<double> evidence({8000});
//    bpInference.updateTree(nodesEvidence, evidence);

//    // Get expectation
//    QVector<QString> nodesExpectation({"P"});
//    QVector<Normal> exp = bpInference.getExpectation(nodesExpectation);
//    for(Normal n : exp) { std::cout << "Expectation: (" << n.mean() << ", " << n.variance() << ")\n"; }


    // Create bayesian network
//    BayesianNetwork bn("../../bn-inference/data/gas-bn.json");

//    // Init inference module
//    Inference bpInference(&bn);
//    QVector<QString> nodes;
//    nodes.append("Sensor1");
//    nodes.append("Sensor2");
//    nodes.append("Sensor3");
//    nodes.append("Sensor4");
//    nodes.append("Sensor5");
//    nodes.append("Sensor6");
//    nodes.append("Sensor7");
//    nodes.append("Sensor8");
//    nodes.append("Sensor9");
//    nodes.append("Sensor10");
//    nodes.append("Sensor11");
//    nodes.append("Sensor12");
//    nodes.append("Sensor13");
//    nodes.append("Sensor14");
//    nodes.append("Sensor15");
//    nodes.append("Sensor16");


//    QVector<QString> nodes1({"CO2", "Ethylene"});

//    // Data 1
//    std::cout << "Data 1:\n";
//    QVector<double> evidence({-50.85, -1.95, -41.82, 1.30, -4.07, -28.73, -13.49, -3.25, 55139.95, 50669.50, 9626.26, 9762.62, 24544.02, 21420.68, 7650.61, 6928.42});
////    QVector<double> evidence({2330.91,3856.25,3488.77,3990.66,2125.76,2553.89,4134.21,4354.47,614.88,760.26,3598.64,3086.82,1106.49,1175.79,3798.68,3481.08});
////    QVector<double> evidence({});
//    bpInference.updateTree(nodes, evidence);

//    QVector<Normal> exp = bpInference.getExpectation(nodes1);
//    for(Normal n : exp) { std::cout << "Expectation: (" << n.mean() << ", " << n.variance() << ")\n"; }

    return app.exec();
}

//CO2,Ethylene,Sensor1,Sensor2,Sensor3,Sensor4,Sensor5,Sensor6,Sensor7,Sensor8,Sensor9,Sensor10,Sensor11,Sensor12,Sensor13,Sensor14,Sensor15,Sensor16
//253.33,6.67,2330.91,3856.25,3488.77,3990.66,2125.76,2553.89,4134.21,4354.47,614.88,760.26,3598.64,3086.82,1106.49,1175.79,3798.68,3481.08
//0.0,0.0,2668.56,134.1,4067.87,4572.36,1833.13,2190.23,4479.64,4787.72,1616.34,1635.48,4806.25,3838.46,861.46,992.63,4613.61,4173.99
//0.0,16.67,1606.25,101.61,7606.1,8190.43,2443.54,2597.39,8824.11,9416.62,1086.85,1126.2,7911.49,6252.68,1048.37,1203.71,8960.62,6723.83
//520.0,0.0,3421.0,6267.74,4499.46,5152.04,2538.26,2961.75,5210.18,5581.65,787.65,960.86,4711.23,4074.63,1237.31,1279.6,4928.93,4510.07
//306.67,6.67,3064.28,66.85,4418.81,4905.75,2065.27,2435.74,5137.48,5456.6,1677.5,1669.74,4963.1,4122.51,994.6,1098.47,5105.84,4657.19
//0.0,0.0,1220.07,15.0,2419.65,2632.62,1251.3,1574.76,2538.26,2713.65,909.48,892.27,2942.64,2221.51,722.33,806.44,3126.52,2210.1
//0.0,6.67,1216.59,15.65,4557.52,4919.14,1651.15,1895.22,5280.16,5623.25,1057.32,1075.66,5429.43,4224.2,792.58,909.28,6181.2,4518.64
//0.0,0.0,1021.62,-5.86,1435.17,1605.37,1088.45,1474.03,1636.8,1750.76,866.64,847.46,1803.58,1440.49,656.49,719.71,1898.21,1474.89
//333.33,0.0,1681.05,35.77,3868.14,4329.03,2003.28,2410.37,4749.02,5008.06,1059.51,1057.92,4173.01,3503.0,988.5,1085.85,4509.04,4115.38
