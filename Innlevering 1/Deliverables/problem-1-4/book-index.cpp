#include "book-index.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace litindex;

/* Remark: Not good style. See Core Guidelines C.49 for better style. */
BookIndex::BookIndex(int c, int s, int p)
{
   this->set_chapter(c);
   this->set_section(s);
   this->set_page(p);
}

/*
 * goes to next chapter, returns new chapter number
 * this implementation also automatically goes to the next page
 */
int BookIndex::next_chapter()
{
   this->chapter++;
   this->section = 1;
   this->page++;
   return chapter;
}

/*
 * goes to next section, returns new section number
 */
int BookIndex::next_section()
{
   this->section++;
   return section;
}

/*
 * goes to next page, returns new page number
 */
int BookIndex::next_page()
{
   this->page++;
   return page;
}

void BookIndex::in(std::istream* source)
{
   int c = 0;
   int s = 0;
   int p = 0;
   if(!(*source >> c >> s >> p))
      return;

   if((c < 0) || (s < 0) || (p < 0))
   {
      source->setstate(std::ios::failbit);
      return;
   }

   this->set_chapter(c);
   this->set_section(s);
   this->set_page(p);
}

/*
 * write to stdout
 */
void BookIndex::out() const
{
   std::cout << "Section " << this->chapter << "." << this->section << ", p. " << this->page << "\n";
}

std::vector<BookIndex> BookIndex::read_from_stream(std::istream& source)
{
   int count = 0;
   if(!(source >> count) || (count < 0))
      throw std::runtime_error("Invalid first line (object count).");

   std::vector<BookIndex> result;
   result.reserve(static_cast<std::size_t>(count));

   for(int i = 0; i < count; i++)
   {
      BookIndex idx;
      if(!(source >> idx))
         throw std::runtime_error("Invalid or missing BookIndex row.");
      result.push_back(idx);
   }

   return result;
}

std::vector<BookIndex> BookIndex::read_from_file(const std::string& filename)
{
   std::ifstream infile(filename);
   if(!infile)
      throw std::runtime_error("Could not open file: " + filename);

   return read_from_stream(infile);
}

std::istream& operator>>(std::istream& is, litindex::BookIndex& idx)
{
   idx.in(&is);
   return is;
}
