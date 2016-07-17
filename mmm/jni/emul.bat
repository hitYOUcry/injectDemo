adb -s emulator-5554 push F:\repos\injectDemo\mmm\libs\armeabi\libhooker.so /data/
adb -s emulator-5554 push F:\repos\injectDemo\mmm\libs\armeabi\./inject /data/
adb -s emulator-5554 push F:\repos\injectDemo\mmm\hook_java\libs\armeabi\libentry.so /data/

adb -s emulator-5554 shell
