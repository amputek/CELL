SetUpCell{
	var location, server;

	*new{arg aServer;
		super.new.initSetUpCell(aServer);
	}

	initSetUpCell{arg aServer;
		var server = aServer;
		aServer.postln;
		"Launching Server...".postln;
		location = "/Users/Rob/Desktop/CMHits/";


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
		Buffer.read(server,location++"main_pad.wav", 0, -1, nil, 5);
		Buffer.read(server,location++"seawater.wav", 0, -1, nil, 17);
		Buffer.read(server,location++"windchimes.wav", 0, -1, nil, 8);

		//PLANKTON
		Buffer.read(server,location++"level_flute.wav", 0, -1, nil, 7);
		Buffer.read(server,location++"glock_high.wav", 0, -1, nil, 16);

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

		//SURFACE
		Buffer.read(server,location++"seagulls.wav", 0, -1, nil, 29);

		//FRIENDLY
		Buffer.read(server,location++"short_chord.wav", 0, -1, nil, 4);
		Buffer.read(server,location++"gliss.wav", 0, -1, nil, 26);

		//URCHIN
		Buffer.read(server,location++"dark_pad.wav", 0, -1, nil, 31);
		Buffer.read(server,location++"chimes.wav", 0, -1, nil, 32);
		//UNUSED
		Buffer.read(server,location++"sax.wav", 0, -1, nil, 23);

		Buffer.read(server,location++"synthnote.wav", 0, -1, nil, 27);
		Buffer.read(server,location++"cello.wav", 0, -1, nil, 21);
		Buffer.read(server,location++"aminor_chord.wav", 0, -1, nil, 0);
		Buffer.read(server,location++"low_whale.wav", 0, -1, nil, 1);


		Buffer.read(server,location++"low_growl.wav", 0, -1, nil, 6);
		Buffer.read(server,location++"resonant_chord.wav", 0, -1, nil, 9);
		Buffer.read(server,location++"note_sequence.wav", 0, -1, nil, 12);
		Buffer.read(server,location++"darker_hit.wav", 0, -1, nil, 19);

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

		//Reverb FX
		SynthDef(\reverb, {|inbus = 16, mix = 0.5, room = 0.9, damp = 0.9, amp = 0.8, length = 1, filter = 100|
			var env, signal, reverb;
			env = Env.new([0,1,1,0],[0.1,length,0.1]);
			signal = In.ar(inbus, 2);
			reverb = FreeVerb2.ar(signal[0], signal[1], mix, room, damp, amp);
			Out.ar(25, reverb);
		}).add;


		SynthDef(\delay, { | in = 25, out = 26, mix=0.9, time=0.375, feedMix=0.7, amp=0.9 |
			var local, inSig, maxdelay=10;

			inSig = In.ar(in);
			amp = Amplitude.kr(Mix.ar(in));
			inSig = inSig * (amp > 0.02); // noise gate
			local = LocalIn.ar(2) * feedMix;
			local = OnePole.ar(local, 0.4);
			local = OnePole.ar(local, -0.08);
			//local = Rotate2.ar(local[0], local[1], 0.2);

			local = DelayN.ar(local, maxdelay, time);
			local = LeakDC.ar(local);
			local = ((local + inSig) * 1.25).softclip;
			LocalOut.ar(local );
			Out.ar(out, local * 0.1);
		}).add;



		//Bandpass filter with low pass and hi pass values
		SynthDef(\filter, {|inbus = 26, lpcutoff = 20000, hpcutoff = 100, amp = 0.8|
			var signal, lp, hp;
			signal = In.ar(inbus, 2);
			lp = BLowPass.ar(signal, lpcutoff, 0.8);
			hp = BHiPass.ar(lp, hpcutoff,0.8);
			Out.ar(0, hp*amp);
		}).add;
	}
}