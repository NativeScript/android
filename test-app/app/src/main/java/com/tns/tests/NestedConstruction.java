package com.tns.tests;

/** Creates objects while its own constructor runs, for the construction-window specs. */
public class NestedConstruction {
    private final Object created;

    public NestedConstruction(Class<?> toCreate, Runnable during) throws Exception {
        created = toCreate == null ? null : toCreate.newInstance();
        if (during != null) {
            during.run();
        }
    }

    public Object getCreated() {
        return created;
    }
}
