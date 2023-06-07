#ifndef SPIEL_HPP
# define SPIEL_HPP

# include "Skat.hpp"

class Spiel
{
	public:
		void	loadSave();
		void	enterResults();
		void	saveResults();
	private:
		static size_t	_spielZahl;
		size_t			_spielID;
		std::string		_datum;
		Spieler			_spieler1;
		Spieler			_spieler2;
		Spieler			_spieler3;
		spieltyp		_spieltyp;
		size_t			_reizwert;
		size_t			_bubenwert;
		size_t			_soloSpieler;
		bool			_hand;
		bool			_schneider;
		bool			_schneiderAngesagt;
		bool			_schwarz;
		bool			_schwarzAngesagt;
		bool			_ouvert;
		bool			_ouvert;
};

#endif