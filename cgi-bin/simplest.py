import cgi
import cgitb
import os
import urllib.parse

def getZodiacSign(month, day):
    if (month == 3 and 21 <= day <= 31) or (month == 4 and 1 <= day <= 19):
        return "Aries"
    elif (month == 4 and 20 <= day <= 30) or (month == 5 and 1 <= day <= 20):
        return "Taurus"
    elif (month == 5 and 21 <= day <= 31) or (month == 6 and 1 <= day <= 20):
        return "Gemini"
    elif (month == 6 and 21 <= day <= 30) or (month == 7 and 1 <= day <= 22):
        return "Cancer"
    elif (month == 7 and 23 <= day <= 31) or (month == 8 and 1 <= day <= 22):
        return "Leo"
    elif (month == 8 and 23 <= day <= 31) or (month == 9 and 1 <= day <= 22):
        return "Virgo"
    elif (month == 9 and 23 <= day <= 30) or (month == 10 and 1 <= day <= 22):
        return "Libra"
    elif (month == 10 and 23 <= day <= 31) or (month == 11 and 1 <= day <= 21):
        return "Scorpio"
    elif (month == 11 and 22 <= day <= 30) or (month == 12 and 1 <= day <= 21):
        return "Sagittarius"
    elif (month == 12 and 22 <= day <= 31) or (month == 1 and 1 <= day <= 19):
        return "Capricorn"
    elif (month == 1 and 20 <= day <= 31) or (month == 2 and 1 <= day <= 18):
        return "Aquarius"
    else:
        return "Pisces"

def main():
	cgitb.enable()
	method = os.environ['REQUEST_METHOD']
	if method == "GET":
		date = dateurllib.parse.parse_qs(os.environ['QUERY_STRING'])
		zodiac = getZodiacSign(date['month'], date['day'])
		print('Hello, ', zodiac, 'your horoscope for the day is: ')
		print
	print('<!DOCTYPE html>')
	print('<html>')
  	print('<head>')
    print('<title>PYTHON Test</title>')
  	print('</head>')
  	print('<body>')
	print("hello python worked")

if __name__ == "__main__":
	main()