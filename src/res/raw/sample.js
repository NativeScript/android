var Main = function () {
    /* this function is a constructor */
    var m = this; // store scope
    // do your init stuff
    this.work = function() {
        console.log("main work");
    };
        
    this.coolConstructor = function () {
        var cc = this; // store scope             
        var sleep = true; // just an example variable
        cc.moveMyself = function () {
            //do something          
            console.log('moveMyself')
        };
        cc.init = function () {
            sleep = false; // just an example
            cc.moveMyself(); // will work
            cc.work(); // will FAIL, because function work is defined after its called
            this.work();
        };
        cc.work = function () {
            // do some work
            console.log('work')
        };
    };
};

var main = new Main(); // make new instance of Main 
//main.boringCollection.doOtherStuff(); // will work

//main.coolConstructor.init(); // will NOT work 
var scrappy = new main.coolConstructor(); // make new instance of m.coolConstructor
scrappy.init(); // will work






var View = function() {

  RegisterInstance("com/fake/View");
   
  
  
   this.OnClickListener: (function(listener) {
               _listener = listener;
      
               RegisterInstance("com/fake/TelerikOnClickListener");
      
               return {
                   OnClick: function() {
                       _listener.onClick();
                   }
               };
    })
 };
};

var Button = function(name) {
        this.Name  = name;
        
        RegisterInstance("com/fake/Button");
  
        return {
          click: function (view) {
              CallMethod("click", onClickListener, "(Lcom/fake/View;)V", view);
          },
            
          setOnClickListener: function (onClickListener) {
              CallMethod("setOnClickListener", "(Lcom/fake/View$OnClickListener;)V", onClickListener);
          }
    };
};