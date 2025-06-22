// A union-find algorithm to identify islands
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct subset {
	int parent;
	int rank;
}subset;

// A utility function to find set of an element i

int Find(subset subsets[], int i){
  if (subsets[i].parent == i){
    return i;
  } else {
    return Find(subsets, subsets[i].parent);
  }
}

// A function that does union of two sets of x and y
// (uses union by rank)
void Union(subset subsets[], int xroot, int yroot){
// Attach smaller rank tree under root of high rank tree
// (Union by Rank)
  if (subsets[xroot].rank < subsets[yroot].rank)
    subsets[xroot].parent = yroot;
  else if (subsets[xroot].rank > subsets[yroot].rank)
    subsets[yroot].parent = xroot;
  // If ranks are same, then make one as root and
  // increment its rank by one
    else {
      subsets[yroot].parent = xroot;
      subsets[xroot].rank++;
    }
}

int rndnode(int nodes){ return (int)(drand48()*nodes); }
int rndweight(int maxweight){ return 1 + (int)(drand48()*maxweight); }
int rndcooldown(int mincooldown, int maxcooldown){ return mincooldown + (int)(drand48()*(maxcooldown-mincooldown+1)); }

int main(int argc, char *argv[]){
  int seed = 1;
  int transportcapacity = 2;
  int transportcost = 20;
  int transportinterval = 100;
  int retrievecost = 1;
  int nodes = 10;
  int numpackets = 100;
  long rtime=10;
  int maxweight = 10;
  double edgeprobability = 0.3;
  int mincooldown = 50;
  int maxcooldown = 150;

  // Parse command-line arguments if provided
  if (argc > 1) transportcapacity = atoi(argv[1]);
  if (argc > 2) transportcost = atoi(argv[2]);
  if (argc > 3) transportinterval = atoi(argv[3]);
  if (argc > 4) retrievecost = atoi(argv[4]);
  if (argc > 5) nodes = atoi(argv[5]);
  if (argc > 6) numpackets = atoi(argv[6]);
  if (argc > 7) rtime = atol(argv[7]);
  if (argc > 8) maxweight = atoi(argv[8]);
  if (argc > 9) edgeprobability = atof(argv[9]);
  if (argc > 10) mincooldown = atoi(argv[10]);
  if (argc > 11) maxcooldown = atoi(argv[11]);

  srand48(seed);

  // Generate weighted directed graph
  int * graph = (int*) malloc(sizeof(int)*nodes*nodes);
  int * cooldowns = (int*) malloc(sizeof(int)*nodes*nodes);
  for (int i=0; i<nodes*nodes; i++) {
    graph[i] = 0;
    cooldowns[i] = 0;
  }
  
  // Generate random directed edges with weights and cooldowns
  for (int i = 0; i < nodes; i++) {
    for (int j = 0; j < nodes; j++) {
      if (i != j && drand48() < edgeprobability) {
        graph[i*nodes+j] = rndweight(maxweight);
        cooldowns[i*nodes+j] = rndcooldown(mincooldown, maxcooldown);
      }
    }
  }
  
  // Ensure graph connectivity by adding a spanning structure
  // Create a directed path through all nodes to guarantee connectivity
  for (int i = 0; i < nodes - 1; i++) {
    int next = (i + 1) % nodes;
    if (graph[i*nodes+next] == 0) {
      graph[i*nodes+next] = rndweight(maxweight);
      cooldowns[i*nodes+next] = rndcooldown(mincooldown, maxcooldown);
    }
  }
  // Add one more edge to make it more connected
  if (graph[(nodes-1)*nodes+0] == 0) {
    graph[(nodes-1)*nodes+0] = rndweight(maxweight);
    cooldowns[(nodes-1)*nodes+0] = rndcooldown(mincooldown, maxcooldown);
  }

  printf("%d\n%d\n%d\n%d\n", transportcapacity, transportcost, 
         transportinterval, retrievecost);

  printf("%d\n",nodes);
  for (int i=0; i<nodes; i++){
    for (int j=0; j<nodes; j++){
      printf("%d %d",graph[i*nodes+j], cooldowns[i*nodes+j]);
      if (j<nodes-1){
        printf(" ");
      } else {
        printf("\n");
      }
    }
  }
 
  printf("%d\n",numpackets);
  long atime = 1;
  int src, dst;
  for (int i=0; i<numpackets; i++){
    do {
      src = rndnode(nodes);
      dst = rndnode(nodes);
    } while (src == dst);
    atime += (long)(drand48()*rtime);
    printf("%ld pac %d org %d dst %d\n",atime,i,src,dst);
  }
  
  return numpackets;
}