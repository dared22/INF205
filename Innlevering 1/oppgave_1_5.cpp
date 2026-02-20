#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>

int main(int argc, char** argv)
{
   std::cout << "\nargv[0] er namnet til den køyrbare fila: " << argv[0] << "\n";

   std::vector<std::string> argumentliste{};
   std::unordered_set<std::string> sett{};

   for(int i = 0; i < argc; i++)
   {
      std::string arg(argv[i]);
      if(sett.insert(arg).second)
      {
         argumentliste.push_back(arg);
      }
   }

   for(const auto& s : argumentliste)
      std::cout << "\targument: \"" << s << "\"\n";
}