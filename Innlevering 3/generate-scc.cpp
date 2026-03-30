// executable should be called as follows:
//
// ./generate-scc <number of nodes> <number of edges> <graph file>
//
// for example: ./generate-scc 256 2048 scc08.dat

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

int main(int argc, char** argv)
{
   assert(argc == 4);  // ./generate-scc <number of nodes> <number of edges> <graph file>
   
   long nodes = std::atol(argv[1]);
   assert(nodes > 0);
   std::cout << "Nodes: " << nodes << "\n";

   long edges = std::atol(argv[2]);
   assert(edges > 0);
   std::cout << "Edges: " << edges << "\n";

   std::cout << "Graph file: " << argv[3] << "\n";
   
   // random seed from chrono
   std::srand(
      std::chrono::duration_cast<std::chrono::nanoseconds>(
         std::chrono::high_resolution_clock::now().time_since_epoch()
      ).count()
   );

   // generate graph file
   std::ofstream gout(argv[3]);
   if(!gout) return EXIT_FAILURE;
   std::cout << "\nGenerating:\n";
   std::cout.flush();
   
   bool* covered = new bool[nodes];
   std::vector< std::set<long> > connected;
   for(long i = 0; i < nodes; i++)
   {
      covered[i] = false;
      connected.push_back(std::set<long>());
   }
   
   long k = 0;
   while((k < edges-nodes+1) && (k < edges))
   {
      long i = nodes * ((double)std::rand()/(RAND_MAX + 1.0));
      long j = nodes * ((double)std::rand()/(RAND_MAX + 1.0));
      if(connected[i].count(j) > 0) continue;

      gout << "n" << i << " r n" << j << ".\n";
      covered[i] = true;
      covered[j] = true;
      connected[i].insert(j);
      k++;
   }
   
   // remaining nodes
   for(long i = 0; i < nodes; i++)
      if(!covered[i])
      {
         gout << "n" << i << " r n" << i << ".\n";
         covered[i] = true;
         connected[i].insert(i);
         k++;
      }

   // remaining edges
   while(k < edges)
   {
      long i = nodes * ((double)std::rand()/(RAND_MAX + 1.0));
      long j = nodes * ((double)std::rand()/(RAND_MAX + 1.0));
      if(connected[i].count(j) > 0) continue;

      gout << "n" << i << " r n" << j << ".\n";
      covered[i] = true;
      covered[j] = true;
      connected[i].insert(j);
      k++;
   }
   
   gout.close();
   std::cout << "Done.\n";
}
