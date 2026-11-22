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
		// TypeScript 2+ constructors pass the instance `super()` returned, which this returns as is.
		// One compiled earlier sets its fields on its own `this` and passes that instead; this moves
		// them to the instance.
		if (thiz.__container__) {
			var result = thiz[nativeInstanceKey];

			for (var prop in thiz) {
				if (thiz.hasOwnProperty(prop)) {
					result[prop] = thiz[prop];
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

	var nativeInstanceKey = Symbol("nativeInstance");

	// Never make the native instance a prototype, not even of the constructor's own `this`:
	// V8 gives a prototype a map of its own, so every store on the instance, such as each
	// field initializer, would miss.
	function linkContainer(thiz, instance) {
		thiz.__container__ = true;
		thiz[nativeInstanceKey] = instance;
		return instance;
	}

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
			var instance = arguments.length > 1 ? Reflect.construct(Extended, Array.prototype.slice.call(arguments, 1)) : new Extended();
			return linkContainer(thiz, instance);
		};

		Parent.apply = function (thiz, args) {
			var Extended = extend(thiz);
			var instance = args && args.length > 0 ? Reflect.construct(Extended, Array.prototype.slice.call(args)) : new Extended();
			return linkContainer(thiz, instance);
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

	if (!global.__ns__worker) {
		global.JavaProxy = JavaProxy;
	}
	global.Interfaces = Interfaces;
})()