package com.tns.tests;

public class BaseClass {
    public int overloadMethod(InterfaceOne ione) {
        return 1;
    }

    public int overloadMethod(InterfaceTwo itwo) {
        return 2;
    }

    public int sum(int x, int y) {
        return x + y;
    }

    public int mul(int x, int y) {
        return x * y;
    }

    public static int multiply(int x, int y) {
        return x * y;
    }
}
