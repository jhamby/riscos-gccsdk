#include <string.h>
#include <ctype.h>
//#include <osfcn.h>
#include <fcntl.h>

#include "BString.h"

///////////////////////////////////////////////////////////////////////////////
//
// Hilfsfunktion zur Ermittlung einer guenstigen Maximallaenge
// (16 Bytes bzw. ab 512 Bytes Laenge 3% mehr als benoetigte Laenge)

inline int trimlen(int l)
{
	return l>0 ? l + ((l/32 < 16) ? 16 : l/32) : 0;
}

// Tabellen zur Umwandlung Gross-Kleinschreibung (inclusive Umlaute)

static char gk_gross[256];
static char gk_klein[256];

// Funktion zum Initialisieren dieser Tabellen

static void gk_init()
{
	static int init=0;

	if( !init )
	{
		int i;
		for(i=0; i<256; i++)
			gk_gross[i] = i;
		for(i='a'; i<='z'; i++)
			gk_gross[i] = i + 'A' - 'a';
		gk_gross[0344] = '\304';
		gk_gross[0366] = '\326';
		gk_gross[0374] = '\334';

		for(i=0; i<256; i++)
			gk_klein[i] = i;
		for(i='A'; i<='Z'; i++)
			gk_klein[i] = i + 'a' - 'A';
		gk_klein[0304] = '\344';
		gk_klein[0326] = '\366';
		gk_klein[0334] = '\374';

		init=1;
	}
}

///////////////////////////////////////////////////////////////////////////////

BString::BString()
{
	init();
}

BString::BString(int i, const char *format)
{
	init();
	setzen(i, format);
}

BString::BString(double d, const char *format)
{
	init();
	setzen(d, format);
}

BString::BString(const char *s)
{
	init();
	setzen(s);
}

BString::BString(const BString &s)
{
	init();
	setzen(s);
}

BString::BString(const BString *s)
{
	init();
	setzen(s);
}

BString::~BString()
{
	if(_str)
		delete _str;
}

///////////////////////////////////////////////////////////////////////////////

int BString::istNurSpaces() const
{
	for(int i=0; i<_len; i++)
		if( !isspace(_str[i]) )
			return 0;

	return 1;
}

///////////////////////////////////////////////////////////////////////////////

int BString::setzen(int i, const char *format)
{
	char	s[64];

	sprintf(s, format ? format : "%d", i);
	setzen(s);
	return i;
}

double BString::setzen(double d, const char *format)
{
	char	s[64];

	sprintf(s, format ? format : "%f", d);
	setzen(s);
	return d;
}

const char *BString::setzen(const char *s, int laenge)
{
	if( !s )
		setzeNull();
	else
		set(laenge, s, 0);

	return s;
}

///////////////////////////////////////////////////////////////////////////////

BString &BString::anhaengen(int i, const char *format)
{
	char	s[64];

	sprintf(s, format ? format : "%d", i);
	return anhaengen(s, strlen(s));
}

BString &BString::anhaengen(double d, const char *format)
 {
	char	s[64];

	sprintf(s, format ? format : "%f", d);
	return anhaengen(s, strlen(s));
}

BString &BString::anhaengen(const char *s, int laenge)
{
	add( laenge<0 ? strlen(s) : laenge, s, 0);
	return *this;
}

BString &BString::anhaengen(const BString *s)
{
	if( s )
		add(s->_len, s->_str, 0);
	return *this;
}

BString &BString::anhaengen(int c, int n)
{
	add( n<0 ? 0 : n, 0, c);
	return *this;
}

///////////////////////////////////////////////////////////////////////////////

BString &BString::einfuegen(const char *s, int pos, int ll)
{
	if( !s)
		return *this;
	if( ll<0 )
		ll = strlen(s);
	if( !ll )
		return *this;

	if( pos<0 )
		;
	else if ( pos<laenge() )
	{
		int		tmplen = _len;
		char	*tmp = new char[_len+1];

		memcpy(tmp, lesen(), _len+1);
		set(pos, tmp, 0);
		add(ll, s, 0);
		add(tmplen-pos, tmp+pos, 0);
		delete tmp;
	}
	else
	{
		add(pos-laenge(), 0, ' ');
		add(ll, s, 0);
	}
	return *this;
}

