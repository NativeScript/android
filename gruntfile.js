module.exports = function(grunt) {

    var pathModule = require("path");

    var outDir = "./dist";
    var rootDir = ".";

    var args = {
        metadataDir: grunt.option("metadataDir") || "../android-metadata-generator/dist",
        libsDir: grunt.option("libsDir") || "./src/libs",
        dexBindingsDir: grunt.option("dexBindingsDir") || "../android-bindings/dexes"
    };

    var generatorDir = rootDir + "/Bindings/Generator";
    var jarCreationDir = rootDir + "/Bindings/JarCreation";

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
        jniDir: rootDir + "/src/jni",
        runtimeVersionHFile: rootDir + "/src/jni/Version.h",
        packageJsonFilePath: rootDir + "/package.json",
        commitVersion: process.env.GIT_COMMIT || ""
    };

    localCfg.packageVersion = getPackageVersion(localCfg.packageJsonFilePath);

    grunt.initConfig({
        pkg: grunt.file.readJSON(rootDir + "/package.json"),
        clean: {
            build: {
                src: [outDir]
            }
        },
        mkdir: {
            build: {
                options: {
                    create: [outDir]
                }
            }
        },
        copy: {
            pkgDef: {
                src: [
                    rootDir + "/package.json"
                ],
                dest: outDir + "/",
                options: {
                    process: updatePackageVersion
                }
            },
            templateProject: {
                expand: true,
                dot: true,
                cwd: pathModule.join(rootDir, "build/project-template"),
                src: [
                    "**/*.*"
                ],
                dest: pathModule.join(outDir, "framework") + "/"
            },
            internalFolder: {
                expand: true,
                cwd: pathModule.join(rootDir, "src/assets"),
                src: ["internal/**/*.*"],
                dest: pathModule.join(outDir, "framework/assets") + "/"
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
            CPPRuntime: {
                expand: true,
                cwd: rootDir + "/src/",
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
                src: "./src/manifest.mf",
                dest: "./src/",
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
                    {src: rootDir + "/src/jni/Application.mk", dest: rootDir + "/src/jni/Application.mk"}
                ]
            }
        },
        exec: {
            npmPack: {
                cmd: "npm pack",
                cwd: outDir + "/"
            },
            generateRuntime: {
                cmd: "npm install && grunt --verbose",
                cwd: "./src"
            }
        }
    });

    grunt.loadNpmTasks("grunt-contrib-clean");
    grunt.loadNpmTasks("grunt-contrib-copy");
    grunt.loadNpmTasks("grunt-mkdir");
    grunt.loadNpmTasks("grunt-exec");
    grunt.loadNpmTasks("grunt-replace");
    //TODO: NEEDED???
    grunt.loadNpmTasks("grunt-contrib-rename");

    grunt.registerTask("generateRuntime", [
                            "exec:generateRuntime"
                       ]);

    grunt.registerTask("generateMetadata", [
            ]);

    grunt.registerTask("default", [
                            "clean:build",
                            "mkdir:build",
                            "copy:templateProject",
                            "copy:pkgDef",
                            "generateRuntime",
//                            "generateMetadata",
//                            "collectBindings",
                            "exec:npmPack"
                        ]);

}
