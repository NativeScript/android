plugins {
    `kotlin-dsl`
    `maven-publish`
    `java-library`
}

repositories {
    google()
    mavenCentral()
    jcenter()
}

buildscript {
    repositories {
        google()
        mavenCentral()
        jcenter()
    }
}

dependencies {
    compileOnly("com.android.tools.build:gradle:3.5.2")

    if("true" == project.extra.get("isCompositeBuild")){
        implementation("com.nativescript:bindings-generator:0.1.0")
    } else {
        implementation(files("$rootDir/../build-tools/bindings-generator:0.1.0.jar"))
    }
}

group = "com.nativescript"
version = "0.1.0"

gradlePlugin {
    plugins {
        register("nativescript-android-plugin") {
            id = "nativescript-android-plugin"
            implementationClass = "com.nativescript.build.NativeScriptBuildPlugin"
        }
    }
}
publishing {
    repositories {
        maven {
            // change to point to your repo, e.g. http://my.org/repo
            mavenLocal()
        }
    }

}

//// make 'jar' task produce a fat jar
//tasks.withType<Jar> {
//    from(configurations.compileClasspath.map { config -> config.map { if (it.isDirectory) it else zipTree(it) } })
//}