BString &BString::einfuegen(int c, int pos, int anz)
{
	if( pos<0 )
		;
	else if( pos<laenge() )
	{
		int		tmplen = _len;
		char	*tmp = new char[_len+1];
		memcpy(tmp, lesen(), _len+1);

		set(pos, tmp, 0);
		add(anz, 0, c);
		add(tmplen-pos, tmp+pos, 0);
		delete tmp;
	}
	else
	{
		add(pos-laenge(), 0, ' ');
		add(anz, 0, c);
	}
	return *this;
}

///////////////////////////////////////////////////////////////////////////////

BString &BString::ausfuegen(int pos, int anz)
{
	if( !_str || pos<0 || pos>=laenge() )
		;
	else
	{
		int tmplen = _len-pos-anz;
		if( tmplen>0 )
		{
			memcpy(_str+pos, _str+pos+anz, tmplen);
			_len = pos+tmplen;
		}
		else
			_len = pos;

		_str[_len] = 0;
		trim();
	}
	return *this;
}

///////////////////////////////////////////////////////////////////////////////

BString &BString::ersetzen(int pos, int anz, const char *s, int len)
{
	return pos>=0 ? ausfuegen(pos, anz).einfuegen(s, pos, len) : *this;
}

BString &BString::ersetzen(int pos, int anz, int c, int n)
{
	return pos>=0 ? ausfuegen(pos, anz).einfuegen(c, pos, n) : *this;
}

///////////////////////////////////////////////////////////////////////////////

BString &BString::ersetzen(const BString &s, const BString &e, int alles)
{
	int pos;
	int len=e.laenge();

	if( alles )
		for(pos=0; (pos=suchen(s, pos))!=-1; pos+=len)
			ersetzen(pos, s.laenge(), e(), len);
	else
		ersetzen(suchen(s), s.laenge(), e(), len);

	return *this;
}

///////////////////////////////////////////////////////////////////////////////

int BString::suchen(const BString &s, int start) const
{
	char	*f=0;

	if( !_str || start<0 || start>=laenge() )
		return -1;
	if( !(f=strstr(_str+start, s())) )
		return -1;
	return f-_str;
}

///////////////////////////////////////////////////////////////////////////////

BString BString::auslesen(int pos, int n) const
{
	BString s;

	auslesen(s, pos, n);
	return s;
}

BString &BString::auslesen(BString &ziel, int pos, int n) const
{
	if( n<0 )
		n = laenge() - pos;

	ziel.kopieren(*this, pos, n);
	return ziel;
}

///////////////////////////////////////////////////////////////////////////////

void BString::kopieren(const BString *s)
{
	setzen(s->lesen(), s->laenge());
}

BString &BString::kopieren(const BString &q, int a, int l)
{
	if( l<=0 )
		set(0, 0, 0);
	else if( q.laenge()<=a )
		set(l, 0, ' ');
	else if( q.laenge() <= a+l)
	{
		set(q.laenge() - a, q._str + a, 0);
		add(a + l - q.laenge(), 0, ' ');
	}
	else
		set(l, q._str + a, 0);

	return *this;
}

///////////////////////////////////////////////////////////////////////////////

int BString::leseToken(BString &s, int &start, const char *trennzeichen) const
{
	int bis;

	s = "";
	if( !_str || start>=laenge() )
		return 0;
	for(bis=start; bis<laenge() && !strchr(trennzeichen,_str[bis]); bis++);
	s.anhaengen(_str+start, bis-start);
	for(start=bis+1; start<laenge()&&strchr(trennzeichen, _str[start]); start++);
	return 1;
}

int BString::leseWort(BString &s, int &start, const char *trennzeichen) const
{
	int bis;

	s = "";
	if( !_str || start>=laenge() )
		return 0;
	for(bis=start; bis<laenge() && !strchr(trennzeichen,_str[bis]); bis++);
	s.anhaengen(_str+start, bis - start);
	start = bis+1;
	return 1;
}

///////////////////////////////////////////////////////////////////////////////

BString BString::gross() const
{
	if( !_str )
		return *this;

	BString t = *this;

	gk_init();
	for(unsigned char *s = (unsigned char*)t._str; *s; s++)
		*s = gk_gross[*s];
	return t;
}

BString BString::klein() const
{
	if( !_str )
		return *this;

	BString t = *this;

	gk_init();
	for(unsigned char *s = (unsigned char*)t._str; *s; s++)
		*s = gk_klein[*s];
	return t;
}

