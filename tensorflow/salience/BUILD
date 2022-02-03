# Description:
# test for stream_executor
load(
    "//tensorflow:tensorflow.bzl",
    "tf_cc_shared_object",
)

package(
    licenses = ["notice"],
)

cc_library(
    name = "salience_internal",
    srcs = ["ppu_stream_exec.cc"],
    hdrs = ["ppu_stream_exec.h"],
    visibility = ["//tensorflow/c:__subpackages__"],
    deps = [
        "//tensorflow/c/experimental/stream_executor:stream_executor_hdrs",
        "//tensorflow/c/experimental/stream_executor:stream_executor_test_util",
	"//tensorflow/c:kernels",
	"//tensorflow/c:ops",
    ],
)

tf_cc_shared_object(
    name = "ppu_pluggable_device.so",
    srcs = ["ppu_pluggable_device.cc"],
    visibility = ["//tensorflow/c:__subpackages__"],
    deps = [
        ":salience_internal",
    ],
)
