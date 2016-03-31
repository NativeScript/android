(function() {
	var __extends_ts = function (d, b) {
	    for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p];
	    function __() { this.constructor = d; }
	    __.prototype = b.prototype;
	    d.prototype = new __();
	};
	
	var __decorate = (this && this.__decorate) || function (decorators, target, key, desc) {
	    if (typeof Reflect === "object" && typeof Reflect.decorate === "function") return Reflect.decorate(decorators, target, key, desc);
	    switch (arguments.length) {
	        case 2: return decorators.reduceRight(function(o, d) { return (d && d(o)) || o; }, target);
	        case 3: return decorators.reduceRight(function(o, d) { return (d && d(target, key)), void 0; }, void 0);
	        case 4: return decorators.reduceRight(function(o, d) { return (d && d(target, key, o)) || o; }, desc);
	    }
	};
	
	var __native = function(thiz) {
		var result = thiz.__proto__;
	
		for (var prop in thiz) 
		{
			if (thiz.hasOwnProperty(prop))
			{
				thiz.__proto__[prop] = thiz[prop];
				delete thiz[prop];
			}
		}
		
		thiz.constructor = undefined;
		thiz.__proto__ = undefined;
		Object.freeze(thiz);
		Object.preventExtensions(thiz)
		return result;
	};
	
	var __extends = function(Child, Parent) {
		
		if (Parent.extend)	{
			if	(Parent.__isPrototypeImplementationObject) {
				throw new Error("Can not extend an already extended native object.");
			}
			
			function extend(child, parent) {
				__log("TS extend called");
				if (!child.__extended) {
		        	child.__extended = parent.extend(child.name, child.prototype);
		        }
		 
		        return child.__extended;
		    };
		    
		    Parent.__activityExtend = function(parent, name, implementationObject) {
		    	__log("__activityExtend called");
		    	return parent.extend(name, implementationObject);
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
		
		if (Parent.extend)	{
			Child.__isPrototypeImplementationObject = true;
			Child.__proto__ = Parent;
			Child.prototype.__parent = Parent;
			Child.prototype.__child = Child;
		}
	}
	
	function JavaProxy(className) {
	    return function (target) {
	    	var extended = target.extend(className, target.prototype)
	    	extended.name = className;
	    	return extended;
	    };
	}
	
	global.__native = __native;
	global.__extends = __extends;
	global.__decorate = __decorate;
	global.JavaProxy = JavaProxy;
})()