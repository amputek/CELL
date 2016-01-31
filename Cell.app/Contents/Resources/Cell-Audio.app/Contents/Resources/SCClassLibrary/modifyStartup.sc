+ OSXPlatform {

	startupFiles {

	}

	startup {
		Document.implementationClass.startup;
		Document.closeAll(false);
	}
}

+ Main {
	startup {
		platform = this.platformClass.new;
		platform.initPlatform;

		super.startup;

		Server.default = Server.internal;
		interpreter.s = Server.default;

		this.platform.startup;

		Server.default.waitForBoot({
			SetUpCell.new(interpreter.s);
			3.0.wait;
			(String.scDir ++ "/cell.scd").load;
		});
	}

}
