
#include <string>
#include <ostream>
#include "libpkg/control.h"
#include "summary.h"

/**
 * Class to write files based on a template with
 * tokens enclosed in percentages.
 */
class Writer
{
public:
	Writer() {_errors=0;};
	virtual ~Writer() {};

	/**
	 * Write the pattern replacing the tokens between %
	 * with the values looked up by resolve
	 *
	 * A colon followed by some text in a token, gives
	 * text to use if the token looks up as ""
	 */
	void write(const std::string &pattern, std::ostream &out);
	int get_error_count() const {return _errors;}

	virtual std::string resolve(const std::string &token) = 0;

protected:
	int _errors;
};

/**
 * Writer for details of a control record
 *
 * control is the control record
 * rel_url is the relative path between the package index file
 * and the directory holding the web pages.
 */
class DetailsWriter : public Writer
{
public:
	DetailsWriter(pkg::control &control, const std::string &rel_url);

	virtual std::string resolve(const std::string &token);

protected:
	pkg::control &_control;
	std::string _rel_url;
};

/**
 * Writer for summary information
 */
class SummaryWriter : public Writer
{
public:
	SummaryWriter(const SummaryInfo &info);

	virtual std::string resolve(const std::string &token);

protected:
	const SummaryInfo &_info;
};

/**
 * Simple writer to replace a single token
 */
class OneReplaceWriter : public Writer
{
public:
	OneReplaceWriter(const std::string &token, const std::string &replace);

	virtual std::string resolve(const std::string &token);

protected:
	std::string _token;
	std::string _replace;
};

/**
 * Writer for statistics page
 */
class StatsWriter : public Writer
{
public:
	StatsWriter(int packages);
	virtual std::string resolve(const std::string &token);

protected:
      std::string _count;
	std::string _date;
};
