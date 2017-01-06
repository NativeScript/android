package com.telerik.metadata;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

@Retention(RetentionPolicy.CLASS)
public @interface MetadataInfo {
    String superClassname() default "";
    boolean skip() default false;
}
