module.exports = function(grunt) {

    var outDir = "./dist";
    var srcDir = ".";

    var generatorDir = srcDir + "/Bindings/Generator";
    var jarCreationDir = srcDir + "/Bindings/JarCreation";
    var kimeraDir = srcDir + "/kimera";

    var testPackageFile = function(filePath)
    {
        var packageFileTester = /package\.json$/ig;
        return packageFileTester.test(filePath);
    };

    var updatePackageVersion = function(content, srcPath)
    {
        if (!testPackageFile(srcPath))
        {
            return content;
        }

        var contentAsObject = JSON.parse(content);
        contentAsObject.version = localCfg.packageVersion;
        return JSON.stringify(contentAsObject, null, "\t");
    };

    function getPackageVersion(packageFilePath)
    {
        packageContent = grunt.file.readJSON(packageFilePath);
        var buildVersion = process.env.PACKAGE_VERSION;
        if (!buildVersion)
        {
            return packageContent.version;
        }
        return packageContent.version + "-" + buildVersion;
    }

    var localCfg = {
        jniDir: srcDir + "/kimera/jni",
        runtimeVersionHFile: srcDir + "/kimera/jni/Version.h",
        packageJsonFilePath: srcDir + "/package.json",
        commitVersion: process.env.GIT_COMMIT || ""
    };

    localCfg.packageVersion = getPackageVersion(localCfg.packageJsonFilePath);

    grunt.initConfig({
        pkg: grunt.file.readJSON(srcDir + "/package.json"),
        clean: {
            build: {
                src: [outDir]
            },
            sourceSolidApi17Jar: {
                src: [outDir + "/framework/assets/bindings/WORKINGDIR/"]
            }
        },
        mkdir: {
            build: {
                options: {
                    create: [outDir]
                }
            },
            solidApi17JarDir: {
                options: {
                    mode: 0777,
                    create: [outDir + "/framework/assets/bindings/WORKINGDIR/"]
                }
            },
        },
        copy: {
            pkgDef: {
                src: [
                    srcDir + "/package.json"
                ],
                dest: outDir + "/",
                options: {
                    process: updatePackageVersion
                }
            },
            shimProject: {
                expand: true,
                dot: true,
                cwd: srcDir + "/Build/Shim/",
                src: [
                    "**/*.*"
                ],
                dest: outDir + "/"
            },
            baseLibJars: {
                expand: true,
                cwd: generatorDir + "/jars/",
                src: [
                    "*.jar",
                    "!android17.jar"
                ],
                dest: outDir + "/framework/libs/"
            },
            internalFolder: {
                expand: true,
                cwd: srcDir + "/kimera/assets",
                src: ["internal/**/*.*"],
                dest: outDir + "/framework/assets"
            },
            metadata: {
                expand: true,
                cwd: generatorDir + "/bin/",
                src: ["*.dat"],
                dest: outDir + "/framework/assets/metadata"
            },
            normalJars: {
                expand: true,
                src: "*.jar",
                cwd: jarCreationDir + "/dist/",
                dest: outDir + "/framework/libs/"
            },
            nativeBridge: {
                expand: true,
                cwd: srcDir + "/kimera/",
                src: [
                    "./libs/armeabi-v7a/libNativeScript.so",
                    "./libs/x86/libNativeScript.so"
                ],
                dest: outDir + "/framework/"
            },
            updateVersionFile: {
                expand: true,
                flatten: true,
                src: localCfg.runtimeVersionHFile,
                dest: localCfg.jniDir,
                options: {
                    process: function(content, srcPath)
                    {
                        var updatedContent = content;
                        updatedContent = updatedContent.replace(/RUNTIME_VERSION_PLACEHOLDER/ig, localCfg.packageVersion);
                        updatedContent = updatedContent.replace(/RUNTIME_COMMIT_SHA_PLACEHOLDER/ig, localCfg.commitVersion);

                        return updatedContent;
                    }
                }
            },
            updateManifestFile: {
                expand: true,
                flatten: true,
                src: "./kimera/manifest.mf",
                dest: "./kimera/",
                options: {
                    process: function(content, srcPath) {
                        var updatedContent = content;
                        updatedContent = updatedContent.replace(/RUNTIME_VERSION_PLACEHOLDER/ig, localCfg.packageVersion);
                        updatedContent = updatedContent.replace(/RUNTIME_COMMIT_SHA_PLACEHOLDER/ig, localCfg.commitVersion);

                        return updatedContent;
                    }
                }
            }
        },
        replace: {
            ndk_configuration: {
                options: {
                    patterns: [
                        {
                            //match: "^APP_OPTIM\\s*?:=\\s*?debug$",
                            match: /APP_OPTIM\s*?\:=\s*?debug/ig,
                            replacement: "APP_OPTIM := release"
                        }
                    ]
                },
                files: [
                    {src: srcDir + "/kimera/jni/Application.mk", dest: srcDir + "/kimera/jni/Application.mk"}
                ]
            }
        },
        exec: {
            npmPack: {
                cmd: "npm pack",
                cwd: outDir + "/"
            },
            generateNormalJars: {
                cmd: "grunt",
                cwd: jarCreationDir
            },
            generateMetadata:
            {
                cmd: "grunt",
                cwd: generatorDir
            },
            createNativeBridgeAntBuildFiles: {
                cmd: "android update project --target 1 --name NativeScriptBridge --path ./",
                cwd: srcDir + "/kimera/"
            },
            revertToCustomizedBuildFiles: {
                cmd: "git checkout -- kimera/project.properties kimera/proguard-project.txt",
            },
            revert_ndk_configuration: {
                cmd: "git checkout -- kimera/jni/Application.mk",
            },
            buildNativeBridge: {
                cmd: "ndk-build",
                cwd: srcDir + "/kimera/"
            },
            buildNativeBridgeClasses: {
                cmd: "ant release",
                cwd: srcDir + "/kimera/"
            },
            setupGeneratorNPM: {
                cmd: "npm install",
                cwd: generatorDir
            },
            setupJarCreationNPM: {
                cmd: "npm install",
                cwd: jarCreationDir
            },
            splitSolidApi17Jar: {
                //TODO: Get the dx.jar as an environment variable, should be $ANDROID_SDK_ROOT/build-tools/xx.x.x/lib/dx.jar
                cmd: "java -jar /var/lib/android-sdk-23/build-tools/20.0.0/lib/dx.jar --dex --multi-dex --output=./ ./WORKINGDIR/",
                cwd: outDir + "/framework/assets/bindings/"
            },
            ensureOriginalVersionFile: {
                cmd: "git checkout -- " + localCfg.runtimeVersionHFile
            },
            jarNativeBridge: {
                cmd: "jar cfm ../../../" + generatorDir +"/jars/nativescript.jar ../../manifest.mf com",
                cwd: srcDir + "/kimera/bin/classes/"
            }
        },
        rename: {
            solidApi17Jar: {
                src: jarCreationDir + "/dist/api17-bindings.jar",
                dest: outDir + "/framework/assets/bindings/WORKINGDIR/api17-bindings.jar"
            },
            dexFile: {
                src: "__DUMMY__",
                dest: "__DUMMY__"
            }
        }
    });

    grunt.registerTask("renameDexFile",
                        "Renames the dex file by increasing its number suffix",
                        function(suffixParam) {
                            var suffix = parseInt(suffixParam);
                            var oldSuffix = suffix.toString();
                            if (suffix == 1) {
                                oldSuffix = "";
                            }
                            
                            var renameTask = ["rename", "dexFile"];
                            var originalConfig = grunt.config(renameTask);
                            var newconfig = grunt.util._.clone(originalConfig);
                            newconfig.src = outDir + "/framework/assets/bindings/classes" + oldSuffix + ".dex";
                            newconfig.dest = outDir + "/framework/assets/bindings/classes" + (suffix+1).toString() + ".dex";
                            grunt.config(renameTask, newconfig);

                            grunt.task.run([
                                renameTask.join(":")
                            ]); 

                        });

    grunt.loadNpmTasks("grunt-contrib-clean");
    grunt.loadNpmTasks("grunt-contrib-copy");
    grunt.loadNpmTasks("grunt-mkdir");
    grunt.loadNpmTasks("grunt-exec");
    grunt.loadNpmTasks("grunt-replace");
    grunt.loadNpmTasks("grunt-contrib-rename");

    grunt.registerTask("prepareChildren", [
                            "exec:setupGeneratorNPM",
                            "exec:setupJarCreationNPM"
                        ]);

    grunt.registerTask("updateVersionFile", [
        "exec:ensureOriginalVersionFile",
        "copy:updateVersionFile"
    ]);

    grunt.registerTask("generateBindingsAndMetadata", [
                            "copy:internalFolder",
                            "exec:createNativeBridgeAntBuildFiles",
                            "exec:revertToCustomizedBuildFiles",
                            "exec:buildNativeBridgeClasses",
                            "copy:updateManifestFile",
                            "exec:jarNativeBridge",
                            "exec:generateMetadata",
                            "copy:metadata",
                            "exec:generateNormalJars",
                            "generateApi17Jars",
                            "copy:normalJars"
                        ]);

    grunt.registerTask("generateNativeBridge", [
                            "replace:ndk_configuration",
                            "updateVersionFile",
                            "exec:buildNativeBridge",
                            "copy:nativeBridge",
                            "exec:revert_ndk_configuration"
                       ]);

    grunt.registerTask("generateApi17Jars", [
                "mkdir:solidApi17JarDir",
                "rename:solidApi17Jar",
                "exec:splitSolidApi17Jar",
                "clean:sourceSolidApi17Jar",
                "renameDexFile:3",
                "renameDexFile:2",
                "renameDexFile:1"
            ]);

    grunt.registerTask("default", [
                            "prepareChildren",
                            "clean:build",
                            "mkdir:build",
                            "copy:shimProject",
                            "copy:pkgDef",
                            "generateNativeBridge",
                            "generateBindingsAndMetadata",
                            "copy:baseLibJars",
                            "exec:npmPack"
                        ]);

}
