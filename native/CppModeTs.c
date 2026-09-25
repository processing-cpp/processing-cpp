#include <jni.h>
#include <string.h>
#include <stdlib.h>
#include "vendor/tree-sitter/lib/include/tree_sitter/api.h"

extern const TSLanguage *tree_sitter_cpp(void);

static jlongArray pack_node(JNIEnv *env, TSNode node) {
    jlongArray arr = (*env)->NewLongArray(env, 4);
    if (!arr) return NULL;
    jlong buf[4];
    memcpy(buf, &node, sizeof(TSNode));
    (*env)->SetLongArrayRegion(env, arr, 0, 4, buf);
    return arr;
}

static TSNode unpack_node(JNIEnv *env, jlongArray arr) {
    TSNode node;
    jlong buf[4];
    (*env)->GetLongArrayRegion(env, arr, 0, 4, buf);
    memcpy(&node, buf, sizeof(TSNode));
    return node;
}

JNIEXPORT jlong JNICALL Java_processing_mode_cpp_Ts_parse(JNIEnv *env, jclass cls, jstring jsource) {
    if (!jsource) return 0L;
    const char *src = (*env)->GetStringUTFChars(env, jsource, NULL);
    if (!src) return 0L;
    jsize len = (*env)->GetStringUTFLength(env, jsource);
    TSParser *parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_cpp());
    TSTree *tree = ts_parser_parse_string(parser, NULL, src, (uint32_t)len);
    ts_parser_delete(parser);
    (*env)->ReleaseStringUTFChars(env, jsource, src);
    return (jlong)(intptr_t)tree;
}

JNIEXPORT void JNICALL Java_processing_mode_cpp_Ts_freeTree(JNIEnv *env, jclass cls, jlong ptr) {
    if (ptr) ts_tree_delete((TSTree *)(intptr_t)ptr);
}

JNIEXPORT jlongArray JNICALL Java_processing_mode_cpp_Ts_rootNode(JNIEnv *env, jclass cls, jlong ptr) {
    if (!ptr) return NULL;
    TSNode root = ts_tree_root_node((TSTree *)(intptr_t)ptr);
    return pack_node(env, root);
}

JNIEXPORT jstring JNICALL Java_processing_mode_cpp_Ts_nodeType(JNIEnv *env, jclass cls, jlongArray arr) {
    const char *type = ts_node_type(unpack_node(env, arr));
    return type ? (*env)->NewStringUTF(env, type) : NULL;
}

JNIEXPORT jint JNICALL Java_processing_mode_cpp_Ts_nodeStartByte(JNIEnv *env, jclass cls, jlongArray arr) {
    return (jint)ts_node_start_byte(unpack_node(env, arr));
}

JNIEXPORT jint JNICALL Java_processing_mode_cpp_Ts_nodeEndByte(JNIEnv *env, jclass cls, jlongArray arr) {
    return (jint)ts_node_end_byte(unpack_node(env, arr));
}

JNIEXPORT jint JNICALL Java_processing_mode_cpp_Ts_nodeStartRow(JNIEnv *env, jclass cls, jlongArray arr) {
    return (jint)ts_node_start_point(unpack_node(env, arr)).row;
}

JNIEXPORT jint JNICALL Java_processing_mode_cpp_Ts_nodeStartCol(JNIEnv *env, jclass cls, jlongArray arr) {
    return (jint)ts_node_start_point(unpack_node(env, arr)).column;
}

JNIEXPORT jint JNICALL Java_processing_mode_cpp_Ts_nodeEndRow(JNIEnv *env, jclass cls, jlongArray arr) {
    return (jint)ts_node_end_point(unpack_node(env, arr)).row;
}

JNIEXPORT jint JNICALL Java_processing_mode_cpp_Ts_nodeEndCol(JNIEnv *env, jclass cls, jlongArray arr) {
    return (jint)ts_node_end_point(unpack_node(env, arr)).column;
}

JNIEXPORT jint JNICALL Java_processing_mode_cpp_Ts_nodeChildCount(JNIEnv *env, jclass cls, jlongArray arr) {
    return (jint)ts_node_child_count(unpack_node(env, arr));
}

JNIEXPORT jint JNICALL Java_processing_mode_cpp_Ts_nodeNamedChildCount(JNIEnv *env, jclass cls, jlongArray arr) {
    return (jint)ts_node_named_child_count(unpack_node(env, arr));
}

JNIEXPORT jlongArray JNICALL Java_processing_mode_cpp_Ts_nodeChild(JNIEnv *env, jclass cls, jlongArray arr, jint i) {
    TSNode child = ts_node_child(unpack_node(env, arr), (uint32_t)i);
    if (ts_node_is_null(child)) return NULL;
    return pack_node(env, child);
}

JNIEXPORT jlongArray JNICALL Java_processing_mode_cpp_Ts_nodeNamedChild(JNIEnv *env, jclass cls, jlongArray arr, jint i) {
    TSNode child = ts_node_named_child(unpack_node(env, arr), (uint32_t)i);
    if (ts_node_is_null(child)) return NULL;
    return pack_node(env, child);
}

JNIEXPORT jlongArray JNICALL Java_processing_mode_cpp_Ts_nodeChildByFieldName(JNIEnv *env, jclass cls, jlongArray arr, jstring jfield) {
    const char *field = (*env)->GetStringUTFChars(env, jfield, NULL);
    TSNode child = ts_node_child_by_field_name(unpack_node(env, arr), field, (uint32_t)strlen(field));
    (*env)->ReleaseStringUTFChars(env, jfield, field);
    if (ts_node_is_null(child)) return NULL;
    return pack_node(env, child);
}

JNIEXPORT jboolean JNICALL Java_processing_mode_cpp_Ts_nodeIsNamed(JNIEnv *env, jclass cls, jlongArray arr) {
    return (jboolean)ts_node_is_named(unpack_node(env, arr));
}

JNIEXPORT jboolean JNICALL Java_processing_mode_cpp_Ts_nodeHasError(JNIEnv *env, jclass cls, jlongArray arr) {
    return (jboolean)ts_node_has_error(unpack_node(env, arr));
}

JNIEXPORT jboolean JNICALL Java_processing_mode_cpp_Ts_nodeIsError(JNIEnv *env, jclass cls, jlongArray arr) {
    return (jboolean)ts_node_is_error(unpack_node(env, arr));
}

JNIEXPORT jboolean JNICALL Java_processing_mode_cpp_Ts_nodeIsMissing(JNIEnv *env, jclass cls, jlongArray arr) {
    return (jboolean)ts_node_is_missing(unpack_node(env, arr));
}
