#Makefile
TARGET = checkers
FILENAME = checkers_marker.cpp
INCLUDE_DIRS = -I/usr/local/include/opencv -I/usr/local/include
LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab -lrt -lpthread -lm -ldl
CXX = g++
CXXFLAGS += -Wall -O2 -DLINUX $(INCLUDE_DIRS)


all:	$(TARGET)

$(TARGET):
		$(CXX) $(FILENAME) $(CXXFLAGS) $(LIBS)  -o $@

clean:
	rm -rf $(TARGET) ./images/roi.png ./images/marked_frame.png