#pragma once

#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

inline std::string loadProgram(const char *filename) {
    std::ifstream file(filename);
    return {std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()};
}

inline std::vector <cl_float4> loadAtoms(const char *filename) {
    std::vector <cl_float4> atoms;
    std::ifstream file(filename);
    while (true) {
        float x, y, z;
        file >> x >> y >> z;
        if (!file)
            break;
        atoms.push_back({x, y, z, 0});
    }
    return atoms;
}

inline std::vector <std::vector<int>> loadBounds(const char *filename,
                                                 size_t atomsCount) {
    std::vector <std::vector<int>> bounds;
    bounds.resize(atomsCount);
    std::ifstream file(filename);
    while (true) {
        size_t x, y;
        file >> x >> y;
        if (!file)
            break;
        bounds[x].emplace_back(y);
        bounds[y].emplace_back(x);
    }
    return bounds;
}

inline std::vector <cl_float> loadCharges(const char *filename) {
    std::vector <cl_float> charges;
    std::ifstream file(filename);
    while (true) {
        cl_float x;
        file >> x;
        if (!file)
            break;
        charges.emplace_back(x);
    }
    return charges;
}

inline std::pair <std::vector<cl_int>, std::vector<cl_int>>
convertMatrix(const std::vector <std::vector<int>> &bounds) {
    std::vector <cl_int> edgesCount;
    edgesCount.resize(bounds.size());
    std::vector <cl_int> edges;
    edges.resize(bounds.size() * bounds.size());

    for (int i = 0; i < bounds.size(); i++) {
        for (int j = 0; j < bounds[i].size(); j++)
            edges[j + i * bounds.size()] = bounds[i][j];
        edgesCount[i] = bounds[i].size();
    }
    return {edges, edgesCount};
}


struct Timer {
    Timer() { start(); }

    void start() { t1 = std::chrono::high_resolution_clock::now(); }

    void stop(const char *message) {
        auto t2 = std::chrono::high_resolution_clock::now();
        std::cout << message << ": " << (t2 - t1).count() / 1e6 << " ms"
                  << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::system_clock> t1;
};
