
#include "writer.h"

#include <iostream>
#include <strstream>

inline std::string lowercase(const std::string &value)
{
	std::string result;
	for (std::string::const_iterator i=value.begin();i!=value.end();++i)
		result += tolower(*i);
	return result;
}

/**
 * Write pattern to output resolving tokens enclosed in '%'.
 */
void Writer::write(const std::string &pattern, std::ostream &out)
{
	std::string::size_type start = 0;
	std::string::size_type end = pattern.find('%');
	std::string token;
	std::string::size_type colon_pos;
	std::string token_text;
	std::string null_text;

	while (end != std::string::npos)
	{
		if (end > start) out.write(&pattern[start], end-start);
		start=end+1;
		end = pattern.find('%', start);
		if (end == std::string::npos)
		{
			std::cerr << "Writer error missing '%'" << std::endl;
			_errors++;
			out << std::endl << "ERROR: missing '%'" << std::endl;
		} else
		{
			token = pattern.substr(start, end - start);
			colon_pos = token.find(':');
			if (colon_pos != std::string::npos)
			{
				null_text = token.substr(colon_pos+1);
				token = token.substr(0,colon_pos);
			} else
				null_text.erase();

			if (token.empty())
			{
				// Two % together are used to escape out %
				token_text = "%";
			} else
			{
				token_text = resolve(token);
				if (token_text.empty()) token_text = null_text;
			}
			out << token_text;
			start = end+1;
			end = pattern.find('%',start);
		}
	}

	if (start < pattern.size())
	{
		out.write(&pattern[start], pattern.size() - start);
	}
}


DetailsWriter::DetailsWriter(pkg::control &c, const std::string &rel_url)
   : _control(c), _rel_url(rel_url)
{
};


/**
 * Resolve string by looking up property in control
 * record.
 */
std::string DetailsWriter::resolve(const std::string &token)
{
	std::string check = lowercase(token);
	std::string text;

	/* Aliases for member of control record */
	if (check == "summary")
	{
		text = _control.short_description();
	} else if (check == "description")
	{
		text = "<p>" + _control.description();

		// Add paragraph markers
		std::string::size_type pos = text.find('\n');
		// Do summary line
		if (pos !=std::string::npos)
		{
			text.insert(pos, "</p>");
			pos += 5;
		        text.insert(pos, "<p>");
			pos = text.find('\n', pos+1);
		}

		while (pos != std::string::npos)
		{
			if (text[pos] == '\n')
			{
			  text.insert(pos, "<br>");
			  pos += 5;
			}
			pos = text.find('\n', pos+1);
		}
		text += "</p>";
	} else if (check == "download")
	{
		pkg::control::iterator i = _control.find("URL");
		if (i != _control.end())
		{
			text = (*i).second;
			std::string::size_type check_up = 0;
			
			while (check_up <= _rel_url.size()-2 && _rel_url.substr(check_up,2) == "..")
			{
				std::string::size_type pos = text.find('/');
				if (pos != std::string::npos)
				{
					text.erase(0,pos+1);
				}
				check_up += 3; // Skip "../"
			}
			if (check_up < _rel_url.size())
			{
				text = _rel_url.substr(check_up) + text;
			}
		} else
		{
			text = "MISSING URL FROM CONTROL RECORD";
			_errors++;
		}

	} else
	{
		pkg::control::iterator i = _control.find(token);
		//TODO: Check valid item and not just missing
		if (i != _control.end())
		{
			text = (*i).second;
		}
	}

	return text;
}

SummaryWriter::SummaryWriter(const SummaryInfo &info) : _info(info)
{
}

std::string SummaryWriter::resolve(const std::string &token)
{
	std::string check = lowercase(token);
	std::string text;

	if (check == "name") text = _info.get_name();
	else if (check == "summary") text = _info.get_summary();
	else if (check == "version") text = _info.get_version();
	else if (check == "location") text = _info.get_location();
	else
	{
		_errors++;
		text = "INVALID SUMMARY ATTRIBUTE " + token;
	}

	return text;
}


OneReplaceWriter::OneReplaceWriter(const std::string &token, const std::string &replace)
{
	_token = lowercase(token);
	_replace = replace;
}

std::string OneReplaceWriter::resolve(const std::string &token)
{
	if (lowercase(token) == _token)
	{
		return _replace;
	} else
	{
		_errors++;
		return "INVALID TOKEN";
	}
}

StatsWriter::StatsWriter(int packages)
{
  // Count of packages field
  std::ostrstream ss;
  ss << packages << std::ends;
  _count = ss.str();

  // Date field
  struct tm *nowtm;
  time_t now;
  time(&now);
  nowtm = gmtime(&now);
  _date = asctime(nowtm); 
}

std::string StatsWriter::resolve(const std::string &token)
{
	std::string check = lowercase(token);
	if (check == "count")
	{
		return _count;
	} else if (check == "date")
	{
		return _date;
	} else
	{
		_errors++;
		return "INVALID TOKEN";
	}
}
