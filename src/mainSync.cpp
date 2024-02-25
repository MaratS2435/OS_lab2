#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

struct Point {
    double x;
    double y;
};

double distance(const Point p1, const Point p2) {
    return std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2);
}

std::vector<std::vector<Point>> kMeansClustering(const std::vector<Point>& points, int k, int maxIterations) {
    int pointsCount = points.size();
    std::vector<Point> centroids(k);
    std::vector<int> clusterAssignments(pointsCount);

    for (int i = 0; i < k; ++i) {
        centroids[i] = points[i];
    }

    for (int i = 0; i < maxIterations; ++i) {
        std::vector<int> clusterCounts(k, 0);
        std::vector<Point> newCentroids(k, {0.0, 0.0});

        for (int i = 0; i < pointsCount; ++i) {
                int closestCluster = 0;
                double closestDistance = std::numeric_limits<double>::max();
                for (int j = 0; j < k; ++j) {
                    double dist = distance(points[i], centroids[j]);
                    if (dist < closestDistance) {
                        closestDistance = dist;
                        closestCluster= j;
                    }
                }
                clusterAssignments[i] = closestCluster;
                ++clusterCounts[closestCluster];
                newCentroids[closestCluster].x += points[i].x;
                newCentroids[closestCluster].y += points[i].y;
        }

        for (int i = 0; i < k; ++i) {
            if (clusterCounts[i] > 0) {
                centroids[i].x = newCentroids[i].x / clusterCounts[i];
                centroids[i].y = newCentroids[i].y / clusterCounts[i];
            }
        }
    }

    std::vector<std::vector<Point>> result(pointsCount);
    for (int i = 0; i < pointsCount; ++i) {
        result[clusterAssignments[i]].push_back(points[i]);
    }
    return result;
}

int main() {
    std::vector<Point> data;
    int maxIterations = 100;
    int clustersCount;
    int point_count;
    int num_threads;

    std::cout << "Enter the number of points: " << std::endl;
    std::cin >> point_count;
    //point_count = 10;
    std::cout << "Enter the number of clusters: " << std::endl;
    std::cin >> clustersCount;
    //std::cout << "Enter the number of threads: " << std::endl;
    //std::cin >> num_threads;

    std::cout << "Enter the coordinates of the points (x y):\n" << std::endl;
    Point temp;
    for (int i = 0; i < point_count; ++i) {
        //std::cin >> temp.x >> temp.y;
        temp.x = std::rand() % 101;
        temp.y = std::rand() % 101;
        data.push_back(temp);
    }

    auto start = std::chrono::steady_clock::now();

    std::vector<std::vector<Point>> result = kMeansClustering(data, clustersCount, maxIterations);

    auto end = std::chrono::steady_clock::now();

    for (int i = 0; i < clustersCount; ++i) {
        for (size_t j = 0; j < result[i].size(); ++j) {
            std::cout << i << ' ' << result[i][j].x << ' ' << result[i][j].y  << std::endl;
        }
    }
    std::cout << "milliseconds: ";
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
}