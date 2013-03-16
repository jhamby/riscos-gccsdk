
#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include "summary.h"
#include "writer.h"

#include "libpkg/control.h"

// Page templates
#include "detailspage.h"
#include "catpage.h"
#include "alphapage.h"
#include "statspage.h"

using namespace std;

int main(int argc, char *argv[])
{
   if (argc < 4)
   {
      cout << "Usage: pkgweb <packages_file> <dest_dir> <url_base>" << endl;
      return 0;
   }

   const char *packages_file = argv[1];
   const char *dest_dir = argv[2];
   const char *url_base = argv[3];

   ifstream control_file(packages_file);
   if (!control_file.is_open())
   {
	   cerr << "Unable to open packages index " << packages_file << endl;
	   return 1;
   }

   list<SectionInfo> sections;
   vector<SummaryInfo> alphabetical;
   string letters;
   char last_letter = 0;
   int packageCount = 0;

   while (control_file.good())
   {
	   pkg::control control;

	   char ch;
	   control_file >> ch;
	   control_file.putback(ch);

	   control_file >> control;
	   if (control_file.good())
	   {
		   packageCount++;
		   pkg::control::const_iterator i = control.find("Section");
		   std::string section;

		   if (i == control.end()) section = "Missing";
		   else section = (*i).second;

		   cout <<"Package " << control.pkgname() << " in section " << section << endl;

		   if (last_letter != *control.pkgname().begin())
		   {
			   last_letter = *control.pkgname().begin();
			   letters += last_letter;
		   }

		   list<SectionInfo>::iterator sec_iter = sections.begin();
		   while (sec_iter != sections.end() && (*sec_iter).get_name() < section) ++sec_iter;

		   string page = dest_dir;
		   page += '/';
		   page += section + "Details.html";

		   ofstream details;
		   if (sec_iter == sections.end() || (*sec_iter).get_name() != section)
		   {
			   sec_iter = sections.insert(sec_iter, SectionInfo(section));

			   details.open(page.c_str(), ios::out);
			   OneReplaceWriter headerWriter("section", section);
			   headerWriter.write(detailHeader, details);
		   } else
			   details.open(page.c_str(), ios::out | ios::app);

		   // Update lists for indices
		   SummaryInfo summary(control, section + "Details.html#" + control.pkgname());
		   (*sec_iter).add_package(summary);
		   alphabetical.push_back(summary);

		   if (details.fail())
		   {
			   cerr << "Failed to open/create details page " << page << endl;
			   return -2;
		   }

		   DetailsWriter dw(control, url_base);
		   dw.write(detailItem, details);

		   details.close();
	   }

   }

   control_file.close();

   // Write category index and end of details pages
   string cat_name(dest_dir);
   cat_name += "/SectionIndex.html";
  
   ofstream cat_file(cat_name.c_str());
   list<SectionInfo>::iterator cat_iter;

   cat_file << catHeader;

   // Write sections to top of cat index page and
   // endings to the category details pages
   
   for (cat_iter = sections.begin(); cat_iter != sections.end(); ++cat_iter)
   {
	   SectionInfo &info = (*cat_iter);
	   OneReplaceWriter sectionWriter("category", info.get_name());

	   sectionWriter.write(catPageLink, cat_file);

	   string page = dest_dir;
	   page += '/';
	   page += info.get_name() + "Details.html";
	   ofstream details(page.c_str(), ios::out | ios::app);
	   details << detailEnd;
	   details.close();
   }

   // Write the catagory index
   for (cat_iter = sections.begin(); cat_iter != sections.end(); ++cat_iter)
   {
	   SectionInfo &info = (*cat_iter);
	   OneReplaceWriter sectionWriter("category", info.get_name());

	   sectionWriter.write(startCat, cat_file);

	   std::vector<SummaryInfo>::const_iterator sum_iter;

	   for (sum_iter = info.begin(); sum_iter != info.end(); ++sum_iter)
	   {
		   const SummaryInfo &summary = (*sum_iter);
		   SummaryWriter summaryWriter(summary);
		   summaryWriter.write(catItem, cat_file);
	   }

	   sectionWriter.write(endCat, cat_file);
   }

   cat_file << catEnd;

   cat_file.close();

   // Alphabetical index
   string alpha_name(dest_dir);
   alpha_name += "/ABCIndex.html";
  
   ofstream alpha_file(alpha_name.c_str());

   alpha_file << alphaHeader;

   for (unsigned int letter = 0; letter < letters.size(); ++letter)
   {
	   OneReplaceWriter letterWriter("letter", letters.substr(letter,1));
	   letterWriter.write(alphaPageLink, alpha_file);
   }

   last_letter = 0;

   vector<SummaryInfo>::const_iterator alpha_iter;
   for (alpha_iter = alphabetical.begin(); alpha_iter != alphabetical.end(); ++alpha_iter)
   {
	   const SummaryInfo &summary = *(alpha_iter);

	   if (summary.get_name()[0] != last_letter)
	   {
		   if (last_letter != 0) alpha_file << endLetter;

		   last_letter = summary.get_name()[0];
		   OneReplaceWriter letterWriter("letter", summary.get_name().substr(0,1));
		   letterWriter.write(startLetter, alpha_file);
	   }
		   
	   SummaryWriter summaryWriter(summary);
	   summaryWriter.write(letterItem, alpha_file);
   }

   if (last_letter != 0) alpha_file << endLetter;

   alpha_file << alphaEnd;

   alpha_file.close();

   // Statistics
   string stats_name(dest_dir);
   stats_name += "/stats.html";
  
   ofstream stats_file(stats_name.c_str());

   StatsWriter statsWriter(packageCount);
   statsWriter.write(statsPage, stats_file);
   stats_file.close();

   return 0;
}
