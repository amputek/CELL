Cell v0.1
=========

CELL is an audio-visual game built in Cinder and SuperCollider, and uses Open Sound Control protocol to allow communication between Cinder and SuperCollider


Cinder: http://libcinder.org/

SuperCollider: http://supercollider.sourceforge.net/

Cinder OSC block: https://github.com/hecspc/Cinder-OSC 


####System Requirements
Mac OS X 10.8


Running Cell
------------

####METHOD 1: Standalone
* Run the standalone Cell.app


####METHOD 2: Cinder and SC standalones
* Run Cell-SC.app
* wait a few seconds
* Run Cell-Cinder.app


####METHOD 3: Developer mode (Requires SuperCollider, Cinder w/ OSC blocks)
* Locate the SuperCollider classes inside the SuperCollider-src/Classes/ folder: ('SetUpCell.sc', 'Spark.sc', 'Triggers.sc' and 'Friendly.sc') 
* Place these in ~/Library/Application Support/SuperCollider/Extensions/
* Open SuperCollider
* Recompile the SuperCollider Class Library (Shift-Option-L in SC 3.6)
* Open up the SuperCollider file 'Cell.scd'
* Double click inside the top brackets, and press Shift-Enter (this 'evaluates' the code, and will prepare it for initiation from XCode)

* Open the Cell.xcodeproj file
* Hit "Run"


