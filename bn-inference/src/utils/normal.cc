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

#include "normal.hh"
#include <math.h>

Normal::Normal(double mean, double variance) {
    _mean = mean;
    _variance = variance;
}

double Normal::getPDF(double x) {
    if(_mean == qInf() || _variance == qInf()) {
        return 0;
    } else {
        return (1/sqrt(2*M_PI*_variance))*exp(-pow(x-_mean,2)/(2*pow(_variance,2)));
    }
}

double Normal::getStdDeviation() {
    return (_variance != qInf())? sqrt(_variance) : qInf();
}
