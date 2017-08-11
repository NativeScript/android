describe("Tests typescript", function () {
	
	var myCustomEquality = function(first, second) {
		return first == second;
	};
	
	beforeEach(function() {
		jasmine.addCustomEqualityTester(myCustomEquality);
	});
	
	it("When_creating_a_typescript_instance_with_constructor_property_it_should_support_this", function () {
	
		__log("TEST: When_creating_a_typescript_instance_with_constructor_property_it_should_support_this");
		var NativeViewGroup = (function (_super) {
		    __extends(NativeViewGroup, _super);
		    function NativeViewGroup(view) {
		        this._view = view;
		        return __native(this);
		    }
		    
		    NativeViewGroup.prototype.Then = function () {
		    	this._view.Do();
		    };
		    
		    return NativeViewGroup;
		})(android.view.ViewGroup);
		
		var doCalled = false;

	    var myView = new NativeViewGroup({ Do: function() { doCalled = true; }});
	    myView.Then();
	    expect(doCalled).toEqual(true);
	    
	    var MyButton = (function (_super) {
	        __extends(MyButton, _super);
	        function MyButton() {
	            _super.call(this);
	            this.myName = "MyName";
	            return __native(this);
	        }
	        
	        MyButton.prototype.echo = function (s) {
	            return "echo: " + this.myName;
	        };
	        
	        MyButton.prototype.toString = function (s) {
	            return "toString: " + this.myName;
	        };
	        
	        return MyButton;
	    })(com.tns.tests.Button1);

	    var b = new MyButton();
	    var exo = b.triggerEcho("exo");
	    expect(exo).toBe("echo: MyName");
	    
	    var toStringResult = b.toString();
	    expect(toStringResult).toBe("toString: MyName");
	});
	
	it("When_creating_a_typescript_instance_it_should_support_overriden_members", function () {
		
		__log("TEST: When_creating_a_typescript_instance_it_should_support_overriden_members");
		
		var initCalled = false;
	    var MyButton1 = (function (_super) {
	        __extends(MyButton1, _super);
	        function MyButton1() {
	            _super.call(this);
	            return __native(this);
	        }
	        
	        MyButton1.prototype.init = function () {
	        	initCalled = true;
	        };

	        MyButton1.prototype.toString = function (s) {
	            return "toString called"
	        };
	        
	        return MyButton1;
	    })(com.tns.tests.Button1);
		
	    var button = new MyButton1();
	    expect(initCalled).toEqual(true);
	    
		var value = button.toString();
	    expect(value).toEqual("toString called");
	});
	
	it("When_creating_a_typescript_instance_it_should_support_calling_super_members_from_overriden_members", function () {
		
		__log("TEST: When_creating_a_typescript_instance_it_should_support_calling_super_members_from_overriden_members");
		
		 var MyButton2 = (function (_super) {
		        __extends(MyButton2, _super);
		        function MyButton2() {
		            _super.call(this);
		            return __native(this);
		        }
		        
		        MyButton2.prototype.superToString = function () {
		            return _super.prototype.toString.call(this);
		        };
		        
		        MyButton2.prototype.toString = function (s) {
		            return this.super.toString();
		        };
		        
		        return MyButton2;
		    })(com.tns.tests.Button1);
		
	    var button = new MyButton2();
	    var button1Label = button.superToString();
	    
	  	expect(button1Label.indexOf("com.tns.tests.Button1_")).not.toEqual(-1);
	  	expect(button1Label.indexOf("_MyButton2")).not.toEqual(-1);
	});
	
	it("When_creating_a_pure_typescript_inheritance_chain_it_should_work", function () {
		
		__log("TEST: When_creating_a_pure_typescript_inheritance_chain_it_should_work");
		
		var Animal = (function () {
		    function Animal(name) {
		        this.name = name;
		    }
		    Animal.prototype.move = function (meters) {
		    	__log(this.name + " moved " + meters + "m.");
		    };
		    return Animal;
		})();

		var Snake = (function (_super) {
		    __extends(Snake, _super);
		    function Snake(name) {
		        _super.call(this, name);
		    }
		    Snake.prototype.move = function () {
		    	__log("Slithering...");
		        _super.prototype.move.call(this, 5);
		    };
		    return Snake;
		})(Animal);

		var Horse = (function (_super) {
		    __extends(Horse, _super);
		    function Horse(name) {
		        _super.call(this, name);
		    }
		    Horse.prototype.move = function () {
		    	__log("Galloping...");
		        _super.prototype.move.call(this, 45);
		    };
		    return Horse;
		})(Animal);
		
		var horse = new Horse();
		var isInstanceOf = horse instanceof Horse;
		expect(isInstanceOf).toEqual(true);
		
		isInstanceOf = horse instanceof Animal;
		expect(isInstanceOf).toEqual(true);
		
		var snake = new Snake();
		isInstanceOf = snake instanceof Snake;
		expect(isInstanceOf).toEqual(true);
		
		isInstanceOf = snake instanceof Animal;
		expect(isInstanceOf).toEqual(true);
		
		var animal = new Animal();
		isInstanceOf = animal instanceof Animal;
		expect(isInstanceOf).toEqual(true);
	});
	
	it("When_creating_a_typescript_instance_it_should_be_a_valid_nativescript_instance", function () {
		
		__log("TEST: When_creating_a_typescript_instance_it_should_be_a_valid_nativescript_instance");
		
		 var MyButton3 = (function (_super) {
		        __extends(MyButton3, _super);
		        function MyButton3() {
		            _super.call(this);
		            return __native(this);
		        }
		        
		        MyButton3.prototype.toString = function (s) {
		            return this.super.toString();
		        };
		        
		        return MyButton3;
		    })(com.tns.tests.Button1);
		
		var button = new MyButton3();
		
		var isInstanceOf = button instanceof MyButton3;
		expect(isInstanceOf).toEqual(true);
		
		isInstanceOf = button instanceof com.tns.tests.Button1;
		expect(isInstanceOf).toEqual(true);
	});
	
	it("When_creating_a_typescript_instance_it_should_be_a_valid_nativescript_instance", function () {
		
		__log("TEST: When_creating_a_typescript_instance_it_should_be_a_valid_nativescript_instance");
		
		var MyButtonWithArgs = (function (_super) {
		    __extends(MyButtonWithArgs, _super);
		    function MyButtonWithArgs(value) {
		        _super.call(this, value);
		        return __native(this);
		    }
		    
		    MyButtonWithArgs.prototype.init = function () {
		    };

		    MyButtonWithArgs.prototype.onClick = function () {
		    	__log("MyButton onClick called");
		    };
		    
		    MyButtonWithArgs.prototype.superToString = function () {
		        return _super.prototype.toString.call(this);
		    };
		      
		    return MyButtonWithArgs;
		})(com.tns.tests.Button1);
		
		var button = new MyButtonWithArgs(5);
		
		var isInstanceOf = button instanceof MyButtonWithArgs;
		expect(isInstanceOf).toEqual(true);
		
		isInstanceOf = button instanceof com.tns.tests.Button1;
		expect(isInstanceOf).toEqual(true);
	});
	
	it("When_creating_a_typescript_instance_it_should_support_member_access", function () {

		__log("TEST: When_creating_a_typescript_instance_it_should_support_member_access");
		
		 var MyButton4 = (function (_super) {
		        __extends(MyButton4, _super);
		        function MyButton4() {
		            _super.call(this);
		            this.my1 = "MyName";
		            return __native(this);
		        }
		        
		        MyButton4.prototype.init = function () {
		        };

		        MyButton4.prototype.echo = function (s) {
		            return "echo: " + this.my1;
		        };
		        
		        return MyButton4;
		    })(com.tns.tests.Button1);
		
		var button = new MyButton4();
		var prop = button.getIMAGE_ID_PROP();
		expect(prop).toEqual("image id prop");
		
		var button1 = new MyButton4();
		var prop1 = button1.getIMAGE_ID_PROP();
		expect(prop1).toEqual("image id prop");
	});
	
	it("When_creating_a_typescript_instance_it_should_support_calling_super_members_from_super_prototype", function () {
		
		__log("TEST: When_creating_a_typescript_instance_it_should_support_calling_super_members_from_super_prototype");
		
		 var MyButton5 = (function (_super) {
		        __extends(MyButton5, _super);
		        function MyButton5() {
		            _super.call(this);
		            this.my1 = "MyName";
		            return __native(this);
		        }
		        
		        MyButton5.prototype.init = function () {
		        };

		        MyButton5.prototype.toString = function (s) {
		        	return this.super.toString();
		        };
		        
		        return MyButton5;
		    })(com.tns.tests.Button1);
	      
	    var button = new MyButton5();
	    var button1Label = button.toString();
	  	expect(button1Label.indexOf("com.tns.tests.Button1_")).not.toEqual(-1);
		expect(button1Label.indexOf("_MyButton5")).not.toEqual(-1);
	});
	
	it("When_extending_an_already_extended_object_it_should_throw_an_error", function () {
		
		__log("TEST: When_extending_an_already_extended_object_it_should_throw_an_error");
		
		var errorThrown = false;
		try	{
			
			 var MyButton6 = (function (_super) {
			        __extends(MyButton6, _super);
			        function MyButton6() {
			            _super.call(this);
			            this.my1 = "MyName";
			            return __native(this);
			        }
			        
			        MyButton6.prototype.init = function () {
			        };

			        MyButton6.prototype.toString = function (s) {
			        	return this.super.toString();
			        };
			        
			        return MyButton6;
			    })(com.tns.tests.Button1);
			 
			var SecondButton = (function (_super) {
			    __extends(SecondButton, _super);
			    
			    function SecondButton() {
			        _super.apply(this, arguments);
			    }
			    
			    return SecondButton;
			})(MyButton6);
		} catch (err)	{
			errorThrown = true;
		}
		
		expect(errorThrown).toEqual(true);
	});

	// currently dissabled because of: https://github.com/NativeScript/android-runtime/issues/626
	// if propper (check if access is possible on current device) solution is implemented uncomment tests
	// for now the users are not using this, and there are no issues about it
//	it("When_accessing_a_static_field_on_a_typescript_instance_it_should_work", function () {
//
//		__log("TEST: When_accessing_a_static_field_on_a_typescript_instance_it_should_work");
//
//		 var MyButton7 = (function (_super) {
//		        __extends(MyButton7, _super);
//		        function MyButton7() {
//		            _super.call(this);
//		            this.my1 = "MyName";
//		        }
//
//		        MyButton7.prototype.init = function () {
//		        };
//
//		        MyButton7.prototype.toString = function (s) {
//		            return "my";
//		        };
//
//		        return MyButton7;
//		    })(com.tns.tests.Button1);
//
//		var valueUsingChild = MyButton7.STATIC_IMAGE_ID;
//		expect(valueUsingChild).toEqual("static image id");
//
//		MyButton7.STATIC_IMAGE_ID = "test";
//		valueUsingChild = MyButton7.STATIC_IMAGE_ID;
//		expect(valueUsingChild).toEqual("test");
//
//		var valueUsingParent = com.tns.tests.Button1.STATIC_IMAGE_ID;
//		expect(valueUsingParent).toEqual("static image id");
//	});

	it("When_calling_a_static_method_on_a_typescript_instance_it_should_work", function () {

		__log("TEST: When_calling_a_static_method_on_a_typescript_instance_it_should_work");


		 var MyButton8 = (function (_super) {
		        __extends(MyButton8, _super);
		        function MyButton8() {
		            _super.call(this);
		            this.my1 = "MyName";
		        }

		        MyButton8.prototype.init = function () {
		        };

		        MyButton8.prototype.onClick = function () {
		        	__log("MyButton onClick called");
		        };

		        MyButton8.prototype.superToString = function () {
		            return _super.prototype.toString.call(this);
		        };

		        MyButton8.prototype.echo = function (s) {
		            return "echo: " + this.my1;
		        };

		        return MyButton8;
		    })(com.tns.tests.Button1);

		MyButton8.setMyStaticIntField(5);
		var valueUsingChild = MyButton8.getMyStaticIntField();
		expect(valueUsingChild).toEqual(5);

		var valueUsingParent = com.tns.tests.Button1.getMyStaticIntField();
		expect(valueUsingParent).toEqual(5);

		com.tns.tests.Button1.setMyStaticIntField(6);
		var valueUsingParent = com.tns.tests.Button1.getMyStaticIntField();
		expect(valueUsingParent).toEqual(6);

		valueUsingChild = MyButton8.getMyStaticIntField();
		expect(valueUsingChild).toEqual(6);
	});
	
	it("When_inherit_from_android_base_class_it_should_create_an_instance", function () {
		
		/*
		// From issue #137 https://github.com/telerik/Kimera/issues/137
		class ListViewAdapter extends android.widget.BaseAdapter {
		    private _items: Array<any>;
		
		    constructor(items: Array<any>) {
		        super();
		
		        this._items = items;
		    }
		
		    get items(): Array<any> {
		        return this._items;
		    }
		    set items(value: Array<any>) {
		        this._items = value;
		    }
		
		    public getCount() {
		        return this._items ? this._items.length : 0;
		    }
		
		    public getItem(i) {
		        return this._items && i < this._items.length ? this._items[i] : null;
		    }
		
		    public getItemId(i) {
		        return long(0);
		    }
		
		    public getView(i, view, viewGroup) {
		        return null;
		    }
		
		    public refresh() {
		        this.notifyDataSetChanged();
		    }
		}
		*/
		var ListViewAdapter = (function (_super) {
		    __extends(ListViewAdapter, _super);
		    function ListViewAdapter(items) {
		        _super.call(this);
		
		        this._items = items;
		        return __native(this);
		    }
		    Object.defineProperty(ListViewAdapter.prototype, "items", {
		        get: function () {
		            return this._items;
		        },
		        set: function (value) {
		            this._items = value;
		        },
		        enumerable: true,
		        configurable: true
		    });
		
		    ListViewAdapter.prototype.getCount = function () {
		        return this._items ? this._items.length : 0;
		    };
		
		    ListViewAdapter.prototype.getItem = function (i) {
		        return this._items && i < this._items.length ? this._items[i] : null;
		    };
		
		    ListViewAdapter.prototype.getItemId = function (i) {
		        return long(0);
		    };
		
		    ListViewAdapter.prototype.getView = function (i, view, viewGroup) {
		        return null;
		    };
		
		    ListViewAdapter.prototype.refresh = function () {
		        this.notifyDataSetChanged();
		    };
		    return ListViewAdapter;
		})(android.widget.BaseAdapter);
		
		var adapter = new ListViewAdapter();
		
		expect(adapter).not.toEqual(null);
	});
	
	it("When_creating_a_typescript_instance_and_anonymous_interfaces_in_its_ctor_it_should_work", function () {
		
		__log("TEST: When_creating_a_typescript_instance_and_anonymous_interfaces_in_its_ctor_it_should_work");
		
	    var MyButton9 = (function (_super) {
	        __extends(MyButton9, _super);
	        
	        function MyButton9() {
	            _super.call(this);
	            
	            var that = __native(this);
	            
	            that.setOnClickListener(new android.view.View.OnClickListener({
	        		onClick : function() {
	        			that.buttonClicked = true;
	        		}
	            }));
	            
	            return that;
	        }

	        return MyButton9;
	    })(com.tns.tests.Button1);
		
	    var button = new MyButton9();
	    var button1 = new MyButton9();
	    button.click(null);
	    
	    expect(button.buttonClicked).toBe(true);
	    expect(button1.buttonClicked).toBe(undefined);
	    
	    button.buttonClicked = false;
	    button1.click(null);
	    
		expect(button.buttonClicked).toBe(false);
		expect(button1.buttonClicked).toBe(true);
	});
});