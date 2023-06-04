#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define EPSILON 0.001

struct point{
    double *cords;
    int dim;
    int c;
    int c_size;
};

typedef struct point Point;

void printPoint(Point p){
    int m;
    for(m = 0; m < p.dim; m++){
        printf("%f,", p.cords[m]);
    }
    printf("\n");
}

void freePointArray(Point* arr, int length) {
    int i;
    for (i = 0; i < length; i++) {
        free((*(arr + i)).cords);
    }
    free(arr);
    return;
}

double* split(char *line, int len){
    double *out;
    int i;
    char *p;
    char* lineBU;

    lineBU = line;

    out = (double*)malloc(sizeof(double)*len);
    if (out == NULL){
        printf("An Error Has Occurred");
        free(lineBU);
        return NULL;
    }

    i = 0;
    while(i < len) {
        p = NULL;
        out[i] = strtod(line, &p);
        i++;
        line = p+1;
    }

    free(lineBU);
    return out;
}

char* retline(){
    int cur_size, i;
    char * curline;
    char c;

    cur_size = 10;
    curline = (char*)malloc(sizeof(char)*cur_size);
    if (curline == NULL) {
        printf("An Error Has Occurred");
        return NULL;
    }
    i = 0;

    while((c = getchar()) != '\n' && c != EOF) {
        if (i >= cur_size) {
            cur_size*=2;
            curline = (char*)realloc(curline, sizeof(char)*cur_size);
            if (curline == NULL) {
                printf("An Error Has Occurred");
                return NULL;
            }
        }
        curline[i] = c;
        i++;
    }
    curline = (char*)realloc(curline, sizeof(char)*(i+1));
    if (curline == NULL) {
        printf("An Error Has Occurred");
        return NULL;
    }
    curline[i]='\0';
    return curline;
}

Point* initPoints(int* lenPointer, int* quanPointer){
    char *line;
    int len, i, cur_size;
    Point *points, p;

    len = 1;
    line = retline();
    if (line == NULL) {
        return NULL;
    }
    for (i=0; line[i]!='\0';i++){
        if (*(line+i) == ',') {
            len++;
        }
    }
    cur_size = 10;
    points = (Point *)malloc(sizeof(Point)*cur_size);
    if (points == NULL) {
        printf("An Error Has Occurred");
        return NULL;
    }

    p.cords = split(line, len);
    if (p.cords == NULL) {
        freePointArray(points, cur_size);
        return NULL;
    }

    p.dim = len;
    p.c = 0;
    p.c_size = 0;

    points[0] = p;
    i = 1;
    while (*(line = retline()) != EOF && *line != '\0'){
        if (line == NULL) {
            freePointArray(points, cur_size);
            printf("An Error Has Occurred");
            return NULL;
        }
        if (i>=cur_size) {
            cur_size *= 2;
            points = (Point*)realloc(points, sizeof(Point)*cur_size);
            if (points == NULL) {
                cur_size /= 2;
                freePointArray(points, cur_size);
                printf("An Error Has Occurred");
                return NULL;
            }
        }
        
        p.cords = split(line, len);
        if (p.cords == NULL) {
            freePointArray(points, cur_size);
            return NULL;
        }
        p.dim = len;
        p.c = 0;
        p.c_size = 0;

        points[i] = p;
        i++;
    }
    free(line);

    points = (Point*)realloc(points, sizeof(Point)*(i));
    if (points == NULL) {
        printf("An Error Has Occurred");
        freePointArray(points, i);
        return NULL;
    }

    *quanPointer = i;
    *lenPointer = len;
    return points;
}

double euclideanDistance(Point x1, Point x2){
    double sum;
    int i;
    sum = 0;
    for (i=0; i< x1.dim;i++){
        sum += pow((x1.cords[i])-(x2.cords[i]),2);
    }
    assert(sum >=0);
    return sqrt(sum);
}

Point copyPoint(Point oldPoint){
    Point newPoint;
    int i;

    newPoint.cords = (double*)malloc(sizeof(double)*oldPoint.dim);
    if (newPoint.cords == NULL) {
        printf("An Error Has Occurred");
        return newPoint;
    }
    newPoint.dim = oldPoint.dim;
    newPoint.c = oldPoint.c;
    newPoint.c_size = oldPoint.c_size;

    for(i = 0; i<oldPoint.dim; i++){
        newPoint.cords[i] = oldPoint.cords[i];
    }
    return newPoint;
}

