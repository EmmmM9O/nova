#!/bin/sh
mkdir build
box64 aapt p -f -m -J src/main/java/ -S src/resources/ -I $ANDROID_HOME/platforms/android-31/android.jar -M AndroidManifest.xml -F build/res.zip -A assets/
mvn package
d8 --min-api 31 ./target/nova-1.0.0.jar --output ./build/
java -cp $ANDROID_HOME/tools/lib/sdklib-25.3.1.jar com.android.sdklib.build.ApkBuilderMain target/source.apk -v -u -z build/res.zip -f build/classes.dex -z ./target/nova-1.0.0.jar
rm -r build
apksigner sign -v --ks nova.keystore --in target/source.apk --out dist/nova.apk
apksigner verify -v dist/nova.apk
