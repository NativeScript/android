# android-binding-generator

This project is used in the [NativeScript runtime for Android](https://github.com/NativeScript/android-runtime) to generate Java subclasses at runtime. Supppose you have the following Java class

```Java
package com.example;

public class MyClass {
   public int add(int x, int y) {
      return x + y;
   }
}
```

In NativeScript, you can inherit from `MyClass` using the following syntax:

```JavaScript
var MyNewClass = com.example.MyClass.extend({
   add: function(x, y) {
      return x * y;
   }
})

var obj = new MyNewClass();
```

Using `extend` function will create a new Java class that looks roughly as follows
```Java
package <some auto-generated package name>;

public <some auto-generated class name> extends com.example.MyClass {
   public int add(int x, int y) {
      java.lang.Object[] params = new Object[2];
      params[0] = x;
      params[1] = y;
      return (int)com.tns.Platform.callJSMethod(this, "add", int.class, params);
   }
}
```
The binding generator uses [ASMDEX library](http://asm.ow2.org/asmdex-index.html) to create the class above and to produce the corresponding `*.dex` file.
