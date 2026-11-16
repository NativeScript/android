# Keep all runtime classes
-keep class com.tns.* { *; }

# Keep SBG-generated classes
-keep class com.tns.gen.** { *; }

# Keep internal support/runtime classes
-keep class com.tns.internal.** { *; }

# Preserve annotation metadata so reflection sees them
-keepattributes RuntimeVisibleAnnotations
# Customizers are only named in the manifest and created by reflection
-keep class * implements com.tns.AppFileLayersCustomizer { <init>(); }
