#ifndef BString_hxx
#define BString_hxx

#include <stdio.h>
#include <string.h>
#include <iostream.h>

class BString
{
public:
	// Konstruktoren (mit Vorbelegung/Formatierung) und Destruktor

	BString();
	BString(int i, const char *format=0);
	BString(double d, const char *format=0);
	BString(const char *s);
	BString(const BString &s);
	BString(const BString *s);
	~BString();

	// Zuweisungsoperatoren

	inline BString	&operator=(int i);
	inline BString	&operator=(double d);
	inline BString	&operator=(const char *s);
	inline BString	&operator=(const BString &s);
	inline BString	&operator=(const BString *s);

	// Auslesen des String (ergibt mindestens "") und Auskunftsfunktionen

	inline const char	*lesen() const;
	inline const char	*operator()() const;
	inline int			laenge() const;
	inline int			istLeer() const;
	inline int			maxLaenge() const;
	int					istNurSpaces() const;

	// Auslesen eines Zeichens (falsche Position pos liefert EOF)

	inline int			operator[](int pos) const;

	// Setzen mit Werten (teilweise mit Formatierung)

	int 				setzen(int i, const char *format=0);
	double 				setzen(double d, const char *format=0);
	inline const char	*setzen(const char *s);
	inline BString		&setzen(const BString& s);
	inline BString		&setzen(const BString* s);

	const char			*setzen(const char *s, int laenge);

	// Anhaengen von Werten (teilweise mit Formatierung)

	BString			&anhaengen(int i, const char *format=0);
	BString 			&anhaengen(double d, const char *format=0);
	BString			&anhaengen(const char *s, int laenge=-1);	// -1: alles
	inline BString		&anhaengen(const BString &s);
	BString			&anhaengen(const BString *s);
	BString			&anhaengen(int c, int n);		// Zeichen c n-mal anhaengen

	inline BString	&operator+=(int i);
	inline BString	&operator+=(long l);
	inline BString	&operator+=(double d);
	inline BString	&operator+=(const char *s);
	inline BString	&operator+=(const BString &s);
	inline BString	&operator+=(const BString *s);

	// Einfuegen von Werten (an Position pos, len=-1: alles)

	BString			&einfuegen(const char *s, int pos, int len=-1);
	inline BString		&einfuegen(const BString &s, int pos, int len=-1);
	BString			&einfuegen(int c, int pos, int n);

	// Ausfuegen (an Position pos n Zeichen)

	BString			&ausfuegen(int pos, int n);

	// Ersetzen von anz Zeichen ab Position pos mit s bzw. n-mal c

	BString			&ersetzen(int pos, int anz, const char *s, int len=-1);
	inline BString		&ersetzen(int pos, int anz, const BString &s, int len=-1);
	BString			&ersetzen(int pos, int anz, int c, int n=1);

	// Ersetzen von s durch e (falls vorhanden) alles=0: einmal, sonst alles

	BString			&ersetzen(const BString &s, const BString &e, int alles=0);

	// Suchen von s ab Position start; liefert pos wenn gefunden -1 sonst

	int					suchen(const BString &s, int start=0) const;

	// Auslesen von n Zeichen ab Position pos (n=-1: Rest auslesen)

	BString			auslesen(int pos, int n) const;
	BString			&auslesen(BString &ziel, int pos, int n) const;

	// Kopieren

	void				kopieren(const BString *s);
	BString			&kopieren(const BString &q, int abWo, int laenge);

	// Lesen eines Tokens/Wortes nach ziel ab Position start (in-out) bis ein
	// Zeichen aus trennzeichen kommt; alle Token lesen: while(leseToken(...))
	// Worte werden durch ein, Token durch beliebig viele Zeichen getrennt
	// (Wort kann leer sein, Token nicht)

	int leseToken(BString &ziel, int &start, const char *trennzeichen) const;
	int leseWort(BString &ziel, int &start, const char *trennzeichen) const;

	// Umwandlung in Gross- bzw. Kleinschreibung

	BString			gross() const;
	BString			klein() const;
	BString			anfangGross() const;	// Woerter in Gross/Kleinschreibung
	BString			&operator++();
	BString			&operator--();
	friend BString	operator+(const BString &s);
	friend BString	operator-(const BString &s);

	// binaeres Concat

	friend BString	operator+(const BString &s1, const BString &s2);

	// Setzen/Aendern der Laenge (Abschneiden/Auffuellen mit Spaces)

	BString		&laenge(int len);
	BString		&plus(int delta);
	BString		&minus(int delta);

	BString		&loescheSpacesEnde();
	BString		&loescheSpacesAnfang();

