#!/bin/sh
mkdir tmp
aapt p -f -m -J src/main/java/ -S res -I $ANDROID_HOME/platforms/android-31/android.jar -M AndroidManifest.xml -F tmp/res.zip -A assets/
mvn package
d8 --min-api 31 ./target/graphics-example-1.0.0.jar --output ./tmp/
cp -r libs/ tmp/
mv tmp/libs/ tmp/lib/
cd tmp
zip -r lib.zip lib/*
cd ..
java -cp $ANDROID_HOME/tools/lib/sdklib-25.3.1.jar com.android.sdklib.build.ApkBuilderMain target/source.apk -v -u -z tmp/res.zip -f tmp/classes.dex -z ./target/graphics-example-1.0.0.jar -z tmp/lib.zip
rm -r tmp
apksigner sign -v --ks nova.keystore --in target/source.apk --out dist/example.apk
apksigner verify -v dist/example.apk
