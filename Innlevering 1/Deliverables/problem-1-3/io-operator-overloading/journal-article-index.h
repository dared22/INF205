#pragma once

#include <iostream>
#include <string>

namespace litindex
{
   class JournalArticleIndex
   {
   public:
      JournalArticleIndex() = default;
      
      std::string get_journal_name() const { return this->journal_name; }
      int get_volume() const { return this->volume; }
      int get_number() const { return this->number; }
      int get_first_page() const { return this->first_page; }
      int get_last_page() const { return this->last_page; }
      int get_page() const { return this->page; } 

      int get_year() const { return this->year; }
      std::string get_persistent_identifier() const { return this->pid; }
      
      void set_bibliographic_data(
         std::string in_journal_name, int in_volume, int in_number,
         int in_first_page, int in_last_page, int in_year, std::string in_pid
      );
      void set_page(int in_page);

      int next_page();
      void in(std::istream* source);  // read from *source
      void out(std::ostream* target) const;  // write to *target
      
   private:
      std::string journal_name{};
      int volume{};
      int number{};
      int first_page{};  // where in the journal does the article begin?
      int last_page{};   // where does it end?
      
      int year{};
      std::string pid{};  // persistent identifier of the literature item, e.g., DOI or ISBN
      
      int page{};  // what page within the article are we referring to?
   };
}

std::ostream& operator<<(std::ostream& os, const litindex::JournalArticleIndex& jai);
std::istream& operator>>(std::istream& os, litindex::JournalArticleIndex& jai);

