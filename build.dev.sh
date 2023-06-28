#!/usr/bin/env bash
# smaller version of build.sh that sets the commit hash to the current git commit hash and uses the package.json version

echo "Ensure adb is in PATH"
export PATH="$ANDROID_HOME/platform-tools:$PATH"
adb version

echo "Update submodule"
git submodule update --init

echo "Cleanup old build and test artefacts"
rm -rf consoleLog.txt
rm -rf test-app/dist/*.xml

./gradlew cleanRuntime

./gradlew -PgitCommitVersion=$(git rev-parse HEAD)
cp dist/nativescript-android-*.tgz dist/nativescript-android.tgz

