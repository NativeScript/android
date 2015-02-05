var Assert = function(condition, failMessage) {
	if	(condition == false) {
		fail(failMessage);
	}
}

var MyButton = (function (_super) {
    __extends(MyButton, _super);
    function MyButton() {
        _super.call(this);
    }
    
    MyButton.prototype.init = function () {
    };

    MyButton.prototype.onClick = function () {
      Log("MyButton onClick called");
    };
    
    MyButton.prototype.superToString = function () {
        return _super.prototype.toString.call(this);
    };
      
    return MyButton;
})(com.tns.tests.Button1);

(function When_creating_a_pure_typescript_inheritance_chain_it_should_work() {
	
	Log("TEST: When_creating_a_pure_typescript_inheritance_chain_it_should_work");
	
	var Animal = (function () {
	    function Animal(name) {
	        this.name = name;
	    }
	    Animal.prototype.move = function (meters) {
	        Log(this.name + " moved " + meters + "m.");
	    };
	    return Animal;
	})();

	var Snake = (function (_super) {
	    __extends(Snake, _super);
	    function Snake(name) {
	        _super.call(this, name);
	    }
	    Snake.prototype.move = function () {
	        Log("Slithering...");
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
	        Log("Galloping...");
	        _super.prototype.move.call(this, 45);
	    };
	    return Horse;
	})(Animal);
	
	var horse = new Horse();
	var isInstanceOf = horse instanceof Horse;
	Assert(isInstanceOf == true, "FAILED: When_creating_a_pure_typescript_inheritance_chain_it_should_work.");
	
	isInstanceOf = horse instanceof Animal;
	Assert(isInstanceOf == true, "FAILED: When_creating_a_pure_typescript_inheritance_chain_it_should_work.");
	
	var snake = new Snake();
	isInstanceOf = snake instanceof Snake;
	Assert(isInstanceOf == true, "FAILED: When_creating_a_pure_typescript_inheritance_chain_it_should_work.");
	
	isInstanceOf = snake instanceof Animal;
	Assert(isInstanceOf == true, "FAILED: When_creating_a_pure_typescript_inheritance_chain_it_should_work.");
	
	var animal = new Animal();
	isInstanceOf = animal instanceof Animal;
	Assert(isInstanceOf == true, "FAILED: When_creating_a_pure_typescript_inheritance_chain_it_should_work.");

})();

(function When_creating_a_typescript_instance_it_should_be_a_valid_nativescript_instance() {
	
	Log("TEST: When_creating_a_typescript_instance_it_should_be_a_valid_nativescript_instance");
	
	var button = new MyButton();
	
	var isInstanceOf = button instanceof MyButton;
	Assert(isInstanceOf == true, "FAILED: When_creating_a_typescript_instance_it_should_be_a_valid_nativescript_instance. Should be instance of MyButton");
	
	isInstanceOf = button instanceof com.tns.tests.Button1;
	Assert(isInstanceOf == true, "FAILED: When_creating_a_typescript_instance_it_should_be_a_valid_nativescript_instance.Should be instance of com.tns.tests.Button1");
})();

(function When_creating_a_typescript_instance_with_arguments_it_should_be_a_valid_nativescript_instance() {
	
	Log("TEST: When_creating_a_typescript_instance_it_should_be_a_valid_nativescript_instance");
	
	var MyButtonWithArgs = (function (_super) {
	    __extends(MyButtonWithArgs, _super);
	    function MyButtonWithArgs(value) {
	        _super.call(this, value);
	    }
	    
	    MyButtonWithArgs.prototype.init = function () {
	    };

	    MyButtonWithArgs.prototype.onClick = function () {
	      Log("MyButton onClick called");
	    };
	    
	    MyButtonWithArgs.prototype.superToString = function () {
	        return _super.prototype.toString.call(this);
	    };
	      
	    return MyButtonWithArgs;
	})(com.tns.tests.Button1);
	
	var button = new MyButtonWithArgs(5);
	
	var isInstanceOf = button instanceof MyButtonWithArgs;
	Assert(isInstanceOf == true, "FAILED: When_creating_a_typescript_instance_it_should_be_a_valid_nativescript_instance. Should be instance of MyButtonWithArgs");
	
	isInstanceOf = button instanceof com.tns.tests.Button1;
	Assert(isInstanceOf == true, "FAILED: When_creating_a_typescript_instance_it_should_be_a_valid_nativescript_instance.Should be instance of com.tns.tests.Button1");
})();


(function When_creating_a_typescript_instance_it_should_support_member_access() {
	
	Log("TEST: When_creating_a_typescript_instance_it_should_support_member_access");
	
	var button = new MyButton();
	var prop = button.getIMAGE_ID_PROP();
	Assert(prop == "image id prop", "FAILED: When_creating_a_typescript_instance_it_should_support_member_access.");
	
	var button1 = new MyButton();
	var prop1 = button1.getIMAGE_ID_PROP();
	Assert(prop1 == "image id prop", "FAILED: When_creating_a_typescript_instance_it_should_support_member_access.");
})();

