#include <jni.h>
#include <string>
#include <cassert>
#include <rmw/rmw.h>
#include <rcl/error_handling.h>
#include <rcl/rcl.h>
#include <rcl/node.h>
#include <rosidl_generator_c/message_type_support.h>
#include <cstdlib>

#include "org_ros2_rcljava_RCLJava.h"

JNIEXPORT void JNICALL Java_org_ros2_rcljava_RCLJava_nativeRCLJavaInit
  (JNIEnv *env, jclass) {
  // TODO(esteve): parse args
  rcl_ret_t ret = rcl_init(0, nullptr, rcl_get_default_allocator());
  if (ret != RCL_RET_OK) {
    jclass exception_class;
    const char *class_name = "java/lang/IllegalStateException";
    std::string message("Failed to init: " + std::string(rcl_get_error_string_safe()));

    exception_class = env->FindClass(class_name);

    assert(exception_class != nullptr);

    env->ThrowNew(exception_class, message.c_str());
  }
}

JNIEXPORT jlong JNICALL Java_org_ros2_rcljava_RCLJava_nativeCreateNodeHandle
  (JNIEnv *env, jclass, jstring jnode_name) {

  const char * node_name_tmp = env->GetStringUTFChars(jnode_name, 0);

  std::string node_name(node_name_tmp);

  env->ReleaseStringUTFChars(jnode_name, node_name_tmp);

  rcl_node_t * node = (rcl_node_t *)malloc(sizeof(rcl_node_t));
  node->impl = nullptr;
  rcl_node_options_t default_options = rcl_node_get_default_options();
  rcl_ret_t ret = rcl_node_init(node, node_name.c_str(), &default_options);
  if (ret != RCL_RET_OK) {
    jclass exception_class;
    const char *class_name = "java/lang/IllegalStateException";
    std::string message("Failed to create node: " + std::string(rcl_get_error_string_safe()));

    exception_class = env->FindClass(class_name);

    assert(exception_class != nullptr);

    env->ThrowNew(exception_class, message.c_str());

    return -1;
  }
  jlong node_handle = reinterpret_cast<jlong>(node);
  return node_handle;
}

JNIEXPORT jstring JNICALL Java_org_ros2_rcljava_RCLJava_nativeGetRMWIdentifier
  (JNIEnv *env, jclass) {
  const char * rmw_implementation_identifier = rmw_get_implementation_identifier();

  return env->NewStringUTF(rmw_implementation_identifier);
}

JNIEXPORT jboolean JNICALL Java_org_ros2_rcljava_RCLJava_nativeOk
  (JNIEnv *, jclass) {
  return rcl_ok();
}

JNIEXPORT jlong JNICALL Java_org_ros2_rcljava_RCLJava_nativeGetZeroInitializedWaitSet
  (JNIEnv *env, jclass) {
  rcl_wait_set_t * wait_set = (rcl_wait_set_t *)malloc(sizeof(rcl_wait_set_t));
  wait_set->subscriptions = nullptr;
  wait_set->size_of_subscriptions = 0;
  wait_set->guard_conditions = nullptr;
  wait_set->size_of_guard_conditions = 0;
  wait_set->timers = nullptr;
  wait_set->size_of_timers = 0;
  wait_set->clients = nullptr;
  wait_set->size_of_clients = 0;
  wait_set->services = nullptr;
  wait_set->size_of_services = 0;
  wait_set->impl = nullptr;
  jlong wait_set_handle = reinterpret_cast<jlong>(wait_set);
  return wait_set_handle;
}

JNIEXPORT void JNICALL Java_org_ros2_rcljava_RCLJava_nativeWaitSetInit
  (JNIEnv *env, jclass, jlong wait_set_handle, jint number_of_subscriptions,
   jint number_of_guard_conditions, jint number_of_timers) {
  jint number_of_clients = 0;
  jint number_of_services = 0;

  rcl_wait_set_t * wait_set = reinterpret_cast<rcl_wait_set_t *>(wait_set_handle);

  rcl_ret_t ret = rcl_wait_set_init(
    wait_set, number_of_subscriptions, number_of_guard_conditions, number_of_timers,
    number_of_clients, number_of_services, rcl_get_default_allocator());
  if (ret != RCL_RET_OK) {
    jclass exception_class;
    const char *class_name = "java/lang/IllegalStateException";
    std::string message("Failed to initialize wait set: " + std::string(rcl_get_error_string_safe()));

    exception_class = env->FindClass(class_name);

    assert(exception_class != nullptr);

    env->ThrowNew(exception_class, message.c_str());
  }
}

