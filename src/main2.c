#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

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
    int valid;
    int num_threads;
} KMeansData;

KMeansData kmeans_data;

void* kmeans_update_centroids(void* arg) {
    int thread_id = *((int*) arg);
    //int capacity = kmeans_data.num_points/kmeans_data.num_clusters;
    //int* points_on_centroid = (int*)malloc(sizeof(int) * capacity);

    double sum_x = 0.0, sum_y = 0.0;
    int points_on_centroid = 0;
    for (int i = 0; i < kmeans_data.num_points; ++i) {
        if (kmeans_data.cluster_assignment[i] == thread_id) {
            // Update centroids
            sum_x += kmeans_data.points[i].x;
            sum_y += kmeans_data.points[i].y;
            ++points_on_centroid;
        }
    }
    sum_x /= points_on_centroid;
    sum_y /= points_on_centroid;
    /*if (kmeans_data.centroids[thread_id].x == sum_x && 
        kmeans_data.centroids[thread_id].y == sum_y) {
            kmeans_data.valid++;

            return NULL;
    }*/
    //kmeans_data.valid = 0;
    pthread_mutex_lock(&kmeans_data.mutex);
    kmeans_data.centroids[thread_id].x = sum_x;
    kmeans_data.centroids[thread_id].y = sum_y;
    pthread_mutex_unlock(&kmeans_data.mutex);

    pthread_exit(NULL);
}

void assign(/*void* arg*/) {
    /*int thread_id = *((int*) arg);
    int start = thread_id * (kmeans_data.num_points / kmeans_data.num_threads);
    int end = start + (kmeans_data.num_points / kmeans_data.num_threads);
    if (thread_id == kmeans_data.num_threads - 1)
        end += kmeans_data.num_points % kmeans_data.num_threads;
    */
    double min_dist = INFINITY;
    int cluster_index = 0;
    for (int i = 0; i < kmeans_data.num_points; ++i) {
        for (int j = 0; j < kmeans_data.num_clusters; ++j) {
                double dist = pow(kmeans_data.points[i].x - kmeans_data.centroids[j].x, 2) +
                              pow(kmeans_data.points[i].y - kmeans_data.centroids[j].y, 2);
                if (dist < min_dist) {
                    min_dist = dist;
                    cluster_index = j;
                }
        }

        //pthread_mutex_lock(&kmeans_data.mutex);
        kmeans_data.cluster_assignment[i] = cluster_index;
        //pthread_mutex_unlock(&kmeans_data.mutex);
    }

    //pthread_exit(NULL);
}

int main(void) {
    printf("Enter the number of points: ");
    scanf("%d", &kmeans_data.num_points);

    printf("Enter the number of clusters: ");
    scanf("%d", &kmeans_data.num_clusters);

    printf("Enter the number of threads: ");
    scanf("%d", &kmeans_data.num_threads);

    if (kmeans_data.num_points <= 0 || kmeans_data.num_clusters <= 0 || kmeans_data.num_threads <= 0) {
        printf("Invalid input.\n");
        return 1;
    }

    printf("Enter the coordinates of the points (x y):\n");
    for (int i = 0; i < kmeans_data.num_points; ++i) {
        scanf("%lf %lf", &kmeans_data.points[i].x, &kmeans_data.points[i].y);
    }

    clock_t start_time = clock();

    pthread_t threads[kmeans_data.num_threads];
    int thread_ids[MAX_POINTS];

    pthread_mutex_init(&kmeans_data.mutex, NULL);

    for (int i = 0; i < kmeans_data.num_clusters; ++i) {
        kmeans_data.centroids[i].x = kmeans_data.points[i].x; //* (kmeans_data.num_points / kmeans_data.num_clusters)].x;
        kmeans_data.centroids[i].y = kmeans_data.points[i].y; //* (kmeans_data.num_points / kmeans_data.num_clusters)].y;
    }

    //int temp_num_clusters = kmeans_data.num_clusters;
    //int num_active_threads = 0;
    int iter = 0;
    while(iter != 100){
        /*for (int i = 0; i < kmeans_data.num_threads; ++i) {
            thread_ids[i] = i;
            pthread_create(&threads[i], NULL, assign, &thread_ids[i]);
        }
        // Join threads
        for (int i = 0; i < kmeans_data.num_threads; ++i) {
            pthread_join(threads[i], NULL);
        }*/

        //temp_num_clusters = kmeans_data.num_clusters;
        assign();
        //int i = 0;
        for (int j = 0; j < kmeans_data.num_threads; ++j) {
            thread_ids[j] = j;
            pthread_create(&threads[j], NULL, kmeans_update_centroids, &thread_ids[j]);
            //i++;
        }
        // Join threads
        for (int j = 0; j < kmeans_data.num_threads; ++j) {
            pthread_join(threads[j], NULL);
        }
        //kmeans_data.valid = 0;
        /*while (temp_num_clusters != 0) {
            if (temp_num_clusters > kmeans_data.num_threads) {
                for (int j = 0; j < kmeans_data.num_threads; ++j) {
                    thread_ids[i] = i;
                    pthread_create(&threads[j], NULL, kmeans_update_centroids, &thread_ids[i]);
                    i++;
                }
                // Join threads
                for (int j = 0; j < kmeans_data.num_threads; ++j) {
                    pthread_join(threads[j], NULL);
                }

                temp_num_clusters -= kmeans_data.num_threads;
            } else {
                for (int j = 0; j < temp_num_clusters; ++j) {
                    thread_ids[i] = i;
                    pthread_create(&threads[j], NULL, kmeans_update_centroids, &thread_ids[i]);
                    i++;
                }
                // Join threads
                for (int j = 0; j < temp_num_clusters; ++j) {
                    pthread_join(threads[j], NULL);
                }

                temp_num_clusters = 0;
            }
        }*/
        iter++; 
    }
    assign();

    /*for (int i = 0; i < kmeans_data.num_threads; ++i) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, assign, &thread_ids[i]);
    }
    // Join threads
    for (int i = 0; i < kmeans_data.num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }*/

    clock_t end_time = clock();
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Cluster assignment:\n");
    for (int i = 0; i < kmeans_data.num_points; ++i) {
        printf("Point (%.2lf, %.2lf) assigned to cluster %d\n", kmeans_data.points[i].x, kmeans_data.points[i].y, kmeans_data.cluster_assignment[i]);
    }

    printf("Execution time: %.6f seconds\n", execution_time);

    pthread_mutex_destroy(&kmeans_data.mutex);

    return 0;
}
