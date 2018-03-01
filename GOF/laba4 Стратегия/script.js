// class StrategyBase 
// {
// 	Execute() : string
// 	{ 
// 		throw "implementation required";
// 	}
// }
var __extends = (this && this.__extends) || (function () {
    var extendStatics = Object.setPrototypeOf ||
        ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
        function (d, b) { for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p]; };
    return function (d, b) {
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
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
var FlyBehavior = /** @class */ (function () {
    function FlyBehavior() {
    }
    FlyBehavior.prototype.fly = function () {
        //console.log("FlyBehavior.fly");
        //return "";
    };
    ;
    return FlyBehavior;
}());
;
var FlyWithWings = /** @class */ (function (_super) {
    __extends(FlyWithWings, _super);
    function FlyWithWings() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    FlyWithWings.prototype.fly = function () {
        //console.log("FlyWithWings.fly");
        return "\n I fly! OMG, I can fly, I did it!! ";
    };
    ;
    return FlyWithWings;
}(FlyBehavior));
;
var FlyNoWay = /** @class */ (function (_super) {
    __extends(FlyNoWay, _super);
    function FlyNoWay() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    FlyNoWay.prototype.fly = function () {
        // console.log("FlyNoWay.fly");
        return "\n I no fly ";
    };
    ;
    return FlyNoWay;
}(FlyBehavior));
;
var QuackBehavior = /** @class */ (function () {
    function QuackBehavior() {
    }
    QuackBehavior.prototype.quack = function () {
        return "";
    };
    ;
    return QuackBehavior;
}());
;
var Quack = /** @class */ (function (_super) {
    __extends(Quack, _super);
    function Quack() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    Quack.prototype.quack = function () {
        return "\n I Quack ";
    };
    ;
    return Quack;
}(QuackBehavior));
;
var Squack = /** @class */ (function (_super) {
    __extends(Squack, _super);
    function Squack() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    Squack.prototype.quack = function () {
        return "\n I pishu ";
    };
    ;
    return Squack;
}(QuackBehavior));
;
var MuteQuack = /** @class */ (function (_super) {
    __extends(MuteQuack, _super);
    function MuteQuack() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    MuteQuack.prototype.quack = function () {
        return "\n I no Quack ";
    };
    ;
    return MuteQuack;
}(QuackBehavior));
;
var Duck = /** @class */ (function () {
    function Duck(id, pFly, pQuack) {
        // console.log("Duck constructor");
        if (id) {
            this.id = id;
            var div = document.createElement('div');
            div.setAttribute("id", this.id);
            var name_1 = document.createElement("div");
            name_1.setAttribute("id", this.id + "_name");
            name_1.setAttribute("class", "name");
            name_1.innerText = "name: " + this.id;
            var flyBehavior = document.createElement("div");
            var quackBehavior = document.createElement("div");
            flyBehavior.setAttribute('id', this.id + '_flyBehavior');
            quackBehavior.setAttribute('id', this.id + '_quackBehavior');
            flyBehavior.setAttribute("class", "FlyBehavior");
            quackBehavior.setAttribute("class", "QuackBehavior");
            var flyButton_1 = document.createElement('button');
            var dontFlyButton_1 = document.createElement('button');
            var quackButton_1 = document.createElement('button');
            var squackButton_1 = document.createElement('button');
            var muteQuackButton_1 = document.createElement('button');
            flyButton_1.setAttribute('id', this.id + '_flyButton');
            dontFlyButton_1.setAttribute('id', this.id + '_dontFlyButton');
            quackButton_1.setAttribute('id', this.id + '_quackButton');
            squackButton_1.setAttribute('id', this.id + '_squackButton');
            muteQuackButton_1.setAttribute('id', this.id + '_muteQuackButton');
            flyButton_1.setAttribute('class', "flyButton");
            dontFlyButton_1.setAttribute('class', "dontFlyButton");
            quackButton_1.setAttribute('class', "quackButton");
            squackButton_1.setAttribute('class', "squackButton");
            muteQuackButton_1.setAttribute('class', "muteQuackButton");
            flyButton_1.innerText = "Fly, duck, fly!";
            dontFlyButton_1.innerText = "Don't fly.";
            quackButton_1.innerText = "quack.";
            squackButton_1.innerText = "squack.";
            muteQuackButton_1.innerText = "mute Quack.";
            var br1 = document.createElement("br");
            var br2 = document.createElement("br");
            var br3 = document.createElement("br");
            var duck_1 = this;
            flyButton_1.addEventListener("click", function () {
                // console.log("this=", this);
                // console.log("duck=", duck);
                duck_1.SetFlyBehavior(new FlyWithWings);
                var id = duck_1.id + "_flyBehavior";
                // console.log("id=", id);
                document.getElementById(duck_1.id + "_flyBehavior").innerText = duck_1.PerformFly();
            });
            dontFlyButton_1.addEventListener("click", function (event) {
                duck_1.SetFlyBehavior(new FlyNoWay);
                // console.log("duck=", duck);
                // console.log("duck.PerformFly()=", duck.PerformFly());
                document.getElementById(duck_1.id + "_flyBehavior").innerText = duck_1.PerformFly();
            });
            quackButton_1.addEventListener("click", function (event) {
                duck_1.SetQuackBehavior(new Quack);
                document.getElementById(duck_1.id + "_quackBehavior").innerText = duck_1.PerformQuack();
            });
            squackButton_1.addEventListener("click", function (event) {
                duck_1.SetQuackBehavior(new Squack);
                document.getElementById(duck_1.id + "_quackBehavior").innerText = duck_1.PerformQuack();
            });
            muteQuackButton_1.addEventListener("click", function (event) {
                duck_1.SetQuackBehavior(new MuteQuack);
                document.getElementById(duck_1.id + "_quackBehavior").innerText = duck_1.PerformQuack();
            });
            div.appendChild(name_1);
            div.appendChild(flyBehavior);
            div.appendChild(quackBehavior);
            div.appendChild(flyButton_1);
            div.appendChild(dontFlyButton_1);
            div.appendChild(br1);
            div.appendChild(quackButton_1);
            div.appendChild(squackButton_1);
            div.appendChild(muteQuackButton_1);
            div.appendChild(br2);
            div.appendChild(br3);
            document.getElementById("stuff").appendChild(div);
        }
        else
            this.id = "";
        if (pFly) {
            this.pFly = pFly;
            this.PerformFly = this.pFly.fly;
            document.getElementById(this.id + "_flyBehavior").innerText = this.PerformFly();
        }
        else
            this.PerformFly = (new FlyBehavior).fly;
        if (pQuack) {
            this.pQuack = pQuack;
            this.PerformQuack = this.pQuack.quack;
            document.getElementById(this.id + "_quackBehavior").innerText = this.PerformQuack();
        }
    }
    ;
    //public fly: any;
    Duck.prototype.SetFlyBehavior = function (f) {
        // console.log("f=", f);
        //console.log("Duck=", this);
        this.pFly = f;
        this.PerformFly = this.pFly.fly;
        console.log("Duck=", this);
        // console.log("this.pFly=", this.pFly);
        return f;
    };
    ;
    Duck.prototype.PerformFly = function () {
        // console.log("PerformFly, this.pFly=", this.pFly);
        // console.log("PerformFly, this.pFly.fly()=", this.pFly.fly());
        return this.pFly.fly();
    };
    Duck.prototype.SetQuackBehavior = function (qu) {
        this.pQuack = qu;
        this.PerformQuack = this.pQuack.quack;
        console.log("Duck=", this);
        return qu;
    };
    ;
    Duck.prototype.PerformQuack = function () {
        return this.pQuack.quack();
    };
    ;
    return Duck;
}());
;
var RubberDuck = /** @class */ (function (_super) {
    __extends(RubberDuck, _super);
    function RubberDuck(id, pFly, pQuack) {
        // console.log("pFly=", pFly);
        return _super.call(this, id = id, pFly = pFly, pQuack = pQuack) || this;
        // if (pFly) {
        // 	this.SetFlyBehavior(pFly);
        // }
    }
    return RubberDuck;
}(Duck));
;
var DecoyDuck = /** @class */ (function (_super) {
    __extends(DecoyDuck, _super);
    function DecoyDuck(id, pFly, pQuack) {
        // console.log("pFly=", pFly);
        return _super.call(this, id = id, pFly = pFly, pQuack = pQuack) || this;
        // if (pFly) {
        // 	this.SetFlyBehavior(pFly);
        // }
    }
    return DecoyDuck;
}(Duck));
;
var Rd = new RubberDuck("Rd", new FlyNoWay);
console.log(Rd);
var Dd = new DecoyDuck("Dd", new FlyWithWings, new Quack);
//document.getElementById("flying").innerText = Rd.PerformFly();
//Rd.SetFlyBehavior(new FlyBehavior);
//document.getElementById("stuff").innerText = Rd.PerformFly();
//Rd.SetFlyBehavior(new FlyWithWings);
//document.getElementById("stuff").innerText = Rd.PerformFly();
var flyButton = document.getElementById("flyButton");
var dontFlyButton = document.getElementById("dontFlyButton");
var quackButton = document.getElementById("quackButton");
var squackButton = document.getElementById("squackButton");
var muteQuackButton = document.getElementById("muteQuackButton");
flyButton.addEventListener("click", function (event) {
    Rd.SetFlyBehavior(new FlyWithWings);
    console.log("Rd=", Rd);
    console.log("Rd.PerformFly()=", Rd.PerformFly());
    document.getElementById("flying").innerText = Rd.PerformFly();
});
dontFlyButton.addEventListener("click", function (event) {
    Rd.SetFlyBehavior(new FlyNoWay);
    console.log("Rd=", Rd);
    console.log("Rd.PerformFly()=", Rd.PerformFly());
    document.getElementById("flying").innerText = Rd.PerformFly();
});
quackButton.addEventListener("click", function (event) {
    Rd.SetQuackBehavior(new Quack);
    document.getElementById("quacking").innerText = Rd.PerformQuack();
});
squackButton.addEventListener("click", function (event) {
    Rd.SetQuackBehavior(new Squack);
    document.getElementById("quacking").innerText = Rd.PerformQuack();
});
muteQuackButton.addEventListener("click", function (event) {
    Rd.SetQuackBehavior(new MuteQuack);
    document.getElementById("quacking").innerText = Rd.PerformQuack();
});
//# sourceMappingURL=script.js.map