void nullifyArray(Point *array, int k){
    int i, j;
    for(i = 0; i < k; i++){
        array[i].c = 0;
        array[i].c_size = 0;
        for(j = 0; j<array[0].dim; j++){
            array[i].cords[j]=0;
        }
    }
}



Point* kMeans(int k, int iter, Point *points, int quan){
    int dim, i, curr_iter, closest, j, m;
    double change, min_dist, temp_dist, delta_centroid;
    Point* centroids;
    Point* new_centroids;
    Point p;

    dim = points[0].dim;
    centroids = (Point*)malloc(sizeof(Point)*k);
    new_centroids = (Point*)malloc(sizeof(Point)*k);
    if (centroids == NULL || new_centroids == NULL) {
        printf("An Error Has Occurred");
        freePointArray(centroids, k);
        freePointArray(new_centroids, k);
        return NULL;
    }
    for (i = 0; i<k; i++){
        centroids[i] = copyPoint(points[i]);
        if (centroids[i].cords == NULL) {
            freePointArray(points, quan);
            freePointArray(centroids, k);
            freePointArray(new_centroids, k);
            return NULL;
        }
        p.cords = (double *)malloc(sizeof(double)*dim);
        if (p.cords == NULL) {
            printf("An Error Has Occurred");
            freePointArray(points, quan);
            freePointArray(centroids, k);
            freePointArray(new_centroids, k);
            return NULL;
        }
        p.dim = dim;
        p.c = 0;
        p.c_size = 0;
        new_centroids[i] = p;
    }
    
    change = EPSILON + 1;
    curr_iter=0;
    while (change > EPSILON && curr_iter < iter){
        change = 0;
        nullifyArray(new_centroids,k);
        for(i = 0; i<quan; i++){
            min_dist = euclideanDistance(points[i], centroids[0]);
            closest = 0;
            for(j = 1; j<k; j++){
                temp_dist = euclideanDistance(points[i], centroids[j]);
                if (temp_dist < min_dist) {
                    min_dist = temp_dist;
                    closest = j;
                }
            }
            new_centroids[closest].c_size++;
            for (m=0; m<dim; m++){
                new_centroids[closest].cords[m] += points[i].cords[m];
            }
        }
        for(i = 0; i < k; i++){
            if (new_centroids[i].c_size>0)
            {
                for(j = 0; j < dim; j++){
                    new_centroids[i].cords[j] /= new_centroids[i].c_size;
                }
            delta_centroid = euclideanDistance(new_centroids[i],centroids[i]);
            change = (change < delta_centroid ? delta_centroid : change);
            }
        }

        for (i=0; i < k; i++){
            for (j=0; j < dim; j++){
                centroids[i].cords[j] = new_centroids[i].cords[j];
            }
            
        }
        curr_iter++;
    }
    
    freePointArray(new_centroids, k);
    return centroids;
}

int main(int argc, char** argv){
    double init_k, init_iter;
    int dim, points_quan, k, iter, j, m;
    Point* points, *centroids;

    dim = 0;
    points_quan = 0;
    points = initPoints(&dim, &points_quan);
    if (points == NULL) {
        return 1;
    }

    init_k = strtod(argv[1], NULL);
    if (init_k != floor(init_k) || init_k <= 1 || init_k >= points_quan){
        printf("Invalid number of clusters!");
        freePointArray(points, points_quan);
        return 1;
    }

    k =  init_k;
    iter = 200;
    if(argc == 3) {   
        init_iter = strtod(argv[2], NULL);
        if (init_iter != floor(init_iter) || init_iter <= 1 || init_iter >= 1000){
            printf("Invalid maximum iteration!");
            freePointArray(points, points_quan);
            return 1;
        }
        iter = init_iter;
    }
    
    centroids = kMeans(k,iter,points,points_quan);
    if (centroids == NULL) {
        return 1;
    }

    for(j = 0; j < k; j++){
        for(m = 0; m < dim; m++){
            if(m == dim-1) {
                printf("%.4f", centroids[j].cords[m]);
            } else {
                printf("%.4f,", centroids[j].cords[m]);
            }
        }
        printf("\n");
    }
    freePointArray(points, points_quan);
    freePointArray(centroids, k);

    return 0;
}