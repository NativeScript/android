package com.tns.tests;

/** Creates an instance of its own runtime class while its constructor runs. */
public class SelfNesting {
    private final int tag;

    public SelfNesting() {
        tag = 1;
    }

    public SelfNesting(boolean nest) throws Exception {
        tag = 2;
        if (nest) {
            SelfNesting nested = getClass().getConstructor().newInstance();
            nested.touch();
        }
    }

    public int getTag() {
        return tag;
    }

    public void touch() {
    }
}
