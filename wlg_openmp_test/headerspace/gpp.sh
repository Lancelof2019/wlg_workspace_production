g++ `pkg-config opencv4 --cflags --libs` -fopenmp  -o result1.out ../functionspace/antiInverseImage.cpp ../functionspace/distanceTransform.cpp ../functionspace/makeImageGrayScale.cpp ../functionspace/removeholesopt.cpp ../functionspace/thresholdMeasure.cpp ../functionspace/processImage.cpp ../functionspace/erosion.cpp ../functionspace/watershed.cpp ../structPixel/structPixel.cpp ../mainspace/WaterShedAlg.cpp
