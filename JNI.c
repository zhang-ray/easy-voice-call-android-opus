// #include ""
#include <jni.h>
#include <opus.h>
#include <string.h>
#include<android/log.h>

#define TAG "EVC"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型   
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型  
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型  
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型   
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型 



#define MAX_FRAME_SIZE 6*960
#define MAX_PACKET_SIZE (3*1276)



OpusDecoder *decoder_ = NULL;
OpusEncoder *encoder_ = NULL;

unsigned char cbits[MAX_PACKET_SIZE];
short out[MAX_FRAME_SIZE];

// JNIEXPORT jstring JNICALL Java_com_zhang_1ray_easyvoicecall_Worker_getVersion
//   (JNIEnv *env, jobject obj, jstring javaString)
// {
//     return "v0.0.1";
// }



JNIEXPORT void JNICALL Java_com_zhang_1ray_easyvoicecall_Worker_reInitEncoder(JNIEnv *env, jobject obj, jint sampleRate){
    int err;
    encoder_ = opus_encoder_create(sampleRate, 1, OPUS_APPLICATION_VOIP, &err);
}



JNIEXPORT void JNICALL Java_com_zhang_1ray_easyvoicecall_Worker_reInitDecoder(JNIEnv *env, jobject obj, jint sampleRate){
    int err;
    decoder_ = opus_decoder_create(sampleRate, 1, &err);
}



JNIEXPORT jbyteArray JNICALL Java_com_zhang_1ray_easyvoicecall_Worker_encode (JNIEnv *env, jobject obj, jbyteArray pcm ) {
    auto bytedata = (*env)->GetByteArrayElements(env, pcm, 0);
    auto nbPcmBytes = (*env)->GetArrayLength(env, pcm);
    // LOGV("nbPcmBytes=%d", nbPcmBytes);
    // LOGV("encoder_=%x", encoder_);
    // LOGV("bytedata=%x", bytedata);
    int nbOpusBytes = opus_encode(encoder_, (const short*)bytedata, nbPcmBytes/2, cbits, MAX_PACKET_SIZE);
    // LOGV("nbBytes=%d", nbOpusBytes);
    if (nbOpusBytes<0) {
        LOGE("nbOpusBytes = %d", nbOpusBytes);
        LOGE("encode failed: %s\n", opus_strerror(nbOpusBytes));
        exit(-1);
    }
    auto jarrRV = (*env)->NewByteArray(env, nbOpusBytes);

        
    (*env)->SetByteArrayRegion(env, jarrRV, 0, nbOpusBytes, cbits);

    return jarrRV;
}



JNIEXPORT jbyteArray JNICALL Java_com_zhang_1ray_easyvoicecall_Worker_decode(JNIEnv *env, jobject obj, jbyteArray encodedData) {
    auto bytedata = (*env)->GetByteArrayElements(env, encodedData, 0);
    auto nbBytes = (*env)->GetArrayLength(env, encodedData);

    auto frame_size = opus_decode(decoder_, bytedata, nbBytes, out, MAX_FRAME_SIZE, 0);
    if (frame_size<0) {
        exit(-1);
    }
    auto jarrRV = (*env)->NewByteArray(env,frame_size*2);

    (*env)->SetByteArrayRegion(env,jarrRV, 0,frame_size*2,out);

    return jarrRV;
}