#!/bin/sh
mkdir tmp
box64 aapt p -f -m -J src/main/java/ -S res -I $ANDROID_HOME/platforms/android-31/android.jar -M AndroidManifest.xml -F tmp/res.zip -A assets/
mvn package
d8 --min-api 31 ./target/nova-1.0.0.jar --output ./tmp/
cp -r libs/ tmp/
mv tmp/libs/ tmp/lib/
zip -r tmp/lib.zip tmp/lib/
java -cp $ANDROID_HOME/tools/lib/sdklib-25.3.1.jar com.android.sdklib.build.ApkBuilderMain target/source.apk -v -u -z tmp/res.zip -f tmp/classes.dex -z ./target/nova-1.0.0.jar -z tmp/lib.zip
rm -r tmp
apksigner sign -v --ks nova.keystore --in target/source.apk --out dist/nova.apk
apksigner verify -v dist/nova.apk
