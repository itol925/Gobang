LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := Gobang
LOCAL_SRC_FILES := AI.cpp chessboard.cpp JChessboard.cpp

include $(BUILD_SHARED_LIBRARY)
