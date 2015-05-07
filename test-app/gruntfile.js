//runs test app on device

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
		wait: {
			timeToRunTests: {
				options: {
					delay: 20000
				}
			}
		},
        clean: {
            build: {
                src: [localCfg.outDir]
            }
        },
        mkdir: {
            build: {
                options: {
                    create: [localCfg.outDir]
                }
            }
        },
        exec: {
			createBuildXml: {
				cmd: "android update project --path ."
			},
			runAntCleanRelease: {
				cmd: "ant release"
			},
			installApkOnDevice: {
				cmd: "node ./tasks/deploy-apk.js ./bin/NativeScriptActivity-release.apk",
				cwd: "."
			},
			startInstalledApk: {
				cmd: "adb shell am start -n com.tns.android_runtime_testapp/com.tns.NativeScriptActivity -a android.intent.action.MAIN -c android.intent.category.LAUNCHER",
				cwd: "./bin"
			},
			copyResultToDist: {
				cmd: "adb pull /sdcard/android_unit_test_results.xml",
				cwd: localCfg.outDir
			}
        }
    });

    grunt.loadNpmTasks("grunt-contrib-clean");
    grunt.loadNpmTasks("grunt-contrib-copy");
    grunt.loadNpmTasks("grunt-mkdir");
    grunt.loadNpmTasks("grunt-exec");
    grunt.loadNpmTasks("grunt-replace");
	grunt.loadNpmTasks('grunt-wait');

    grunt.registerTask("default", [
                            "clean:build",
                            "mkdir:build",
							"exec:createBuildXml",
							"exec:runAntCleanRelease", 
                            "exec:installApkOnDevice",
                            "exec:startInstalledApk",
							"wait:timeToRunTests",
							"exec:copyResultToDist"
                        ]);

}
