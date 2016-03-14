(
s = Server.local;
SetUpCell.new(s);
)

(
var trigger;
//SYNTHS
var pads, filter, reverb, delay, surfaceSynth, eggSynth, sparks, friendlies, urchinPad;
//ENTITY BOOLEANS
var twirlActive, puffActive, jellyActive, feelerActive, surfaceActive, eggActive, urchinActive, urchinPadActive, loopActive, beatNumber;

trigger = Triggers.new();
a = Group.basicNew(s,1);
n = NetAddr("127.0.0.1", 12080);

sparks =     List.fill(0);
friendlies = List.fill(0);
pads = Array.fill(3);

//Initialise SynthDef FX
reverb = Synth.tail(a, \reverb);
delay = Synth.tail(a, \delay);
filter = Synth.tail(a, \filter, [\lpcutoff, 600, \hpcutoff,500]);

//BOOLEAN VARIABLES
twirlActive = false;
puffActive = false;
jellyActive = false;
feelerActive = false;
surfaceActive = false;
eggActive = false;
urchinActive = false;
urchinPadActive = false;

OSCFunc.new({|msg,time,addr,recvPort|
	n.sendMsg("/confirmReply");
}, '/confirm');


//CINDER HAS LOADED, START LOOP
OSCFunc.new({|msg,time,addr,recvPort|
	"STARTING LOOP".postln;

	//INITIAL SOUNDS
	pads[0] = trigger.sample("main_pad", 0);
	pads[1] = trigger.sample("main_pad", -12);
	pads[2] = trigger.sample("main_pad", -5);
	trigger.sample("windchimes", 0);
	trigger.sample("seawater", 0);

	{
	inf.do{arg bars;
		{
		16.do{arg beat;
			beatNumber = beat;

			sparks.size.do{arg i;
				if(sparks[i].beatNumber == beat,{
					case
					{ sparks[i].type == 0 }{ trigger.sample("piano", sparks[i].frequency ) }
					{ sparks[i].type == 1 }{ trigger.sample("glock", sparks[i].frequency ) }
					{ sparks[i].type == 2 }{ trigger.sample("rhodes", sparks[i].frequency ) };
					n.sendMsg("/sparkPulse", i );
				});
			};

			friendlies.size.do{arg i;
				if(friendlies[i].born == true,{
					if(0.5.coin == true,{
						trigger.sample("bloop", trigger.getNote(0,2,3), friendlies[i].pan, friendlies[i].distance);
						n.sendMsg("/friendlyPulse", i );
					});
				});
			};

			0.25.wait;
		};
		}.fork;
		4.0.wait;
	};
	}.fork;

}, '/startLoop');


//'LEVEL UP' FLUTE
OSCFunc({|msg,time,addr,recvPort|
	trigger.sample("level_flute",0);
}, '/levelUp');


//SURFACE
OSCFunc({|msg,time,addr,recvPort|
	msg.postln;
	if(msg[1] == 1,{
		if(surfaceActive == false,{
			surfaceActive = true;
			filter.set(\lpcutoff, 20000);
			trigger.sample( "water_drop", 0 );
			pads[0].set(\amp, 0);
			pads[1].set(\amp, 0);
			pads[2].set(\amp, 0);
		});
	},{
		if(surfaceActive == true,{
			surfaceActive = false;
			trigger.sample( "water_drop", 0 );
			trigger.sample("bubbles",0);
			pads[0].set(\amp, 0.5);
			pads[1].set(\amp, 0.5);
			pads[2].set(\amp, 0.5);
		});
	});
}, '/surface');


//GRASS
OSCFunc({|msg,time,addr,recvPort|
	msg.postln;
	trigger.sample("short_stab", trigger.getNote(-2,2,0) );
}, '/grass');


//URCHIN
OSCFunc({|msg,time,addr,recvPort|
	msg.postln;
	if( msg[1] < 500, {
		if(urchinPadActive == false,{
			urchinPadActive = true;
			urchinPad = trigger.sample("dark_pad", 0);
		});
		urchinPad.set( \amp, ((500 - msg[1]) * 0.001) );
	},{
		if(urchinPadActive == true,{
			urchinPadActive = false;
			urchinPad.free;
		});
	});

	if(msg[2] > 0,{
		if(urchinActive == false,{
			urchinActive = true;

			{
				trigger.sample("chimes", 12.rand);
				0.2.rand.wait;
				urchinActive = false;
			}.fork;
		});
	},{
		if(urchinActive == true,{
			urchinActive = false;
		});
	});
}, '/urchin');

//SPORE BOOP
OSCFunc({|msg,time,addr,recvPort|
	msg.postln;
	msg[1].do{
		trigger.sample("bloop", trigger.getNote(0,2,3), -1.0 );
	};
	if(msg[1] == 4,{
		trigger.sample("egg_near_burst", trigger.getNote(0,1,0) );
	});
}, '/sporeBoop');


//NEW SPARK
OSCFunc({|msg,time,addr,recvPort|
	var midiNote, type;
	midiNote = 0;
	type = msg[1];
	("New Spark, type: " + type + "on beat " + beatNumber).postln;

	case
	{ type == 0 }{ midiNote = trigger.getNote(-1,2,8)  }//ORANGE - PIANO
	{ type == 1 }{ midiNote = trigger.getNote(1,2,0)  } //GREEN - GLOCK
	{ type == 2 }{ midiNote = trigger.getNote(-2,2,0) };//PINK - RHODES

	sparks.add( Spark.new(beatNumber, midiNote, type ) );

	trigger.sample("egg_burst", trigger.getNote(0,1,2) );
	trigger.sample("water_drop", 0);

}, '/newSpark');



//NEW FRIENDLY
OSCFunc({|msg,time,addr,recvPort|
	msg.postln;
	friendlies.add( Friendly.new( 0, 0.1 ) );
}, '/newFriendly');


//BORN FRIENDLY
OSCFunc({|msg,time,addr,recvPort|
	var index = msg[1];
	msg.postln;
	trigger.sample("egg_burst", -24 );
	trigger.sample("chord", 0);
	friendlies[index].setBorn;
}, '/bornFriendly');


//UPDATE FRIENDLIES
OSCFunc({|msg,time,addr,recvPort|
	var index, pan, dist = 0;
	index = msg[1];
	pan = msg[2] * 0.002;

	dist = (0.2 - (msg[3]*0.00025));

	if(dist < 0,{
		dist = 0;
	});

	friendlies[index].setPan( pan );
	friendlies[index].setDistance( dist );
}, '/updateFriendly');



//JELLYFISH
OSCFunc({|msg,time,addr,recvPort|
	msg.postln;
	if(jellyActive == false,{
		jellyActive = true;
		{
			(msg.size - 1).do{arg i;
				if(msg[i + 1] == 1,{
					trigger.sample("short_stab", trigger.getNote(0,2,0) );
				});
				0.1.wait;
				jellyActive = false;
			};
		}.fork;
	});
}, '/jelly');


//PLANKTON EAT
OSCFunc.new({|msg,time,addr,recvPort|
	var pan, dist;
	pan = 0;
	msg.postln;
	pan = msg[2] * 0.0002;
	dist = (0.2 - (msg[3]*0.0005));

	if(dist < 0,{
		dist = 0;
	});

	("plankton eaten: " + msg[1] + ": " + pan + ": " + dist).postln;
	case
	{ msg[1] == 0 }{ trigger.beep("sine",         trigger.getNote(5,2,0),  pan, dist) }
	{ msg[1] == 1 }{ trigger.beep("pulse",        trigger.getNote(5,2,0),  pan, dist) }
	{ msg[1] == 2 }{ trigger.cluster(false, "sine", 5, 2,                  pan, dist) }
	{ msg[1] == 3 }{ trigger.sample("glock_high", trigger.getNote(-1,2,0), pan, dist) }
	{ msg[1] == 4 }{ trigger.cluster(true, "glock_high", -1, 2,            pan, dist) };
}, '/planktonEat');


//EGG
OSCFunc.new({|msg,time,addr,recvPort|
	msg.postln;
	if(msg[1] == 1,{
		if(eggActive == false,{
			eggSynth = trigger.sample( "choral", 0 );
			trigger.beep("bass", 24);
			trigger.sample("bubbles",0);
			trigger.sample( "water_drop", 0 );
			filter.set(\lpcutoff, 300);
			pads[0].set(\amp, 0.0);
			pads[1].set(\amp, 0.0);
			pads[2].set(\amp, 0.0);
			eggActive = true;
		});
	},{
		if(eggActive == true,{
			eggSynth.free;
			trigger.sample( "water_drop", 0 );
			trigger.sample("bubbles",0);
			pads[0].set(\amp, 0.5);
			pads[1].set(\amp, 0.5);
			pads[2].set(\amp, 0.5);
			eggActive = false;
		});

	});

}, '/egg');


//CHORD CHANGE
OSCFunc({|msg, time, addr, recvPort|
	var root = 0;
	root = trigger.changeChord();

	("new root: " + root).postln;

	trigger.beep("bass", (root+24));
	trigger.beep("bass", (root+36));

	sparks.size.do{arg i;
		var type, midiNote;
		midiNote = 0;
		type = sparks[i].type;

		case
		{ type == 0 }{ midiNote = trigger.getNote(0,1,8) }
		{ type == 1 }{ midiNote = trigger.getNote(1,1,0) }
		{ type == 2 }{ midiNote = trigger.getNote(-2,1,0)};

		sparks[i].setFrequency(midiNote);
	};

	pads[0].set(  \midi, root - 12);
	pads[1].set(  \midi, root );
	pads[2].set(  \midi, root + 12);
}, '/changeChord');


//SET DEPTH
OSCFunc({|msg, time, addr, recvPort|
	var depth;
	depth = (msg[1] * 4000) + 200;

	//depth.postln;

	if(surfaceActive == false,{
		if(eggActive == false,{
			filter.set(\lpcutoff,depth);
		});
	});
}, '/setDepth');


//QUIT
OSCFunc({|msg, time, addr, recvPort|
	"quitting".postln;
	s.freeAll;
}, '/quit');


)



(
s = Server.local;
SetUpCell.new(s);
)

b = NetAddr("127.0.0.1", 57120);
b.sendMsg("/jelly", 0, 1, 0, 0, 1, 1, 1, 0 );
b.sendMsg("/setDepth", 0.5,0);
b.sendMsg("/planktonEat",5.rand,600,125.6125);
b.sendMsg("/changeChord");
b.sendMsg("/sporeBoop", 2);
b.sendMsg("/startLoop");
b.sendMsg("/quit");
b.sendMsg("/newFriendly");
b.sendMsg("/egg", 0);
b.sendMsg("/surface", 1);
b.sendMsg("/surface", 0);
(
{
	10.do{arg i;
		b.sendMsg("/newSpark", 3.rand, i);
			0.3.wait;
	};
}.fork;
)