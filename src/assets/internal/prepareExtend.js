var __extends_ts = function (d, b) {
    for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p];
    function __() { this.constructor = d; }
    __.prototype = b.prototype;
    d.prototype = new __();
};

var __extends = function(Child, Parent) {
	
	if (Parent.extend)	{
		if	(Parent.__isPrototypeImplementationObject) {
			throw new Error("Can not extend an already extended native object.");
		}
	
		function replace(func, child) {
			var tmp = child.prototype[func];
			child.prototype[func] = function() 
			{
				var that = this.__that;
				
				if (that)
				{
					return tmp.apply(that, arguments);
				}
				else
				{
					return tmp.apply(this, arguments);
				}
			}
		}
		
		function extend(child, parent) {
	        
			if (!child.__extended) {
	        	child.__extended = parent.extend(child.prototype);
	        	
	        	for (var f in child.prototype)
				{
	        		if (child.prototype.hasOwnProperty(f))
					{
						if (typeof(child.prototype[f]) === 'function' && f !== 'constructor' && f !== '__parent' && f !== '__child')
						{
							replace(f, child);
						}
					}
				}
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
			
			thiz.__proto__.__that = thiz;
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
			
			thiz.__proto__.__that = thiz;
		};
	}
	
	__extends_ts(Child, Parent);
	
	
	if (Parent.extend)	{
		Child.__isPrototypeImplementationObject = true;
		Child.__proto__ = Parent;
		Child.prototype.__parent = Parent;
		Child.prototype.__child = Child;
	}
}

global.__extends = __extends;
