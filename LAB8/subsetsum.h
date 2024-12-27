#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits> // Para std::numeric_limits

struct Element {
    double value;
    std::vector<double> partials;
};

std::vector<Element> trim(const std::vector<Element>& data, double delta) {
    std::vector<Element> output;
    double last = -std::numeric_limits<double>::infinity(); // Inicializamos con -∞ para no descartar 0 inicial
    for (const auto& element : data) {
        if (element.value > last * (1 + delta) || std::fabs(element.value - last) > 1e-6) {
            output.push_back(element);
            last = element.value;
        }
        else {
            std::cout << "Eliminando: " << element.value << std::endl;
        }
    }
    return output;
}

std::vector<Element> merge_lists(const std::vector<Element>& m, const std::vector<Element>& n) {
    std::vector<Element> merged(m.begin(), m.end());
    merged.insert(merged.end(), n.begin(), n.end());
    std::sort(merged.begin(), merged.end(), [](const Element& a, const Element& b) {
        return a.value < b.value;
        });
    return merged;
}

Element approximate_subset_sum(const std::vector<double>& data, double target, double epsilon) {
    std::vector<Element> acc = { {0, {}} };
    size_t count = data.size();

    for (double d : data) {
        std::vector<Element> augmented_list;
        for (const auto& a : acc) {
            Element new_element;
            new_element.value = a.value + d;
            new_element.partials = a.partials;
            new_element.partials.push_back(d);
            augmented_list.push_back(new_element);

            std::cout << "Nuevo elemento: " << new_element.value << " con subconjunto: ";
            for (double v : new_element.partials) {
                std::cout << v << " ";
            }
            std::cout << std::endl;
        }

        acc = merge_lists(acc, augmented_list);
        acc = trim(acc, epsilon / (2 * count));

        acc.erase(std::remove_if(acc.begin(), acc.end(), [target](const Element& e) {
            return e.value > target; // Remover valores mayores al objetivo
            }), acc.end());
    }

    Element closest = { 0, {} };
    double max_value = 0;

    for (const auto& e : acc) {
        if (e.value <= target && e.value > max_value) {
            max_value = e.value;
            closest = e;
        }
    }

    return closest;
}