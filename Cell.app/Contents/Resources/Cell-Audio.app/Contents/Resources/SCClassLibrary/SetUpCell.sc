SetUpCell{
	var location, server;

	*new{arg aServer;
		super.new.initSetUpCell(aServer);
	}

	initSetUpCell{arg aServer;
		var server = aServer;
		aServer.postln;
		"Launching Server...".postln;
		location = (String.scDir ++ "/sounds/");


		server.options.verbosity = -2;
		"Loading Buffers and Synth Defs...".postln;
		server.waitForBoot({
			this.loadBuffers;
			this.loadSynthDefs;
			"Ready.".postln;
		});

		("SERVER=" + server).postln;
	}

	//Load Buffers with Samples
	loadBuffers{
		//PAD
		Buffer.read(server,location++"main_pad.wav",    0, -1, nil, 5);
		Buffer.read(server,location++"seawater.wav",    0, -1, nil, 17);
		Buffer.read(server,location++"windchimes.wav",  0, -1, nil, 8);
		Buffer.read(server,location++"second_pad.wav",  0, -1, nil, 34);

		//PLANKTON
		Buffer.read(server,location++"level_flute.wav", 0, -1, nil, 7);
		Buffer.read(server,location++"glock_high.wav",  0, -1, nil, 16);
		Buffer.read(server,location++"pluck.wav",       0, -1, nil, 33);

		//FIELDS
		Buffer.read(server,location++"piano.wav", 0, -1, nil, 22);
		Buffer.read(server,location++"glock.wav", 0, -1, nil, 24);
		Buffer.read(server,location++"rhodes.wav", 0, -1, nil, 25);
		Buffer.read(server,location++"egg_burst.wav", 0, -1, nil, 20);
		Buffer.read(server,location++"water_drop.wav", 0, -1, nil, 18);
		Buffer.read(server,location++"egg_near_burst.wav", 0, -1, nil, 3);
		Buffer.read(server,location++"a_bloop.wav", 0, -1, nil, 2);

		//JELLYFISH
		Buffer.read(server,location++"short_stab.wav", 0, -1, nil, 14);

		//EGG
		Buffer.read(server,location++"choral_pad.wav", 0, -1, nil, 11);
		Buffer.read(server,location++"bubbles.wav", 0, -1, nil, 30);

		//FRIENDLY
		Buffer.read(server,location++"short_chord.wav", 0, -1, nil, 4);
		Buffer.read(server,location++"gliss.wav", 0, -1, nil, 26);

		//URCHIN
		Buffer.read(server,location++"dark_pad.wav", 0, -1, nil, 31);
		Buffer.read(server,location++"chimes.wav", 0, -1, nil, 32);
	}

	//Initialise Synth Defs
	loadSynthDefs{

		//Play Sample at specified midi note
		SynthDef(\sample, {arg pan=0, amp=0.5, bufnum = 0, outbus = 0, midi = 0, loop = 0;
			var sample;
			sample = PlayBuf.ar(1, bufnum, (midi.midiratio) * BufRateScale.kr(bufnum), 1, 0, loop, 2);
			Out.ar(outbus,Pan2.ar((sample*amp), pan))
		}).add;

		//Sine Beep
		SynthDef(\beep,{arg freq=440,amp=0.1, pan=0.0, dur=0.1, attack = 0.5, outbus = 16;
			var env = EnvGen.kr(Env.perc(attack,dur),doneAction:2);
			OffsetOut.ar(outbus,Pan2.ar(SinOsc.ar(freq,0,amp)*env,pan))
		}).add;

		//Pulse Beep
		SynthDef(\pulse,{arg freq=440,amp=0.1, pan=0.0, dur=0.1, attack = 0.5, outbus = 16;
			var env = EnvGen.kr(Env.perc(attack,dur),doneAction:2);
			OffsetOut.ar(outbus,Pan2.ar(Pulse.ar(freq,0.5,amp)*env,pan))
		}).add;

		//Delay
		SynthDef(\delay, { | inbus = 16, outbus = 18, mix=0.5, time=0.375, feedMix=0.375, amp=0.5 |
			var local, input, maxdelay=10;

			input = In.ar(inbus, 2);
			local = LocalIn.ar(2) * feedMix;
			local = OnePole.ar(local, 0.4);
			local = OnePole.ar(local, -0.08);

			local = DelayN.ar(local, maxdelay, time);
			local = LeakDC.ar(local);
			local = ((local + input) * 1.25).softclip;
			LocalOut.ar(local);
			Out.ar(outbus, local * 0.5);
		}).add;

		//Reverb FX
		SynthDef(\reverb, {| inbus = 18, outbus = 20, mix = 0.5, room = 0.9, damp = 0.9, amp = 0.9, length = 1 |
			var env, signal, reverb;
			env = Env.new([0,1,1,0],[0.1,length,0.1]);
			signal = In.ar(inbus, 2);
			reverb = FreeVerb2.ar(signal[0], signal[1], mix, room, damp, amp);
			Out.ar(outbus, reverb);
		}).add;

		//Bandpass filter with low pass and hi pass values
		SynthDef(\filter, {|inbus = 20, lpcutoff = 20000, hpcutoff = 100, amp = 1.0, t_trig = 0|
			var signal, lp, hp;
			signal = In.ar(inbus, 2);
			lp = BLowPass.ar(signal, lpcutoff, 2.0);
			hp = BHiPass.ar(lp, hpcutoff,2.0);
			Out.ar(0, hp*amp);
		}, [0, 1] ).add;   //[0,1] is trigger rates, so that cutoff values wont change immediately
	}
}
