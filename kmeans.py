import sys

class Point():
    def __init__(self,L,cluster=-1):
       self.p = L
       self.dim = len(L)
       self.cluster = cluster
       
       
    def __add__(self,p2):
        assert self.dim == p2.dim  
        return Point([self.p[i]+p2.p[i] for i in range(self.dim)])

    def dist(self,p2):
        assert self.dim == p2.dim
        sum = 0
        for i in range(p2.dim):
            sum += (self.p[i]-p2.p[i])**2
        return sum**0.5
    
    def update_cluster(self,centroids):
        argmin = 0
        min_dist = self.dist(centroids[0])
        for i in range(len(centroids)):
            temp_dist = self.dist(centroids[i])
            if temp_dist < min_dist:
                min_dist = temp_dist
                argmin = i
        self.cluster = argmin
        
    def __repr__(self):
        out = ''
        for cord in self.p:
            out += "{c:.4f},".format(c=cord)
        out = out.rstrip(',')
        return out        
    
        
def cent_delta(centroids, new_centroids, eps):
    delta = eps+1
    for i in range(len(centroids)):
        delta = min(delta, centroids[i].dist(new_centroids[i]))
    return delta

def kmeans(k, iter, points,eps=0.001):        
    centroids = [Point(list.copy(points[i].p)) for i in range(k)]
    new_centroids = [Point([0 for i in range(centroids[0].dim)]) for j in range(k)]
    delta = eps + 1
    curr_iter = 0
    while (delta > eps and curr_iter < iter):
        for point in points:
            point.update_cluster(centroids)
        
        cluster_nums = [0 for i in range(k)]
        for point in points:
            new_centroids[point.cluster] += point
            cluster_nums[point.cluster] += 1
            
        for i,p in enumerate(new_centroids):
            for j in range(p.dim):
                assert cluster_nums[i] != 0
                p.p[j] /= cluster_nums[i]
        
        for i in range(len(centroids)):
            for j in range(len(centroids[i].p)):
                centroids[i].p[j] = new_centroids[i].p[j]
                new_centroids[i].p[j] = 0
        
        delta = cent_delta(centroids, new_centroids, eps)
        curr_iter += 1
    
    for x in centroids:
        print(x)

          


if (len(sys.argv) == 4):
    (_,k,iter,file_loc) = sys.argv
    if (not iter.isdigit()):
        print("Invalid maximum iteration!")
        exit()
    iter = float(iter)
    if (iter != iter//1 or iter <= 1 or iter >= 1000):
        print("Invalid maximum iteration!")
        exit()
    iter = int(iter)
else:
    (_,k,file_loc) = sys.argv
    iter = 200
    
points = []
    
with open(file_loc,'r') as f:
    line = f.readline()
    while (line != ""):
        points.append(Point([float(x) for x in line.split(',')]))
        line = f.readline()
    
if (not k.isdigit()):
    print("Invalid number of clusters!")
    exit()
k = float(k)
if (k != k//1 or k <= 1 or k >= len(points)):
    print("Invalid number of clusters!")
    exit()
    
k = int(k)


        
kmeans(k,iter,points)