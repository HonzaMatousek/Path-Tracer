//
// Created by honza on 31.05.20.
//

#include <cmath>
#include "BlackBodyRadiation.h"

double PlanckLaw(double f, double T) {
    static double h = 6.62607015e-34; // Planck constant
    static double c = 299792458;      // Speed of light
    static double k = 1.380649e-23;   // Boltzmann constant
    return (2 * h * f * f * f) / (c * c * (std::exp(h*f/k/T)));
}

BlackBodyRadiation::BlackBodyRadiation(double temperatureInKelvins) {
    static double c = 299792458;      // Speed of light
    r = PlanckLaw(c / 608e-9, temperatureInKelvins);
    g = PlanckLaw(c / 552e-9, temperatureInKelvins);
    b = PlanckLaw(c / 463e-9, temperatureInKelvins);
}
