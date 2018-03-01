

// class StrategyBase 
// {
// 	Execute() : string
// 	{ 
// 		throw "implementation required";
// 	}
// }

// class Strategy1 extends StrategyBase
// {
// 	Execute() : string
// 	{ 
// 		return "Imp 1"; 
// 	}
// }

// class Strategy2 extends StrategyBase
// {
// 	Execute() : string
// 	{ 
// 		return "Imp 2rtyrtyr"; 
// 	}
// }

// class Program
// {
// 	static Main()
// 	{
// 		Program.doStuff(new Strategy2(), new RubberDuck());
		
// 	}
// 	Rd = new RubberDuck;

// 	static doStuff(strat : StrategyBase, Rd: RubberDuck)
// 	{
// 		document.getElementById("stuff").innerText = strat.Execute();
// 	}
// }

//Program.Main();

class FlyBehavior {
	public fly() : any{
		//console.log("FlyBehavior.fly");
		//return "";
	};
};
class FlyWithWings extends FlyBehavior {
	public fly(): string { 
		//console.log("FlyWithWings.fly");
		return "\n I fly! OMG, I can fly, I did it!! ";
	};
};
class FlyNoWay extends FlyBehavior {
	public fly(): string { 
		// console.log("FlyNoWay.fly");
		return "\n I no fly ";
	};
};

class QuackBehavior {
	public quack() : any{
		return "";
	};
};
class Quack extends QuackBehavior {
	public quack(): any { 
		return "\n I Quack ";
	};
};
class Squack extends QuackBehavior {
	public quack(): any { 
		return "\n I pishu ";
	};
};
class MuteQuack extends QuackBehavior {
	public quack(): any { 
		return "\n I no Quack ";
	};
};

