// Page layout information for category index page
//

// Start of page
const char *catHeader =
  "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n"
  "<html>\n"
  "<head>\n"
  "<title>Packages by Category</title>\n"
  "</head>\n"
  "<body>"
  "<p>Index of packages by categories</p>"
  "\n";

// Links from list of categories to further down the page
// %category% is substituted for the category name
const char *catPageLink =
  " <a href=\"#%category%\">%category%</a>";

// Start of a new category
// %category% is substituted for the category name
const char *startCat = 
  "<h3><a name=\"%category%\">%category%</a></h3>\n"
  "<table>\n";

// Details of category item
// %name% - name, %version% - version, %summary% summary
// %location% - url to page with more details
const char *catItem =
  "<tr>\n"
  "<td><a href = \"%location%\">%name%%name_suffix%</a></td>\n"
  "<td>%version%</td><td>%summary%</td>\n"
  "</tr>\n";

const char *endCat = "</table>\n";

const char *catEnd =
  "</body>\n"
  "</html>\n";
