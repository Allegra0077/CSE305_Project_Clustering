// CLI entry point for the MST-based single-link clustering executable.
// Usage: ./hc_mst_cpu <dataset.csv>
//
// For now, this just prints the MST. We'll add dendrogram output in the next step.

#include "boruvka_sequential.h"
#include "common/dataset_loader.h"

#include <chrono>
#include <iostream>
#include <iomanip>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <dataset.csv>\n";
        return 1;
    }

    std::vector<Point> points = load_dataset(argv[1]);
    std::cerr << "Loaded " << points.size() << " points.\n";

    auto t0 = std::chrono::steady_clock::now();
    std::vector<MSTEdge> mst = boruvka_mst_sequential(points);
    auto t1 = std::chrono::steady_clock::now();

    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    std::cerr << "runtime_ms: " << ms << "\n";

    // For now, print MST edges to stdout.
    std::cout << "u,v,weight\n";
    std::cout << std::setprecision(10);
    for (const auto& e : mst) {
        std::cout << e.u << "," << e.v << "," << e.weight << "\n";
    }
    return 0;
}