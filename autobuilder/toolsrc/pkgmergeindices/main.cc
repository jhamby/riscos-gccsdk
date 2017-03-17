
#include <iostream>
#include <fstream>
#include <map>

#include "libpkg/binary_control.h"

using namespace std;

/** Return lower case version of a string */
inline string lowercase(const std::string &text)
{
	string::const_iterator char_iter;
	string lower;

	for (char_iter = text.begin(); char_iter != text.end(); ++char_iter)
		lower += tolower(*char_iter);

	return lower;
}

int main(int argc, char *argv[])
{
   if (argc < 3)
   {
      cout << "Usage: pkgmergeindices <output_index_file> <index_file1> <index_file2> ..." << endl;
      return 0;
   }

   const char *output_index = argv[1];
   char **input_index = &argv[2];
   int num_indices = argc - 2;
   
   
   std::map<string, pkg::binary_control *> merged;
   
   for (int index = 0; index < num_indices; ++index)
   {
      ifstream control_file(input_index[index]);
      if (!control_file.is_open())
      {
          cerr << "Unable to open packages index " << input_index[index] << endl;
          return 1;
      }
	  
      cout << "Processing index file " << input_index[index] << endl;

      while (control_file.good())
      {
         pkg::binary_control *control = new pkg::binary_control();
         bool delete_control = true;

         char ch;
         control_file >> ch;
         control_file.putback(ch);

         control_file >> *control;
         if (control_file.good())
         {
	    string pkgname = lowercase(control->pkgname());
            if (merged.find(pkgname) == merged.end())
            {
               merged[pkgname] = control;
               delete_control = false;
            }
            cout << "  Package " << control->pkgname();
            if (delete_control) cout << " not";
            cout << " used " << endl;
         }
         if (delete_control) delete control;
      }
      control_file.close();
   }
   
   // Write merged index  
   cout << "Writing merged file" << endl;
   ofstream merged_file(output_index);
   
   for (map<string, pkg::binary_control*>::iterator i = merged.begin(); i != merged.end(); ++i)
   {
      merged_file << *(i->second) << endl;
      delete i->second;
   }

   cout << "Finished" << endl;
   
   return 0;
}

