# CS467_Phoenix
Shared repository for CS467 Team Phoenix Week 04 branch

For the latest changelog see
https://docs.google.com/document/d/1qrRssbhig6v8y5otEunce0lCNV8vEc7iDRUUC3Kh1qI


To play: 
	make game;
	./game

This will print provisional instructions. You can also "make test" but
the test program is currently uninteresting. To turn on debugging use
the boolean flags in utilities.hpp

Program Files:
	utilities
		Global settings, including flags to turn debugging on/off

	engine
		GameState and Choice classes
		playTurn is the central loop of the game

	house
		Builds the house from files on disk

	room
	feature
	parser