	// Vergleichsfunktion und Vergleichsoperatoren

	int			vergleich(const BString &s) const;

	friend int	operator< (const BString &s1, const BString &s2);
	friend int	operator<=(const BString &s1, const BString &s2);
	friend int	operator==(const BString &s1, const BString &s2);
	friend int	operator>=(const BString &s1, const BString &s2);
	friend int	operator> (const BString &s1, const BString &s2);
	friend int	operator!=(const BString &s1, const BString &s2);

	friend int	operator< (const BString &s1, const char *s2);
	friend int	operator<=(const BString &s1, const char *s2);
	friend int	operator==(const BString &s1, const char *s2);
	friend int	operator>=(const BString &s1, const char *s2);
	friend int	operator> (const BString &s1, const char *s2);
	friend int	operator!=(const BString &s1, const char *s2);

	// Ausgabe auf Stream

	friend ostream& operator<<(ostream &out, const BString &s);

	// Ein-/Ausgabe auf Stdio/File
	// liefert Anzahl gelesener/geschriebener Bytes oder -1 bei Fehler

	int		schreibeDatei(const BString &filename) const;
	int		leseDatei(const BString &filename);

	// liest ab Position start eine Zeile nach s (bis CR oder EOT)
	// liefert 1 wenn noch eine Zeile gefunden wurde, 0 sonst

	int		leseZeile(BString &s, int &start) const;

	// Internes

	void	setzeNull();				// Null-String (braucht kein Speicher)

	BString	&operator<<(int i);
	BString	&operator<<(double d);
	BString	&operator<<(const char *s);
	BString	&operator<<(const unsigned char *s);
	BString	&operator<<(const BString &s);
	BString	&operator<<(BString & (*s)(BString &));

	friend BString	&endl(BString &s);

protected:
	char	*_str;				// Zeiger auf Zeichenkette (kann 0 sein)
	int		_len;				// Laenge der Zeichenkette
	int		_size;				// Groesse des reservierten Speichers

	inline void	init();			// Variable auf 0 setzen (kein delete)

	// Grundfunktionen zum Aendern des Strings (nur hier sind new/deletes)

	void	trim();				// BString auf Maximallaenge kuerzen
	int		set(int len, const char *s, int fuellbyte);
	int		add(int len, const char *s, int fuellbyte);
};

///////////////////////////////////////////////////////////////////////////////

inline BString &BString::operator=(int i)
{
	setzen(i); return *this;
}

inline BString &BString::operator=(double d)
{
	setzen(d); return *this;
}

inline BString &BString::operator=(const char *s)
{
	setzen(s); return *this;
}

inline BString &BString::operator=(const BString &s)
{
	kopieren(&s); return *this;
}

inline BString &BString::operator=(const BString *s)
{
	if(s) kopieren(s); else setzen(""); return *this;
}

inline const char* BString::lesen() const
{
	return _str ? _str : "";
}

inline const char *BString::operator()() const
{
	return lesen();
}

inline int BString::laenge() const
{
	return _len;
}

inline int BString::istLeer() const
{
	return _len==0;
}

inline int BString::maxLaenge() const
{
	return _size;
}

inline int BString::operator[](int pos) const
{
	return (pos>=0 && pos<laenge()) ? ((char*)_str)[pos] : EOF;
}

inline const char *BString::setzen(const char *s)
{
	return s ? setzen((s), strlen(s)) : setzen("", 0);
}

inline BString &BString::setzen(const BString &s)
{
	return setzen(&s);
}

inline BString &BString::setzen(const BString *s)
{
	kopieren(s); return *this;
}

inline BString &BString::anhaengen(const BString& s)
{
	return anhaengen(&s);
}

inline BString &BString::operator+=(int i)
{
	return anhaengen(i);
}

inline BString &BString::operator+=(long l)
{
	return anhaengen((int) l);
}

inline BString &BString::operator+=(double d)
{
	return anhaengen(d);
}

inline BString &BString::operator+=(const char *s)
{
	return anhaengen(s);
}

inline BString &BString::operator+=(const BString &s)
{
	return anhaengen(&s);
}

inline BString &BString::operator+=(const BString *s)
{
	return anhaengen(s);
}

inline BString &BString::einfuegen(const BString &s, int pos, int len)
{
	return einfuegen(s(), pos, len>=0 ? len : s.laenge());
}

inline BString &BString::ersetzen(int pos, int anz, const BString &s, int len)
{
	return ersetzen(pos, anz, s(), len>=0 ? len : s.laenge());
}

inline void BString::init()
{
	_str=0; _len=0; _size=0;
}

#endif

