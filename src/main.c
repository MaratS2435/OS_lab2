#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#define MAX_POINTS 1000

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point points[MAX_POINTS];
    int num_points;
    int num_clusters;
    Point centroids[MAX_POINTS];
    int cluster_assignment[MAX_POINTS];
    pthread_mutex_t mutex;
} KMeansData;

KMeansData kmeans_data;

void* kmeans_thread(void* arg) {
    int thread_id = *((int*) arg);
    int num_points_per_thread = kmeans_data.num_points / kmeans_data.num_clusters;
    int start = thread_id * num_points_per_thread;
    int end = start + num_points_per_thread;

    // Update centroids
    double sum_x = 0.0, sum_y = 0.0;
    for (int i = start; i < end; ++i) {
        sum_x += kmeans_data.points[i].x;
        sum_y += kmeans_data.points[i].y;
    }
    pthread_mutex_lock(&kmeans_data.mutex);
    kmeans_data.centroids[thread_id].x = sum_x / num_points_per_thread;
    kmeans_data.centroids[thread_id].y = sum_y / num_points_per_thread;
    pthread_mutex_unlock(&kmeans_data.mutex);

    // Assign points to clusters
    for (int i = 0; i < kmeans_data.num_points; ++i) {
        if (i >= start && i < end) {
            double min_dist = INFINITY;
            int cluster_index = 0;
            for (int j = 0; j < kmeans_data.num_clusters; ++j) {
                double dist = sqrt(pow(kmeans_data.points[i].x - kmeans_data.centroids[j].x, 2) +
                                   pow(kmeans_data.points[i].y - kmeans_data.centroids[j].y, 2));
                if (dist < min_dist) {
                    min_dist = dist;
                    cluster_index = j;
                }
            }
            pthread_mutex_lock(&kmeans_data.mutex);
            kmeans_data.cluster_assignment[i] = cluster_index;
            pthread_mutex_unlock(&kmeans_data.mutex);
        }
    }

    return NULL;
}

int main() {
    printf("Enter the number of points: ");
    scanf("%d", &kmeans_data.num_points);

    printf("Enter the number of clusters: ");
    scanf("%d", &kmeans_data.num_clusters);

    if (kmeans_data.num_points <= 0 || kmeans_data.num_clusters <= 0) {
        printf("Invalid input.\n");
        return 1;
    }

    printf("Enter the coordinates of the points (x y):\n");
    for (int i = 0; i < kmeans_data.num_points; ++i) {
        scanf("%lf %lf", &kmeans_data.points[i].x, &kmeans_data.points[i].y);
    }

    clock_t start_time = clock();

    pthread_t threads[MAX_POINTS];
    int thread_ids[MAX_POINTS];

    pthread_mutex_init(&kmeans_data.mutex, NULL);

    // Create threads
    for (int i = 0; i < kmeans_data.num_clusters; ++i) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, kmeans_thread, &thread_ids[i]);
    }

    // Join threads
    for (int i = 0; i < kmeans_data.num_clusters; ++i) {
        pthread_join(threads[i], NULL);
    }

    clock_t end_time = clock();
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // Print cluster assignment
    printf("Cluster assignment:\n");
    for (int i = 0; i < kmeans_data.num_points; ++i) {
        printf("Point (%.2lf, %.2lf) assigned to cluster %d\n", kmeans_data.points[i].x, kmeans_data.points[i].y, kmeans_data.cluster_assignment[i]);
    }

    printf("Execution time: %.6f seconds\n", execution_time);

    pthread_mutex_destroy(&kmeans_data.mutex);

    return 0;
}
