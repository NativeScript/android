package com.tns.tests;

public class OuterClass {
    public class InnerClass {
        private InnerClass() {}
        public static final String NAME = "OuterClass.InnerClass";
        public static final int NUM = 42;
    }

    public class InnerWithNestedClass {
        public static final String NAME = "OuterClass.InnerWithNestedClass";

        public class InnerClass {
            private int number;

            public InnerClass(int num) {
                this.number = num;
            }

            public static final String NAME = "InnerWithNestedClass.InnerClass";

            public int getNumber() {
                return this.number;
            }
        }
    }
}
