#ifndef SPIELER_HPP
# define SPIELER_HPP

# include "Skat.hpp"

class Spieler
{
	public:

	private:
		std::string	_name;
		int			_punkte;
		size_t		_spieleGesamt;
		size_t		_spieleGewonnen;
		size_t		_spieleVerloren;

};

#endif