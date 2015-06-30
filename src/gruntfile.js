//Builds the NativeScript runtime. Produces the nativescript.jar and the two .so files
// to the dist folder as an output.

module.exports = function(grunt) {

    var pathModule = require("path");

    var args = {
        buildVersion: grunt.option("build-version") || "0.0.1",
        commitVersion: grunt.option("commit-version") || process.env.GIT_COMMIT || ""
    };

    var localCfg = {
        rootDir: ".",
        outDir: "./dist",
        commitVersion: args.commitVersion,
    };

    localCfg.jniDir = pathModule.join(localCfg.rootDir, "/jni"),
    localCfg.runtimeVersionHFile = pathModule.join(localCfg.rootDir, "/jni/Version.h"),
    localCfg.applicationMkFile = pathModule.join(localCfg.rootDir, "/jni/Application.mk"),

    grunt.initConfig({
        clean: {
            build: {
                src: [localCfg.outDir]
            },
            builtCPPRuntime: {
                src: [
                        pathModule.join(localCfg.rootDir, "libs", "x86"),
                        pathModule.join(localCfg.rootDir, "libs", "armeabi-v7a")
                     ]
            }
        },
        mkdir: {
            build: {
                options: {
                    create: [localCfg.outDir]
                }
            }
        },
        copy: {
            CPPRuntime: {
                expand: true,
                cwd: pathModule.join(localCfg.rootDir, "libs"),
                src: [
                    "./armeabi-v7a/libNativeScript.so",
                    "./x86/libNativeScript.so"
                ],
                dest: pathModule.join(localCfg.outDir, "libs") + "/"
            },
            updateVersionFile: {
                expand: true,
                flatten: true,
                src: localCfg.runtimeVersionHFile,
                dest: localCfg.jniDir + "/",
                options: {
                    process: function(content, srcPath)
                    {
                        var updatedContent = content;
                        updatedContent = updatedContent.replace(/RUNTIME_VERSION_PLACEHOLDER/ig, localCfg.buildVersion);
                        updatedContent = updatedContent.replace(/RUNTIME_COMMIT_SHA_PLACEHOLDER/ig, localCfg.commitVersion);

                        return updatedContent;
                    }
                }
            },
            updateManifestFile: {
                expand: true,
                flatten: true,
                src: "./manifest.mf",
                dest: "./",
                options: {
                    process: function(content, srcPath) {
                        var updatedContent = content;
                        updatedContent = updatedContent.replace(/RUNTIME_VERSION_PLACEHOLDER/ig, localCfg.buildVersion);
                        updatedContent = updatedContent.replace(/RUNTIME_COMMIT_SHA_PLACEHOLDER/ig, localCfg.commitVersion);

                        return updatedContent;
                    }
                }
            },
            jarJavaRuntimeToDist: {
                src: "./bin/NativeScriptRuntime.jar",
                dest: "./dist/libs/nativescript.jar"
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
                    {src: localCfg.applicationMkFile, dest: localCfg.applicationMkFile}
                ]
            }
        },
        exec: {
            createBindingGeneratorAntBuildFiles: {
                cmd: "android update lib-project --target 1 --path ../binding-generator/Generator/",
                cwd: localCfg.rootDir + "/"
            },
            createJavaRuntimeAntBuildFiles: {
                cmd: "android update project --target 1 --name NativeScriptRuntime --path ./ --library ../binding-generator/Generator/",
                cwd: localCfg.rootDir + "/"
            },
            revertToCustomizedBuildFiles: {
                cmd: "git checkout -- ./project.properties ./proguard-project.txt",
            },
            revert_ndk_configuration: {
                cmd: "git checkout -- ./jni/Application.mk",
            },
            buildCPPRuntime: {
                cmd: "ndk-build",
                cwd: localCfg.rootDir + "/"
            },
            buildJavaRuntimeClasses: {
                cmd: "ant clean release",
                cwd: localCfg.rootDir + "/"
            },
            ensureOriginalVersionFile: {
                cmd: "git checkout -- " + localCfg.runtimeVersionHFile
            },
            ensureOriginalManifestFile: {
                cmd: "git checkout -- ./manifest.mf"
            },
            jarJavaRuntime: {
                //cmd: "jar cfm ../../" + "/dist/libs/nativescript.jar ../../manifest.mf com",
                cmd: "jar umf ./manifest.mf ./bin/NativeScriptRuntime.jar"
                //,cwd: pathModule.join(localCfg.rootDir, "/bin/classes")
            }
        }
    });

    grunt.loadNpmTasks("grunt-contrib-clean");
    grunt.loadNpmTasks("grunt-contrib-copy");
    grunt.loadNpmTasks("grunt-mkdir");
    grunt.loadNpmTasks("grunt-exec");
    grunt.loadNpmTasks("grunt-replace");

    grunt.registerTask("updateVersionFile", [
        "exec:ensureOriginalVersionFile",
        "copy:updateVersionFile"
    ]);

    grunt.registerTask("generateJavaRuntime", [
                            "exec:createBindingGeneratorAntBuildFiles",
                            "exec:createJavaRuntimeAntBuildFiles",
                            "exec:revertToCustomizedBuildFiles",
                            "exec:buildJavaRuntimeClasses",
                            "exec:ensureOriginalManifestFile",
                            "copy:updateManifestFile",
                            "exec:jarJavaRuntime",
                            "copy:jarJavaRuntimeToDist",
                            "exec:ensureOriginalManifestFile"
                        ]);

    grunt.registerTask("generateCPPRuntime", [
                            "replace:ndk_configuration",
                            "updateVersionFile",
                            "exec:buildCPPRuntime",
                            "copy:CPPRuntime",
                            "clean:builtCPPRuntime",
                            "exec:revert_ndk_configuration",
                            "exec:ensureOriginalVersionFile"
                       ]);

    grunt.registerTask("default", [
                            "clean:build",
                            "mkdir:build",
                            "generateCPPRuntime",
                            "generateJavaRuntime"
                        ]);

}
