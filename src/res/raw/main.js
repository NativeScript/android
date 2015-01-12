var View = function () { 
    RegisterInstance(this, "com/telerik/kimera/View"); 
}

View.OnClickListener = function(listener) { 
    this._listener = listener; 
    RegisterInstance(this, "com/telerik/kimera/TelerikOnClickListener");
}

View.OnClickListener.prototype.onClick = function() {
    this._listener.onClick(); 
}

var Button = function(name) {
    this.Name = name;
    this.onClickListener = {};
    RegisterInstance(this, "com/telerik/kimera/Button");
}

Button.prototype.click = function (view) {
    CallMethod(this, "click", "(Lcom/telerik/kimera/View;)V", view);
}

Button.prototype.setOnClickListener = function (onClickListener) {
    CallMethod(this, "setOnClickListener", "(Lcom/telerik/kimera/View$OnClickListener;)V", onClickListener);
}

function main() 
{ 
  var button = new Button("myButton");
  
  button.setOnClickListener(new View.OnClickListener({
    onClick : function () {
      button.Name = button.Name + " clicked";
      Log("onClick callback in javascript");
    }}));
  
  var view = new View();
  button.click(view);
};

main();