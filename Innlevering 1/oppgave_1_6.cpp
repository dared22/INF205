#include <iostream>
#include <array>

namespace
{
   void increment_at_position_by_value(int p, std::array<char, 7> arr)
   {
      std::cout << "increment_at_position(int p, std::array<char, 7> arr)  [by value]:\n";
      arr[p]++;
      std::cout << "\tarr inside the function: \"" << arr.data() << "\"\n";
   }

   void increment_at_position_by_reference(int p, std::array<char, 7>& arr)
   {
      std::cout << "increment_at_position(int p, std::array<char, 7>& arr)  [by reference]:\n";
      arr[p]++;
      std::cout << "\tarr inside the function: \"" << arr.data() << "\"\n";
   }
}

int main()
{
   std::array<char, 7> arr_by_val = {'I', 'N', 'F', '2', '0', '5', '\0'};
   increment_at_position_by_value(5, arr_by_val);
   std::cout << "\tarr_by_val after the function: \"" << arr_by_val.data() << "\"\n\n";

   std::array<char, 7> arr_by_ref = {'I', 'N', 'F', '2', '0', '5', '\0'};
   increment_at_position_by_reference(5, arr_by_ref);
   std::cout << "\tarr_by_ref after the function: \"" << arr_by_ref.data() << "\"\n";
}
