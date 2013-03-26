TriggerClass {
	
var <>drumFunction, voxFunction, bassFunction, beepFunction, spokenFunction, introFunction, keyFunction, beatFunc, arpFunction;
var <>hits3, hit3bar;
var <>introLength;
var <>pos;
var <>scramblePhrase, scrambleBar, numberOfScrambles;
var <>cutoff, leave, tempo;
var <>phrases, bars, beats;

*new{
	super.new.initTriggerClass();
}


initTriggerClass{arg s, c, y, b;

s = Server.internal;

//setting up variables
introLength = ((4.rand)+3);			//random intro length
pos = Array.fill(8);				//positions in acapella sample
numberOfScrambles = ((9.rand)+2);	//number of bars to apply 'scramble' fx to
tempo = (2.15+(0.15.rand));			//random tempo (within range)
phrases = 0;						//global counter of phrases
bars = 0;							//...of bars,
beats = 0;						//...and of beats
cutoff = 10000;					//global cutoff value
leave = 0;						//leaving first few beats out on multiple instruments

("Tempo: " + tempo).postln;
("Intro Length: " + introLength).postln;

//calculating scramble bars
scramblePhrase = Array.fill(numberOfScrambles);
scrambleBar = Array.fill(numberOfScrambles);
(numberOfScrambles + "scramble bars (phrase . bar):").postln;
numberOfScrambles.do{arg i;
	scramblePhrase[i] = (((16-introLength).rand)+introLength);
	scrambleBar[i] = 8.rand;
	if(i == (numberOfScrambles - 1),{
		("(" + scramblePhrase[i] + "." + scrambleBar[i] + ")").postln;
	},{
		("(" + scramblePhrase[i] + "." + scrambleBar[i] + "),").post;
	});
};



//--------COMPOSITION TRIGGERING STARTS FROM HERE--------\\

//trigger ambient sample
s.bind(Synth(\sample,[\bufnum, 12, \startPos,0, \dur,32*tempo, \amp, 0.15, \envelope, 0]));	

//16 phrases of 8 bars = around 5 minutes
{16.do{arg ph;
	phrases = ph;
	
	//introduction section
	if(phrases < (introLength-1),{
		introFunction.value();
	});
	
	//hard-coded phrase before main section
	if(phrases == (introLength-1),{
		("._______________________.").postln;
		("| d | b | f | v | b | a |").postln;
		("|---|---|---|---|---|---||").postln;
		beatFunc.value(0,0,1,1,1,0);
	});
	
	//replay ambient sample for outro
	if(phrases == (introLength+9),{
		s.bind(Synth(\sample,[\bufnum, 12, \startPos,0, \dur,32*tempo, \amp, 0.15, \envelope, 0]));
	});
	
	//main section
	if(phrases > (introLength-1),{		
		if(phrases%4==0,{
			s.bind(Synth(\whitenoise));
		});	
	
		if(phrases < (introLength+9),{
			leave = [0,1].choose;	
			beatFunc.value(
				[0,1,2].wchoose([3, 15, phrases].normalizeSum),//drums
				[0,1,2].wchoose([(15-phrases), 15, phrases].normalizeSum),//bass
				[0,1].choose,				//filter on
				[0,1,2].wchoose([(phrases-4), 15, 2].normalizeSum),//vox
				[0,1].choose, 			//beeps
				[0,1].wchoose([(18-phrases), (phrases-4)].normalizeSum)//arpeggio
			);
		},{
			introFunction.value();
		});
	});
										
	(8*tempo).wait;
};

	("|_______________________|").postln;
}.fork;


//INTRODUCTION FUNCTION
introFunction = {
	
	//choosing positions from vocal sample - these are predefined positions which I know sound good (not in points of silence for example)
	7.do{arg i;
		pos[i] = [0.04, 0.9, 0.5, 0.15, 0.7, 0.8, 0.97, 0.54].choose;
	};
	pos[1] = [0.04, 0.9, 0.7, 0.8].choose;
	
	//8 bars
	{8.do{arg ba;
		var voxReverse;
		bars = ba;
			
			//choose direction for voxFunction
		if(0.7.coin == true,{
			voxReverse = 0;		//play samples forward
		},{
			voxReverse = 1;		//play samples in reverse
		});
			
		//8 beats in a bar
		{8.do{arg bts;	
			beats = bts;	
					
			if(phrases < 10,{	
				keyFunction.value(1, phrases);
			});
					
			c = Group.basicNew(s, 1); //generating group to apply reverb to vocals
				
			cutoff = (30+((((phrases)*8)+bars)*30)); //cutoff increases as phrases and bars progress
				
			voxFunction.value(1,voxReverse);		//play samples in chosen direction

			//reverb quality changes as piece progress
			y = Synth.tail(c, \reverb,[\length, (3-(((phrases*8)+beats)*0.09)), \mix, (0.6-(((phrases*8)+beats)*0.018)), \filter, (300+(phrases*160)+(beats*20))]);
			(tempo/8).wait;			
		};
		}.fork;
		tempo.wait;

	};
	}.fork;
};


//BEAT FUNCTION
beatFunc  = {arg drums = 0, bass = 0, filterOn = 0, vox = 0, beeps = 0, arps = 0;
var voxReverse;


("|" + (drums) + "|" + (bass) + "|" +(filterOn) + "|" +(vox) + "|" +(beeps) + "|" + (arps) + "|").postln;

//choosing positions from vocal sample
7.do{arg i;
	pos[i] = [0.04, 0.9, 0.5, 0.15, 0.7, 0.8, 0.97, 0.54].choose;
};
pos[1] = [0.04, 0.9, 0.7, 0.8].choose;

//allows a certain pattern of kicks+bass noises - chooses a bar in which to place this specific pattern
hits3 = 0.5.coin;
if(hits3 == true,{
	hit3bar = [5,6,7].choose;	
});

//8 bars
{8.do{arg ba;
	var hasSpoken = false;
	bars = ba;
	
	//cutoff increases with phrase progress
	if(filterOn == 1,{
		cutoff = 200*(bars+1)-100;
	},{
		cutoff = 20000;	
	});
	
	b = 0.5.coin;	//a choice that needs to be global - affects multiple instruments
	
	//set up group for a-scramblin'
	numberOfScrambles.do{arg i;
		if(phrases == scramblePhrase[i],{
			if(bars == scrambleBar[i],{
				c = Group.basicNew(s, 1);	
			});
		});
	};
	
	//choosing in what direction to play vocal samples
	if(0.85.coin == true,{
		voxReverse = 0;		//play samples forward
	},{
		voxReverse = 1;		//play samples in reverse
	});
	
	//8 beats in a bar
	{8.do{arg bts;
		beats = bts;
		
		//cutoff increaes with bar progress
		if(filterOn == 1,{
			cutoff=(cutoff+(20));
		});
		
		//instrument functions
		drumFunction.value(drums);
		voxFunction.value(vox, voxReverse);
		bassFunction.value(bass);
		beepFunction.value(beeps);
		arpFunction.value(arps);
		
		//if no bass and no vocals, play spoken word sample
		if(bass == 0,{
			if(vox == 0,{
				spokenFunction.value();
				hasSpoken = true;
			});
		});
		
		//don't play arpeggios if spoken word sample is playing
		if(hasSpoken == false,{
			arpFunction.value(arps);
		});
		
		(tempo/8).wait;
	};	
	
	//apply scramble effect		
	numberOfScrambles.do{arg i;
		if(phrases == scramblePhrase[i],{
			if(bars == scrambleBar[i],{
				Synth.tail(c, \scrambler,[\barLength, tempo]);
			});
		});
	};
	
	}.fork;

	tempo.wait;
};
}.fork;

};




//ARPEGGIO FUNCTION - uses the 'keys' synth
arpFunction = {arg value;
	var ta = 0;
	var arpArray = Array.fill(6);
	var root;
	
	if(value == 1,{
		//root value changes depending on the bar number (different chord for each bar)
		if(bars%4 == 0,{
			root = 62;	
		});
		if(bars%4 == 1,{
			root = 60;	
		});
		if(bars%4 == 2,{
			root = 57;
		});
		if(bars%4 == 3,{
			root = 55;
		});
		//arpeggio is a 6 note array
		arpArray = [72,70,67,70,root,74];
		{	
			//'beat%3=' means that the 6 note array repeats over the 8 beats a bar
			//duration increases with position in bar
			//pan switches for each note
			s.bind(Synth(\keys,[\freq, arpArray[((beats%3)*2)].midicps, \amp,0.05,\pan,1,\dur,((beats+1)*0.05)]));
			(tempo/16).wait;
			s.bind(Synth(\keys,[\freq, arpArray[((beats%3)*2)+1].midicps, \amp,0.05,\pan,-1,\dur,((beats+1)*0.05)]));	
			(tempo/16).wait;	
		}.fork;
	});
};




//KEYS FUNCTION
keyFunction = {arg value, pretendPhrase;
var note, scaledNote;
var ti = 0;
				
{
	if(value == 1,{
		if(beats < 7,{
			if((phrases/10).coin == true,{	
				note = [7,10,12,14,17,19].choose;				//midi note
				scaledNote = (note+(12*(pretendPhrase+1)));		//midi note scaled up octaves depending on input phrase value
				s.bind(Synth(\keys,[\freq, scaledNote.midicps, \amp,0.15,\pan,((2.0.rand)-1),\dur,((5.0.rand)+1.01)]));
			});
							
			if((phrases/10).coin == true,{					//key note for offbeats
				(tempo/16).wait;
				ti = ti + (tempo/16);
				note = [7,10,12,14,17,19].choose;
				scaledNote = (note+(12*(pretendPhrase+1)));	
				s.bind(Synth(\keys,[\freq, scaledNote.midicps, \amp,0.15,\pan,((2.0.rand)-1),\dur,((5.0.rand)+0.01)]));
			});	
		});		
	});
	((tempo/8)-ti).wait;
}.fork;
};




//DRUM FUNCTION
drumFunction = {arg value;
var aGroup, sound, fx;
var td = 0;
{	
	if((beats*2)%3==0,{
		if(value > 0,{
			s.bind(Synth(\sample,[\bufnum, 9, \startPos,0, \dur,1, \pan,[-0.5,0.5].choose])); //Agogo
		});
	});
	
	if(leave == 1,{
		if(beats < 7,{	//ALL BEATS EXCEPT 7
			if(value == 2,{
				s.bind(Synth(\sample,[\bufnum, 9, \startPos,0, \dur,0.2, \pan,0])); //Wood
			});				
		});
	});
	
	if(leave == 1,{	
		if(beats == 0,{
			if(value > 0,{
				if(0.9.coin == true,{	
					s.bind(Synth(\sample,[\bufnum, 3, \startPos,0, \dur,1])); //Kick
				});
				if(0.5.coin == true,{
					if(hits3 == false,{
						td=td+(tempo/16);	//wait half a beat
						(tempo/16).wait;
						s.bind(Synth(\sample,[\bufnum, 3, \startPos,0, \dur,1])); //Kick
					});
				});
			});
		});
			
		if(beats == 1,{
			td=td+(tempo/16);
			(tempo/16).wait;
			if(value > 0,{
				s.bind(Synth(\sample,[\bufnum, 2, \startPos,0, \dur,1])); //Hat
				s.bind(Synth(\sample,[\bufnum, 6, \startPos,0, \dur,1, \pan,-0.5])); //Agogo
				if(bars == hit3bar,{
					if(hits3 == true,{
						s.bind(Synth(\sample,[\bufnum, 3, \startPos,0, \dur,1])); //Kick
					});
				});
			});	
		});			
	});
	
	//BEAT 2
	if(beats == 2,{
		if(value > 0,{
			if(0.5.coin == true,{
				s.bind(Synth(\sample,[\bufnum, 1, \startPos,0, \dur,1])); //Snare
			},{
				aGroup = Group.new(s, 2);
				sound = Synth.head(aGroup, \sample,[\bufnum, 1, \startPos,0, \dur,1]); //Snare
			     fx = Synth.tail(aGroup, \reverb,[\mix, 0.9, \room, 0.7, \damp, 0.1, \amp, 0.3]); //Reverb
			});
		});
	});
	
	//BEAT 3	
	if(beats == 3,{
		if(value == 2,{
			if(bars == 7,{
				s.bind(Synth(\sample,[\bufnum, 10, \startPos,0, \dur,7])); //Reverse Cymbal
			});	
		});
		if(value > 0,{
			if(bars == hit3bar,{
				if(hits3 == true,{
					s.bind(Synth(\sample,[\bufnum, 3, \startPos,0, \dur,1])); //Kick
				});
			});
		});
	});
			
	//BEAT 4	
	if(beats == 4,{
		td=td+(tempo/16);
		(tempo/16).wait;
		if(value > 0,{
			s.bind(Synth(\sample,[\bufnum, 2, \startPos,0, \dur,1])); //Hat
		});
	});	
		
	//BEAT 5			
	if(beats == 5,{
		if(value > 0,{
			if(0.5.coin == true,{
				if(0.4.coin == true,{
					s.bind(Synth(\sample,[\bufnum, 4, \startPos,0, \dur,1]));  //Gadget
				},{
					s.bind(Synth(\sample,[\bufnum, 11, \startPos,0.2, \dur,1]));  //Gadget
				});
			},{
				aGroup = Group.new(s, 2);
				sound = Synth.head(aGroup, \sample,[\bufnum, 4, \startPos,0, \dur,1]); //Snare
			     fx = Synth.tail(aGroup, \reverb,[\mix, 0.9, \room, 0.7, \damp, 0.1, \amp, 0.5]); //Reverb
			});
		});
	});	
			
	//BEAT 6	
	if(beats == 6,{	
		if(value > 0,{
			if(bars < 6,{						
				s.bind(Synth(\sample,[\bufnum, 2, \startPos,0, \dur,1]));//Hat
			},{
				td=td+(tempo/16);
				(tempo/16).wait;
				s.bind(Synth(\sample,[\bufnum, 2, \startPos,0, \dur,1]));  //Hat
			});
					
			if(bars > 3,{
				td=td+(tempo/16);
				(tempo/16).wait;
				s.bind(Synth(\sample,[\bufnum, 3, \startPos,0, \dur,1]));//Kick
			});
		});
	});
	
	//BEAT 7		
	if(beats == 7,{
				
	});	
						
	//ON ALL BEATS	
	if(value == 2,{
		s.bind(Synth(\sample,[\bufnum, 9, \startPos,0, \dur,0.2, \pan,0])); //Wood				
	});		
	
	((tempo/16)-td).wait;	
}.fork;
};




//VOX FUNCTION
voxFunction = {arg vox, rev;
var tv = 0;
var position = ((100.0.rand)/100);
var grains;
var randPos;
{
	if(beats < 7,{
		if(vox == 1,{
			if(leave == 0,{
				if(beats > 2,{
					s.bind(Synth(\sample,[\bufnum, 0, \startPos,pos[1], \dur,0.2, \cutoff, cutoff, \reverse, rev;]));	
				});
			},{
				s.bind(Synth(\sample,[\bufnum, 0, \startPos,pos[1], \dur,0.2, \cutoff, cutoff, \reverse, rev;]));		
			});
		});
		
		if(vox == 2,{		//-----------VOX=2 HERE----------\\
			{	
				position = (position+0.002);	//gradually moves through sample
				grains = (4.rand);				//pick a number of grains/speed
				3.do{arg n;
					n=n+1;
					if(grains == n,{
						((2**grains)*2).do{		//play number of grains
							s.bind(Synth(\sample,[\bufnum, 0, \startPos,position, \dur,(tempo/(2**(grains+3))), \cutoff, cutoff, \reverse, rev;]));
							(tempo/(2**(grains+3))).wait;      //wait time scales according to number of grains - few grains means longer wait. will always add up to one beat
						};			
					});
				};				
			}.fork;
		});		
	});
			
	//BEAT 0
	if(beats == 0,{
		if(leave == 1,{	
			if(0.8.coin == true,{
				if(vox == 1,{
					s.bind(Synth(\sample,[\bufnum, 0, \startPos,pos[2], \dur,0.4, \pan,1, \cutoff, cutoff, \reverse, rev;]));	
				});
			},{
				if(vox == 1,{
					tv=tv+(tempo/16);
					(tempo/16).wait;
					s.bind(Synth(\sample,[\bufnum, 0, \startPos,pos[2]+0.03, \dur,0.1, \pan,1, \cutoff, cutoff, \reverse, rev;]));	
				});
			});
		});			
	});
	
	//BEAT 2
	if(beats == 2,{	
		if(vox == 1,{
			if(0.5.coin == true,{	
				s.bind(Synth(\sample,[\bufnum, 0, \startPos,pos[3], \pan,-1, \cutoff, cutoff, \reverse, rev;]));	
			},{
				s.bind(Synth(\sample,[\bufnum, 0, \startPos,pos[3]+0.02, \pan,-1, \cutoff, cutoff, \reverse, rev;]));	
			});
		});
									
		tv=tv+(tempo/16);
		(tempo/16).wait;
				
		if(vox == 1,{
			if(b == true,{
				s.bind(Synth(\sample,[\bufnum, 0, \startPos, pos[7] , \dur,0.2, \cutoff, cutoff, \reverse, rev;]));	  	
			});
		});
	});	
			
	//BEAT 3	
	if(beats == 3,{
		if(vox == 1,{
			s.bind(Synth(\sample,[\bufnum, 0, \startPos,0, \dur,0.4, \cutoff, cutoff, \reverse, rev;]));	
		});
	});
			
	//BEAT 4	

	//BEAT 5			
	if(beats == 5,{
		if(vox == 1,{
			s.bind(Synth(\sample,[\bufnum, 0, \startPos,pos[6], \pan,-1, \cutoff, cutoff, \reverse, rev;]));	
		});		
	});	
				
	//BEAT 6	
	if(beats == 6,{
		if(vox == 1,{
			if(0.5.coin == true,{
				s.bind(Synth(\sample,[\bufnum, 0, \startPos,pos[4], \pan,1, \cutoff, cutoff, \reverse, rev;]));	
			},{
				tv=tv+(tempo/16);
				(tempo/16).wait;
				s.bind(Synth(\sample,[\bufnum, 0, \startPos,pos[5], \dur,0.2, \pan,1, \cutoff, cutoff, \reverse, rev;]));					
			});	
		});			
	});
			
	//BEAT 7		
	
	//OTHERS			
	if(b == false,{
		if(vox == 1,{
			if(beats%3 == 0,{
				tv=tv+(tempo/16);
				(tempo/16).wait;
				s.bind(Synth(\sample,[\bufnum, 0, \startPos, 0.04 , \dur,0.2, \cutoff, cutoff, \reverse, rev;]));
			},{
				if(0.3.coin == true,{	//short vocal 'stutter'
					randPos = 10.rand/10;
					tv=tv+(tempo/16);
					4.do{
						(tempo/64).wait;
						s.bind(Synth(\sample,[\bufnum, 0, \startPos, randPos , \dur,0.05, \cutoff, cutoff, \reverse, rev;]));	
					};
				});	
			});
		});
	});			
				
	//ON ALL (mostly off) BEATS	
	if(vox == 1,{
		s.bind(Synth(\sample,[\bufnum, 0, \startPos,pos[1]+0.02, \dur,0.1, \cutoff, cutoff, \reverse, rev;]));	
	});
	((tempo/16)-tv).wait;		
}.fork;
};


//BASS FUNCTION
bassFunction = {arg bass;
var tb = 0;
{
	if(leave == 1,{
		
		if(beats < 7,{
			if(bass == 1,{
				//donk-donk-donk bass
				s.bind(Synth(\dropbass,[\freq, 392,\amp,0.2,\pan,0,\dur,0.1]));
			});
		});
		
		if(beats == 0,{
			if(bars > 3,{
				//dropping-pitch bass
				if(bars == hit3bar,{
					if(hits3 == true,{
						s.bind(Synth(\dropbass,[\freq, 40+((5.rand)*10),\amp,0.6,\pan,-1,\dur,1]));
					},{
						s.bind(Synth(\dropbass,[\freq, 40+((5.rand)*10),\amp,0.6,\pan,0,\dur,4]));
					});
				},{
					s.bind(Synth(\dropbass,[\freq, 40+((5.rand)*10),\amp,0.6,\pan,0,\dur,4]));
				});
			});
		});
	
		if(beats == 1,{
			if(bass == 2,{
				//melodic bass #1
				s.bind(Synth(\melodybass,[\freq, 146.83,\amp,0.4,\pan,0,\dur,0.2]));
			});
				
			tb=tb+(tempo/16);
			(tempo/16).wait;
			
			if(hits3 == true,{
				if(bars == hit3bar,{
					//dropping-pitch bass (on off beat)
					s.bind(Synth(\dropbass,[\freq, 40+((5.rand)*10),\amp,0.6,\pan,0,\dur,1]));
				});
			});	
			if(bass == 2,{
				if(bars > 3,{
					//melodic bass #1.5
					s.bind(Synth(\melodybass,[\freq, 293.66,\amp,0.4,\pan,0,\dur,0.2]));
				});	
			});
		});
	

	},{
		if(beats > 2,{
			if(beats < 7,{
				if(bass == 1,{
					//dropping pitch bass
					s.bind(Synth(\dropbass,[\freq, 392,\amp,0.2,\pan,0,\dur,0.1]));
				});
			});
		});
	});			

	//BEAT 2
	if(beats == 2,{
		if(bass == 2,{
			//melodic bass #2
			s.bind(Synth(\melodybass,[\freq, 130.81,\amp,0.4,\pan,0,\dur,0.2]));
		});
	});
			
	//BEAT 3	
	if(beats == 3,{
		if(bass == 2,{
			//melodic bass #3
			if(0.8.coin == true,{
				s.bind(Synth(\melodybass,[\freq, 98,\amp,0.4,\pan,0,\dur,0.2]));
			},{
				s.bind(Synth(\melodybass,[\freq, 146.83,\amp,0.4,\pan,0,\dur,0.2]));
			});		
		});	
		
		if(hits3 == true,{
			if(bars == hit3bar,{
				if(leave == 1,{
					//dropping-pitch bass
					s.bind(Synth(\dropbass,[\freq, 40+((5.rand)*10),\amp,0.6,\pan,1,\dur,3]));
				});
			});
		});
	});
			
	//nothing for beat 4-7
		
	((tempo/16)-tb).wait;	
}.fork;
};



//SPOKEN-WORD FUNCTION - moves through a sample in tiny 'clips' with varying cutoff
spokenFunction = {
var ts = 0;
{
	keyFunction.value(1, 3);
	s.bind(Synth(\sample,[\bufnum, 8, \startPos,0.0+(bars*0.06)+(beats*0.01), \amp,0.4, \dur,0.5, \pan,(-1+(0.2*bars)), \cutoff,(2000-(bars*200))]));
	(tempo/16).wait;
	ts = ts + (tempo/16);		//opposite pan on offbeats
	s.bind(Synth(\sample,[\bufnum, 8, \startPos,0.0+(bars*0.08)+(beats*0.02), \amp,0.4, \dur,0.5, \pan,(1-(0.2*bars)), \cutoff,(1000-(bars*100))]));
	((tempo/16)-ts).wait;
}.fork;
};



//BEEP FUNCTION
beepFunction = {arg beeps;
var tbeep = 0;
{	
	if(0.5.coin == true,{
		if(beats != 2,{
			if(0.8.coin == true,{
				tbeep=tbeep+(tempo/16);	//take off 16th of wait time
				(tempo/16).wait;
				s.bind(Synth(\blip,[\freq, [880, 784, 440, 392].choose,\amp,0.2,\pan,((2.0.rand)-1.0),\dur,(5.rand*0.01)]));	
			},{
				tbeep=tbeep+(tempo/16);		//'stutter'
				(tempo/32).wait;
				s.bind(Synth(\blip,[\freq, [880, 784, 440, 392].choose,\amp,0.3,\pan,((2.0.rand)-1.0),\dur,(10.rand*0.01)]));
				(tempo/32).wait;
				s.bind(Synth(\blip,[\freq, [880, 784, 440, 392].choose,\amp,0.2,\pan,((2.0.rand)-1.0),\dur,0.03]));		
			});
		});
	});
		
	((tempo/16)-tbeep).wait;
	
}.fork;
};

}
	
}