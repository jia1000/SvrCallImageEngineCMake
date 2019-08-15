trymkdir() {
    if [ ! -e $1 ];then
        mkdir -p $1
    fi
}

echo ">>>>> 正在 生成ImagingEngineLib.so..."
cd ImagingEngineLib
trymkdir build
cd build
cmake ..
make -j

cd ../../..
trymkdir build
cd src
./generate_dicom_engine.sh

echo ">>>>> 正在 复制build/.so 到output文件夹..."
cd ..
trymkdir output
cp ./build/lib/libcharls.so ./output/
cp ./build/lib/libcontroller.so ./output/
cp ./build/lib/libdicom_engine.so ./output/
cp ./build/lib/libImagingEngineLib.so ./output/
cp ./build/lib/libSvrCallImageEngine.so ./output/
