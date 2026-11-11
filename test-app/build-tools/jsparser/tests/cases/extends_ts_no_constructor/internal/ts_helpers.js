(function () {

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

	// For backward compatibility.
	var __native = function (thiz) {
		// we are setting the __container__ property to the base class when the super method is called
		// if the constructor returns the __native(this) call we will use the old implementation
		// copying all the properties to the result
		// otherwise if we are using the result from the super() method call we won't need such logic
		// as thiz already contains the parent properties
		// this way we now support both implementations in typescript generated constructors:
		// 1: super(); return __native(this);
		// 2: return super() || this;
		if (thiz.__container__) {
			var result = thiz.__proto__;

			for (var prop in thiz) {
				if (thiz.hasOwnProperty(prop)) {
					thiz.__proto__[prop] = thiz[prop];
					delete thiz[prop];
				}
			}

			thiz.constructor = undefined;
			thiz.__proto__ = undefined;
			Object.freeze(thiz);
			Object.preventExtensions(thiz);
			return result;
		} else {
			return thiz;
		}
	};

	var __extends = function (Child, Parent) {
		var extendNativeClass = !!Parent.extend && (Parent.extend.toString().indexOf("[native code]") > -1);
		if (!extendNativeClass) {
			__extends_ts(Child, Parent);
			return;
		}
		if (Parent.__isPrototypeImplementationObject) {
			throw new Error("Can not extend an already extended native object.");
		}

		function extend(thiz) {
			var child = thiz.__proto__.__child;
			if (!child.__extended) {
				var parent = thiz.__proto__.__parent;
				child.__extended = parent.extend(child.name, child.prototype, true);
				// This will deal with "i instanceof child"
				child[Symbol.hasInstance] = function (instance) {
					return instance instanceof this.__extended;
				}
			}
			return child.__extended;
		};

		Parent.__activityExtend = function (parent, name, implementationObject) {
			__log("__activityExtend called");
			return parent.extend(name, implementationObject);
		};

		Parent.call = function (thiz) {
			var Extended = extend(thiz);
			thiz.__container__ = true;
			if (arguments.length > 1) {
				thiz.__proto__ = new (Function.prototype.bind.apply(Extended, [null].concat(Array.prototype.slice.call(arguments, 1))));
			}
			else {
				thiz.__proto__ = new Extended()
			}
			return thiz.__proto__;
		};

		Parent.apply = function (thiz, args) {
			var Extended = extend(thiz);
			thiz.__container__ = true;
			if (args && args.length > 0) {
				thiz.__proto__ = new (Function.prototype.bind.apply(Extended, [null].concat(args)));
			}
			else {
				thiz.__proto__ = new Extended();
			}
			return thiz.__proto__;
		};
		__extends_ns(Child, Parent);
		Child.__isPrototypeImplementationObject = true;
		Child.__proto__ = Parent;
		Child.prototype.__parent = Parent;
		Child.prototype.__child = Child;
	}

	var __extends_ts = function (child, parent) {
		extendStaticFunctions(child, parent);
		assignPrototypeFromParentToChild(parent, child);
	};

	var __extends_ns = function (child, parent) {
		if (!parent.extend) {
			assignPropertiesFromParentToChild(parent, child);
		}

		assignPrototypeFromParentToChild(parent, child);
	};

	var extendStaticFunctions =
		Object.setPrototypeOf
		|| (hasInternalProtoProperty() && function (child, parent) { child.__proto__ = parent; })
		|| assignPropertiesFromParentToChild;

	function hasInternalProtoProperty() {
		return { __proto__: [] } instanceof Array;
	}

	function assignPropertiesFromParentToChild(parent, child) {
		for (var property in parent) {
			if (parent.hasOwnProperty(property)) {
				child[property] = parent[property];
			}
		}
	}

	function assignPrototypeFromParentToChild(parent, child) {
		function __() {
			this.constructor = child;
		}

		if (parent === null) {
			child.prototype = Object.create(null);
		} else {
			__.prototype = parent.prototype;
			child.prototype = new __();
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
			if (interfacesArr instanceof Array) {
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