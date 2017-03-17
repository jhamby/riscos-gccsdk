// Templates for the details page

// Header %section% is replaced by section name
const char *detailHeader = 
  "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n"
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
   "<td><a name=\"%link%\"><b><font size=5>%Package%%name_suffix%</font></b></a></td>\n"
   "<td><b>Version:</b> %Version%</td>\n"
   "<td align=\"right\">&nbsp;</td>\n"
   "</tr>\n"
   "</table>\n"
   "<h3>Description:</h3>\n"
  "%Description%\n"
  "<ul>\n"
  "<li>Architecture: %Architecture:arm%</li>\n"
  "<li>Section: %Section%</li>\n"
  "<li>Priority: %Priority%</li>\n"
  "<li>Licence: %Licence%</li>\n"
  "<li>Package size: %Size%</li>\n"
  "<li>Components: %Components:None%\n"
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