(function When_creating_a_typescript_instance_it_should_support_overriden_members() {
	
	Log("TEST: When_creating_a_typescript_instance_it_should_support_overriden_members");
	
	var initCalled = false;
	MyButton.prototype.init = function () {
		initCalled = true;
    };
    
    MyButton.prototype.toString = function () {
    	return "toString called"
      };
      
    var button = new MyButton();
    Assert(initCalled == true, "FAILED: When_creating_a_typescript_instance_it_should_support_overriden_members. Init not called");

	var value = button.toString();
    Assert(value == "toString called", "FAILED: When_creating_a_typescript_instance_it_should_support_overriden_members. toString not called");
})();

(function When_creating_a_typescript_instance_it_should_support_calling_super_members_from_overriden_members() {
	
	Log("TEST: When_creating_a_typescript_instance_it_should_support_calling_super_members_from_overriden_members");
	
    MyButton.prototype.toString = function () {
    	return this.super.toString();
      };
      
    var button = new MyButton();
    var button1Label = button.superToString();
  	Assert(button1Label.indexOf("com.tns.tests.Button1@") != -1, "FAILED: When_creating_a_typescript_instance_it_should_support_calling_super_members_from_overriden_members");
})();

(function When_creating_a_typescript_instance_it_should_support_calling_super_members_from_super_prototype() {
	
	Log("TEST: When_creating_a_typescript_instance_it_should_support_calling_super_members_from_super_prototype");
	
    MyButton.prototype.toString = function () {
    	return this.super.toString();
      };
      
    var button = new MyButton();
    var button1Label = button.toString();
  	Assert(button1Label.indexOf("com.tns.tests.Button1@") != -1, "FAILED: When_creating_a_typescript_instance_it_should_support_calling_super_members_from_super_prototype");
})();

(function When_extending_an_already_extended_object_it_should_throw_an_error() {
	
	Log("TEST: When_extending_an_already_extended_object_it_should_throw_an_error");
	
	var errorThrown = false;
	try	{
		var SecondButton = (function (_super) {
		    __extends(SecondButton, _super);
		    
		    function SecondButton() {
		        _super.apply(this, arguments);
		    }
		    
		    return SecondButton;
		})(MyButton);
	} catch (err)	{
		errorThrown = true;
	}
	
	Assert(errorThrown == true, "FAILED: When_extending_an_already_extended_object_it_should_throw_an_error.");
})();


(function When_accessing_a_static_field_on_a_typescript_instance_it_should_work() {
	
	Log("TEST: When_accessing_a_static_field_on_a_typescript_instance_it_should_work");
	
	var valueUsingChild = MyButton.STATIC_IMAGE_ID;
	Assert(valueUsingChild == "static image id", "FAILED: When_accessing_a_static_field_on_a_typescript_instance_it_should_work. MyButton.STATIC_IMAGE_ID should be 5");
	
	MyButton.STATIC_IMAGE_ID = "test";
	valueUsingChild = MyButton.STATIC_IMAGE_ID;
	Assert(valueUsingChild == "test", "FAILED: When_accessing_a_static_field_on_a_typescript_instance_it_should_work. MyButton.STATIC_IMAGE_ID should be 5");
	
	var valueUsingParent = com.tns.tests.Button1.STATIC_IMAGE_ID;
	Assert(valueUsingParent == "static image id", "FAILED: When_accessing_a_static_field_on_a_typescript_instance_it_should_work. com.tns.tests.Button1.STATIC_IMAGE_ID should be 5");
})();

(function When_calling_a_static_method_on_a_typescript_instance_it_should_work() {
	
	Log("TEST: When_calling_a_static_method_on_a_typescript_instance_it_should_work");

	MyButton.setMyStaticIntField(5);
	var valueUsingChild = MyButton.getMyStaticIntField();
	Assert(valueUsingChild == 5, "FAILED: When_calling_a_static_method_on_a_typescript_instance_it_should_work.");

	var valueUsingParent = com.tns.tests.Button1.getMyStaticIntField(); 
	Assert(valueUsingParent == 5, "FAILED: When_calling_a_static_method_on_a_typescript_instance_it_should_work.");
	
	com.tns.tests.Button1.setMyStaticIntField(6);
	var valueUsingParent = com.tns.tests.Button1.getMyStaticIntField();
	Assert(valueUsingParent == 6, "FAILED: When_calling_a_static_method_on_a_typescript_instance_it_should_work.");
		
	valueUsingChild = MyButton.getMyStaticIntField();
	Assert(valueUsingChild == 6, "FAILED: When_calling_a_static_method_on_a_typescript_instance_it_should_work.");
})();

(function When_inherit_from_android_base_class_it_should_create_an_instance() {
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
	
	Assert(adapter != null, "FAILED: When_inherit_from_android_base_class_it_should_create_an_instance.");
	
})();
