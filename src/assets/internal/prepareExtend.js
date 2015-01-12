var __extends_ts = function (d, b) {
    for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p];
    function __() { this.constructor = d; }
    __.prototype = b.prototype;
    d.prototype = new __();
};

var __extends = function(Child, Parent) {
	
	if (Parent.extends)	{
		if	(Parent.__extended) {
			throw new Error("Can not extend an already extended native object.");
		}
	
		function extend(child, parent) {
	        
			if (!child.__extended) {
	        	child.__extended = parent.extends(child.prototype);
	        }
	 
	        return child.__extended;
	    };
	    
		Parent.call = function(thiz) {
			var Extended = extend(thiz.__proto__.__child, thiz.__proto__.__parent);
			if (arguments.length > 1)
			{
				thiz.__proto__ = new (Function.prototype.bind.apply(Extended, [null].concat(Array.prototype.slice.call(arguments, 1))));
			}
			else
			{
				thiz.__proto__ = new Extended();
			}
		};
		
		Parent.apply = function(thiz, args) {
			var Extended = extend(thiz.__proto__.__child, thiz.__proto__.__parent);
			if (args && args.length > 0)
			{
				thiz.__proto__ = new (Function.prototype.bind.apply(Extended, [null].concat(args)));
			}
			else
			{
				thiz.__proto__ = new Extended();
			}
		};
	}
	
	__extends_ts(Child, Parent);
	
	
	if (Parent.extends)	{
		Child.__isPrototypeImplementationObject = true;
		Child.__proto__ = Parent;
		Child.prototype.__parent = Parent;
		Child.prototype.__child = Child;
	}
}

global.__extends = __extends;
