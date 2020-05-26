#!/usr/bin/env bash
###############################################################################################
# Android Runtime build script for CI.
# This file is used by the CI only and it's not meant for regular development.
###############################################################################################

echo "Ensure adb is in PATH"
export PATH="$ANDROID_HOME/platform-tools:$PATH"
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
    echo "Building Android Runtime with paramerter packageVersion: $ANDROID_PACKAGE_VERSION and commit: $GIT_COMMIT"
    if [ "$NO_CCACHE" == 'true' ]; then
        ./gradlew -PpackageVersion=$ANDROID_PACKAGE_VERSION -PgitCommitVersion=$GIT_COMMIT -PnoCCache
    else
        ./gradlew -PpackageVersion=$ANDROID_PACKAGE_VERSION -PgitCommitVersion=$GIT_COMMIT
    fi

    cp dist/tns-android-*.tgz dist/tns-android.tgz
else
    echo "Building Android Runtime for x86 unit tests with paramerter packageVersion: $ANDROID_PACKAGE_VERSION and commit: $GIT_COMMIT"
    if [ "$NO_CCACHE" == 'true' ]; then
        ./gradlew -PpackageVersion=$ANDROID_PACKAGE_VERSION -PgitCommitVersion=$GIT_COMMIT -PskipUnoptimized -PonlyX86 -PnoCCache
    else
        ./gradlew -PpackageVersion=$ANDROID_PACKAGE_VERSION -PgitCommitVersion=$GIT_COMMIT -PskipUnoptimized -PonlyX86
    fi
    cp dist/tns-android-*.tgz dist/tns-android.tgz
fi

if [ "$2" != '' ]; then
    listOfEmulators=$2
else
    listOfEmulators="Emulator-Api28-Google Emulator-Api23-Default Emulator-Api19-Default"
fi

# Run static binding generator unit tests
./gradlew runSbgTests

for emulator in $listOfEmulators; do
    echo "Start emulator $emulator"
    $ANDROID_HOME/emulator/emulator -avd ${emulator} -verbose -wipe-data -gpu on&
    find ~/.android/avd/${emulator}.avd -type f -name 'config.ini' -exec cat {} +

    echo "Run Android Runtime unit tests for $emulator"
    $ANDROID_HOME/platform-tools/adb wait-for-device
    $ANDROID_HOME/platform-tools/adb -s emulator-5554 logcat -c
    $ANDROID_HOME/platform-tools/adb -s emulator-5554 logcat > consoleLog.txt&
    $ANDROID_HOME/platform-tools/adb -s emulator-5554 logcat > consoleLog$emulator.txt&

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