class Duck {
	id: string;
	pFly: FlyBehavior;
	pQuack: QuackBehavior;
	constructor()
	constructor(id?: string, pFly?: FlyBehavior, pQuack?: QuackBehavior)
	constructor(id?: string, pFly?: FlyBehavior, pQuack?: QuackBehavior) {
		// console.log("Duck constructor");
		if (id) {
			this.id = id;
			let div = document.createElement('div');
			div.setAttribute("id", this.id);
			let name = document.createElement("div");
			name.setAttribute("id", this.id+"_name");
			name.setAttribute("class", "name");
			name.innerText = "name: "+this.id;
			let flyBehavior = document.createElement("div");
			let quackBehavior = document.createElement("div");
			flyBehavior.setAttribute('id', this.id+'_flyBehavior');
			quackBehavior.setAttribute('id', this.id+'_quackBehavior');
			flyBehavior.setAttribute("class", "FlyBehavior");
			quackBehavior.setAttribute("class", "QuackBehavior");
			let flyButton = document.createElement('button');
			let dontFlyButton = document.createElement('button');
			let quackButton = document.createElement('button');
			let squackButton = document.createElement('button');
			let muteQuackButton = document.createElement('button');
			flyButton.setAttribute('id', this.id+'_flyButton');
			dontFlyButton.setAttribute('id', this.id+'_dontFlyButton');
			quackButton.setAttribute('id', this.id+'_quackButton');
			squackButton.setAttribute('id', this.id+'_squackButton');
			muteQuackButton.setAttribute('id', this.id+'_muteQuackButton');
			flyButton.setAttribute('class', "flyButton");
			dontFlyButton.setAttribute('class', "dontFlyButton");
			quackButton.setAttribute('class', "quackButton");
			squackButton.setAttribute('class', "squackButton");
			muteQuackButton.setAttribute('class', "muteQuackButton");
			flyButton.innerText = "Fly, duck, fly!";
			dontFlyButton.innerText = "Don't fly.";
			quackButton.innerText = "quack.";
			squackButton.innerText = "squack.";
			muteQuackButton.innerText = "mute Quack.";
			let br1 = document.createElement("br");
			let br2 = document.createElement("br");
			let br3 = document.createElement("br");
			let duck: Duck = this;
			flyButton.addEventListener("click", function() {
				// console.log("this=", this);
				// console.log("duck=", duck);
				duck.SetFlyBehavior(new FlyWithWings);
				let id = duck.id+"_flyBehavior";
				// console.log("id=", id);
				document.getElementById(duck.id+"_flyBehavior").innerText = duck.PerformFly();
			});
			dontFlyButton.addEventListener("click", function(event) {    
				duck.SetFlyBehavior(new FlyNoWay);
				// console.log("duck=", duck);
				// console.log("duck.PerformFly()=", duck.PerformFly());
				document.getElementById(duck.id+"_flyBehavior").innerText = duck.PerformFly();
			});
			quackButton.addEventListener("click", function(event) {
				duck.SetQuackBehavior(new Quack);	
				document.getElementById(duck.id+"_quackBehavior").innerText = duck.PerformQuack();
			});
			squackButton.addEventListener("click", function(event) {
				duck.SetQuackBehavior(new Squack);	
				document.getElementById(duck.id+"_quackBehavior").innerText = duck.PerformQuack();
			});
			muteQuackButton.addEventListener("click", function(event) {
				duck.SetQuackBehavior(new MuteQuack);	
				document.getElementById(duck.id+"_quackBehavior").innerText = duck.PerformQuack();
			});

			div.appendChild(name);
			div.appendChild(flyBehavior);
			div.appendChild(quackBehavior);
			div.appendChild(flyButton);
			div.appendChild(dontFlyButton);
			div.appendChild(br1);
			div.appendChild(quackButton);
			div.appendChild(squackButton);
			div.appendChild(muteQuackButton);
			div.appendChild(br2);
			div.appendChild(br3);
			document.getElementById("stuff").appendChild(div);
		}
		else this.id = "";
		if (pFly) {
			this.pFly = pFly;
			this.PerformFly = this.pFly.fly;
			document.getElementById(this.id+"_flyBehavior").innerText = this.PerformFly();
		} else this.PerformFly = (new FlyBehavior).fly;
		if (pQuack) {
			this.pQuack = pQuack;
			this.PerformQuack = this.pQuack.quack;
			document.getElementById(this.id+"_quackBehavior").innerText = this.PerformQuack();
		}
	};
	//public fly: any;
	public SetFlyBehavior(f: FlyBehavior): any {
		// console.log("f=", f);
		//console.log("Duck=", this);
		this.pFly = f;
		this.PerformFly = this.pFly.fly;
		console.log("Duck=", this);
		// console.log("this.pFly=", this.pFly);
        return f;
    };
	public PerformFly(): any {
		// console.log("PerformFly, this.pFly=", this.pFly);
		// console.log("PerformFly, this.pFly.fly()=", this.pFly.fly());
		return this.pFly.fly();
	}

	
	public SetQuackBehavior(qu: QuackBehavior): any {
		this.pQuack = qu;
		this.PerformQuack = this.pQuack.quack;
		console.log("Duck=", this);
        return qu;
    };
	public PerformQuack(): any {
		return this.pQuack.quack();
	};
};
class RubberDuck extends Duck {
	constructor(id?: string, pFly?: FlyBehavior, pQuack?: QuackBehavior) {
		// console.log("pFly=", pFly);
		super(id=id , pFly=pFly, pQuack=pQuack);
		// if (pFly) {
		// 	this.SetFlyBehavior(pFly);
		// }
	}
};
class DecoyDuck extends Duck {
	constructor(id?: string, pFly?: FlyBehavior, pQuack?: QuackBehavior) {
		// console.log("pFly=", pFly);
		super(id=id , pFly=pFly, pQuack=pQuack);
		// if (pFly) {
		// 	this.SetFlyBehavior(pFly);
		// }
	}
};
var Rd = new RubberDuck("Rd", new FlyNoWay);
console.log(Rd);
var Dd = new DecoyDuck("Dd", new FlyWithWings, new Quack);
//document.getElementById("flying").innerText = Rd.PerformFly();
//Rd.SetFlyBehavior(new FlyBehavior);
//document.getElementById("stuff").innerText = Rd.PerformFly();
//Rd.SetFlyBehavior(new FlyWithWings);
//document.getElementById("stuff").innerText = Rd.PerformFly();
let flyButton = document.getElementById("flyButton");
let dontFlyButton = document.getElementById("dontFlyButton");
let quackButton = document.getElementById("quackButton");
let squackButton = document.getElementById("squackButton");
let muteQuackButton = document.getElementById("muteQuackButton");
flyButton.addEventListener("click", function(event) {    
	Rd.SetFlyBehavior(new FlyWithWings);
	console.log("Rd=", Rd);
	console.log("Rd.PerformFly()=", Rd.PerformFly());
    document.getElementById("flying").innerText = Rd.PerformFly();
});
dontFlyButton.addEventListener("click", function(event) {    
    Rd.SetFlyBehavior(new FlyNoWay);
	console.log("Rd=", Rd);
	console.log("Rd.PerformFly()=", Rd.PerformFly());
    document.getElementById("flying").innerText = Rd.PerformFly();
});
quackButton.addEventListener("click", function(event) {
	Rd.SetQuackBehavior(new Quack);	
    document.getElementById("quacking").innerText = Rd.PerformQuack();
});
squackButton.addEventListener("click", function(event) {
	Rd.SetQuackBehavior(new Squack);	
    document.getElementById("quacking").innerText = Rd.PerformQuack();
});
muteQuackButton.addEventListener("click", function(event) {
	Rd.SetQuackBehavior(new MuteQuack);	
    document.getElementById("quacking").innerText = Rd.PerformQuack();
});