Triggers {

	var probabilities, aMinorProbabilities, eMinorProbabilities, gMajorProbabilities, cMajorProbabilities, bMinorProbabilities, fSharpProbabilities;
	var probablitiesChords;

	*new {
		^super.new.initTriggers();
	}


	//Set up Probabilities of the various Chords
	initTriggers{
		probabilities = Array.fill(12);
		//...............................C....C#..D....D#.E....F..F#..G....G#..A....A#.B......
		aMinorProbabilities = Array.with(100, 0,  0,   0, 50,  0, 0,  0,   0,  100, 0, 20  );
		eMinorProbabilities = Array.with(20,  0,  30,  0, 100, 0, 20, 100, 0,  40,  0, 100 );
		cMajorProbabilities = Array.with(100, 0,  60,  0, 80,  0, 0,  60,  0,  40,  0, 20  );
		gMajorProbabilities = Array.with(0,   40, 50,  0, 0,   0,  0, 100, 0,  40,  0, 20  );
		bMinorProbabilities = Array.with(0,   0,  100, 0, 20,  0, 60, 20,  0,  20,  0, 100 );
		fSharpProbabilities = Array.with(0,   100,20,  0, 20,  0, 100, 0,  0,  80,  0, 20  );

		//begin on C Major7
		probabilities = cMajorProbabilities;
	}

	//obtain a note from the probability tables
	getNote{arg startOctave, numberOctaves, startNote;
		var note;
		note = [0,1,2,3,4,5,6,7,8,9,10,11].wchoose(probabilities.normalizeSum);
		note = note + (12 * startOctave);
		note = note + (12 * numberOctaves.rand);
		note = note + startNote;
		^note;
	}

	//play a specified sample, with amp, note and loop as arguments
	sample{arg sample, note, panarg, amparg;
		var bufnum, amp, midiNote, pan, loop;

		case
		{sample == "main_pad"}{       bufnum=5;  amp = 0.7;  midiNote = note; loop = 1 }
		{sample == "seawater"}{       bufnum=17; amp = 0.1;  midiNote = 0;    loop = 1 }
		{sample == "windchimes"}{     bufnum=8;  amp = 0.12; midiNote = 0;    loop = 1 }
		{sample == "level_flute"}{    bufnum=7;  amp = 0.4;  midiNote = 0;    loop = 0 }
		{sample == "piano"}{          bufnum=22; amp = 0.3;  midiNote = note; loop = 0 }
		{sample == "glock"}{          bufnum=24; amp = 0.3;  midiNote = note; loop = 0 }
		{sample == "rhodes"}{         bufnum=25; amp = 0.3;  midiNote = note; loop = 0 }
		{sample == "egg_burst"}{      bufnum=20; amp = 0.3;  midiNote = note; loop = 0 }
		{sample == "water_drop"}{     bufnum=18; amp = 0.6;  midiNote = note; loop = 0 }
		{sample == "egg_near_burst"}{ bufnum=3;  amp = 0.2;  midiNote = note; loop = 0 }
		{sample == "glock_high"}{     bufnum=16; amp = 0.7;  midiNote = note; loop = 0 }
		{sample == "bloop"}{          bufnum=2;  amp = 0.2;  midiNote = note; loop = 0 }
		{sample == "short_stab"}{     bufnum=14; amp = 0.6;  midiNote = note; loop = 0 }
		{sample == "bubbles"}{        bufnum=30; amp = 0.6;  midiNote = note; loop = 0 }
		{sample == "choral"}{         bufnum=11; amp = 1.0;  midiNote = note; loop = 0 }
		{sample == "chord"}{          bufnum=4;  amp = 0.6;  midiNote = note; loop = 0 }
		{sample == "dark_pad"}{       bufnum=31; amp = 0.6;  midiNote = note; loop = 1 }
		{sample == "chimes"}{         bufnum=32; amp = 0.6;  midiNote = note; loop = 0 }
		{sample == "pluck"}{          bufnum=33; amp = 0.9;  midiNote = note; loop = 0 }
		{sample == "second_pad"}{     bufnum=34; amp = 0.3;  midiNote = note; loop = 1 };

		if(amparg != nil,{
			amp = amparg;
		});

		^Synth(\sample,[\bufnum,bufnum, \amp,amp, \midi, midiNote, \rate, 1, \outbus,16, \loop, loop, \pan, panarg]);
	}

	//Play an enveloped oscillator synth
	beep{arg type, midiNote, pan, amparg = 0.1;
		var freq;
		freq = midiNote.midicps;

		case
		{type == "sine"}{  ^Synth(\beep,[\amp, amparg, \freq, freq, \pan, pan, \dur,0.2,  \attack, 0.1, \outbus,16]) }
		{type == "pulse"}{ ^Synth(\pulse,[\amp, amparg, \freq, freq, \pan, pan, \dur,0.2,  \attack, 0.1, \outbus,16]) }
		{type == "bass"}{  ^Synth(\beep,[\amp, amparg,  \freq, freq, \pan, pan, \dur,10.0, \attack, 0.1, \outbus,0])  }
	}

	//causes change in probability table and root note. chord is chosen at random
	changeChord{
		var prob, root;
		root = 0;
		prob = 6.rand;

		case
		{prob == 0}{ probabilities = aMinorProbabilities; root = 9 }
		{prob == 1}{ probabilities = eMinorProbabilities; root = 4 }
		{prob == 2}{ probabilities = cMajorProbabilities; root = 0 }
		{prob == 3}{ probabilities = gMajorProbabilities; root = 7 }
		{prob == 4}{ probabilities = bMinorProbabilities; root = 11}
		{prob == 5}{ probabilities = fSharpProbabilities; root = 6 }
		^root;
	}


}