BString BString::anfangGross() const
{
	if( !_str)
		return *this;

	BString	t = *this;
	int		sp = 1;

	gk_init();
	for(unsigned char *s = (unsigned char*)t._str; *s; s++)
	{
		*s = sp ? gk_gross[*s] : gk_klein[*s];
		sp = isspace(*s);
	}
	return t;
}

BString &BString::operator++()
{
	if( _str )
		*this = gross();
	return *this;
}

BString &BString::operator--()
{
	if( _str )
		*this = klein();
	return *this;
}

BString operator+(const BString &q)
{
	return q.gross();
}

BString operator-(const BString &q)
{
	return q.klein();
}

///////////////////////////////////////////////////////////////////////////////

BString operator+(const BString &s1, const BString &s2)
{
	BString z(s1);
	z += s2;
	return z;
}

///////////////////////////////////////////////////////////////////////////////

BString &BString::laenge(int len)
{
	if( len>laenge() )
		plus(len-laenge());
	else
		minus(laenge()-len);
	return *this;
}

BString &BString::plus(int delta)
{
	if( delta>0 )
		add(delta, 0, ' ');
	return *this;
}

BString &BString::minus(int delta)
{
	if( _str && _len && delta>0 )
	{
		if( _len>delta )
			_len -= delta;
		else
			_len = 0;
		_str[_len] = 0;
		trim();
	}
	return *this;
}

BString &BString::loescheSpacesEnde()
{
	int i;

	for(i=_len; i>0 && isspace((int)(unsigned char)(_str[i-1])); i--);
	if( i!=_len )
		laenge(i);

	return *this;
}

BString &BString::loescheSpacesAnfang()
{
	int i;

	for(i=0; i<_len && isspace((int)(unsigned char)(_str[i])); i++);
	if( i==_len)
		setzen("");
	else
		if( i )
			return ausfuegen(0, i);

	return *this;
}

///////////////////////////////////////////////////////////////////////////////

int BString::vergleich(const BString &s) const
{
	int			l0, l1;
	const char	*s0, *s1;

	for(s0=lesen(),l0=laenge(),s1=s.lesen(),l1=s.laenge(); l0||l1;
		s0++,s1++,l0--,l1--)
	{
		if( !l0 )
			return -1;
		else if( !l1 )
			return 1;
		else if (*s0 != *s1)
			return (int)*s0 - (int)*s1;
	}
	return 0;
}

int operator<(const BString &s1, const BString &s2)
{
	return s1.vergleich(s2)<0;
}

int operator<=(const BString &s1, const BString &s2)
{
	return s1.vergleich(s2)<=0;
}

int operator==(const BString &s1, const BString &s2)
{
	return s1.vergleich(s2)==0;
}

int operator>=(const BString &s1, const BString &s2)
{
	return s1.vergleich(s2)>=0;
}

int operator>(const BString &s1, const BString &s2)
{
	return s1.vergleich(s2)>0;
}

int operator!=(const BString &s1, const BString &s2)
{
	return s1.vergleich(s2)!=0;
}

int operator<(const BString &s1, const char *s2)
{
	return s1.vergleich(BString(s2))<0;
}

int operator<=(const BString &s1, const char *s2)
{
	return s1.vergleich(BString(s2))<=0;
}

int operator==(const BString &s1, const char *s2)
{
	return s1.vergleich(BString(s2))==0;
}

int operator>=(const BString &s1, const char *s2)
{
	return s1.vergleich(BString(s2))>=0;
}

int operator>(const BString &s1, const char *s2)
{
	return s1.vergleich(BString(s2))>0;
}

int operator!=(const BString &s1, const char *s2)
{
	return s1.vergleich(BString(s2))!=0;
}

///////////////////////////////////////////////////////////////////////////////

ostream& operator<<(ostream &out, const BString &s)
{
	return out << s.lesen();
}

///////////////////////////////////////////////////////////////////////////////

int BString::schreibeDatei(const BString &filename) const
{
	int		len, art;
	FILE	*out;

	if( !filename.laenge() )
	{
		art = 0;
		out = stdout;
	}
	else
	{
		art = 1;
		out=fopen(filename(), "w");
	}
	if( !out )
		return -1;

	len = _str ? fwrite(_str, 1, laenge(), out) : 0;

	switch( art )
	{
	case 0:	fflush(out);	break;
	case 1:	fclose(out);	break;
	}
	return len;
}

