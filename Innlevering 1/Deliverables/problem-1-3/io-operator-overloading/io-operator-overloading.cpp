#include "journal-article-index.h"
#include <iostream>
#include <fstream> //instead of sstream

int main()
{
   litindex::JournalArticleIndex jai_i;
   jai_i.set_bibliographic_data(
      "IEEE_Access", 9, 0, 152333, 152346, 2021, "doi:10.1109/access.2021.3126429"
   );
   jai_i.set_page(152345);
   std::clog << "JournalArticleIndex jai_i:\n" << jai_i << "\n";

   std::clog << "\nWriting jai_i into a file.\n";
   std::ofstream outfile("temp.txt");
  {
      std::ofstream outfile("temp.txt");
      outfile << jai_i;   // serialize jai_i into the file
   }  // outfile closes here when it goes out of scope

   litindex::JournalArticleIndex jai_j;
   std::clog << "\nReading jai_j from the file.\n";
  {
      std::ifstream infile("temp.txt");
      infile >> jai_j;    // deserialize jai_j from the file
   }
   std::clog << "JournalArticleIndex jai_j:\n" << jai_j << "\n";
}
