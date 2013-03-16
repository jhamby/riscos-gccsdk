// Page layout information for alphabetical index page
//

// Start of page
const char *alphaHeader =
  "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n"
  "<html>\n"
  "<head>\n"
  "<title>Alphabetical index of Packages</title>\n"
  "</head>\n"
  "<body>\n"
  "<p>Alphabetical index of packages</p>"
  "\n";

// Links from list of categories to further down the page
// %letter% is substituted for the letter to be linked to
const char *alphaPageLink =
  " <a href=\"#%letter%\">%letter%</a>";

// Start of a new letter
// %letter% is substituted for the category name
const char *startLetter = 
  "\n<h3><a name=\"%letter%\">%letter%</a></h3>\n"
  "<table>\n";

// Summary of the item
// %name% - name, %version% - version, %summary% summary
// %location% - url to page with more details
const char *letterItem =
  "<tr>\n"
  "<td><a href = \"%location%\">%name%</a></td>\n"
  "<td>%version%</td><td>%summary%</td>\n"
  "</tr>\n";

const char *endLetter = "</table>\n";

const char *alphaEnd =
  "</body>\n"
  "</html>\n";
