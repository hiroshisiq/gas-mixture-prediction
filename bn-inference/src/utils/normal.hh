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

#ifndef NORMAL_HH
#define NORMAL_HH

#include <QtGlobal>
#include <iostream>

class Normal {
public:
    Normal(double mean = 0, double variance = qInf());

    // Get values
    double mean() { return _mean; }
    double variance() { return _variance; }
    double getPDF(double x);
    double getStdDeviation();

    // Set values
    void setMean(double mean) { _mean = mean; }
    void setVariance(double variance) { _variance = variance; }

private:
    double _mean;
    double _variance;
};

#endif // NORMAL_HH
