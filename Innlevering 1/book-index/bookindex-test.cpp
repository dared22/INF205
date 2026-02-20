#include "book-index.h"
#include <iostream>

namespace
{
   void start_chapter(litindex::BookIndex b)
   {
      std::cout << "start_chapter(litindex::BookIndex b)\n";

      b.next_chapter();
      b.out();  // print status
   }
}

int main()
{
   std::cout << "\nBookIndex example:\n";
   litindex::BookIndex idx(1, 11, 24);
   
   litindex::BookIndex* address = &idx;
   
   idx.out();  // print status
   start_chapter(idx);
   idx.out();  // print status
}
