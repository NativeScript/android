module.exports = function(grunt) {

    var pathModule = require("path");

    var outDir = "./dist";
    var rootDir = ".";

    var args = {
        metadataGen: grunt.option("metadataGen") || "../android-metadata-generator/dist/tns-android-metadata-generator-0.0.1.tgz",
        libsDir: grunt.option("libsDir") || "./src/libs",
        devmode: (grunt.option("devmode") == true) || false,
        metadataGenSrc: grunt.option("metadataGenSrc") || "../android-metadata-generator"
    };

    //var generatorDir = rootDir + "/Bindings/Generator";
    //var jarCreationDir = rootDir + "/Bindings/JarCreation";

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
        commitVersion: process.env.GIT_COMMIT || "",
        metadataGenPath: args.metadataGen,
        androidSrcJar: pathModule.join(process.env.ANDROID_HOME, "platforms/android-17/android.jar"),
        localAndroidJar: pathModule.join(rootDir, "src", "libs", "android.jar"),
        runtimeDir: pathModule.join(rootDir, "src"),
        libsDir: pathModule.join(rootDir, "src", "libs"),
        runtimeBinariesDir: pathModule.join(rootDir, "src", "dist"),
        metadataRuntimeJarSrc: pathModule.join(rootDir, "src", "dist", "libs", "nativescript.jar"),
        metadataRuntimeJar: pathModule.join(rootDir, "src", "libs", "nativescript.jar")
    };

    localCfg.packageVersion = getPackageVersion(localCfg.packageJsonFilePath);

    grunt.initConfig({
        pkg: grunt.file.readJSON(rootDir + "/package.json"),
        clean: {
            build: {
                src: [outDir]
            },
            androidLib: {
                src: localCfg.localAndroidJar
            },
            metadataGenRuntimeJar: {
                src: localCfg.metadataRuntimeJar
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
            collectRuntime: {
                expand: true,
                cwd: localCfg.runtimeBinariesDir,
                src: [
                    "**/*.*",
                    "**/*"
                ],
                dest: outDir + "/framework/"
            },
            collectLibs: {
                expand: true,
                cwd: localCfg.libsDir,
                src: ["**/*.*"],
                dest: pathModule.join(outDir, "framework", "libs") + "/"
            },
            runtimeJarToLibs: {
                src: [localCfg.metadataRuntimeJarSrc],
                dest: localCfg.metadataRuntimeJar
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
            },
            androidLib: {
                src: localCfg.androidSrcJar,
                dest: localCfg.localAndroidJar
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
            },
            buildMetadataGenerator: {
                cmd: "npm install && grunt --verbose",
                cwd: args.metadataGenSrc
            },
            installMetadataGenerator: {
                cmd: "npm install " + localCfg.metadataGenPath
            },
            runMetadataGenerator: {
                cmd: pathModule.normalize("./node_modules/.bin/generate-metadata") + " " + localCfg.libsDir + " ./dist/framework/assets/metadata"
            },
			runTests: {
				cmd: "npm install && grunt --verbose",
                cwd: "./test-app"
			},
			antCleanBindingGenerator: {
				cmd: "ant clean",
				cwd: "./binding-generator/Generator/"
			},
			antCleanRunTime: {
				cmd: "ant clean",
				cwd: "./src/"
			},
        }
    });

    grunt.loadNpmTasks("grunt-contrib-clean");
    grunt.loadNpmTasks("grunt-contrib-copy");
    grunt.loadNpmTasks("grunt-mkdir");
    grunt.loadNpmTasks("grunt-exec");
    grunt.loadNpmTasks("grunt-replace");

    grunt.registerTask("generateRuntime", [
				"exec:generateRuntime"
            ]);

    grunt.registerTask("generateMetadata", [
                "exec:installMetadataGenerator",
                "copy:androidLib",
                "copy:runtimeJarToLibs",
                "exec:runMetadataGenerator",
                "clean:androidLib",
                "clean:metadataGenRuntimeJar"
            ]);

    grunt.registerTask("buildMetadataGenerator", (function(){
        if (args.devmode) {
            localCfg.metadataGenPath = pathModule.join(args.metadataGenSrc, "dist", "tns-android-metadata-generator-*.tgz");
            grunt.config(["exec", "installMetadataGenerator"], { cmd: "npm install " + localCfg.metadataGenPath } );

            return [
                "exec:buildMetadataGenerator"
            ];
        } else {
            return [];
        }
    })());
	
	grunt.registerTask("test", [
				"exec:antCleanBindingGenerator",
				"exec:antCleanRunTime",
				"exec:runTests"
            ]);

    grunt.registerTask("default", [
                            "clean:build",
                            "mkdir:build",
                            "copy:templateProject",
                            "copy:internalFolder",
                            "copy:pkgDef",
                            "generateRuntime",
                            "buildMetadataGenerator",
                            "generateMetadata",
                            "copy:collectRuntime",
                            "copy:collectLibs",
                            //"copy:collectBindings",
                            "exec:npmPack"
            ]);

}
