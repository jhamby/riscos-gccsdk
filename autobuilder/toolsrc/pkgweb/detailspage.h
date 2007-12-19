// Templates for the details page

// Header %section% is replaced by section name
const char *detailHeader = 
  "<html>\n"
  "<head>\n"
  "<title>Packages in category %section%</title>\n"
  "</head>\n"
  "<body>\n";

// HTML repeated for each item
// Use the names from the package in replacement
// and summary, description and download.
const char *detailItem =
   "<table cellpadding=10 cellspacing=0 width=\"100%%\">\n"
   "<tr bgcolor=\"#ddffdd\">\n"
   "<td><a name = \"#%Package%\"><h3>%Package%</h3></a></td>\n"
   "<td><b>Version:</b> %Version%</td>\n"
   "<td align=\"right\">&nbsp</td>\n"
   "</tr>\n"
   "</table>\n"
   "<h3>Description:</h3>\n"
  "%Description%\n"
  "<ul>\n"
  "<li>Section: %Section%</li>\n"
  "<li>Priority: %Priority%</li>\n"
  "<li>Licence: %Licence%</li>\n"
  "<li>Package size: %Size%</li>\n"
  "</ul>\n"
  "<b>Related packages</b>\n"
  "<table>\n"
  "<tr><td>Required</td><td>%Depends:Nothing%</td></tr>\n"
  "<tr><td>Recommended</td><td>%Recommends:Nothing%</td></tr>\n"
  "<tr><td>Suggested</td><td>%Suggests:Nothing%</td></tr>\n"
  "<tr><td>Conflicts with</td><td>%Conflicts:Nothing%</td></tr>\n"
  "</table>\n"
  "<p><a href=\"%download%\">Get package zip file</a></p>";


// End of page
const char *detailEnd =
  "</body>\n"
  "</html>\n";
