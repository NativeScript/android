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
rm -rf test-app/dist/android_unit_test_results.xml

echo "Stopping running emulators if any"
for KILLPID in `ps ax | grep 'emulator' | grep -v 'grep' | awk ' { print $1;}'`; do kill -9 $KILLPID; done
for KILLPID in `ps ax | grep 'qemu' | grep -v 'grep' | awk ' { print $1;}'`; do kill -9 $KILLPID; done
for KILLPID in `ps ax | grep 'adb' | grep -v 'grep' | awk ' { print $1;}'`; do kill -9 $KILLPID; done

echo "Start emulator"
$ANDROID_HOME/tools/emulator -avd Emulator-Api19-Default -wipe-data -gpu on &

echo "Building Android Runtime with paramerter packageVersion: $PACKAGE_VERSION and commit: $GIT_COMMIT"
./gradlew -PpackageVersion=$PACKAGE_VERSION -PgitCommitVersion=$GIT_COMMIT

echo "Run Android Runtime unit tests"
cp dist/tns-android-*.tgz dist/tns-android.tgz
$ANDROID_HOME/platform-tools/adb devices
$ANDROID_HOME/platform-tools/adb -e logcat -c
$ANDROID_HOME/platform-tools/adb -e logcat > consoleLog.txt &
cd test-app
./gradlew runtest

echo "Stopping running emulators"
for KILLPID in `ps ax | grep 'emulator' | grep -v 'grep' | awk ' { print $1;}'`; do kill -9 $KILLPID; done || true
for KILLPID in `ps ax | grep 'qemu' | grep -v 'grep' | awk ' { print $1;}'`; do kill -9 $KILLPID; done || true
for KILLPID in `ps ax | grep 'adb' | grep -v 'grep' | awk ' { print $1;}'`; do kill -9 $KILLPID &> /dev/null; done || true

