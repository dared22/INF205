#include <cassert>
#include "journal-article-index.h"

using namespace litindex;

void JournalArticleIndex::set_bibliographic_data(
   std::string in_journal_name, int in_volume, int in_number,
   int in_first_page, int in_last_page, int in_year, std::string in_pid
) {
   this->journal_name = in_journal_name;
   
   assert(in_volume >= 0);
   this->volume = in_volume;
   
   assert(in_number >= 0);
   this->number = in_number;
   
   assert(in_first_page >= 0);
   this->first_page = in_first_page;
   if(this->page < this->first_page) this-> page = this->first_page;
   
   assert(in_last_page >= in_first_page);
   this->last_page = in_last_page;
   if(this->page > this->last_page) this-> page = this->last_page;
   
   this->year = in_year;
   this->pid = in_pid;
}

void JournalArticleIndex::set_page(int in_page)
{
   assert((in_page >= this->first_page) && (this->last_page >= in_page));
   this->page = in_page;
}
      
int JournalArticleIndex::next_page()
{
   if(this->page < this->last_page) this->page++;
   return this->page;
}

void JournalArticleIndex::in(std::istream* source)
{
   std::string in_journal_name = "", in_pid = "";
   int in_page = 0, in_volume = 0, in_number = 0, in_first_page = 0, in_last_page = 0, in_year = 0;
   
   char single_symbol = '\0';
   
   *source >> in_page;
   
   *source >> single_symbol;
   assert(single_symbol = '\'');
   for(*source >> single_symbol; single_symbol != '\''; *source >> single_symbol)
      in_journal_name += single_symbol;
   
   *source >> in_volume >> in_number >> in_first_page >> in_last_page >> in_year;
   
   *source >> single_symbol;
   assert(single_symbol = '\'');
   for(*source >> single_symbol; single_symbol != '\''; *source >> single_symbol)
      in_pid += single_symbol;
   
   this->set_bibliographic_data(
      in_journal_name, in_volume, in_number, in_first_page, in_last_page, in_year, in_pid
   );
   this->set_page(in_page);
}

void JournalArticleIndex::out(std::ostream* target) const
{
   *target << this->page << "\t'" << this->journal_name << "'\t" << this->volume << "\t"
           << this->number << "\t" << this->first_page << "\t" << this->last_page<< "\t"
           << this->year << "\t'" << this->pid << "'\t";
}

std::ostream& operator<<(std::ostream& os, const JournalArticleIndex& jai)
{
   jai.out(&os);
   return os;
}

std::istream& operator>>(std::istream& is, litindex::JournalArticleIndex& jai)
{
   jai.in(&is);
   return is;
}
