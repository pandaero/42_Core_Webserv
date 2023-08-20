#!/usr/bin/env python3
import cgi
import cgitb
import os
from urllib.parse import parse_qs

def getZodiacSign(month, day):
	if (month == 3 and 21 <= day <= 31) or (month == 4 and 1 <= day <= 19):
		return "Aries ♈️"
	elif (month == 4 and 20 <= day <= 30) or (month == 5 and 1 <= day <= 20):
		return "Taurus ♉︎"
	elif (month == 5 and 21 <= day <= 31) or (month == 6 and 1 <= day <= 20):
		return "Gemini ♊️"
	elif (month == 6 and 21 <= day <= 30) or (month == 7 and 1 <= day <= 22):
		return "Cancer ♋️"
	elif (month == 7 and 23 <= day <= 31) or (month == 8 and 1 <= day <= 22):
		return "Leo ♌️"
	elif (month == 8 and 23 <= day <= 31) or (month == 9 and 1 <= day <= 22):
		return "Virgo ♍️"
	elif (month == 9 and 23 <= day <= 30) or (month == 10 and 1 <= day <= 22):
		return "Libra ♎️"
	elif (month == 10 and 23 <= day <= 31) or (month == 11 and 1 <= day <= 21):
		return "Scorpio ♏️"
	elif (month == 11 and 22 <= day <= 30) or (month == 12 and 1 <= day <= 21):
		return "Sagittarius ♐️"
	elif (month == 12 and 22 <= day <= 31) or (month == 1 and 1 <= day <= 19):
		return "Capricorn ♑️"
	elif (month == 1 and 20 <= day <= 31) or (month == 2 and 1 <= day <= 18):
		return "Aquarius ♒️"
	else:
		return "Pisces ♓︎"

def getHoroscope(zodiac):
	if zodiac == "Aries":
		return (" This could be the luckiest day of the month, Aries. But astrology also takes your role into account. In other words, luck is something you have to work for. If you seem to have an incredible streak of luck today, it's probably because you did something earlier to make it happen!")
	elif zodiac == "Taurus":
		return ("If you get asked to lead a team in your personal or professional life, jump at the chance. Today you will get all the support you need, Taurus. Don't be afraid that you're not good enough. Things will come together without your having to do much at all. Besides, you already know that people are on your side!")
	elif zodiac == "Gemini":
		return ("You couldn't dream of a better day to deal with all the little problems in your daily life. If your doctor has given you a prescription, you can expect it would work like a miracle drug. Gemini, take care of the little things. It will take less time than you think!")
	elif zodiac == "Cancer":
		return ("Perhaps you can't believe it, but it's time to say goodbye to your rigid attitude. A little pleasure among all that seriousness and responsibility won't do you any harm. This day could help you change your point of view on life. This will feel great!")
	elif zodiac == "Leo":
		return ("Life is helping you out at the moment, Leo. Whatever it is, your guardian angel is always by your side. Perhaps you'd like to begin something new in your life, like moving or changing lifestyles. At the moment you can do anything you want to do. It will all work out just fine!")
	elif zodiac == "Virgo":
		return ("Have you ever thought about writing or working for a company in which you can use your excellent communication skills, Virgo? The planetary alignment emphasizes writing and communication. It's time to show the world that you have a gift and that people can count on you to do a great job. Think about the publishing business.")
	elif zodiac == "Libra":
		return ("If you've been thinking of living somewhere else, Libra, today will push you to really want to move far away from the place you're living now. Such a move could have consequences for your work situation or family life. It may be time to think seriously about what's holding you back. Wouldn't the people closest to you be happy to go along?")
	elif zodiac == "Scorpio":
		return ("You have a great day ahead of you, Scorpio. Everything will work out like clockwork. By the end of the day, you'll still have the energy to do something fun with your evening. It's a wonderful day for group activities. If you aren't involved in any, why not try a sport, hobby, music, or art?")
	elif zodiac == "Sagittarius":
		return ("You may have received some bad news concerning your finances, Sagittarius. This is probably the best day you could ask for out of the whole month to deal with these problems. If you want to come out of this situation a winner, it may be necessary to approach things from a different angle than usual.")
	elif zodiac == "Capricorn":
		return ("Today is a relatively calm day that's sure to please you, Capricorn. You may even receive gifts from family and friends as marks of their esteem or love for you. This is just the kind of reassurance you need. Though you have a fairly subtle influence in your relationships with others, it's nevertheless essential to you to be a good friend.")
	elif zodiac == "Aquarius":
		return ("Everyone knows that nothing can stop you when you're determined to get something done. As long as other people are on your side today, you will be amazed at all the things you can accomplish. But don't get too excited, Aquarius, because it's time to take care of those thankless tasks that no one wants to deal with.")
	else:
		return ("You may be considering taking a trip or planning a party with some friends, Pisces. You may find out today that it's up to you to do all the organizing. This won't bother you too much because you know you'll do it right. Why don't you think about really trying to outdo yourself and plan something special that your friends will never forget?")


def main():
	cgitb.enable()
	method = os.environ['REQUEST_METHOD']
	print(method)
	for param in os.environ.keys():
		print ("<b>%20s</b>: %s<\br>" % (param, os.environ[param]))
		print('</br>')
	print('--------------------')
	# print(method)
	print('--------------------')
	print('<!DOCTYPE html>')
	print('<html>')
	print('<head>')
	print('<title>PYTHON Test</title>')
	print('</head>')
	print('<body>')
	if method == "GET":
		qs = os.environ['QUERY_STRING']
		print(qs)
		date = parse_qs(qs)
		zodiac = getZodiacSign(date['month'], date['day'])
		print('<h2>Hello, ', zodiac, 'your horoscope for the day is: </h2>')
		print('<p>', getHoroscope(zodiac), '</p>')
	elif method == "POST":
		body = os.environ['POST_BODY'].split(' ')
		message = body[0].split('=')[1]
		receiver = body[1].split('=')[1]
		print(message, receiver)
		if message == "" or receiver == "":
			print('<h2>🔴Error!🔴</h2>')
			print('<p>Please fill out the fields correctly</p>')
		else:
			print('<h2>Thank you for your message to', receiver[:2] + (len(receiver) - 2) * "*" + '</h2>')
			print('<p>Length of your message: ' + str(len(message)) + '</p>')
	print('</body>')
	print('</html>')


if __name__ == "__main__":
	main()