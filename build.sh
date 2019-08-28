tar -zxvf opus-1.1.2.tar.gz 
wget https://dl.google.com/android/repository/android-ndk-r17c-linux-x86_64.zip
unzip android-ndk-r17c-linux-x86_64.zip
./android-ndk-r17c/ndk-build APP_BUILD_SCRIPT=Android.mk NDK_PROJECT_PATH=build
