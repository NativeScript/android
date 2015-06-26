//runs test app on device

module.exports = function(grunt) {

    var pathModule = require("path");

    var localCfg = {
        rootDir: ".",
        outDir: "./dist",
		binDir: "./bin/*"
    };

    grunt.initConfig({
		wait: {
			timeToRunTests: {
				options: {
					delay: 300000
				}
			}
		},
        clean: {
            build: {
                src: [localCfg.outDir]
            },
			metadata: {
				src: "./assets/metadata/*"
			},
			binDir: {
				src: [localCfg.binDir]
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
			deletePreviousResultXml: {
				cmd: "adb shell rm /sdcard/android_unit_test_results.xml"
			},
			createBuildXml: {
				cmd: "android update project --path ."
			},
			runAntRelease: {
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
			waitForUnitTestResultFile: {
				cmd: "node ./try_to_find_test_result_file.js",
				cwd: "."
			},
			copyResultToDist: {
				cmd: "adb pull /sdcard/android_unit_test_results.xml",
				cwd: localCfg.outDir
			}
        },
		copy: {
			//these .so files need to be in the src/libs folder because the test-app refers them 
			//later if we want to separate the tests from the build, these files can be taken from k:distributions ... stable/android-runtime/ ...
			generatedLibraries: {
				expand: true,
				cwd: "../src/dist/libs/",
				src: [
					"**/armeabi-v7a/*",
					"**/x86/*"
				],
				dest: "../src/libs/"
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
							"clean:binDir",
							"copy:generatedLibraries",
							"exec:createBuildXml",
							
							//currently runAntRelease step includes an ant custom build step which generates latest greatest metadata
							//currently we generate metadata using the target sdk declared in the AndroidManifest file and if the sdk is missing the build will fail
							"exec:runAntRelease", 
							
                            "exec:installApkOnDevice",
							"exec:deletePreviousResultXml",
                            "exec:startInstalledApk",
							// "wait:timeToRunTests",
							"exec:waitForUnitTestResultFile",
							"exec:copyResultToDist"
                        ]);

}