int BString::leseDatei(const BString &filename)
{
	const int	buflen = 0x10000;
	int			filelen=0, l, art;
	FILE		*in;

	setzen("");

	if( !filename.laenge() )
	{
		art = 0;
		in = stdin;
	}
	else
	{
		art = 1;
		in = fopen(filename(), "r");
	}
	if( !in )
		return -1;

	while( 1 )
	{
		plus(buflen);
		if( (l=fread(_str+filelen, 1, buflen, in)) <= 0 )
			break;
		filelen += l;
	}
	laenge(filelen);

	if( filelen )
		_str[filelen] = 0;
	trim();

	switch( art )
	{
	case 0:					break;
	case 1: fclose(in);		break;
	}
	return filelen;
}

///////////////////////////////////////////////////////////////////////////////

int BString::leseZeile(BString &s, int &start) const
{
	int bis;

	if( !_str || start>=laenge() )
		return 0;

	for(bis=start; bis<laenge() && _str[bis] != '\n'; bis++);
	s = "";
	s.anhaengen(_str+start, bis-start);
	start = bis+1;
	return 1;
}

///////////////////////////////////////////////////////////////////////////////

void BString::setzeNull()
{
	set(0, 0, 0);
}

BString &BString::operator<<(const BString &s)
{
	*this += s;
	return *this;
}

BString &BString::operator<<(const char *s)
{
	*this += s;
	return *this;
}

BString &BString::operator<<(const unsigned char *s)
{
	*this += (const char*)s;
	return *this;
}

BString &BString::operator<<(int i)
{
	*this += i;
	return *this;
}

BString &BString::operator<<(double d)
{
	*this += d;
	return *this;
}

BString &BString::operator<<(BString & (*s)(BString &))
{
	(*s)(*this);
	return *this;
}

BString &endl(BString &s)
{
	s += "\n";
	return s;
}

///////////////////////////////////////////////////////////////////////////////

void BString::trim()
{
	int		new_size;
	char	*new_str;

	if( (new_size=trimlen(_len)) < _size )
	{
		if( new_str=new char[new_size] )
		{
			memcpy(new_str, _str, _len+1);
			delete _str;
			_str = new_str;
			_size=new_size;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//	String auf len Bytes setzen
//	wenn s!=0, dann s kopieren sonst mit fuellbyte auffuellen
//	liefert !=0: geaendert
//
//	falls !_str, dann _str auf s setzen
//	falls !s, dann _str auf null setzen
//	falls Laengen unterschiedlich, dann setzen
//	falls Inhalt unterschiedlich, dann setzen

int BString::set(int len, const char *s, int fuellbyte)
{
	if( (!_str && len) || (_str && !len) || _len!=len || memcmp(_str, s, _len) )
	{
		_len = 0;				// alten Inhalt loeschen

		if( len )				// falls Quelle, dann anhaengen und fertig
		{
			return add(len, s, fuellbyte);
		}
		else					// sonst Ziel leeren
		{
			delete _str;
			init();
			return 1;
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
//	len Bytes an String anhaengen
//	wenn s!=0, dann s anhaengen sonst mit fuellbyte auffuellen
//
//	wenn mehr Speicher benoetigt wird, dann groesseren Block anfordern
//	wenn auch weniger Speicher genuegt (inkl. Reserve), kleineren Block holen
//	meistens sollte aber kein new/delete noetig sein...

int BString::add(int len, const char *s, int fuellbyte)
{
	char	*newstr;
	int		newlen = _len+len;
	int		newsize = trimlen(newlen);

	if( newlen+1>_size || newsize<_size )
	{
		if( newsize )
		{
			newstr = new char[newsize];
			if( _str && _len)
				memcpy(newstr, _str, _len);
			if( s )
				memcpy(newstr+_len, s, len);
			else
				memset(newstr+_len, fuellbyte, len);

			newstr[newlen] = 0;
			delete _str;

			_str = newstr;
			_size = newsize;
			_len = newlen;
		}
		else
		{
			delete _str;
			init();
		}
	}
	else
	{
		if( s )
			memcpy(_str+_len, s, len);
		else
			memset(_str+_len, fuellbyte, len);

		_str[_len += len] = 0;
	}
	return len!=0;
}

///////////////////////////////////////////////////////////////////////////////