JNIEXPORT void JNICALL Java_org_ros2_rcljava_RCLJava_nativeWaitSetClearSubscriptions
  (JNIEnv *env, jclass, jlong wait_set_handle) {

  rcl_wait_set_t * wait_set = reinterpret_cast<rcl_wait_set_t *>(wait_set_handle);
  rcl_ret_t ret = rcl_wait_set_clear_subscriptions(wait_set);
  if (ret != RCL_RET_OK) {
    jclass exception_class;
    const char *class_name = "java/lang/IllegalStateException";
    std::string message("Failed to clear subscriptions from wait set: " + std::string(rcl_get_error_string_safe()));

    exception_class = env->FindClass(class_name);

    assert(exception_class != nullptr);

    env->ThrowNew(exception_class, message.c_str());
  }
}

JNIEXPORT void JNICALL Java_org_ros2_rcljava_RCLJava_nativeWaitSetAddSubscription
  (JNIEnv *env, jclass, jlong wait_set_handle, jlong subscription_handle) {

  rcl_wait_set_t * wait_set = reinterpret_cast<rcl_wait_set_t *>(wait_set_handle);
  rcl_subscription_t * subscription = reinterpret_cast<rcl_subscription_t *>(subscription_handle);
  rcl_ret_t ret = rcl_wait_set_add_subscription(wait_set, subscription);
  if (ret != RCL_RET_OK) {
    jclass exception_class;
    const char *class_name = "java/lang/IllegalStateException";
    std::string message("Failed to add subscription to wait set: " + std::string(rcl_get_error_string_safe()));

    exception_class = env->FindClass(class_name);

    assert(exception_class != nullptr);

    env->ThrowNew(exception_class, message.c_str());
  }
}

JNIEXPORT void JNICALL Java_org_ros2_rcljava_RCLJava_nativeWait
  (JNIEnv *env, jclass, jlong wait_set_handle) {
  rcl_wait_set_t * wait_set = reinterpret_cast<rcl_wait_set_t *>(wait_set_handle);
  rcl_ret_t ret = rcl_wait(wait_set, RCL_S_TO_NS(1));
  if (ret != RCL_RET_OK && ret != RCL_RET_TIMEOUT) {
    jclass exception_class;
    const char *class_name = "java/lang/IllegalStateException";
    std::string message("Failed to wait on wait set: " + std::string(rcl_get_error_string_safe()));

    exception_class = env->FindClass(class_name);

    assert(exception_class != nullptr);

    env->ThrowNew(exception_class, message.c_str());
  }
}

JNIEXPORT jobject JNICALL Java_org_ros2_rcljava_RCLJava_nativeTake
  (JNIEnv *env, jclass, jlong subscription_handle, jclass jmessage_class) {

  rcl_subscription_t * subscription = reinterpret_cast<rcl_subscription_t *>(subscription_handle);

  jmethodID jfrom_mid = env->GetStaticMethodID(jmessage_class, "getFromJavaConverter", "()J");
  jlong jfrom_java_converter = env->CallStaticLongMethod(jmessage_class, jfrom_mid);

  using convert_from_java_signature = void * (*)(jobject);
  convert_from_java_signature convert_from_java =
    reinterpret_cast<convert_from_java_signature>(jfrom_java_converter);

  jmethodID jconstructor = env->GetMethodID(jmessage_class, "<init>", "()V");
  jobject jmsg = env->NewObject(jmessage_class, jconstructor);

  void * taken_msg = convert_from_java(jmsg);

  rcl_ret_t ret = rcl_take(subscription, taken_msg, nullptr);

  if (ret != RCL_RET_OK && ret != RCL_RET_SUBSCRIPTION_TAKE_FAILED) {
    jclass exception_class;
    const char *class_name = "java/lang/IllegalStateException";
    std::string message("Failed to take from a subscription: " + std::string(rcl_get_error_string_safe()));

    exception_class = env->FindClass(class_name);

    assert(exception_class != nullptr);

    env->ThrowNew(exception_class, message.c_str());
  }

  if (ret != RCL_RET_SUBSCRIPTION_TAKE_FAILED) {
    jmethodID jto_mid = env->GetStaticMethodID(jmessage_class, "getToJavaConverter", "()J");
    jlong jto_java_converter = env->CallStaticLongMethod(jmessage_class, jto_mid);

    using convert_to_java_signature = jobject (*)(void *);
    convert_to_java_signature convert_to_java =
      reinterpret_cast<convert_to_java_signature>(jto_java_converter);

    jobject jtaken_msg = convert_to_java(taken_msg);

    return jtaken_msg;
  }

  return nullptr;
}

JNIEXPORT void JNICALL Java_org_ros2_rcljava_RCLJava_nativeShutdown
  (JNIEnv *env, jclass) {
  rcl_ret_t ret = rcl_shutdown();
  if (ret != RCL_RET_OK) {
    jclass exception_class;
    const char *class_name = "java/lang/IllegalStateException";
    std::string message("Failed to shutdown: " + std::string(rcl_get_error_string_safe()));

    exception_class = env->FindClass(class_name);

    assert(exception_class != nullptr);

    env->ThrowNew(exception_class, message.c_str());
  }
}
