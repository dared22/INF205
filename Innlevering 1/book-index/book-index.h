#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace litindex
{
   class BookIndex
   {
   public:
      BookIndex() = default;
      BookIndex(int c, int s, int p);  
      ~BookIndex()
      {
         std::cout << "Deallocating.\n";
      }
      
      int get_chapter() const { return this->chapter; }
      int get_section() const { return this->section; }
      int get_page() const { return this->page; }
      
      void set_chapter(int c) { assert(c >= 0); this->chapter = c; }
      void set_section(int s) { assert(s >= 0); this->section = s; }
      void set_page(int p) { assert(p >= 0); this->page = p; }
      
      int next_chapter();  /* goes to next chapter, returns new chapter number */
      int next_section();  /* goes to next section, returns new section number */
      int next_page();     /* goes to next page, returns new page number */
   
      void in(std::istream* source);  /* read one object from *source */
      void out() const;    /* output status information */
      static std::vector<BookIndex> read_from_stream(std::istream& source);
      static std::vector<BookIndex> read_from_file(const std::string& filename);
   
   private:
      int chapter{};
      int section{};
      int page{};
   };
}

std::istream& operator>>(std::istream& is, litindex::BookIndex& idx);
