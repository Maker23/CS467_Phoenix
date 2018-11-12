# CS467_Phoenix
Shared repository for CS467 Team Phoenix Week 04 branch

Recent changelogs:

* [Nov 11 changelog](https://docs.google.com/document/d/1i1ym1oiwkVLkDJnnqWR0Xu9vIKqjrqg7QbATehCOPgs)
* [Nov  6 changelog](https://docs.google.com/document/d/195l83ZnKcqkBQI3VFZ3RitSjwrIvR9FeTapJ03upab0)
* [Oct 27 changelog](https://docs.google.com/document/d/1qrRssbhig6v8y5otEunce0lCNV8vEc7iDRUUC3Kh1qI)
* [Oct 28 changelog](https://docs.google.com/document/d/1aMw9Vvtfe2F1FCO6yy3UXWrd-49YYMJTFAl5mVH3XAo)

Github repository:

* [CS467_Phoenix](https://github.com/Maker23/CS467_Phoenix)


To play: 
```
	make game
	./game
```
To Test:
```
  ./game -test <instructionsfile>
	./game -test walkthrough.txt
```

The game will print short instructions when it starts up; you can get
full instructions with the "help" command.

To turn on debugging use the boolean flags in **utilities.hpp** and then recompile.

#### Program Files:

	utilities
		Global settings, including flags to turn debugging on/off

	engine
		GameState and Choice classes
		playerTurn is the central loop of the game

	house
		Builds the house from files on disk

	room
		Room class functions; reads and parses room files.

	feature
		Feature class functions; reads and parses feature files.

	parser
		Command-line parse functions.
