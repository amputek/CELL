Friendly {

	var pan;
	var distance;
	var born;

	*new {arg p, d;
		^super.new.initFriendly(p, d);
	}

	initFriendly{arg p, d;
		pan = p;
		distance = d;
		born = false;
	}

	pan{
		^pan;
	}

	distance{
		^distance;
	}

	born{
		^born;
	}

	setPan{arg p;
		pan = p;
	}

	setDistance{arg d;
		distance = d;
	}

	setBorn{
		born = true;
	}

}