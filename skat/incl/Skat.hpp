#ifndef SKAT_HPP
# define SKAT_HPP

# include <iostream>

# include "Spiel.hpp"
# include "Spieler.hpp"

typedef enum
{
	Grand,
	Kreuz,
	Pik,
	Herz,
	Karo,
	Null,
	Ramsch
}	spieltyp;

typedef struct
{
	std::string	_spielart;
	contra;
	ramsch;
} punkte;


#endif