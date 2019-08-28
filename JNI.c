// #include ""
#include <jni.h>
#include <opus.h>
#include <string.h>

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



JNIEXPORT void JNICALL Java_com_zhang_1ray_easyvoicecall_Worker_reInitEncoder
  (JNIEnv *env, jobject obj, jint sampleRate)
{
    int err;
    encoder_ = opus_encoder_create(sampleRate, 1, OPUS_APPLICATION_VOIP, &err);
}



JNIEXPORT void JNICALL Java_com_zhang_1ray_easyvoicecall_Worker_reInitDecoder
  (JNIEnv *env, jobject obj, jint sampleRate)
{
    int err;
    decoder_ = opus_decoder_create(sampleRate, 1, &err);
}



JNIEXPORT jbyteArray JNICALL Java_com_zhang_1ray_easyvoicecall_Worker_encode (JNIEnv *env, jobject obj, jbyteArray pcm) {
    jbyte* bytedata = (*env)->GetByteArrayElements(env, pcm, 0);
    jsize nbPcmBytes = (*env)->GetArrayLength(env, pcm);
    int nbBytes = opus_encode(encoder_, (const short*)bytedata, nbPcmBytes/2, cbits, MAX_PACKET_SIZE);
    if (nbBytes<0) {
        exit(-1);
    }
    jbyteArray jarrRV = (*env)->NewByteArray(env,cbits);

        
    (*env)->SetByteArrayRegion(env,jarrRV, 0,nbBytes,cbits);

    return jarrRV;
}



JNIEXPORT jbyteArray JNICALL Java_com_zhang_1ray_easyvoicecall_Worker_decode
  (JNIEnv *env, jobject obj, jbyteArray encodedData) {



  jbyte* bytedata = (*env)->GetByteArrayElements(env, encodedData, 0);
  jsize nbBytes = (*env)->GetArrayLength(env, encodedData);

    
  // parse the data
         //below is the return 's bytearray lens



        auto frame_size = opus_decode(decoder_, bytedata, nbBytes, out, MAX_FRAME_SIZE, 0);
        if (frame_size<0) {
            // fprintf(stderr, "decoder failed: %s\n", opus_strerror(frame_size));
            // return EXIT_FAILURE;
            exit(-1);
        }
    jbyteArray jarrRV = (*env)->NewByteArray(env,frame_size);

        
    (*env)->SetByteArrayRegion(env,jarrRV, 0,frame_size*2,out);

    return jarrRV;
}