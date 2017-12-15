(function() {
	var __extends_ts = function (d, b) {
		if (!b.extend) {
			for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p];
		}

	    function __() { this.constructor = d; }
	    __.prototype = b.prototype;
	    d.prototype = new __();
	};
	
	var __decorate = (this && this.__decorate) || function (decorators, target, key, desc) {
        var c = arguments.length;
        var r = c < 3 ? target : desc === null ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;

        if (typeof global.Reflect === "object" && typeof global.Reflect.decorate === "function") {
            r = global.Reflect.decorate(decorators, target, key, desc);
        }
        else {
            for (var i = decorators.length - 1; i >= 0; i--) {
                if (d = decorators[i]) {
                    r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r;
                }
            }
        }
        return c > 3 && r && Object.defineProperty(target, key, r), r;
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

	function Interfaces(interfacesArr) {
	    return function (target) {
            if(interfacesArr instanceof Array) {
                // attach interfaces: [] to the object
                target.prototype.interfaces = interfacesArr;
            }
	    }
	}
	
	Object.defineProperty(global, "__native", { value: __native });
	Object.defineProperty(global, "__extends", { value: __extends });
	Object.defineProperty(global, "__decorate", { value: __decorate });

	global.JavaProxy = JavaProxy;
	global.Interfaces = Interfaces;
})()
