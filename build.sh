#!/usr/bin/env bash
###############################################################################################
# Android Runtime build script for CI.
# This file is used by the CI only and it's not meant for regular development.
###############################################################################################

echo "Ensure adb is in PATH"
export PATH=$ANDROID_HOME/platform-tools:$PATH
adb version

echo "Update submodule"
git submodule update --init

echo "Cleanup old build and test artefacts"
rm -rf consoleLog.txt
rm -rf test-app/dist/*.xml

echo "Stopping running emulators if any"
for KILLPID in `ps ax | grep 'emulator' | grep -v 'grep' | awk ' { print $1;}'`; do kill -9 $KILLPID; done
for KILLPID in `ps ax | grep 'qemu' | grep -v 'grep' | awk ' { print $1;}'`; do kill -9 $KILLPID; done
for KILLPID in `ps ax | grep 'adb' | grep -v 'grep' | awk ' { print $1;}'`; do kill -9 $KILLPID; done

./gradlew cleanRuntime
if [ "$1" != 'unit_tests_only' ]; then
    echo "Building Android Runtime with paramerter packageVersion: $PACKAGE_VERSION and commit: $GIT_COMMIT"
    ./gradlew -PpackageVersion=$PACKAGE_VERSION -PgitCommitVersion=$GIT_COMMIT
    cp dist/tns-android-*.tgz dist/tns-android.tgz
else
    echo "Building Android Runtime for x86 unit tests with paramerter packageVersion: $PACKAGE_VERSION and commit: $GIT_COMMIT"
    ./gradlew -PpackageVersion=$PACKAGE_VERSION -PgitCommitVersion=$GIT_COMMIT -PskipUnoptimized -PonlyX86
    cp dist/tns-android-*.tgz dist/tns-android.tgz
fi

if [ "$2" != '' ]; then
    listOfEmulators=$2
else
    listOfEmulators="Emulator-Api19-Default"
fi

# Run static binding generator unit tests
./gradlew runSbgTests

for emulator in $listOfEmulators; do
    echo "Start emulator $emulator"
    $ANDROID_HOME/emulator/emulator -avd ${emulator} -wipe-data -gpu on&

    echo "Run Android Runtime unit tests for $emulator"
    $ANDROID_HOME/platform-tools/adb devices
    $ANDROID_HOME/platform-tools/adb -e logcat -c
    $ANDROID_HOME/platform-tools/adb -e logcat > consoleLog.txt&

    if [ "$1" != 'unit_tests_only' ]; then
        ./gradlew runtests
    else
        ./gradlew runtests -PonlyX86
    fi

    echo "Rename unit test result"
    (
        cd ./test-app/dist
        mv android_unit_test_results.xml $emulator.xml
    )

    echo "Stopping running emulators"
    for KILLPID in `ps ax | grep 'emulator' | grep -v 'grep' | awk ' { print $1;}'`; do kill -9 $KILLPID; done
    for KILLPID in `ps ax | grep 'qemu' | grep -v 'grep' | awk ' { print $1;}'`; do kill -9 $KILLPID; done
    for KILLPID in `ps ax | grep 'adb' | grep -v 'grep' | awk ' { print $1;}'`; do kill -9 $KILLPID; done
done

echo $cwd
cd $cwd