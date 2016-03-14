Spark {

	var beatNumber;
	var frequency;
	var type;

	*new {arg b, f, t;
		^super.new.initSpark(b, f, t);
	}

	initSpark{arg b, f, t;
		beatNumber = b;
		frequency = f;
		type = t;
	}

	beatNumber{
		^beatNumber;
	}

	frequency{
		^frequency;
	}

	type{
		^type;
	}

	setFrequency{arg f;
		frequency = f;
	}

}