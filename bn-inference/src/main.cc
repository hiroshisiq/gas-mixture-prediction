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
#include <utils/utils.hh>

#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Bayesian Network
    BayesianNetwork bn("../../bn-inference/data/gas-bn-v1.json");

    // Init inference module
    Inference bpInference(&bn);

    // Load test file
    QFile file("../../bn-inference/data/test_sample.csv");
    if (!file.open(QIODevice::ReadOnly)) {
        std::cout << file.errorString().toStdString() << "\n";
        return 1;
    }

    // Open test write file
    QFile fileOut("../../bn-inference/data/test_output.csv");
    if (fileOut.open(QFile::WriteOnly | QFile::Truncate)==false) {
        return 0;
    }

    // Create stream to file
    QTextStream stream(&fileOut);
    stream << "CO2" << ", " << "Ethylene" << "\n"; // this writes first line with two columns

    // Evidence nodes
    QVector<QString> nodes({"Sensor1", "Sensor2", "Sensor3", "Sensor4", "Sensor5", "Sensor6", "Sensor7", "Sensor8", "Sensor9", "Sensor10", "Sensor11", "Sensor12", "Sensor13", "Sensor14", "Sensor15", "Sensor16"});

    // Row counter
    int rowCounter = 0;

    // Run inference
    while (!file.atEnd()) {
        // Read Line
        QByteArray line = file.readLine();
        QString string_line = QString::fromLocal8Bit(line.data());
        QStringList wordList(string_line.split(','));

        // Skip header
        if(rowCounter++==0) { continue; }

        // Convert line to evidence array
        QVector<double> evidence;
        for(int i=2; i<wordList.size(); i++) {
            // Remove any extra information from number
            wordList[i] = wordList[i].simplified();

            // Convert to double
            bool ok;
            double number = wordList[i].toDouble(&ok);

            // Return error in case conversion don go well
            if(ok!=true) return 1;

            // Append to evidence list
            evidence.append(number);
        }

        // Update with evidence
        bpInference.updateTree(nodes, evidence);

        // Get expectation of some nodes
        QVector<QString> expNodes({"CO2", "Ethylene"});
        QHash<QString, Normal> exp = bpInference.getExpectation(expNodes);

        // Write expectation into file
        for(QString name : expNodes) {
            // Get values
            double mean = exp[name].mean();
            double var = exp[name].variance();

            // Write CO2
            if(name=="CO2") {
                // Max and Min values
                mean = (mean>600)? 600 : mean;
                mean = (mean<0)?     0 : mean;

                // Write to file
                stream << mean << ",";
            }

            // Write CO2
            if(name=="Ethylene") {
                // Max and Min values
                mean = (mean>20)? 20 : mean;
                mean = (mean<0)?   0 : mean;

                // Write to file
                stream << mean << "\n";
            }

        }

        // Reset
        bpInference.resetEvidences();

        // Print progress
        Utils::printProgress(rowCounter, 200000);
    }

    file.close();
    fileOut.close();
    return 0;
    return app.exec();
}
